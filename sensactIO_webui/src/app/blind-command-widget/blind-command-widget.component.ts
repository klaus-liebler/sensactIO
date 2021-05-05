import { Component, Input, OnInit } from '@angular/core';
import { flatbuffers } from 'flatbuffers'; // located in node_modules
import * as C from '../webui_core_comm_generated';

import M=C.sensact.comm;

@Component({
  selector: 'app-blind-command-widget',
  templateUrl: './blind-command-widget.component.html',
  styleUrls: ['./blind-command-widget.component.scss']
})
export class BlindCommandWidgetComponent implements OnInit {

  constructor() { }

  @Input() public applicationId:number;
  @Input() set state(val:M.tState){
    
  }

  ngOnInit(): void {
  }

}
