import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { Widget } from '../config-ui/config-ui.component';
import { flatbuffers } from 'flatbuffers';
import * as C from '../webui_core_comm_generated';
import M = C.sensact.comm;


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
    if(w.rawData.configType()!=M.uConfig.tSinglePwmConfig) return;
    this.pwmFirst=w.rawData.config(new M.tSinglePwmConfig()).pwmFirst();
    this.pwmLast=w.rawData.config(new M.tSinglePwmConfig()).pwmLast();
    this.idOfStandbyController=w.rawData.config(new M.tSinglePwmConfig()).idOfStandbyController();
    this.autoOffMsecs=w.rawData.config(new M.tSinglePwmConfig()).autoOffMsecs();  
  }

  @Output() deleteItemEvent = new EventEmitter<number>();
  @Output() moveItemUpEvent = new EventEmitter<number>();
  @Output() moveItemDownEvent = new EventEmitter<number>();

  onBtnDeleteClicked=()=>this.deleteItemEvent.emit(this._widgetData.index);
  onBtnUpClicked=()=>this.moveItemUpEvent.emit(this._widgetData.index);
  onBtnDownClicked=()=>this.moveItemDownEvent.emit(this._widgetData.index);

  BuildAndReturnConfigWrapper(builder: flatbuffers.Builder):flatbuffers.Offset {
    let cfg2=M.tSinglePwmConfig.createtSinglePwmConfig(builder, this.pwmFirst, this.pwmLast, this.idOfStandbyController, this.autoOffMsecs);
    return M.tConfigWrapper.createtConfigWrapper(builder, C.sensact.comm.uConfig.tSinglePwmConfig, cfg2);
  }
  constructor() { }

  ngOnInit(): void {
  }


}
