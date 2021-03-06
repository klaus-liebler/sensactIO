import { AfterViewInit, Component, ElementRef, Input, OnInit, ViewChild } from '@angular/core';
import { flatbuffers } from 'flatbuffers'; // located in node_modules
import { CommunicationService } from '../communication.service';
import * as C from '../webui_core_comm_generated';

import M=C.sensact.comm;

@Component({
  selector: 'app-on-off-command-widget',
  templateUrl: './on-off-command-widget.component.html',
  styleUrls: ['./on-off-command-widget.component.scss']
})
export class OnOffCommandWidgetComponent implements OnInit, AfterViewInit {
  public selectorWidth=0;
  public selectorLeft=0;
  public selectorText="";
  public selectorBackgroundColor="black";
  public selectorForegroundColor="white";
  public selectorVisibility="hidden";

  @ViewChild('selector')
  public selector: ElementRef;

  @ViewChild('butOn')
  public butOn: ElementRef;
  @ViewChild('butAuto')
  public butAuto: ElementRef;
  @ViewChild('butOff')
  public butOff: ElementRef;

  private onOffstate: M.eOnOffState = M.eOnOffState.AUTO_OFF;

  constructor(private comm:CommunicationService) { }
  ngAfterViewInit(): void {
    //this.updateUI(M.eOnOffState.AUTO_OFF);
  }

  @Input() public applicationId:number;
  @Input() set state(r:M.tState){
    let i=this.applicationId-1;
    if (r.states(i)?.stateType() != M.uState.tOnOffState) {
      return;
    }
    let onOffstate = r.states(i)!.state(new M.tOnOffState())!.state()
    if(this.selector && onOffstate!=this.onOffstate ){
      this.updateUI(onOffstate);
    }
  }

  private onCommand(c:M.eOnOffCommand){
    let builder = new flatbuffers.Builder(1024);
    let cmd=M.tOnOffCommand.createtOnOffCommand(builder, c, 0);
    let wcmd = M.tCommand.createtCommand(builder, this.applicationId, M.uCommand.tOnOffCommand, cmd);
    builder.finish(wcmd);
    let arr:Uint8Array = builder.asUint8Array();
    this.comm.putCommand(arr).subscribe((state:M.tState)=>{this.state=state});
  }

  onBtnOnClicked(){
    console.log(`Send ON with applicationId ${this.applicationId}`);
    this.onCommand(M.eOnOffCommand.ON);
  }

  onBtnAutoClicked(){
    console.log(`Send AUTO with applicationId ${this.applicationId}`);
    this.onCommand(M.eOnOffCommand.AUTO);
    
  }

  onBtnOffClicked(){
    console.log(`Send OFF with applicationId ${this.applicationId}`);
    this.onCommand(M.eOnOffCommand.OFF);
  }

  private updateUI(newState: C.sensact.comm.eOnOffState) {
    this.onOffstate = newState;
    switch (this.onOffstate) {
      case M.eOnOffState.AUTO_OFF:
        this.selectorLeft = this.butOn.nativeElement.clientWidth;
        this.selectorWidth = this.butAuto.nativeElement.clientWidth;
        this.selectorText = "AUTO";
        this.selectorBackgroundColor = "black";
        this.selectorForegroundColor = "white";
        this.selectorVisibility = "visible";
        break;
      case M.eOnOffState.AUTO_ON:
        this.selectorLeft = this.butOn.nativeElement.clientWidth;
        this.selectorWidth = this.butAuto.nativeElement.clientWidth;
        this.selectorText = "AUTO";
        this.selectorBackgroundColor = "green";
        this.selectorForegroundColor = "white";
        this.selectorVisibility = "visible";
        break;
      case M.eOnOffState.MANUAL_ON:
        this.selectorLeft = 0;
        this.selectorWidth = this.butOn.nativeElement.clientWidth;
        this.selectorText = "ON";
        this.selectorBackgroundColor = "green";
        this.selectorForegroundColor = "black";
        this.selectorVisibility = "visible";
        break;
      case M.eOnOffState.MANUAL_OFF:
        this.selectorLeft = this.butOn.nativeElement.clientWidth + this.butAuto.nativeElement.clientWidth + 1;
        this.selectorWidth = this.butOff.nativeElement.clientWidth;
        this.selectorText = "OFF";
        this.selectorBackgroundColor = "black";
        this.selectorForegroundColor = "white";
        this.selectorVisibility = "visible";
        break;
    }
  }

  ngOnInit(): void {
  }

}
