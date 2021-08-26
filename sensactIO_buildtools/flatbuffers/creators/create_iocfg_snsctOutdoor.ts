import * as fs from 'fs';
import * as path from 'path';
import { flatbuffers } from 'flatbuffers'; // located in node_modules
import * as C from '../../generated/flatbuffer_idl/webui_core_comm_generated';
import M=C.sensact.comm;

function create(b:flatbuffers.Builder):void{
    
}

let builder = new flatbuffers.Builder(1024);
let vect=[];
{
    let cfg=M.tBlindConfig.createtBlindConfig(builder, 1,2,M.eRelayInterlockMode.R1_DOWN__R2_UP, 120*1000, 120*1000);
    let wCfg = M.tConfigWrapper.createtConfigWrapper(builder, C.sensact.comm.uConfig.tBlindConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=M.tBlindConfig.createtBlindConfig(builder, 3,4,M.eRelayInterlockMode.R1_DOWN__R2_UP, 120*1000, 120*1000);
    let wCfg = M.tConfigWrapper.createtConfigWrapper(builder, C.sensact.comm.uConfig.tBlindConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=M.tSinglePwmConfig.createtSinglePwmConfig(builder, 12, 12, 4, 6*60*60*1000);
    let wCfg = M.tConfigWrapper.createtConfigWrapper(builder, C.sensact.comm.uConfig.tSinglePwmConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=M.tOnOffConfig.createtOnOffConfig(builder, 5, 10000, M.eOnOffState.AUTO_OFF);
    let wCfg = M.tConfigWrapper.createtConfigWrapper(builder, C.sensact.comm.uConfig.tOnOffConfig, cfg);
    vect.push(wCfg);
}
let cfg_vect = C.sensact.comm.tIoConfig.createConfigsVector(builder, vect);
let cfg= C.sensact.comm.tIoConfig.createtIoConfig(builder, 4714, cfg_vect);
builder.finish(cfg);
let bytes = builder.asUint8Array();
let full_path = process.argv[2];
fs.mkdirSync(path.dirname(full_path), { recursive: true });
fs.writeFileSync(full_path, bytes, "binary");

let bytesRead = fs.readFileSync(full_path);
let buf = new flatbuffers.ByteBuffer(bytesRead);

let cfgRead = C.sensact.comm.tIoConfig.getRootAstIoConfig(buf);
console.log(`Timestamp is ${cfgRead.timestamp()} and configsLength is ${cfgRead.configsLength()}. Flatbuffers file written to ${full_path}`);