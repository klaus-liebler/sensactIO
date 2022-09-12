// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

import { eBlindState } from '../../sensact/comm/e-blind-state';


export class tBlindState {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):tBlindState {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAstBlindState(bb:flatbuffers.ByteBuffer, obj?:tBlindState):tBlindState {
  return (obj || new tBlindState()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAstBlindState(bb:flatbuffers.ByteBuffer, obj?:tBlindState):tBlindState {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new tBlindState()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

position():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint16(this.bb_pos + offset) : 0;
}

state():eBlindState {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : eBlindState.STOP;
}

static starttBlindState(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addPosition(builder:flatbuffers.Builder, position:number) {
  builder.addFieldInt16(0, position, 0);
}

static addState(builder:flatbuffers.Builder, state:eBlindState) {
  builder.addFieldInt8(1, state, eBlindState.STOP);
}

static endtBlindState(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createtBlindState(builder:flatbuffers.Builder, position:number, state:eBlindState):flatbuffers.Offset {
  tBlindState.starttBlindState(builder);
  tBlindState.addPosition(builder, position);
  tBlindState.addState(builder, state);
  return tBlindState.endtBlindState(builder);
}
}