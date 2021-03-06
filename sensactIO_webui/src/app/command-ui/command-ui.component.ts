import { Component, NgZone, OnDestroy, OnInit } from '@angular/core';
import { Subscription } from 'rxjs';
import { CommunicationService } from '../communication.service';
import * as C from '../webui_core_comm_generated';
import M = C.sensact.comm;

interface Widget{
  applicationId:number;
  type:M.uState;
}

@Component({
  selector: 'app-command-ui',
  templateUrl: './command-ui.component.html',
  styleUrls: ['./command-ui.component.scss']
})
export class CommandUiComponent implements OnInit, OnDestroy {
  public uState=M.uState;
  private info:Subscription|null;
  public widgets:Array<Widget>=new Array<Widget>();
  public state:M.tState;
  
  constructor(private ngZone: NgZone, public ioState:CommunicationService) { }
  ngOnDestroy(): void {
    this.info?.unsubscribe();
  }

  ngOnInit(): void {
    this.info = this.ioState.getIoState().subscribe((r)=>{
      let len = r.statesLength();
      if(this.widgets.length!=len){
        this.widgets= new Array<Widget>(len);
      }
      for(let i=0;i<len;i++){
        if(this.widgets[i]==null 
            || this.widgets[i].applicationId!=r.states(i).applicationId()
            || this.widgets[i].type!=r.states(i).stateType()){
              this.widgets[i]={applicationId:r.states(i).applicationId(), type:r.states(i).stateType()};
              console.log("Created "+this.widgets[i].type);
            } 
      }
      this.state=r;
    });
  }

}
