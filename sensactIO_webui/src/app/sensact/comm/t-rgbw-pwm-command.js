"use strict";
// automatically generated by the FlatBuffers compiler, do not modify
Object.defineProperty(exports, "__esModule", { value: true });
exports.tRgbwPwmCommand = void 0;
var flatbuffers = require("flatbuffers");
var e_rgbw_pwm_command_js_1 = require("../../sensact/comm/e-rgbw-pwm-command.js");
var tRgbwPwmCommand = /** @class */ (function () {
    function tRgbwPwmCommand() {
        this.bb = null;
        this.bb_pos = 0;
    }
    tRgbwPwmCommand.prototype.__init = function (i, bb) {
        this.bb_pos = i;
        this.bb = bb;
        return this;
    };
    tRgbwPwmCommand.getRootAstRgbwPwmCommand = function (bb, obj) {
        return (obj || new tRgbwPwmCommand()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tRgbwPwmCommand.getSizePrefixedRootAstRgbwPwmCommand = function (bb, obj) {
        bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
        return (obj || new tRgbwPwmCommand()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tRgbwPwmCommand.prototype.cmd = function () {
        var offset = this.bb.__offset(this.bb_pos, 4);
        return offset ? this.bb.readInt8(this.bb_pos + offset) : e_rgbw_pwm_command_js_1.eRgbwPwmCommand.TOGGLE;
    };
    tRgbwPwmCommand.prototype.payload = function () {
        var offset = this.bb.__offset(this.bb_pos, 6);
        return offset ? this.bb.readFloat32(this.bb_pos + offset) : 0.0;
    };
    tRgbwPwmCommand.starttRgbwPwmCommand = function (builder) {
        builder.startObject(2);
    };
    tRgbwPwmCommand.addCmd = function (builder, cmd) {
        builder.addFieldInt8(0, cmd, e_rgbw_pwm_command_js_1.eRgbwPwmCommand.TOGGLE);
    };
    tRgbwPwmCommand.addPayload = function (builder, payload) {
        builder.addFieldFloat32(1, payload, 0.0);
    };
    tRgbwPwmCommand.endtRgbwPwmCommand = function (builder) {
        var offset = builder.endObject();
        return offset;
    };
    tRgbwPwmCommand.createtRgbwPwmCommand = function (builder, cmd, payload) {
        tRgbwPwmCommand.starttRgbwPwmCommand(builder);
        tRgbwPwmCommand.addCmd(builder, cmd);
        tRgbwPwmCommand.addPayload(builder, payload);
        return tRgbwPwmCommand.endtRgbwPwmCommand(builder);
    };
    return tRgbwPwmCommand;
}());
exports.tRgbwPwmCommand = tRgbwPwmCommand;
