import { Component, NgZone, OnDestroy, OnInit, Input, ViewChild, ElementRef, AfterViewInit } from '@angular/core';
import iro from '@jaames/iro';
import { IroColorPicker } from '@jaames/iro/dist/ColorPicker';
import { flatbuffers } from 'flatbuffers'; // located in node_modules
import { CommunicationService } from '../communication.service';
import * as C from '../webui_core_comm_generated';

import M=C.sensact.comm;

@Component({
  selector: 'app-single-pwmcommand-widget',
  templateUrl: './single-pwmcommand-widget.component.html',
  styleUrls: ['./single-pwmcommand-widget.component.scss']
})
export class SinglePWMCommandWidgetComponent implements OnInit, OnDestroy, AfterViewInit {

  public on:boolean=false;
  public intensity_0_100:number=0;
  private spotsPicker: IroColorPicker;
  private firstCallOfProcessIoCtrl=true;

  public selectedIntensity: number;

  @ViewChild('iro')
  public iroDiv: ElementRef;

  private _applicationId:number;

  @Input() set applicationId(value:number){
    this._applicationId=value;
    console.log(`In SinglePWMCommandWidgetComponent: set applD to ${value}.`);
  }

  @Input() set state(r:M.tState){
    let i=this._applicationId-1;
    if (r.states(i)?.stateType() != M.uState.tSinglePwmState) {
      return;
    }
    this.on = r.states(i)!.state(new M.tSinglePwmState())!.on();
    this.intensity_0_100 = r.states(i)!.state(new M.tSinglePwmState())!.intensity0100();
    //this.butSpotsOnOff.style.backgroundColor=on?"green":"grey";
    if(this.spotsPicker && this.firstCallOfProcessIoCtrl){
      this.spotsPicker.color.setChannel("hsv", "v", this.intensity_0_100);
      this.firstCallOfProcessIoCtrl=false;
    }
  }


  public ngOnInit():void {


  }



  public ngOnDestroy(): void {
    this.spotsPicker.off('color:change', (color, changes)=>this.ngZone.run(() => this.onColorChange(color, changes)));
    this.spotsPicker=null;
    //<p>{{intensity_0_100}}</p>
  }
  

  public onColorChange(color, changes) {
    this.selectedIntensity = color.hsv.v;
    console.log("Send CHANGE_INTENSITY "+this.selectedIntensity);
    let builder = new flatbuffers.Builder(1024);
    let cmd=M.tSinglePwmCommand.createtSinglePwmCommand(builder, M.eSinglePwmCommand.CHANGE_INTENSITY, this.selectedIntensity);
    let wcmd = M.tCommand.createtCommand(builder, this.applicationId, M.uCommand.tSinglePwmCommand, cmd);
    builder.finish(wcmd);
    this.comm.putCommand(builder).subscribe((state:M.tState)=>{this.state=state});
  }

  public onToggle(){
    console.log(`Send TOGGLE with applicationId ${this.applicationId}`);
    let builder = new flatbuffers.Builder(1024);
    let cmd=M.tSinglePwmCommand.createtSinglePwmCommand(builder, M.eSinglePwmCommand.TOGGLE, this.spotsPicker.color.hsv.v);
    let wcmd = M.tCommand.createtCommand(builder, this.applicationId, M.uCommand.tSinglePwmCommand, cmd);
    builder.finish(wcmd);
    this.comm.putCommand(builder).subscribe((state:M.tState)=>{this.state=state});
  }

  constructor(public ngZone: NgZone, private comm:CommunicationService) {}

  ngAfterViewInit(): void {
    let width = Math.min(400, 0.9 * this.iroDiv.nativeElement.offsetWidth);
    this.spotsPicker = iro.ColorPicker(this.iroDiv.nativeElement, {
      width: width,
      layoutDirection: 'vertical',
      color: "#000",
      layout: [
        {
          component: iro.ui.Slider,
          options: {
            sliderType: 'value',
          }
        },
      ]
    });
    this.spotsPicker.on('color:change', (color, changes)=>this.ngZone.run(() => this.onColorChange(color, changes)));
  }


}
