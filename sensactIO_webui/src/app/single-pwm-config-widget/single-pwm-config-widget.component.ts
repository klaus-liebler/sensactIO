import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { Widget } from '../config-ui/config-ui.component';
import * as fb from 'flatbuffers';
import * as CFG from '../config_generated';


@Component({
  selector: 'app-single-pwm-config-widget',
  templateUrl: './single-pwm-config-widget.component.html',
  styleUrls: ['./single-pwm-config-widget.component.scss']
})
export class SinglePwmConfigWidgetComponent implements OnInit {

  _widgetData:Widget;

  public pwmFirst:number;
  public pwmLast:number;
  public idOfStandbyController:number;
  public autoOffMsecs:number;
  
  
  @Input() set widgetData(w:Widget){
    this._widgetData=w;
    w.configWrapperCreator=this;
    if(w.rawData==null)return
    if(w.rawData.configType()!=CFG.uConfig.tSinglePwmConfig) return;
    this.pwmFirst=w.rawData.config(new CFG.tSinglePwmConfig()).pwmFirst();
    this.pwmLast=w.rawData.config(new CFG.tSinglePwmConfig()).pwmLast();
    this.idOfStandbyController=w.rawData.config(new CFG.tSinglePwmConfig()).idOfStandbyController();
    this.autoOffMsecs=w.rawData.config(new CFG.tSinglePwmConfig()).autoOffMsecs();  
  }

  @Output() deleteItemEvent = new EventEmitter<number>();
  @Output() moveItemUpEvent = new EventEmitter<number>();
  @Output() moveItemDownEvent = new EventEmitter<number>();

  onBtnDeleteClicked=()=>this.deleteItemEvent.emit(this._widgetData.index);
  onBtnUpClicked=()=>this.moveItemUpEvent.emit(this._widgetData.index);
  onBtnDownClicked=()=>this.moveItemDownEvent.emit(this._widgetData.index);

  BuildAndReturnConfigWrapper(builder: fb.Builder):fb.Offset {
    let cfg2=CFG.tSinglePwmConfig.createtSinglePwmConfig(builder, this.pwmFirst, this.pwmLast, this.idOfStandbyController, this.autoOffMsecs);
    return CFG.tConfigWrapper.createtConfigWrapper(builder, CFG.uConfig.tSinglePwmConfig, cfg2);
  }
  constructor() { }

  ngOnInit(): void {
  }


}
