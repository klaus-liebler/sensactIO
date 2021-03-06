#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <array>

#include "esp_err.h"
#include "esp_log.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"
#include "http_handlers.hh"
#include "manager.hh"
#include "paths_and_files.hh"

#define TAG "HTTP_handler"

constexpr size_t SCRATCH_BUFSIZE = 8192;
char scratch[SCRATCH_BUFSIZE];

constexpr size_t MAX_FILE_SIZE  = (200*1024); // 200 KB
constexpr const char* MAX_FILE_SIZE_STR="File size must be less than 200KB!";

class BinaryWriterImpl : public BinaryWriter
{
public:
    std::array<uint32_t, 128> buf{0};
    size_t statusBufferPtr{0};
    uint32_t currentApplicationId{0};
    size_t nextBufferPtr{0};

    ErrorCode StartWriting(uint32_t applicationId, size_t sizeIn4byteUnits) override
    {
        //check, if previous has been finished
        if (applicationId != 0)
            return ErrorCode::INVALID_STATE;
        if (statusBufferPtr + 2 + sizeIn4byteUnits >= buf.size())
            return ErrorCode::INDEX_OUT_OF_BOUNDS;
        currentApplicationId = applicationId;
        nextBufferPtr = statusBufferPtr + 2 + sizeIn4byteUnits;
        buf[statusBufferPtr++] = (uint32_t)applicationId;
        buf[statusBufferPtr++] = sizeIn4byteUnits;
        return ErrorCode::OK;
    }
    ErrorCode WriteU32(uint32_t value) override
    {
        if (statusBufferPtr >= nextBufferPtr)
            return ErrorCode::NONE_AVAILABLE;
        buf[statusBufferPtr++] = value;
        return ErrorCode::OK;
    }
    ErrorCode WriteF32(float value) override
    {
        if (statusBufferPtr >= nextBufferPtr)
            return ErrorCode::NONE_AVAILABLE;
        uint32_t value1 = *(uint32_t *)&value;
        buf[statusBufferPtr++] = value1;
        return ErrorCode::OK;
    }
    ErrorCode EndWriting(uint32_t applicationId) override
    {
        if (currentApplicationId != applicationId)
            return ErrorCode::INVALID_STATE;
        currentApplicationId = 0;
        statusBufferPtr = nextBufferPtr;
        return ErrorCode::OK;
    }
};


extern const char index_html_gz_start[] asm("_binary_index_html_gz_start");
extern const char index_html_gz_end[] asm("_binary_index_html_gz_end");
extern const size_t index_html_gz_size asm("index_html_gz_length");

esp_err_t handle_get_common(httpd_req_t *req)//browser get application itself
{
    ESP_LOGI(TAG, "HTTPd get_root for file with lenght %d", index_html_gz_size);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    ESP_ERROR_CHECK(httpd_resp_send(req, index_html_gz_start, index_html_gz_size)); // -1 = use strlen()
    return ESP_OK;
}

esp_err_t handle_get_iostate(httpd_req_t *req) //brwoser gets state of IOs
{
    Manager *manager = (Manager *)(req->user_ctx);
    flatbuffers::FlatBufferBuilder builder(512);
    manager->FillBuilderWithStateForWebUI(&builder);
    //ESP_LOGI(TAG, "HTTPd returns iostate len  %d", builder.GetSize());
    httpd_resp_set_type(req, "application/octet-stream");
    httpd_resp_send(req, (const char *)builder.GetBufferPointer(), builder.GetSize());
    return ESP_OK;
}

esp_err_t handle_put_iostate(httpd_req_t *req) //browser sends commands to change IOs
{
    Manager *manager = (Manager *)(req->user_ctx);
    int ret = 0;
    int remaining = req->content_len;
    uint8_t buf[req->content_len];
    while (remaining > 0)
    {
        // Read the data for the request
        if ((ret = httpd_req_recv(req, (char *)buf, MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                // Retry receiving if timeout occurred
                continue;
            }
            return ESP_FAIL;
        }
        remaining -= ret;
    }
    // End response
    // Get a pointer to the root object inside the buffer.
    //ESP_LOGI(TAG, "Received put_iostate length %d", req->content_len);
    auto cmd = flatbuffers::GetRoot<tCommand>(buf);
    manager->HandleCommand(cmd);
    flatbuffers::FlatBufferBuilder builder(512);
    manager->FillBuilderWithStateForWebUI(&builder);
    httpd_resp_set_type(req, "application/octet-stream");
    httpd_resp_send(req, (const char *)builder.GetBufferPointer(), builder.GetSize());
    return ESP_OK;
}

esp_err_t helper_get_binary_file(httpd_req_t *req, const char *filepath)//helper
{
    FILE *fd = NULL;
    struct stat file_stat;
    ESP_LOGI(TAG, "helper_get_binary_file : %s", filepath);
    if (stat(filepath, &file_stat) == -1){
        ESP_LOGE(TAG, "Failed to stat file : %s", filepath);
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
        return ESP_FAIL;
    }
    fd = fopen(filepath, "r");
    if (!fd){
        ESP_LOGE(TAG, "Failed to read existing file : %s", filepath);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }
    httpd_resp_set_type(req, "application/octet-stream");
    /* Retrieve the pointer to scratch buffer for temporary storage */
    char *chunk = scratch;
    size_t chunksize;
    do{
        /* Read file in chunks into the scratch buffer */
        chunksize = fread(chunk, 1, SCRATCH_BUFSIZE, fd);
        if (chunksize > 0){
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, chunksize) != ESP_OK){
                fclose(fd);
                ESP_LOGE(TAG, "File %s sending failed!", filepath);
                httpd_resp_sendstr_chunk(req, NULL);
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (chunksize != 0);
    fclose(fd);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

esp_err_t helper_put_binary_file(httpd_req_t *req, const char *filepath, bool overwrite)//helper
{
    FILE *fd = NULL;
    struct stat file_stat;
    ESP_LOGI(TAG, "Trying to store : %s", filepath);
    if (stat(filepath, &file_stat) == 0) {
        if(overwrite)
        {
            unlink(filepath);
        }
        else
        {
            ESP_LOGE(TAG, "File already exists : %s", filepath);
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File already exists");
            return ESP_FAIL;
        }
    }

    /* File cannot be larger than a limit */
    if (req->content_len > MAX_FILE_SIZE) {
        ESP_LOGE(TAG, "File too large : %d bytes", req->content_len);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, MAX_FILE_SIZE_STR);
        /* Return failure to close underlying connection else the
         * incoming file content will keep the socket busy */
        return ESP_FAIL;
    }

    fd = fopen(filepath, "w");
    if (!fd) {
        ESP_LOGE(TAG, "Failed to create file : %s", filepath);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to create file");
        return ESP_FAIL;
    }
    char *buf = scratch;
    int received;

    /* Content length of the request gives the size of the file being uploaded */
    int remaining = req->content_len;

    while (remaining > 0) {
        if ((received = httpd_req_recv(req, buf, MIN(remaining, SCRATCH_BUFSIZE))) <= 0) {
            if (received == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            /* In case of unrecoverable error,
             * close and delete the unfinished file*/
            fclose(fd);
            unlink(filepath);

            ESP_LOGE(TAG, "File %s reception failed!", filepath);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive file");
            return ESP_FAIL;
        }

        /* Write buffer content to file on storage */
        if (received && (received != fwrite(buf, 1, received, fd))) {
            /* Couldn't write everything to file! Storage may be full? */
            fclose(fd);
            unlink(filepath);
            ESP_LOGE(TAG, "File %s write failed!", filepath);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to write file to storage");
            return ESP_FAIL;
        }
        /* Keep track of remaining size of the file left to be uploaded */
        remaining -= received;
    }

    /* Close file upon upload completion */
    fclose(fd);
    httpd_resp_sendstr(req, "File uploaded successfully");
    ESP_LOGI(TAG, "File stored successfully: %s", filepath);
    return ESP_OK;
}

esp_err_t handle_get_iocfg(httpd_req_t *req)//browser gets configuration of IOs to draw the correct user interface
{
    return helper_get_binary_file(req, Paths::DEFAULTCFG_PATH);
}

esp_err_t handle_put_iocfg(httpd_req_t *req)//browser sends new configuraion of IOs
{
    return helper_put_binary_file(req, Paths::DEFAULTCFG_PATH, true);
}

esp_err_t handle_put_devctrl(httpd_req_t *req)//browser sends new configuraion of IOs
{
    httpd_resp_sendstr(req, "Device will be restarted");
    ESP_LOGI(TAG, "Device will be restarted");
    esp_restart();
    return ESP_OK;
}
#undef TAG