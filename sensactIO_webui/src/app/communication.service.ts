import { Injectable, OnDestroy, OnInit } from '@angular/core';
import { Observable, timer, Subscription, Subject } from 'rxjs';
import { HttpClient } from '@angular/common/http';
import { switchMap, map, tap, share, retry, takeUntil, filter } from 'rxjs/operators';
import { flatbuffers } from 'flatbuffers';
import * as C from './webui_core_comm_generated';

import M = C.sensact.comm;

const URL = "/iostate"

@Injectable({
  providedIn: 'root'
})
export class CommunicationService implements OnInit, OnDestroy{
  ngOnInit(): void {
    
  }
  ngOnDestroy(): void {
    this.stopPolling.next();
  }


  private ioState: Observable<M.tState>;
  private stopPolling = new Subject();
  private lastTimePUT:number=0;

  private static randomBlindState():M.eBlindState{
    let v=Math.random();
    return v<0.33?M.eBlindState.STOP:v<0.66?M.eBlindState.UP:M.eBlindState.DOWN;
  }

  private static randomOnOffState():M.eOnOffState{
    let v=Math.random();
    return v<0.50?M.eOnOffState.AUTO_OFF:M.eOnOffState.AUTO_ON;
  }

  public static CreateIoState():ArrayBuffer{
    let builder = new flatbuffers.Builder(1024);
    let cfg1=M.tBlindState.createtBlindState(builder, 8347, this.randomBlindState());
    let wCfg1 = M.tStateWrapper.createtStateWrapper(builder, 1, M.uState.tBlindState, cfg1);
    let cfg2=M.tSinglePwmState.createtSinglePwmState(builder, 50+10*Math.random(), Math.random()>0.5);
    let wCfg2 = M.tStateWrapper.createtStateWrapper(builder, 2, C.sensact.comm.uState.tSinglePwmState, cfg2);
    let cfg3=M.tOnOffState.createtOnOffState(builder, this.randomOnOffState());
    let wCfg3 = M.tStateWrapper.createtStateWrapper(builder, 3, M.uState.tOnOffState, cfg3);
    let cfg_vect = M.tState.createStatesVector(builder, [wCfg1, wCfg2, wCfg3]);
    let cfg= M.tState.createtState(builder, 0, cfg_vect);
    builder.finish(cfg);
    return builder.asUint8Array();
}

public putCommand(builder:any): Observable<M.tState> {
  this.lastTimePUT= Date.now()+100000;
  let arr:Uint8Array = builder.asUint8Array();
  let newArr= new Uint8Array(arr.byteLength);
  
  for(let i=0;i<arr.byteLength;i++){
    newArr[i]=arr[i];
  }

  let s:string="";
  let buffer = newArr.buffer;
  for(let i=0;i<newArr.byteLength;i++){
    s+=newArr[i].toString(16)+" ";
  }

  console.info(`About to send buffer with length ${buffer.byteLength} and content ${s}.`);
  return this.http.put(URL, newArr.buffer, {responseType: 'arraybuffer', headers: {"Content-Type": "application/octet-stream"},}).pipe(
    map((arraybuffer)=>{
      this.lastTimePUT=Date.now();
      let buf = new flatbuffers.ByteBuffer(new Uint8Array(arraybuffer));
      return M.tState.getRootAstState(buf);
    })
  );
}

public getIoState(): Observable<M.tState> {
  return this.ioState.pipe(   
    tap(() => console.log('data sent to subscriber'))
  );
}
  

  constructor(private http: HttpClient) {
    this.ioState = timer(1, 5000).pipe(
      filter(()=>{return Date.now()>this.lastTimePUT+1000}),
      switchMap(() => http.get(URL, {responseType: 'arraybuffer'})),
      //map(()=>{return CommunicationService.CreateIoState()}),
      tap((x)=>{console.log(`Received an ioState message with len ${x.byteLength}`)}),
      map((arraybuffer)=>{
        let buf = new flatbuffers.ByteBuffer(new Uint8Array(arraybuffer));
        return M.tState.getRootAstState(buf);
      }),
      retry(),
      share(),
      takeUntil(this.stopPolling)
   );
  }
}
