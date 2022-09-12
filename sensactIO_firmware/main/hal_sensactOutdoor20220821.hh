#pragma once
#include "common_in_project.hh"
#include <limits>
#include <cmath>
#include <array>

#include <driver/mcpwm.h>
#include <driver/ledc.h>
#include <driver/i2c.h>
#include <driver/rmt.h>

#include "applicationBase.hh"
#include "rgbled.hh"

#include <i2c.hh>

//Outputs 1...6 sind die sechs Relays
//Outputs 7..11 sind die fünf MOSFETS
//Output 12 ist der Constant Current LED Treiber

#define TAG "HAL"

typedef gpio_num_t Pintype;



static constexpr Pintype PIN_PROG_IO0 = (Pintype)0;
static constexpr Pintype PIN_PROG_TX = (Pintype)1;
static constexpr Pintype PIN_SPI_CSN = (Pintype)2;
static constexpr Pintype PIN_PROG_RX = (Pintype)3;
static constexpr Pintype PIN_I2C_SCL = (Pintype)4;
static constexpr Pintype PIN_RGBLED = (Pintype)5;
static constexpr Pintype PIN_PWM5 = (Pintype)12;
//static constexpr Pintype PIN_CAN_TX = (Pintype)12;
static constexpr Pintype PIN_PWM_CC = (Pintype)13;
static constexpr Pintype PIN_SPI_SCLK = (Pintype)14;
static constexpr Pintype PIN_SPI_CE = (Pintype)15;
static constexpr Pintype PIN_I2C_SDA = (Pintype)16;
static constexpr Pintype PIN_K6 = (Pintype)17;
static constexpr Pintype PIN_K5 = (Pintype)18;
static constexpr Pintype PIN_K4 = (Pintype)19;
static constexpr Pintype PIN_K3 = (Pintype)21;
static constexpr Pintype PIN_K2 = (Pintype)22;
static constexpr Pintype PIN_K1 = (Pintype)23;
static constexpr Pintype PIN_PWM3 = (Pintype)25;
static constexpr Pintype PIN_PWM4 = (Pintype)26;
static constexpr Pintype PIN_SPI_MOSI = (Pintype)27;
static constexpr Pintype PIN_PWM1 = (Pintype)32;
static constexpr Pintype PIN_PWM2 = (Pintype)33;
static constexpr Pintype PIN_SPI_MISO = (Pintype)34;
static constexpr Pintype PIN_THERM = (Pintype)35;
static constexpr Pintype PIN_MAINS = (Pintype)36;    //VP
//static constexpr Pintype PIN_CAN_RX = (Pintype)36; //VP
static constexpr Pintype PIN_IRQ = (Pintype)39; //VN

static constexpr std::array<Pintype, 6> RELAY_PINS{PIN_K1, PIN_K2, PIN_K3, PIN_K4, PIN_K5, PIN_K6};
static constexpr std::array<Pintype, 6> PWM_PINS{PIN_PWM1, PIN_PWM2, PIN_PWM3, PIN_PWM4, PIN_PWM5, PIN_PWM_CC};

static constexpr size_t LED_NUMBER {2};
static constexpr rmt_channel_t CHANNEL_RX470C{RMT_CHANNEL_6};
static constexpr i2c_port_t I2C_PORT{I2C_NUM_1};

static constexpr auto PWM_RESOLUTION{LEDC_TIMER_10_BIT};
static constexpr uint32_t PWM_FREQ{500};

class HAL_SensactOutdoor : public HAL
{
private:
    bool needLedStripUpdate = false;
    RGBLED<LED_NUMBER, DeviceType::PL9823> *strip = NULL;
    float airTemperatureDegCel[2]{std::numeric_limits<float>::quiet_NaN()};
    float airPressurePa{std::numeric_limits<float>::quiet_NaN()};
    float airRelHumidityPercent{std::numeric_limits<float>::quiet_NaN()};
    float ambientBrightnessLux{std::numeric_limits<float>::quiet_NaN()};

public:
    HAL_SensactOutdoor() {}

    ErrorCode ConfigureIO(uint16_t io, IOMode mode){
        //SensactOutdoor IOs are not configurable!
        return ErrorCode::OK;
    }

    ErrorCode HardwareTest() override{

        ColorizeLed(0, CRGB::Red);
        ColorizeLed(1, CRGB::Red);
        ESP_LOGI(TAG, "WS2812 RED");
        strip->Refresh(100);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ColorizeLed(0, CRGB::Green);
        ColorizeLed(1, CRGB::Green);
        ESP_LOGI(TAG, "WS2812 Green");
        strip->Refresh(100);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ColorizeLed(0, CRGB::Blue);
        ColorizeLed(1, CRGB::Blue);
        ESP_LOGI(TAG, "WS2812 Blue");
        strip->Refresh(100);
        vTaskDelay(500/portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "RELAY1");
        SetU16Output(pins::sensactOutdoor::RELAY1, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "RELAY2");
        SetU16Output(pins::sensactOutdoor::RELAY2, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "RELAY3");
        SetU16Output(pins::sensactOutdoor::RELAY3, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "RELAY4");
        SetU16Output(pins::sensactOutdoor::RELAY4, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "RELAY5");
        SetU16Output(pins::sensactOutdoor::RELAY5, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "RELAY6");
        SetU16Output(pins::sensactOutdoor::RELAY6, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "ALL OFF");
        SetU16Output(pins::sensactOutdoor::RELAY1, 0);
        SetU16Output(pins::sensactOutdoor::RELAY2, 0);
        SetU16Output(pins::sensactOutdoor::RELAY3, 0);
        SetU16Output(pins::sensactOutdoor::RELAY4, 0);
        SetU16Output(pins::sensactOutdoor::RELAY5, 0);
        SetU16Output(pins::sensactOutdoor::RELAY6, 0);
        vTaskDelay(500/portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "MOSFET1");
        SetU16Output(pins::sensactOutdoor::MOSFET1, 32000);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MOSFET1 + 2");
        SetU16Output(pins::sensactOutdoor::MOSFET1, UINT16_MAX);
        SetU16Output(pins::sensactOutdoor::MOSFET2, 32000);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MOSFET2 + 3");
        SetU16Output(pins::sensactOutdoor::MOSFET2, UINT16_MAX);
        SetU16Output(pins::sensactOutdoor::MOSFET3, 32000);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MOSFET3 + 4");
        SetU16Output(pins::sensactOutdoor::MOSFET3, UINT16_MAX);
        SetU16Output(pins::sensactOutdoor::MOSFET4, 32000);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MOSFET4 + 5");
        SetU16Output(pins::sensactOutdoor::MOSFET4, UINT16_MAX);
        SetU16Output(pins::sensactOutdoor::MOSFET5, 32000);
        vTaskDelay(500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MOSFET5 + LED_CC");
        SetU16Output(pins::sensactOutdoor::MOSFET5, UINT16_MAX);
        SetU16Output(pins::sensactOutdoor::LED_CC, 16000);
        vTaskDelay(250/portTICK_PERIOD_MS);
        SetU16Output(pins::sensactOutdoor::LED_CC, 32000);
        vTaskDelay(250/portTICK_PERIOD_MS);
        SetU16Output(pins::sensactOutdoor::LED_CC, UINT16_MAX);
        vTaskDelay(250/portTICK_PERIOD_MS);
         SetU16Output(pins::sensactOutdoor::LED_CC, 32000);
        vTaskDelay(250/portTICK_PERIOD_MS);
         SetU16Output(pins::sensactOutdoor::LED_CC, 16000);
        vTaskDelay(250/portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "All MOSFET off & LED_CC off");
        SetU16Output(pins::sensactOutdoor::MOSFET1, 0);
        SetU16Output(pins::sensactOutdoor::MOSFET2, 0);
        SetU16Output(pins::sensactOutdoor::MOSFET3, 0);
        SetU16Output(pins::sensactOutdoor::MOSFET4, 0);
        SetU16Output(pins::sensactOutdoor::MOSFET5, 0);
        SetU16Output(pins::sensactOutdoor::LED_CC, 0);
        return ErrorCode::OK;
    }

    ErrorCode GetBoolInputs(uint32_t *ptr){
        *ptr=0;
        return ErrorCode::OK;
    }
    ErrorCode SetU16Output(uint16_t pinId, uint16_t state) override
    {
        if(pinId==0) return ErrorCode::OK;

        if(pinId>=pins::sensactOutdoor::RELAY1 && pinId<=pins::sensactOutdoor::RELAY6){
            pinId-=pins::sensactOutdoor::RELAY1;
            gpio_set_level(RELAY_PINS[pinId], state); //alles !=0 wird als logisch 1 erkannt
            return ErrorCode::OK;
        }
        if(pinId>=pins::sensactOutdoor::MOSFET1 && pinId <=pins::sensactOutdoor::LED_CC){
            pinId-=pins::sensactOutdoor::MOSFET1;
            uint32_t duty = state>>(16-PWM_RESOLUTION);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)pinId, duty);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)pinId);
            return ErrorCode::OK;
        }
        return ErrorCode::NONE_AVAILABLE;
    }

    size_t GetPinCnt(){
        return pins::sensactOutdoor::PIN_MAX;
    }

    ErrorCode ColorizeLed(uint8_t index, CRGB color) override
    {
        if(!strip) return ErrorCode::TEMPORARYLY_NOT_AVAILABLE;
        RETURN_ERRORCODE_ON_ERROR((strip->SetPixel((uint8_t)index, color)), ErrorCode::GENERIC_ERROR);
        //TODO: Hier Prüfung, ob sich tatsächlich etwas verändert hat und ein Update tatsächlich erforderlich ist
        this->needLedStripUpdate = true;
        return ErrorCode::OK;
    }

    ErrorCode UnColorizeAllLed() override
    {
        if(!strip) return ErrorCode::TEMPORARYLY_NOT_AVAILABLE;
        strip->Clear(1000);
        this->needLedStripUpdate = true;
        return ErrorCode::OK;
    }

    int64_t IRAM_ATTR GetMicros() override
    {
        return esp_timer_get_time();
    }

    uint32_t GetMillis() override
    {
        return (uint32_t)(esp_timer_get_time() / 1000ULL);
    }

    int64_t GetMillis64() override
    {
        return esp_timer_get_time() / 1000ULL;
    }

    void SensorLoop_ForInternalUseOnly()
    {








        while (true)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    ErrorCode GetAirTemperature(float *degreesCelcius)
    {
        if (!std::isnan(this->airTemperatureDegCel[0]))
        {
            *degreesCelcius = this->airTemperatureDegCel[0];
            return ErrorCode::OK;
        }
        else if (!std::isnan(this->airTemperatureDegCel[1]))
        {
            *degreesCelcius = this->airTemperatureDegCel[1];
            return ErrorCode::OK;
        }
        return ErrorCode::NONE_AVAILABLE;
    }

    ErrorCode GetAirPressure(float *pa)
    {
        *pa = this->airPressurePa;
        return ErrorCode::OK;
    }
    ErrorCode GetAirRelHumidity(float *percent)
    {
        *percent = this->airRelHumidityPercent;
        return ErrorCode::OK;
    }

    ErrorCode GetAmbientBrightness(float *lux)
    {
        *lux = this->ambientBrightnessLux;
        return ErrorCode::OK;
    }

    ErrorCode SetupAndRun() override
    {
        gpio_pad_select_gpio((uint8_t)PIN_MAINS);
        gpio_set_direction(PIN_MAINS, GPIO_MODE_INPUT);
        gpio_set_pull_mode(PIN_MAINS, GPIO_FLOATING);

        gpio_pad_select_gpio((uint8_t)PIN_IRQ);
        gpio_set_direction(PIN_IRQ, GPIO_MODE_INPUT);
        gpio_set_pull_mode(PIN_IRQ, GPIO_FLOATING);

        for (const auto &pin : RELAY_PINS)
        {
            gpio_set_level(pin, 0);
            gpio_pad_select_gpio((uint8_t)pin);
            gpio_set_direction(pin, GPIO_MODE_OUTPUT);
            gpio_set_pull_mode(pin, GPIO_FLOATING);
        }

        for (const auto &pin : PWM_PINS)
        {
            gpio_set_level(pin, 0);
            gpio_pad_select_gpio((uint8_t)pin);
            gpio_set_direction(pin, GPIO_MODE_OUTPUT);
            gpio_set_pull_mode(pin, GPIO_FLOATING);
        }

        //PWM Output
        ledc_timer_config_t ledc_timer;
        ledc_timer.duty_resolution = PWM_RESOLUTION; // resolution of PWM duty
        ledc_timer.freq_hz = PWM_FREQ;                 // frequency of PWM signal
        ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;            // timer mode
        ledc_timer.timer_num = LEDC_TIMER_0;                     // timer index
        ledc_timer.clk_cfg = LEDC_AUTO_CLK;
        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

        for (size_t i = 0; i < PWM_PINS.size(); i++)
        {
            ledc_channel_config_t ledc_channel;
            ledc_channel.channel = (ledc_channel_t)i;
            ledc_channel.duty = 0;
            ledc_channel.gpio_num = PWM_PINS[i];
            ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
            ledc_channel.hpoint = 0;
            ledc_channel.timer_sel = LEDC_TIMER_0;
            ledc_channel.intr_type=LEDC_INTR_DISABLE;
            ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        }

        //I2C Master
        ESP_ERROR_CHECK(I2C::Init(I2C_PORT, PIN_I2C_SCL, PIN_I2C_SDA));

        //LED Strip
        strip = new RGBLED<LED_NUMBER, DeviceType::PL9823>();
        ESP_ERROR_CHECK(strip->Init(VSPI_HOST, PIN_RGBLED, 2 ));
        ESP_ERROR_CHECK(strip->Clear(100));
        
        //Read Sensors
  
        
        return ErrorCode::OK;
    }

    ErrorCode BeforeLoop()
    {

        return ErrorCode::OK;
    }

    ErrorCode AfterLoop()
    {
        if (needLedStripUpdate)
        {
            strip->Refresh(100);
            needLedStripUpdate = false;
        }
        return ErrorCode::OK;
    }
};



#undef TAG