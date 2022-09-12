import * as fs from 'fs';
import * as path from 'path';
import * as fb from 'flatbuffers';
import * as C from '../../generated/flatbuffer_idl/config_generated';


let builder:any = new fb.Builder(1024);
let vect:number[]=[];
{
    let cfg=C.tBlindConfig.createtBlindConfig(builder, 1,2,C.eRelayInterlockMode.R1_DOWN__R2_UP, 120*1000, 120*1000);
    let wCfg = C.tConfigWrapper.createtConfigWrapper(builder, C.uConfig.tBlindConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=C.tBlindConfig.createtBlindConfig(builder, 3,4,C.eRelayInterlockMode.R1_DOWN__R2_UP, 120*1000, 120*1000);
    let wCfg = C.tConfigWrapper.createtConfigWrapper(builder, C.uConfig.tBlindConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=C.tSinglePwmConfig.createtSinglePwmConfig(builder, 12, 12, 4, 6*60*60*1000);
    let wCfg = C.tConfigWrapper.createtConfigWrapper(builder, C.uConfig.tSinglePwmConfig, cfg);
    vect.push(wCfg);
}
{
    let cfg=C.tOnOffConfig.createtOnOffConfig(builder, 5, 10000, C.eOnOffState.AUTO_OFF);
    let wCfg = C.tConfigWrapper.createtConfigWrapper(builder, C.uConfig.tOnOffConfig, cfg);
    vect.push(wCfg);
}
let cfg_vect = C.tIoConfig.createConfigsVector(builder, vect);
let cfg= C.tIoConfig.createtIoConfig(builder, 4714, cfg_vect);
builder.finish(cfg);
let bytes = builder.asUint8Array();
let full_path = process.argv[2];
fs.mkdirSync(path.dirname(full_path), { recursive: true });
fs.writeFileSync(full_path, bytes, "binary");

let bytesRead = fs.readFileSync(full_path);
let buf:any = new fb.ByteBuffer(bytesRead);

let cfgRead = C.tIoConfig.getRootAstIoConfig(buf);
console.log(`Timestamp is ${cfgRead.timestamp()} and configsLength is ${cfgRead.configsLength()}. Flatbuffers file written to ${full_path}`);