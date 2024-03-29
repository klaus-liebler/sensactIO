"use strict";
// automatically generated by the FlatBuffers compiler, do not modify
Object.defineProperty(exports, "__esModule", { value: true });
exports.unionListTouConfig = exports.unionTouConfig = exports.uConfig = void 0;
var t_blind_config_js_1 = require("../../sensact/comm/t-blind-config.js");
var t_on_off_config_js_1 = require("../../sensact/comm/t-on-off-config.js");
var t_rgbw_pwm_config_js_1 = require("../../sensact/comm/t-rgbw-pwm-config.js");
var t_single_pwm_config_js_1 = require("../../sensact/comm/t-single-pwm-config.js");
var uConfig;
(function (uConfig) {
    uConfig[uConfig["NONE"] = 0] = "NONE";
    uConfig[uConfig["tBlindConfig"] = 1] = "tBlindConfig";
    uConfig[uConfig["tSinglePwmConfig"] = 2] = "tSinglePwmConfig";
    uConfig[uConfig["tOnOffConfig"] = 3] = "tOnOffConfig";
    uConfig[uConfig["tRgbwPwmConfig"] = 4] = "tRgbwPwmConfig";
})(uConfig = exports.uConfig || (exports.uConfig = {}));
function unionTouConfig(type, accessor) {
    switch (uConfig[type]) {
        case 'NONE': return null;
        case 'tBlindConfig': return accessor(new t_blind_config_js_1.tBlindConfig());
        case 'tSinglePwmConfig': return accessor(new t_single_pwm_config_js_1.tSinglePwmConfig());
        case 'tOnOffConfig': return accessor(new t_on_off_config_js_1.tOnOffConfig());
        case 'tRgbwPwmConfig': return accessor(new t_rgbw_pwm_config_js_1.tRgbwPwmConfig());
        default: return null;
    }
}
exports.unionTouConfig = unionTouConfig;
function unionListTouConfig(type, accessor, index) {
    switch (uConfig[type]) {
        case 'NONE': return null;
        case 'tBlindConfig': return accessor(index, new t_blind_config_js_1.tBlindConfig());
        case 'tSinglePwmConfig': return accessor(index, new t_single_pwm_config_js_1.tSinglePwmConfig());
        case 'tOnOffConfig': return accessor(index, new t_on_off_config_js_1.tOnOffConfig());
        case 'tRgbwPwmConfig': return accessor(index, new t_rgbw_pwm_config_js_1.tRgbwPwmConfig());
        default: return null;
    }
}
exports.unionListTouConfig = unionListTouConfig;
