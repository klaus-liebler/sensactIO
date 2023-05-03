// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

import { tConfigWrapper } from '../../sensact/comm/t-config-wrapper.js';


export class tIoConfig {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):tIoConfig {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAstIoConfig(bb:flatbuffers.ByteBuffer, obj?:tIoConfig):tIoConfig {
  return (obj || new tIoConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAstIoConfig(bb:flatbuffers.ByteBuffer, obj?:tIoConfig):tIoConfig {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new tIoConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('SCFG');
}

timestamp():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint32(this.bb_pos + offset) : 0;
}

configs(index: number, obj?:tConfigWrapper):tConfigWrapper|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new tConfigWrapper()).__init(this.bb!.__indirect(this.bb!.__vector(this.bb_pos + offset) + index * 4), this.bb!) : null;
}

configsLength():number {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__vector_len(this.bb_pos + offset) : 0;
}

static starttIoConfig(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addTimestamp(builder:flatbuffers.Builder, timestamp:number) {
  builder.addFieldInt32(0, timestamp, 0);
}

static addConfigs(builder:flatbuffers.Builder, configsOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, configsOffset, 0);
}

static createConfigsVector(builder:flatbuffers.Builder, data:flatbuffers.Offset[]):flatbuffers.Offset {
  builder.startVector(4, data.length, 4);
  for (let i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]!);
  }
  return builder.endVector();
}

static startConfigsVector(builder:flatbuffers.Builder, numElems:number) {
  builder.startVector(4, numElems, 4);
}

static endtIoConfig(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishtIoConfigBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'SCFG');
}

static finishSizePrefixedtIoConfigBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'SCFG', true);
}

static createtIoConfig(builder:flatbuffers.Builder, timestamp:number, configsOffset:flatbuffers.Offset):flatbuffers.Offset {
  tIoConfig.starttIoConfig(builder);
  tIoConfig.addTimestamp(builder, timestamp);
  tIoConfig.addConfigs(builder, configsOffset);
  return tIoConfig.endtIoConfig(builder);
}
}
