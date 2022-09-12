// automatically generated by the FlatBuffers compiler, do not modify

import * as flatbuffers from 'flatbuffers';

import { eBlindCommand } from '../../sensact/comm/e-blind-command';


export class tBlindCommand {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):tBlindCommand {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAstBlindCommand(bb:flatbuffers.ByteBuffer, obj?:tBlindCommand):tBlindCommand {
  return (obj || new tBlindCommand()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAstBlindCommand(bb:flatbuffers.ByteBuffer, obj?:tBlindCommand):tBlindCommand {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new tBlindCommand()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

cmd():eBlindCommand {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : eBlindCommand.STOP;
}

static starttBlindCommand(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addCmd(builder:flatbuffers.Builder, cmd:eBlindCommand) {
  builder.addFieldInt8(0, cmd, eBlindCommand.STOP);
}

static endtBlindCommand(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createtBlindCommand(builder:flatbuffers.Builder, cmd:eBlindCommand):flatbuffers.Offset {
  tBlindCommand.starttBlindCommand(builder);
  tBlindCommand.addCmd(builder, cmd);
  return tBlindCommand.endtBlindCommand(builder);
}
}
