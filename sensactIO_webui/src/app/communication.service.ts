import { Injectable, OnDestroy, OnInit } from '@angular/core';
import { Observable, timer, Subscription, Subject, of } from 'rxjs';
import { HttpClient } from '@angular/common/http';
import { switchMap, map, tap, share, retry, takeUntil, filter, catchError } from 'rxjs/operators';
import * as fb from 'flatbuffers';
import * as CFG from './config_generated';
import * as S from './state_generated';

import { environment } from 'src/environments/environment';

const IOSTATE_URL = "/iostate";
const IOCONFIG_URL = "/iocfg";

@Injectable({
  providedIn: 'root'
})
export class CommunicationService implements OnInit, OnDestroy{
  
  ngOnInit(): void {
    
  }
  ngOnDestroy(): void {
    this.stopPolling.next(true);
  }


  private ioState: Observable<S.tState>;
  private stopPolling = new Subject();
  private lastTimePUT:number=0;

  private static randomBlindState():S.eBlindState{
    let v=Math.random();
    return v<0.33?S.eBlindState.STOP:v<0.66?S.eBlindState.UP:S.eBlindState.DOWN;
  }

  private static randomOnOffState():S.eOnOffState{
    let v=Math.random();
    return v<0.50?S.eOnOffState.AUTO_OFF:S.eOnOffState.AUTO_ON;
  }

  public static CreateIoState():ArrayBuffer{
    let builder = new fb.Builder(1024);
    let cfg1=S.tBlindState.createtBlindState(builder, 8347, this.randomBlindState());
    let wCfg1 = S.tStateWrapper.createtStateWrapper(builder, 1, S.uState.tBlindState, cfg1);
    let cfg2=S.tSinglePwmState.createtSinglePwmState(builder, 50+10*Math.random(), Math.random()>0.5);
    let wCfg2 = S.tStateWrapper.createtStateWrapper(builder, 2, S.uState.tSinglePwmState, cfg2);
    let cfg3=S.tOnOffState.createtOnOffState(builder, this.randomOnOffState());
    let wCfg3 = S.tStateWrapper.createtStateWrapper(builder, 3, S.uState.tOnOffState, cfg3);
    let cfg_vect = S.tState.createStatesVector(builder, [wCfg1, wCfg2, wCfg3]);
    let cfg= S.tState.createtState(builder, 0, cfg_vect);
    builder.finish(cfg);
    return builder.asUint8Array();
}

public static CreateIoConfig():CFG.tIoConfig{
  let builder = new fb.Builder(1024);
  let cfg1=CFG.tBlindConfig.createtBlindConfig(builder, 2,3,CFG.eRelayInterlockMode.R1_DOWN__R2_UP, 10000, 5042);
  let wCfg1 = CFG.tConfigWrapper.createtConfigWrapper(builder, CFG.uConfig.tBlindConfig, cfg1);
  let cfg2=CFG.tSinglePwmConfig.createtSinglePwmConfig(builder, 1,3,0, 10000);
  let wCfg2 = CFG.tConfigWrapper.createtConfigWrapper(builder, CFG.uConfig.tSinglePwmConfig, cfg2);
  let cfg3=CFG.tOnOffConfig.createtOnOffConfig(builder, 3, 9900, CFG.eOnOffState.AUTO_OFF);
  let wCfg3 = CFG.tConfigWrapper.createtConfigWrapper(builder, CFG.uConfig.tOnOffConfig, cfg3);
  let cfg_vect = CFG.tIoConfig.createConfigsVector(builder, [wCfg1, wCfg2, wCfg3]);
  let cfg= CFG.tIoConfig.createtIoConfig(builder, 0, cfg_vect);
  builder.finish(cfg);
  let arr= builder.asUint8Array();
  let buf = new fb.ByteBuffer(new Uint8Array(arr));
      return CFG.tIoConfig.getRootAstIoConfig(buf);
}

public getIoConfig():Observable<CFG.tIoConfig>{
  if(!environment.production){
    return of(CommunicationService.CreateIoConfig());
  }
  return this.http.get(IOCONFIG_URL, {responseType: 'arraybuffer', }).pipe(
    map((arraybuffer)=>{
      this.lastTimePUT=Date.now();
      let buf = new fb.ByteBuffer(new Uint8Array(arraybuffer));
      return CFG.tIoConfig.getRootAstIoConfig(buf);
    })
  );
}

private handleError<T>(operation = 'operation', result?: T) {
  return (error: any): Observable<T> => {

    // TODO: send the error to remote logging infrastructure
    console.error(error); // log to console instead

    // TODO: better job of transforming error for user consumption
    //this.log(`${operation} failed: ${error.message}`);

    // Let the app keep running by returning an empty result.
    return of(result as T);
  };
}

public putIoConfig(arr: Uint8Array):Observable<{}> {
  let buffer = arr.buffer.slice(arr.byteOffset, arr.byteOffset+arr.byteLength)
  
  let arr1=new Uint8Array(buffer);
  let s=""
  if(!environment.production){
    for(let i=0;i<arr1.length;i++){
      s+=arr[i].toString(16)+", ";
    }
  }
  console.log(`arr.byteOffset: ${arr.byteOffset} arr.byteLenght: ${arr.byteLength} content: ${s}`);
  return this.http.put(IOCONFIG_URL, buffer, {headers: {"Content-Type": "application/octet-stream"}}).pipe(
    tap( () => console.log('tapped putIoConfig')),
    catchError(this.handleError<object>('putIoConfig', []))
  );
}

public putCommand(arr: Uint8Array): Observable<S.tState> {
  this.lastTimePUT= Date.now()+100000;
  console.log(`arr.byteOffset: ${arr.byteOffset} arr.byteLenght: ${arr.byteLength} lenght ${arr.length}`);
  let buffer = arr.buffer.slice(arr.byteOffset, arr.byteOffset+arr.byteLength)
  return this.http.put(IOSTATE_URL, buffer, {responseType: 'arraybuffer', headers: {"Content-Type": "application/octet-stream"},}).pipe(
    map((arraybuffer)=>{
      this.lastTimePUT=Date.now();
      let buf = new fb.ByteBuffer(new Uint8Array(arraybuffer));
      return S.tState.getRootAstState(buf);
    })
  );
}

public getIoState(): Observable<S.tState> {
  return this.ioState.pipe(   
    tap(() => console.log('data sent to subscriber'))
  );
}
  

  constructor(private http: HttpClient) {
    this.ioState = timer(1, 5000).pipe(
      filter(()=>{return Date.now()>this.lastTimePUT+1000}),
      environment.production?switchMap(() => http.get(IOSTATE_URL, {responseType: 'arraybuffer'})):map(()=>{return CommunicationService.CreateIoState()}),
      tap((x)=>{console.log(`Received an ioState message with len ${x.byteLength}`)}),
      map((arraybuffer)=>{
        let buf = new fb.ByteBuffer(new Uint8Array(arraybuffer));
        return S.tState.getRootAstState(buf);
      }),
      retry(),
      share(),
      takeUntil(this.stopPolling)
   );
  }
}
