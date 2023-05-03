"use strict";
// automatically generated by the FlatBuffers compiler, do not modify
Object.defineProperty(exports, "__esModule", { value: true });
exports.tIoConfig = void 0;
var flatbuffers = require("flatbuffers");
var t_config_wrapper_js_1 = require("../../sensact/comm/t-config-wrapper.js");
var tIoConfig = /** @class */ (function () {
    function tIoConfig() {
        this.bb = null;
        this.bb_pos = 0;
    }
    tIoConfig.prototype.__init = function (i, bb) {
        this.bb_pos = i;
        this.bb = bb;
        return this;
    };
    tIoConfig.getRootAstIoConfig = function (bb, obj) {
        return (obj || new tIoConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tIoConfig.getSizePrefixedRootAstIoConfig = function (bb, obj) {
        bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
        return (obj || new tIoConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    };
    tIoConfig.bufferHasIdentifier = function (bb) {
        return bb.__has_identifier('SCFG');
    };
    tIoConfig.prototype.timestamp = function () {
        var offset = this.bb.__offset(this.bb_pos, 4);
        return offset ? this.bb.readUint32(this.bb_pos + offset) : 0;
    };
    tIoConfig.prototype.configs = function (index, obj) {
        var offset = this.bb.__offset(this.bb_pos, 6);
        return offset ? (obj || new t_config_wrapper_js_1.tConfigWrapper()).__init(this.bb.__indirect(this.bb.__vector(this.bb_pos + offset) + index * 4), this.bb) : null;
    };
    tIoConfig.prototype.configsLength = function () {
        var offset = this.bb.__offset(this.bb_pos, 6);
        return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
    };
    tIoConfig.starttIoConfig = function (builder) {
        builder.startObject(2);
    };
    tIoConfig.addTimestamp = function (builder, timestamp) {
        builder.addFieldInt32(0, timestamp, 0);
    };
    tIoConfig.addConfigs = function (builder, configsOffset) {
        builder.addFieldOffset(1, configsOffset, 0);
    };
    tIoConfig.createConfigsVector = function (builder, data) {
        builder.startVector(4, data.length, 4);
        for (var i = data.length - 1; i >= 0; i--) {
            builder.addOffset(data[i]);
        }
        return builder.endVector();
    };
    tIoConfig.startConfigsVector = function (builder, numElems) {
        builder.startVector(4, numElems, 4);
    };
    tIoConfig.endtIoConfig = function (builder) {
        var offset = builder.endObject();
        return offset;
    };
    tIoConfig.finishtIoConfigBuffer = function (builder, offset) {
        builder.finish(offset, 'SCFG');
    };
    tIoConfig.finishSizePrefixedtIoConfigBuffer = function (builder, offset) {
        builder.finish(offset, 'SCFG', true);
    };
    tIoConfig.createtIoConfig = function (builder, timestamp, configsOffset) {
        tIoConfig.starttIoConfig(builder);
        tIoConfig.addTimestamp(builder, timestamp);
        tIoConfig.addConfigs(builder, configsOffset);
        return tIoConfig.endtIoConfig(builder);
    };
    return tIoConfig;
}());
exports.tIoConfig = tIoConfig;