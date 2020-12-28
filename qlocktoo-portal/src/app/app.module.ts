import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { MatSliderModule } from '@angular/material/slider';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ClockComponent } from './clock/clock.component';
import { SwirlComponent } from './swirl/swirl.component';
import { TestComponent } from './test/test.component';
import { ColorPickerModule } from '@iplab/ngx-color-picker';
import { NoopAnimationsModule } from '@angular/platform-browser/animations';

@NgModule({
  declarations: [
    AppComponent,
    ClockComponent,
    SwirlComponent,
    TestComponent
  ],
  imports: [
    BrowserModule,
    ColorPickerModule,
    NoopAnimationsModule,
    AppRoutingModule,
    HttpClientModule,
    MatSliderModule,
    NoopAnimationsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
