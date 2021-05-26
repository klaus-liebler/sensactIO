#include "ccs811.hh"
#include <i2c.hh>
#include "regbits.hh"

constexpr uint32_t TIME_UNTIL_ACTIVE_AFTER_POWER_ON_MS = 20;

namespace reg
{
    constexpr uint8_t STATUS = 0x00;
    constexpr uint8_t MEAS_MODE = 0x01;
    constexpr uint8_t ALG_RESULT_DATA = 0x02;
    constexpr uint8_t RAW_DATA = 0x03;
    constexpr uint8_t ENV_DATA = 0x05;
    constexpr uint8_t NTC = 0x06;
    constexpr uint8_t THRESHOLDS = 0x10;
    constexpr uint8_t BASELINE = 0x11;
    constexpr uint8_t HW_ID = 0x20; //value sh
    constexpr uint8_t HW_ID_VALUE = 0x81;
    constexpr uint8_t HW_Version = 0x21;
    constexpr uint8_t HW_Version_MIN_VALUE = 0x10;
    constexpr uint8_t FW_Boot_Version = 0x23;
    constexpr uint8_t FW_App_Version = 0x24;
    constexpr uint8_t ERROR_ID = 0xE0;
    constexpr uint8_t BOOTLOADER_APP_ERASE = 0xF1;
    constexpr uint8_t BOOTLOADER_APP_DATA = 0xF2;
    constexpr uint8_t BOOTLOADER_APP_VERIFY = 0xF3;
    constexpr uint8_t BOOTLOADER_APP_START = 0xF4;
    constexpr uint8_t SW_RESET = 0xFF;
    constexpr uint8_t SW_RESET_VALUES[] = {0x11, 0xE5, 0x72, 0x8A};
};

union CSS811_STATUS_REG
{
    struct
    {
        uint8_t ERROR : 1;
        uint8_t reserved1 : 2;
        uint8_t DATA_READY : 1;
        uint8_t APP_VALID : 1;
        uint8_t reserved2 : 2;
        uint8_t FW_MODE : 1;
    };
    uint8_t val;
};



union MEAS_MODE
{
    struct
    {
        uint8_t reserved1 : 2;
        uint8_t THRESH : 1;
        uint8_t CCS811_INTERRUPT : 1;
        CCS811_OPERATIONMODE DRIVE_MODE : 3;
        uint8_t reserved2 : 1;
    };
    uint8_t val;
};


CCS811::CCS811(i2c_port_t i2c_num, CCS811_ADRESS address) : i2c_num(i2c_num), address(address)
{
}
esp_err_t CCS811::Init(CCS811_OPERATIONMODE operation)
{
    if (SoftwareReset() != ESP_OK)
        return ESP_FAIL;
    vTaskDelay(TIME_UNTIL_ACTIVE_AFTER_POWER_ON_MS / portTICK_PERIOD_MS);
    uint8_t reg_data{0};
    if (I2C::ReadReg(i2c_num, (uint8_t)address, reg::HW_ID, &reg_data, 1) != ESP_OK)
        return ESP_FAIL;
    if (reg_data != reg::HW_ID_VALUE)
        return ESP_FAIL;
    if (I2C::WriteReg(i2c_num, (uint8_t)address, reg::BOOTLOADER_APP_START, NULL, 0) != ESP_OK)
        return ESP_FAIL;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    CSS811_STATUS_REG s;
    s.val=0;
    if (I2C::ReadReg(i2c_num, (uint8_t)address, reg::STATUS, &(s.val), 1) != ESP_OK)
        return ESP_FAIL;
    if (s.ERROR)
        return ESP_FAIL;

    if (this->ChangeOperationMode(CCS811_OPERATIONMODE::IAQ_1SEC) != ESP_OK)
        return ESP_FAIL;
    return ESP_OK;
}

esp_err_t CCS811::ChangeOperationMode(CCS811_OPERATIONMODE newMode)
{
    MEAS_MODE m;
    m.val=0;
    m.DRIVE_MODE = newMode;
    return I2C::WriteReg(i2c_num, (uint8_t)address, reg::MEAS_MODE, &m.val, 1);
}

esp_err_t CCS811::SoftwareReset()
{
    return I2C::WriteReg(i2c_num, (uint8_t)address, reg::SW_RESET, reg::SW_RESET_VALUES, 4);
}
esp_err_t CCS811::Read(uint16_t *eCO2, uint16_t *TVOC)
{
    uint8_t buf[5];
    if (I2C::ReadReg(i2c_num, (uint8_t)address, reg::ALG_RESULT_DATA, buf, 5) != ESP_OK){
        return ESP_FAIL;
    } 
    CSS811_STATUS_REG s;
    s.val = buf[4];
    if (s.ERROR) return ESP_FAIL;
    *eCO2 = ((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]);
    *TVOC = ((uint16_t)buf[2] << 8) | ((uint16_t)buf[3]);
    return ESP_OK;
}

esp_err_t CCS811::SetEnvironmentalData(float humidity, float temperature)
{
    /* Humidity is stored as an unsigned 16 bits in 1/512%RH. The
  default value is 50% = 0x64, 0x00. As an example 48.5%
  humidity would be 0x61, 0x00.*/

    /* Temperature is stored as an unsigned 16 bits integer in 1/512
  degrees; there is an offset: 0 maps to -25°C. The default value is
  25°C = 0x64, 0x00. As an example 23.5% temperature would be
  0x61, 0x00.
  The internal algorithm uses these values (or default values if
  not set by the application) to compensate for changes in
  relative humidity and ambient temperature.*/

    uint16_t hum_conv = humidity * 512.0f + 0.5f;
    uint16_t temp_conv = (temperature + 25.0f) * 512.0f + 0.5f;

    uint8_t buf[] = {
        (uint8_t)((hum_conv >> 8) & 0xFF), (uint8_t)(hum_conv & 0xFF),
        (uint8_t)((temp_conv >> 8) & 0xFF), (uint8_t)(temp_conv & 0xFF)};

    return I2C::WriteReg(i2c_num, (uint8_t)address, reg::ENV_DATA, buf, 4);
}