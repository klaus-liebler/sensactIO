import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { SinglePWMCommandWidgetComponent } from './single-pwmcommand-widget/single-pwmcommand-widget.component';
import { BlindCommandWidgetComponent } from './blind-command-widget/blind-command-widget.component';
import { OnOffCommandWidgetComponent } from './on-off-command-widget/on-off-command-widget.component';

@NgModule({
  declarations: [
    AppComponent,
    SinglePWMCommandWidgetComponent,
    BlindCommandWidgetComponent,
    OnOffCommandWidgetComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpClientModule,
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
