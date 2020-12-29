import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { MatSliderModule } from '@angular/material/slider';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ClockComponent } from './clock/clock.component';
import { SwirlComponent } from './swirl/swirl.component';
import { DrawComponent } from './draw/draw.component';
import { ColorPickerModule } from '@iplab/ngx-color-picker';
import { NoopAnimationsModule } from '@angular/platform-browser/animations';
import { ImageComponent } from './image/image.component';
import { PixelComponent } from './image/pixel.component';

@NgModule({
  declarations: [
    AppComponent,
    ClockComponent,
    SwirlComponent,
    DrawComponent,
    ImageComponent,
    PixelComponent
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
