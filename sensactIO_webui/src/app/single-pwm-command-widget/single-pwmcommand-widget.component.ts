import { Component, NgZone, OnDestroy, OnInit, Input, ViewChild, ElementRef, AfterViewInit } from '@angular/core';
import iro from '@jaames/iro';
import { IroColorPicker } from '@jaames/iro/dist/ColorPicker';
import { CommunicationService } from '../communication.service';

import * as fb from 'flatbuffers';
import * as S from '../state_generated';
import * as C from '../command_generated';

@Component({
  selector: 'app-single-pwmcommand-widget',
  templateUrl: './single-pwm-command-widget.component.html',
  styleUrls: ['./single-pwm-command-widget.component.scss']
})
export class SinglePwmCommandWidgetComponent implements OnInit, OnDestroy, AfterViewInit {

  public on:boolean=false;
  public intensity_0_1:number=0;
  private spotsPicker: IroColorPicker;
  private firstCallOfProcessIoCtrl=true;

  @ViewChild('iro')
  public iroDiv: ElementRef;

  private _applicationId:number;

  @Input() set applicationId(value:number){
    this._applicationId=value;
  }

  @Input() set state(r:S.tState){
    let i=this._applicationId-1;
    if (r.states(i)?.stateType() != S.uState.tSinglePwmState) {
      return;
    }
    this.on = r.states(i)!.state(new S.tSinglePwmState())!.on();
    this.intensity_0_1 = r.states(i)!.state(new S.tSinglePwmState())!.intensity01();
    //this.butSpotsOnOff.style.backgroundColor=on?"green":"grey";
    console.log(JSON.stringify({on:this.on, intensity_0_100:this.intensity_0_1, firstCallOfProcessIoCtrl:this.firstCallOfProcessIoCtrl}));
    if(this.spotsPicker && this.firstCallOfProcessIoCtrl){
      this.spotsPicker.color.setChannel("hsv", "v", this.intensity_0_1*100);
      this.firstCallOfProcessIoCtrl=false;
    }
  }

  public ngOnInit():void {}

  public ngOnDestroy(): void {
    this.spotsPicker.off('color:change', (color, changes)=>this.ngZone.run(() => this.onColorChange(color, changes)));
    this.spotsPicker=null;
  }
  

  public onColorChange(color:iro.Color, changes) {
    this.intensity_0_1 = color.hsv.v?color.hsv.v/100:0;
    let builder = new fb.Builder(1024);
    let cmd=C.tSinglePwmCommand.createtSinglePwmCommand(builder, C.eSinglePwmCommand.CHANGE_INTENSITY, this.intensity_0_1);
    let wcmd = C.tCommand.createtCommand(builder, this._applicationId, C.uCommand.tSinglePwmCommand, cmd);
    builder.finish(wcmd);
    let arr:Uint8Array = builder.asUint8Array();
    this.comm.putCommand(arr).subscribe((state:S.tState)=>{this.state=state});
  }

  public onToggle(){
    console.log(`Send TOGGLE with applicationId ${this._applicationId}`);
    let builder = new fb.Builder(1024);
    let cmd=C.tSinglePwmCommand.createtSinglePwmCommand(builder, C.eSinglePwmCommand.TOGGLE, this.spotsPicker.color.hsv.v);
    let wcmd = C.tCommand.createtCommand(builder, this._applicationId, C.uCommand.tSinglePwmCommand, cmd);
    builder.finish(wcmd);
    let arr:Uint8Array = builder.asUint8Array();
    this.comm.putCommand(arr).subscribe((state:S.tState)=>{this.state=state});
  }

  constructor(public ngZone: NgZone, private comm:CommunicationService) {
  }

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
