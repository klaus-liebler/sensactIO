// automatically generated by the FlatBuffers compiler, do not modify

import { tBlindConfig } from '../../sensact/comm/t-blind-config.js';
import { tOnOffConfig } from '../../sensact/comm/t-on-off-config.js';
import { tRgbwPwmConfig } from '../../sensact/comm/t-rgbw-pwm-config.js';
import { tSinglePwmConfig } from '../../sensact/comm/t-single-pwm-config.js';


export enum uConfig {
  NONE = 0,
  tBlindConfig = 1,
  tSinglePwmConfig = 2,
  tOnOffConfig = 3,
  tRgbwPwmConfig = 4
}

export function unionTouConfig(
  type: uConfig,
  accessor: (obj:tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig) => tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig|null
): tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig|null {
  switch(uConfig[type]) {
    case 'NONE': return null; 
    case 'tBlindConfig': return accessor(new tBlindConfig())! as tBlindConfig;
    case 'tSinglePwmConfig': return accessor(new tSinglePwmConfig())! as tSinglePwmConfig;
    case 'tOnOffConfig': return accessor(new tOnOffConfig())! as tOnOffConfig;
    case 'tRgbwPwmConfig': return accessor(new tRgbwPwmConfig())! as tRgbwPwmConfig;
    default: return null;
  }
}

export function unionListTouConfig(
  type: uConfig, 
  accessor: (index: number, obj:tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig) => tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig|null, 
  index: number
): tBlindConfig|tOnOffConfig|tRgbwPwmConfig|tSinglePwmConfig|null {
  switch(uConfig[type]) {
    case 'NONE': return null; 
    case 'tBlindConfig': return accessor(index, new tBlindConfig())! as tBlindConfig;
    case 'tSinglePwmConfig': return accessor(index, new tSinglePwmConfig())! as tSinglePwmConfig;
    case 'tOnOffConfig': return accessor(index, new tOnOffConfig())! as tOnOffConfig;
    case 'tRgbwPwmConfig': return accessor(index, new tRgbwPwmConfig())! as tRgbwPwmConfig;
    default: return null;
  }
}
