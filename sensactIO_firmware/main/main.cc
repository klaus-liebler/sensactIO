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
#include "spiffs.hh"

#define TAG "main"
I2C_IO * const i2c_io = new I2C_IO();
I2C_MemoryEmulation i2c_mem(I2C_NUM_0, i2c_io);
std::vector<IOSource*> ioSources{i2c_io};



//HAL *hal = new HAL_WroverKitV41();
HAL *hal = new HAL_SensactOutdoorV1();
Manager *manager=new Manager(hal, ioSources);

PT2262Decoder decoder;
RX470C rx470c{CHANNEL_RX470C, &decoder, manager};

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
    const TickType_t xTimeIncrement = 200/portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    manager->Setup();
    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
        manager->Loop();
    }
}




static const httpd_uri_t put_iostate = {
    .uri       = "/iostate",
    .method    = HTTP_PUT,
    .handler   = handle_put_iostate,
    .user_ctx = manager,
};

static const httpd_uri_t get_iostate = {
    .uri       = "/iostate",
    .method    = HTTP_GET,
    .handler   = handle_get_iostate,
    .user_ctx = manager,
};

static const httpd_uri_t get_iocfg = {
    .uri       = "/iocfg",
    .method    = HTTP_GET,
    .handler   = handle_get_iocfg,
    .user_ctx = manager,
};

static const httpd_uri_t put_iocfg = {
    .uri       = "/iocfg",
    .method    = HTTP_PUT,
    .handler   = handle_put_iocfg,
    .user_ctx = manager,
};

static const httpd_uri_t put_devctrl = {
    .uri       = "/devctrl",
    .method    = HTTP_PUT,
    .handler   = handle_put_devctrl,
    .user_ctx = manager,
};

static const httpd_uri_t get_common = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = handle_get_common,
    .user_ctx = manager,
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    //config.uri_match_fn = httpd_uri_match_wildcard;
    //config.max_uri_handlers = 12;
    config.uri_match_fn = httpd_uri_match_wildcard;
    const char * hostnameptr;
    tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA, &hostnameptr);
    if (httpd_start(&server, &config) != ESP_OK) {
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

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        *server = start_webserver();
    }
}

void _lab_error_check_failed(ErrorCode rc, const char *file, int line, const char *function, const char *expression)
{
    printf("Error %d occured in File %s in line %d in expression %s", (int)rc, file, line, expression);
}

#ifdef NDEBUG
#define LAB_ERROR_CHECK(x) do {                                         \
        esp_err_t __err_rc = (x);                                       \
        (void) sizeof(__err_rc);                                        \
    } while(0)
#elif defined(CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT)
#define LAB_ERROR_CHECK(x) do {                                         \
        esp_err_t __err_rc = (x);                                       \
        if (__err_rc != ESP_OK) {                                       \
            abort();                                                    \
        }                                                               \
    } while(0)
#else
#define LAB_ERROR_CHECK(x) do {                                         \
        ErrorCode __err_rc = (x);                                       \
        if (__err_rc != ErrorCode::OK) {                                       \
            _lab_error_check_failed(__err_rc, __FILE__, __LINE__,       \
                                    __ASSERT_FUNC, #x);                 \
        }                                                               \
    } while(0)
#endif

void app_main(void)
{
    int i = 0;
    printf("=======================================================\n");
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_ERROR_CHECK(SpiffsManager::Init());
    ESP_LOGI(TAG, "Init rx470c");
    ESP_ERROR_CHECK(rx470c.Init(PIN_IRQ, 1));
    ESP_ERROR_CHECK(rx470c.Start());
    
    ESP_LOGI(TAG, "Init HAL");
    LAB_ERROR_CHECK(hal->Setup());
    //LAB_ERROR_CHECK(hal->HardwareTest());
    ESP_LOGI(TAG, "Init i2c_mem adapter @%d ",CONFIG_I2C_SLAVE_ADDRESS);
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