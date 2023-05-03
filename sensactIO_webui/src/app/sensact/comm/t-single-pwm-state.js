"use strict";
// automatically generated by the FlatBuffers compiler, do not modify
Object.defineProperty(exports, "__esModule", { value: true });
exports.tSinglePwmState = void 0;
var flatbuffers = require("flatbuffers");
var tSinglePwmState = /** @class */ (function () {
    function tSinglePwmState() {
        this.bb = null;
        this.bb_pos = 0;
    }
    tSinglePwmState.prototype.__init = function (i, bb) {
        this.bb_pos = i;
        this.bb = bb;
        return this;
    };
    tSinglePwmState.getRootAstSinglePwmState = function (bb, obj) {
        return (obj || new tSinglePwmState()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tSinglePwmState.getSizePrefixedRootAstSinglePwmState = function (bb, obj) {
        bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
        return (obj || new tSinglePwmState()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tSinglePwmState.prototype.intensity01 = function () {
        var offset = this.bb.__offset(this.bb_pos, 4);
        return offset ? this.bb.readFloat32(this.bb_pos + offset) : 0.0;
    };
    tSinglePwmState.prototype.on = function () {
        var offset = this.bb.__offset(this.bb_pos, 6);
        return offset ? !!this.bb.readInt8(this.bb_pos + offset) : false;
    };
    tSinglePwmState.starttSinglePwmState = function (builder) {
        builder.startObject(2);
    };
    tSinglePwmState.addIntensity01 = function (builder, intensity01) {
        builder.addFieldFloat32(0, intensity01, 0.0);
    };
    tSinglePwmState.addOn = function (builder, on) {
        builder.addFieldInt8(1, +on, +false);
    };
    tSinglePwmState.endtSinglePwmState = function (builder) {
        var offset = builder.endObject();
        return offset;
    };
    tSinglePwmState.createtSinglePwmState = function (builder, intensity01, on) {
        tSinglePwmState.starttSinglePwmState(builder);
        tSinglePwmState.addIntensity01(builder, intensity01);
        tSinglePwmState.addOn(builder, on);
        return tSinglePwmState.endtSinglePwmState(builder);
    };
    return tSinglePwmState;
}());
exports.tSinglePwmState = tSinglePwmState;
