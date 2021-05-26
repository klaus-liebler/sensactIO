import { Component, Input, OnInit, Output, EventEmitter} from '@angular/core';
import { ConfigWrapperCreator, Widget } from '../config-ui/config-ui.component';
import * as C from '../webui_core_comm_generated';
import M = C.sensact.comm;




@Component({
  selector: 'app-blind-config-widget',
  templateUrl: './blind-config-widget.component.html',
  styleUrls: ['./blind-config-widget.component.scss'],
})
export class BlindConfigWidgetComponent implements OnInit, ConfigWrapperCreator {
  RelayInterlockModes=M.eRelayInterlockMode;
  _widgetData:Widget;

  public relay1:number;
  public relay2:number;
  public timeUpMsecs:number;
  public timeDownMsecs:number;
  public relayInterlockMode:M.eRelayInterlockMode;

  @Input() set widgetData(w:Widget){
    this._widgetData=w;
    w.configWrapperCreator=this;
    if(w.rawData==null)return
    if(w.rawData.configType()!=M.uConfig.tBlindConfig) return;
    this.relay1=w.rawData.config(new M.tBlindConfig()).relay1();
    this.relay2=w.rawData.config(new M.tBlindConfig()).relay2();
    this.timeUpMsecs=w.rawData.config(new M.tBlindConfig()).timeUpMsecs();
    this.timeDownMsecs=w.rawData.config(new M.tBlindConfig()).timeDownMsecs();
    this.relayInterlockMode=w.rawData.config(new M.tBlindConfig()).mode();
  }

  @Output() deleteItemEvent = new EventEmitter<number>();
  @Output() moveItemUpEvent = new EventEmitter<number>();
  @Output() moveItemDownEvent = new EventEmitter<number>();

  onBtnDeleteClicked=()=>this.deleteItemEvent.emit(this._widgetData.index);
  onBtnUpClicked=()=>this.moveItemUpEvent.emit(this._widgetData.index);
  onBtnDownClicked=()=>this.moveItemDownEvent.emit(this._widgetData.index);

  constructor() { }
  BuildAndReturnConfigWrapper(builder: flatbuffers.Builder):flatbuffers.Offset {
    let cfg1=M.tBlindConfig.createtBlindConfig(builder, this.relay1,this.relay2,this.relayInterlockMode, this.timeUpMsecs, this.timeDownMsecs);
    return M.tConfigWrapper.createtConfigWrapper(builder, M.uConfig.tBlindConfig, cfg1);
  }

  ngOnInit(): void {
  }

}
