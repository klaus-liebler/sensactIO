#pragma once
#include "esp_log.h"
#include "common.hh"
#include "applicationBase.hh"
#include <rx470c.hh>
#include <vector>
#include <freertos/semphr.h>

class Manager:public PostOffice, public InputOutput
{
private:
    HAL * const hal;
    SensactContext ctx;
    std::vector<cApplication *> apps;
    std::vector<IOSource*> ioSources;
    std::vector<uint16_t> outputU16Buffer;
    uint32_t outputBufferChangedBits;
    uint32_t inputsBuffer;
    std::vector<IOMode> configurationBuffer;
    bool reconfigurationRequestOccured{false};
    SemaphoreHandle_t handleCommandSemaphore = NULL;
public:
    Manager(HAL *hal, std::vector<IOSource *> ioSources);
    ErrorCode HandleCommand(const sensact::comm::tCommand *cmd);
    ErrorCode FillBuilderWithStateForWebUI(flatbuffers::FlatBufferBuilder *builder);
    ErrorCode Setup();
    ErrorCode Loop();
    ErrorCode PostCommand(const tCommand* cmd) override;
    ErrorCode ConfigureIO(uint16_t index, IOMode mode) override;
    ErrorCode SetU16Output(uint16_t index, uint16_t state) override;
    ErrorCode GetBoolInputs(uint32_t *value) override;
};