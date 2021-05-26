#pragma once

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"


enum class CCS811_ADRESS:uint8_t
{
    ADDR_LOW=0x5A,
    ADDR_HIGH=0x5B,
};

enum class CCS811_OPERATIONMODE:uint8_t
{
    
    IDLE=0,
    IAQ_1SEC=1,
    IAQ_10SEC=2,
    LOW_POWER_60SEC=3,
    SENSOR_MEASUREMENT_250ms=4
};


class CCS811{
private:
    i2c_port_t i2c_num;
    CCS811_ADRESS address;
public:
    CCS811(const i2c_port_t i2c_num, CCS811_ADRESS adress);
    ~CCS811();
    esp_err_t Init(CCS811_OPERATIONMODE operation);
    esp_err_t Read(uint16_t *eCO2, uint16_t *TVOC);
    esp_err_t SoftwareReset();
    esp_err_t ChangeOperationMode(CCS811_OPERATIONMODE newMode);
    esp_err_t SetEnvironmentalData(float humidity, float temperature);
};