import { Component, Input, OnInit } from '@angular/core';
import { flatbuffers } from 'flatbuffers'; // located in node_modules
import { CommunicationService } from '../communication.service';
import * as C from '../webui_core_comm_generated';
import M=C.sensact.comm;

@Component({
  selector: 'app-blind-command-widget',
  templateUrl: './blind-command-widget.component.html',
  styleUrls: ['./blind-command-widget.component.scss']
})
export class BlindCommandWidgetComponent implements OnInit {
  eBlindState=M.eBlindState;
  constructor(private comm:CommunicationService) { }
  public blState:M.eBlindState;

  @Input() public applicationId:number;
  @Input() set state(r:M.tState){
    let i=this.applicationId-1;
    if (r.states(i)?.stateType() != M.uState.tBlindState) {
      return;
    }
    this.blState = r.states(i)!.state(new M.tBlindState()).state();
  }

  private onCommand(c:M.eBlindCommand){
    let builder = new flatbuffers.Builder(1024);
    let cmd=M.tBlindCommand.createtBlindCommand(builder, c);
    let wcmd = M.tCommand.createtCommand(builder, this.applicationId, M.uCommand.tBlindCommand, cmd);
    builder.finish(wcmd);
    let arr:Uint8Array = builder.asUint8Array();
    this.comm.putCommand(arr).subscribe((state:M.tState)=>{this.state=state});
  }

  onBtnUpClicked(){
    console.log(`Send UP with applicationId ${this.applicationId}`);
    this.onCommand(M.eBlindCommand.UP);
  }

  onBtnDownClicked(){
    console.log(`Send DOWN with applicationId ${this.applicationId}`);
    this.onCommand(M.eBlindCommand.DOWN);
    
  }

  onBtnStopClicked(){
    console.log(`Send STOP with applicationId ${this.applicationId}`);
    this.onCommand(M.eBlindCommand.STOP);
  }
  
  

  ngOnInit(): void {
  }

}
