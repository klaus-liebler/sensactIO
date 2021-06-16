#include "esp_log.h"
#include "common.hh"
#include <vector>
#include "manager.hh"
#include "aBlind.hh"
#include "aSinglePwm.hh"
#include "aOnOff.hh"
#include "paths_and_files.hh"
#include "cJSON.h"
#include <vector>

using namespace sensact::comm;

#define TAG "manager"

Manager::Manager(HAL *hal, std::vector<IOSource *> ioSources) : hal(hal), ioSources(ioSources)
{
    ctx.io = this;
    ctx.postOffice = this;
    ctx.now = hal->GetMillis();
    this->outputU16Buffer.resize(hal->GetPinCnt()+1); //+1, weil die Pins mit id 1 beginnen
    this->configurationBuffer.resize(hal->GetPinCnt()+1);//+1, weil die Pins mit id 1 beginnen
    this->outputBufferChangedBits=0;
    this->handleCommandSemaphore = xSemaphoreCreateMutex();
    assert(this->handleCommandSemaphore!=NULL);
}
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
void Manager::ReceivedFromRx470c(uint32_t val){
    flatbuffers::FlatBufferBuilder builder(128);
    flatbuffers::Offset<tCommand> cmd;
    switch (val)
    {
    case 16579944:
    {
        auto xcmd = CreatetBlindCommand(builder, eBlindCommand_DOWN_OR_STOP);
	    cmd = CreatetCommand(builder, 1, uCommand::uCommand_tBlindCommand, xcmd.Union());
        break;
    }
    case 16579940:
    {
        auto xcmd = CreatetBlindCommand(builder, eBlindCommand_UP_OR_STOP);
	    cmd = CreatetCommand(builder, 1, uCommand::uCommand_tBlindCommand, xcmd.Union());
        break;
    }
    case 16579948:
    {
        auto xcmd = CreatetBlindCommand(builder, eBlindCommand_DOWN_OR_STOP);
	    cmd = CreatetCommand(builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
        break;
    }
    case 16579938:
    {
        auto xcmd = CreatetBlindCommand(builder, eBlindCommand_UP_OR_STOP);
	    cmd = CreatetCommand(builder, 2, uCommand::uCommand_tBlindCommand, xcmd.Union());
        break;
    }
    case 16579946:
    {
        auto xcmd = CreatetSinglePwmCommand(builder, eSinglePwmCommand_TOGGLE);
	    cmd = CreatetCommand(builder, 3, uCommand::uCommand_tSinglePwmCommand, xcmd.Union());
        break;
    }
    default:
        return;
    }
    builder.Finish(cmd);
    auto parsed_cmd = flatbuffers::GetRoot<tCommand>(builder.GetBufferPointer());
    this->HandleCommand(parsed_cmd);

}

ErrorCode Manager::ConfigureIO(uint16_t pinId, IOMode mode)
{
    if(pinId==0) return ErrorCode::OK;
    if(pinId>=this->configurationBuffer.size()) return ErrorCode::PIN_NOT_AVAILABLE;
    reconfigurationRequestOccured=true;
    this->configurationBuffer[pinId] = mode;
    return ErrorCode::OK;
}
ErrorCode Manager::SetU16Output(uint16_t pinId, uint16_t state)
{
    if(pinId==0) return ErrorCode::OK;
    if(pinId>=this->outputU16Buffer.size()) return ErrorCode::PIN_NOT_AVAILABLE;
    outputU16Buffer[pinId]=state;
    SetBitIdx(outputBufferChangedBits, pinId);
    return ErrorCode::OK;
}
ErrorCode Manager::GetBoolInputs(uint32_t *value)
{
    *value=inputsBuffer;
    return ErrorCode::OK;
}

ErrorCode Manager::HandleCommand(const sensact::comm::tCommand *cmd)
{
    uint32_t appId = cmd->applicationId();
    if (appId == 0) return ErrorCode::OK;
   
    uint16_t appIdx = appId - 1;
    if(appIdx>=apps.size()) {
        ESP_LOGE(TAG, "HandleCommand INVALID_APPLICATION_ID %d", appId);
        return ErrorCode::INVALID_APPLICATION_ID;
    }
    //ESP_LOGI(TAG, "HandleCommand for applicationId %d", appId);
    
    cApplication *app = apps.at(appIdx);
    if (!app)
    {
        ESP_LOGE(TAG, "HandleCommand APPLICATION is NULL %d", appId);
        return ErrorCode::INVALID_APPLICATION_ID;
    }
    ESP_LOGI(TAG, "HandleCommand for ApplicationId %d", appId);
    if( xSemaphoreTake( this->handleCommandSemaphore, ( TickType_t ) 10 ) == pdTRUE )
    {
        auto v = app->ProcessCommand(cmd);
        xSemaphoreGive( this->handleCommandSemaphore);
        return v;
    }
    return ErrorCode::SEMAPHORE_NOT_AVAILABLE;
    
}

ErrorCode Manager::FillBuilderWithStateForWebUI(flatbuffers::FlatBufferBuilder *builder)
{

    std::vector<flatbuffers::Offset<tStateWrapper>> status_vector;
    for (const auto &app : apps)
    {
        app->FillStatus(builder, &status_vector);
    }
    auto states = builder->CreateVector(status_vector);
    auto state_obj = CreatetState(*builder, hal->GetMillis(), states);
    builder->Finish(state_obj);
    return ErrorCode::OK;
}

//Read configuration file
ErrorCode Manager::Setup()
{
    ctx.now = this->hal->GetMillis();
    FILE *fd = NULL;
    struct stat file_stat;
    ESP_LOGI(TAG, "Trying to open %s", Paths::DEFAULTCFG_PATH);
    if (stat(Paths::DEFAULTCFG_PATH, &file_stat) == -1)
    {
        ESP_LOGI(TAG, "Default PLC file %s does not exist. Exiting..", Paths::DEFAULTCFG_PATH);
        return ErrorCode::NO_CONFIGURATION_FOUND;
    }
    fd = fopen(Paths::DEFAULTCFG_PATH, "r");
    if (!fd)
    {
        ESP_LOGE(TAG, "Failed to read existing file : %s", Paths::DEFAULTCFG_PATH);
        return ErrorCode::FILE_SYSTEM_ERROR;
    }
    uint8_t buf[file_stat.st_size];
    size_t size_read = fread(buf, 1, file_stat.st_size, fd);
    if (size_read != file_stat.st_size)
    {
        ESP_LOGE(TAG, "Unable to read file completely : %s", Paths::DEFAULTCFG_PATH);
        return ErrorCode::FILE_SYSTEM_ERROR;
    }
    ESP_LOGI(TAG, "Successfully read %s into buffer", Paths::DEFAULTCFG_PATH);
    auto cfg = flatbuffers::GetRoot<tIoConfig>(buf);
    auto cfg_vect = cfg->configs();
    auto cfg_vect_size = cfg_vect->size();
    auto timestamp = cfg->timestamp();
    ESP_LOGI(TAG, "Found %d app configurations and timestamp %d", cfg_vect_size, timestamp);

    this->apps.resize(cfg_vect_size);
    for (int i = 0; i < cfg_vect_size; i++)
    {
        int id = i + 1;
        auto x = cfg_vect->Get(i);
        switch (x->config_type())
        {
        case uConfig_tBlindConfig:
            apps[i] = cBlind::Build(id, x);
            break;
        case uConfig_tOnOffConfig:
            apps[i] = cOnOff::Build(id, x);
            break;
        case uConfig_tSinglePwmConfig:
            apps[i] = cSinglePWM::Build(id, x);
        default:
            continue;
        }
    }
    for (const auto &app : apps)
    {
        app->Setup(&this->ctx);
    }
    return ErrorCode::OK;
}

ErrorCode Manager::Loop()
{
    ctx.now = this->hal->GetMillis();
    outputBufferChangedBits=0;
    hal->BeforeLoop();
    //Jede IOSource und ganz zum Schluss die Webui bekommen zugang zu den hier gepufferten Inputs und Outputs
    //Jede schreibt ihren Wunsch hier rein. Je weiter hinten eine IOsource im Vktor ist, desto mehr Priorität hat sie
    //oberste Priorität haben die Befehle der webui, deshalb wird ganz zum schluss über die Apps der Webui gelooped

    hal->GetBoolInputs(&this->inputsBuffer);
    for (std::size_t i = 0; i < ioSources.size(); ++i)
    {
        ioSources[i]->Configure(this);
        ioSources[i]->SetInputs(this);
        ioSources[i]->SetOutputs(this);
    }
    for (const auto &app : apps)
    {
        app->Loop(&this->ctx);
    }
    if(this->outputBufferChangedBits){
        for (std::size_t i = 1; i < this->outputU16Buffer.size(); i++)
        {
            hal->SetU16Output(i, this->outputU16Buffer[i]);
        }
    }
    if(this->reconfigurationRequestOccured){
        this->reconfigurationRequestOccured=false;
        for (std::size_t i = 1; i < this->configurationBuffer.size(); i++)
        {
            hal->ConfigureIO(i, this->configurationBuffer[i]);
        }
    }

    hal->AfterLoop();
    return ErrorCode::OK;
}

ErrorCode Manager::PostCommand(const tCommand *cmd)
{
    if (cmd->applicationId() == 0)
    {
        return ErrorCode::OK;
    }
    cApplication *app = this->apps.at(cmd->applicationId() - 1);
    if (!app)
    {
        return ErrorCode::NONE_AVAILABLE;
    }
    return app->ProcessCommand(cmd);
}

#undef TAG