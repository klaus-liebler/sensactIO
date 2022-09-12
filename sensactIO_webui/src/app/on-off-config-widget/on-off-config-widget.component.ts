import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { Widget } from '../config-ui/config-ui.component';
import * as fb from 'flatbuffers';
import * as S from '../state_generated';
import * as CFG from '../config_generated';

@Component({
  selector: 'app-on-off-config-widget',
  templateUrl: './on-off-config-widget.component.html',
  styleUrls: ['./on-off-config-widget.component.scss']
})
export class OnOffConfigWidgetComponent implements OnInit {
  OnOffStates=S.eOnOffState;

  RelayInterlockModes=CFG.eRelayInterlockMode;
  _widgetData:Widget;

  public relay:number;
  public initialState:CFG.eOnOffState;
  public autoOffMsecs:number;
  
  
  @Input() set widgetData(w:Widget){
    this._widgetData=w;
    w.configWrapperCreator=this;
    if(w.rawData==null)return
    if(w.rawData.configType()!=CFG.uConfig.tOnOffConfig) return;
    this.relay=w.rawData.config(new CFG.tOnOffConfig()).relay();
    this.initialState=w.rawData.config(new CFG.tOnOffConfig()).initialState();
    this.autoOffMsecs=w.rawData.config(new CFG.tOnOffConfig()).autoOffMsecs();  
  }

  @Output() deleteItemEvent = new EventEmitter<number>();
  @Output() moveItemUpEvent = new EventEmitter<number>();
  @Output() moveItemDownEvent = new EventEmitter<number>();

  onBtnDeleteClicked=()=>this.deleteItemEvent.emit(this._widgetData.index);
  onBtnUpClicked=()=>this.moveItemUpEvent.emit(this._widgetData.index);
  onBtnDownClicked=()=>this.moveItemDownEvent.emit(this._widgetData.index);

  BuildAndReturnConfigWrapper(builder: fb.Builder):fb.Offset {
    let cfg3=CFG.tOnOffConfig.createtOnOffConfig(builder, this.relay, this.autoOffMsecs, this.initialState);
    return CFG.tConfigWrapper.createtConfigWrapper(builder, CFG.uConfig.tOnOffConfig, cfg3);
  }
  constructor() { }

  ngOnInit(): void {
  }

}
