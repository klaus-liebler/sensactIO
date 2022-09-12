import { Component, Input, OnInit } from '@angular/core';
import * as fb from 'flatbuffers'; // located in node_modules
import { CommunicationService } from '../communication.service';
import * as C from '../command_generated';
import * as S from '../state_generated';

@Component({
  selector: 'app-blind-command-widget',
  templateUrl: './blind-command-widget.component.html',
  styleUrls: ['./blind-command-widget.component.scss']
})
export class BlindCommandWidgetComponent implements OnInit {
  eBlindState=S.eBlindState;
  constructor(private comm:CommunicationService) { }
  public blState:S.eBlindState;

  @Input() public applicationId:number;
  @Input() set state(r:S.tState){
    let i=this.applicationId-1;
    if (r.states(i)?.stateType() != S.uState.tBlindState) {
      return;
    }
    this.blState = r.states(i)!.state(new S.tBlindState()).state();
  }

  private onCommand(c:C.eBlindCommand){
    let builder = new fb.Builder(1024);
    let cmd=C.tBlindCommand.createtBlindCommand(builder, c);
    let wcmd = C.tCommand.createtCommand(builder, this.applicationId, C.uCommand.tBlindCommand, cmd);
    builder.finish(wcmd);
    let arr:Uint8Array = builder.asUint8Array();
    this.comm.putCommand(arr).subscribe((state:S.tState)=>{this.state=state});
  }

  onBtnUpClicked(){
    console.log(`Send UP with applicationId ${this.applicationId}`);
    this.onCommand(C.eBlindCommand.UP);
  }

  onBtnDownClicked(){
    console.log(`Send DOWN with applicationId ${this.applicationId}`);
    this.onCommand(C.eBlindCommand.DOWN);
    
  }

  onBtnStopClicked(){
    console.log(`Send STOP with applicationId ${this.applicationId}`);
    this.onCommand(C.eBlindCommand.STOP);
  }
  
  

  ngOnInit(): void {
  }

}
