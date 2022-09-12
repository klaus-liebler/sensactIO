#include <stdio.h>
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <esp_spi_flash.h>

#include <esp_adc_cal.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <sys/param.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_ota_ops.h>
#include <esp_http_client.h>
#include <esp_https_ota.h>
#include <wifimanager.hh>
#include <otamanager.hh>
#include <esp_http_server.h>
#include "http_handlers.hh"
#include <httpd_helper.hh>

#include "common_in_project.hh"

#include "rgbled.hh"
#include "esp_log.h"
#include "hal_sensactOutdoor20220821.hh"
#include "manager.hh"
//#include "i2c_io.hh"
//#include "i2c_mem.hh"
#include "rx470c.hh"
#include "milight.hh"
#include "spiffs.hh"
#include <map>
#include "milight2manager.hh"

#define TAG "main"
//I2C_IO *const i2c_io = new I2C_IO();
//I2C_MemoryEmulation i2c_mem(I2C_NUM_0, i2c_io);
//std::vector<IOSource *> ioSources{i2c_io};
std::vector<IOSource *> ioSources{};
//HAL *hal = new HAL_WroverKitV41();
HAL *hal = new HAL_SensactOutdoor();

Manager *manager = new Manager{hal, ioSources};
MilightSensactOutdoor milight2manager{manager};
Milight milight{&milight2manager};
uint8_t http_scatchpad[CONFIG_HTTP_SCRATCHPAD_SIZE] ALL4;



static httpd_handle_t SetupAndRunWebserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_uri_handlers = 12;
    config.global_user_ctx = http_scatchpad;
    
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting HTTPd!");
        esp_restart();
    }
   
    const char *hostnameptr;
    ESP_ERROR_CHECK(esp_netif_get_hostname(WIFIMGR::wifi_netif_ap, &hostnameptr));
    tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA, &hostnameptr);
    ESP_LOGI(TAG, "Webserver successfully started for website http://%s:%d", hostnameptr, config.server_port);
    return server;
}

extern "C" void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    esp_log_level_set("wifi", ESP_LOG_WARN);
    ESP_ERROR_CHECK(SpiffsManager::Init());

    ESP_LOGI(TAG, "Init milight");
    ESP_ERROR_CHECK(milight.Init(HSPI_HOST, 1, PIN_SPI_CE, PIN_SPI_CSN, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCLK));
    ESP_LOGI(TAG, "Init milight FINISHED");
    ESP_ERROR_CHECK(milight.Start());
    ESP_LOGI(TAG, "Init HAL");
    ERRORCODE_CHECK(hal->SetupAndRun());
    ERRORCODE_CHECK(hal->HardwareTest());
    //ESP_LOGI(TAG, "Init i2c_mem adapter @%d ", CONFIG_I2C_SLAVE_ADDRESS);
    //ESP_ERROR_CHECK(i2c_mem.Setup(PIN_I2C_SCL, PIN_I2C_SDA, CONFIG_I2C_SLAVE_ADDRESS, false, 0));
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_ERROR_CHECK(WIFIMGR::InitAndRun(false, http_scatchpad, sizeof(http_scatchpad)));
    otamanager::M otamanager;
    //otamanager.InitAndRun(CONFIG_OTA_URL);

    httpd_handle_t httpd_handle = SetupAndRunWebserver();
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/iostate", HTTP_PUT, handle_put_iostate, manager));
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/iostate", HTTP_GET, handle_get_iostate, manager));
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/iocfg", HTTP_GET, handle_get_iocfg, manager));
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/iocfg", HTTP_PUT, handle_put_iocfg, manager));
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/devctrl", HTTP_PUT, handle_put_devctrl, manager));
    WIFIMGR::RegisterHTTPDHandlers(httpd_handle);
    ESP_ERROR_CHECK(RegisterHandler(httpd_handle, "/*", HTTP_GET, handle_get_common, manager));
    
    manager->SetupAndRun();

    while (true)
    {
        ESP_LOGI(TAG, "Free heap: %d",  esp_get_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
