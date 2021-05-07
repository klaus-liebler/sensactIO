import { Component, NgZone, OnDestroy, OnInit } from '@angular/core';
import iro from '@jaames/iro';
import { Subscription } from 'rxjs';
import { CommunicationService } from './communication.service';
import { flatbuffers } from 'flatbuffers';
import * as C from './webui_core_comm_generated';
import M = C.sensact.comm;

interface Widget{
  applicationId:number;
  type:M.uState;
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit, OnDestroy {
  title = 'sensactIO WebApp';
  public uState=M.uState;

  private info:Subscription|null;
  
  public widgets:Array<Widget>=new Array<Widget>();

  public state:M.tState;

  ngOnInit() {

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

  ngOnDestroy(): void {

    this.info?.unsubscribe();
  }


  constructor(private ngZone: NgZone, public ioState:CommunicationService) {
   
  }
  
}

