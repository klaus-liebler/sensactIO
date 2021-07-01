#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <esp_system.h>
#include "esp_spi_flash.h"

#include "esp_adc_cal.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <sys/param.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include "connect.h"
#include <esp_http_server.h>
#include "http_handlers.hh"

#include "common.hh"

#include "ws2812_strip.hh"
#include "esp_log.h"
#include "hal_sensactOutdoorV1.hh"
#include "manager.hh"
#include "i2c_io.hh"
#include "i2c_mem.hh"
#include "rx470c.hh"
#include "milight.hh"
#include "spiffs.hh"
#include <map>

#define TAG "main"
I2C_IO *const i2c_io = new I2C_IO();
I2C_MemoryEmulation i2c_mem(I2C_NUM_0, i2c_io);
std::vector<IOSource *> ioSources{i2c_io};

class Milight2Manager:public MilightCallback{
public:
    void ReceivedFromMilight(uint8_t milightcmd, uint8_t arg)
    {
        flatbuffers::FlatBufferBuilder builder(128);
        flatbuffers::Offset<tCommand> cmd;
        switch (milightcmd)
        {
        case 1:
            if (keyCode2command.find(arg) == keyCode2command.end()){
                ESP_LOGW(TAG, "Key %d has no associated handler", arg);
                return;
            }
            cmd = (keyCode2command[arg])(&builder);
            break;
        case 5:{
            auto xcmd = CreatetSinglePwmCommand(builder, eSinglePwmCommand_CHANGE_INTENSITY, arg);
            cmd = CreatetCommand(builder, 3, uCommand::uCommand_tSinglePwmCommand, xcmd.Union());
            }
            break;
        default:
            ESP_LOGW(TAG, "Command %d has no associated handler", milightcmd);
            return;
        }
        builder.Finish(cmd);
        auto parsed_cmd = flatbuffers::GetRoot<tCommand>(builder.GetBufferPointer());
        manager->HandleCommand(parsed_cmd);
    }
    Milight2Manager(Manager *manager) : manager(manager) {}
private:
    Manager *manager;
    std::map<uint8_t, std::function<flatbuffers::Offset<tCommand>(flatbuffers::FlatBufferBuilder *)>> keyCode2command{
        {0, [](flatbuffers::FlatBufferBuilder *builder)
         {
            auto xcmd = CreatetSinglePwmCommand(*builder, eSinglePwmCommand_TOGGLE);
            return CreatetCommand(*builder, 3, uCommand::uCommand_tSinglePwmCommand, xcmd.Union());
         }},
        {9, [](flatbuffers::FlatBufferBuilder *builder)
         {
            auto xcmd = CreatetSinglePwmCommand(*builder, eSinglePwmCommand_TOGGLE);
            return CreatetCommand(*builder, 3, uCommand::uCommand_tSinglePwmCommand, xcmd.Union());
         }},
        {10, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_DOWN_OR_STOP);
             return CreatetCommand(*builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
        {1, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_UP_OR_STOP);
             return CreatetCommand(*builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
    };
};

class RemoteControl2Manager : public Rx470cCallback
{
    /*
    1:16579944
    2:16579940
    3:16579948
    4:16579938
    5:16579946
    6:16579942
    7:16579950
    8:16579937
    9:16579945
    10:16579941
    */
private:
    Manager *manager;
    std::map<uint32_t, std::function<flatbuffers::Offset<tCommand>(flatbuffers::FlatBufferBuilder *)>> keyCode2command{
        {16579944, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_DOWN_OR_STOP);
             return CreatetCommand(*builder, 1, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
        {16579940, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_UP_OR_STOP);
             return CreatetCommand(*builder, 1, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
        {16579948, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_DOWN_OR_STOP);
             return CreatetCommand(*builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
        {16579938, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetBlindCommand(*builder, eBlindCommand_UP_OR_STOP);
             return CreatetCommand(*builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
         }},
        {16579946, [](flatbuffers::FlatBufferBuilder *builder)
         {
             auto xcmd = CreatetSinglePwmCommand(*builder, eSinglePwmCommand_TOGGLE);
             return CreatetCommand(*builder, 3, uCommand::uCommand_tSinglePwmCommand, xcmd.Union());
         }},
    };

public:
    void ReceivedFromRx470c(uint32_t val)
    {
        flatbuffers::FlatBufferBuilder builder(128);
        flatbuffers::Offset<tCommand> cmd;
        if (keyCode2command.find(val) == keyCode2command.end())
        {
            ESP_LOGW(TAG, "KeyCode %d has no associated handler", val);
            return;
        }
        cmd = (keyCode2command[val])(&builder);

        builder.Finish(cmd);
        auto parsed_cmd = flatbuffers::GetRoot<tCommand>(builder.GetBufferPointer());
        manager->HandleCommand(parsed_cmd);
    }
    RemoteControl2Manager(Manager *manager) : manager(manager) {}
};

//HAL *hal = new HAL_WroverKitV41();
HAL *hal = new HAL_SensactOutdoorV1();

Manager *manager = new Manager{hal, ioSources};
//RemoteControl2Manager remoteControl2Manager{manager};
Milight2Manager milight2manager{manager};
//PT2262Decoder decoder;
//RX470C rx470c{CHANNEL_RX470C, &decoder, &remoteControl2Manager};
Milight milight{&milight2manager};

extern "C"
{
    void app_main();
    void managerTask(void *);
}

void managerTask(void *pvParameters)
{
    //Setup'ed is: HAL and i2c_mem
    ESP_LOGI(TAG, "managerTask started");
    TickType_t xLastWakeTime{0};
    const TickType_t xTimeIncrement = 200 / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    manager->Setup();
    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
        manager->Loop();
    }
}

static const httpd_uri_t put_iostate = {
    .uri = "/iostate",
    .method = HTTP_PUT,
    .handler = handle_put_iostate,
    .user_ctx = manager,
};

static const httpd_uri_t get_iostate = {
    .uri = "/iostate",
    .method = HTTP_GET,
    .handler = handle_get_iostate,
    .user_ctx = manager,
};

static const httpd_uri_t get_iocfg = {
    .uri = "/iocfg",
    .method = HTTP_GET,
    .handler = handle_get_iocfg,
    .user_ctx = manager,
};

static const httpd_uri_t put_iocfg = {
    .uri = "/iocfg",
    .method = HTTP_PUT,
    .handler = handle_put_iocfg,
    .user_ctx = manager,
};

static const httpd_uri_t put_devctrl = {
    .uri = "/devctrl",
    .method = HTTP_PUT,
    .handler = handle_put_devctrl,
    .user_ctx = manager,
};

static const httpd_uri_t get_common = {
    .uri = "/*",
    .method = HTTP_GET,
    .handler = handle_get_common,
    .user_ctx = manager,
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    //config.uri_match_fn = httpd_uri_match_wildcard;
    //config.max_uri_handlers = 12;
    config.uri_match_fn = httpd_uri_match_wildcard;
    const char *hostnameptr;
    tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA, &hostnameptr);
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting HTTPd!");
        esp_restart();
    }
    ESP_LOGI(TAG, "HTTPd successfully started for website http://%s:%d", hostnameptr, config.server_port);
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &put_iostate));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &get_iostate));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &put_iocfg));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &get_iocfg));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &put_devctrl));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &get_common));
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

static void disconnect_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server)
    {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void *arg, esp_event_base_t event_base,
                            int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server == NULL)
    {
        *server = start_webserver();
    }
}

void _lab_error_check_failed(ErrorCode rc, const char *file, int line, const char *function, const char *expression)
{
    printf("Error %d occured in File %s in line %d in expression %s", (int)rc, file, line, expression);
}

#ifdef NDEBUG
#define LAB_ERROR_CHECK(x)        \
    do                            \
    {                             \
        esp_err_t __err_rc = (x); \
        (void)sizeof(__err_rc);   \
    } while (0)
#elif defined(CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT)
#define LAB_ERROR_CHECK(x)        \
    do                            \
    {                             \
        esp_err_t __err_rc = (x); \
        if (__err_rc != ESP_OK)   \
        {                         \
            abort();              \
        }                         \
    } while (0)
#else
#define LAB_ERROR_CHECK(x)                                        \
    do                                                            \
    {                                                             \
        ErrorCode __err_rc = (x);                                 \
        if (__err_rc != ErrorCode::OK)                            \
        {                                                         \
            _lab_error_check_failed(__err_rc, __FILE__, __LINE__, \
                                    __ASSERT_FUNC, #x);           \
        }                                                         \
    } while (0)
#endif

void app_main(void)
{
    int i = 0;
    printf("=======================================================\n");
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_ERROR_CHECK(SpiffsManager::Init());
    //ESP_LOGI(TAG, "Init rx470c");
    //ESP_ERROR_CHECK(rx470c.Init(PIN_IRQ, 1));
    //ESP_ERROR_CHECK(rx470c.Start());

    ESP_LOGI(TAG, "Init milight");
    ESP_ERROR_CHECK(milight.Init(HSPI_HOST, 2, PIN_SPI_CE, PIN_SPI_CSN, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCLK));
    ESP_ERROR_CHECK(milight.Start());

    ESP_LOGI(TAG, "Init HAL");
    LAB_ERROR_CHECK(hal->Setup());
    LAB_ERROR_CHECK(hal->HardwareTest());
    ESP_LOGI(TAG, "Init i2c_mem adapter @%d ", CONFIG_I2C_SLAVE_ADDRESS);
    ESP_ERROR_CHECK(i2c_mem.Setup(PIN_I2C_SCL, PIN_I2C_SDA, CONFIG_I2C_SLAVE_ADDRESS, false, 0));

    xTaskCreate(managerTask, "managerTask", 4096 * 4, NULL, 6, NULL);

    static httpd_handle_t server;
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(connect());
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    server = start_webserver(); //Start the server for the first time

    //xTaskCreate(experimentTask, "experimentTask", 1024 * 2, NULL, 5, NULL);
    while (true)
    {
        printf("Start was %d seconds ago. Free heap: %d\n", i, esp_get_free_heap_size());
        i += 5;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
#undef TAG