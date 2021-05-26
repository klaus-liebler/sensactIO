import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { Widget } from '../config-ui/config-ui.component';
import * as C from '../webui_core_comm_generated';
import M = C.sensact.comm;

@Component({
  selector: 'app-on-off-config-widget',
  templateUrl: './on-off-config-widget.component.html',
  styleUrls: ['./on-off-config-widget.component.scss']
})
export class OnOffConfigWidgetComponent implements OnInit {
  OnOffStates=M.eOnOffState;

  RelayInterlockModes=M.eRelayInterlockMode;
  _widgetData:Widget;

  public relay:number;
  public initialState:M.eOnOffState;
  public autoOffMsecs:number;
  
  
  @Input() set widgetData(w:Widget){
    this._widgetData=w;
    w.configWrapperCreator=this;
    if(w.rawData==null)return
    if(w.rawData.configType()!=M.uConfig.tOnOffConfig) return;
    this.relay=w.rawData.config(new M.tOnOffConfig()).relay();
    this.initialState=w.rawData.config(new M.tOnOffConfig()).initialState();
    this.autoOffMsecs=w.rawData.config(new M.tOnOffConfig()).autoOffMsecs();  
  }

  @Output() deleteItemEvent = new EventEmitter<number>();
  @Output() moveItemUpEvent = new EventEmitter<number>();
  @Output() moveItemDownEvent = new EventEmitter<number>();

  onBtnDeleteClicked=()=>this.deleteItemEvent.emit(this._widgetData.index);
  onBtnUpClicked=()=>this.moveItemUpEvent.emit(this._widgetData.index);
  onBtnDownClicked=()=>this.moveItemDownEvent.emit(this._widgetData.index);

  BuildAndReturnConfigWrapper(builder: flatbuffers.Builder):flatbuffers.Offset {
    let cfg3=M.tOnOffConfig.createtOnOffConfig(builder, this.relay, this.autoOffMsecs, this.initialState);
    return M.tConfigWrapper.createtConfigWrapper(builder, M.uConfig.tOnOffConfig, cfg3);
  }
  constructor() { }

  ngOnInit(): void {
  }

}
