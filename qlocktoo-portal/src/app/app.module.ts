import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { MatSliderModule } from '@angular/material/slider';
import { MatButtonModule } from '@angular/material/button';
import { MatCheckbox, MatCheckboxModule } from '@angular/material/checkbox';
import { MatExpansionModule } from '@angular/material/expansion';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ClockComponent } from './clock/clock.component';
import { SwirlComponent } from './swirl/swirl.component';
import { DrawComponent } from './draw/draw.component';
import { ColorPickerModule } from '@iplab/ngx-color-picker';
import { NoopAnimationsModule } from '@angular/platform-browser/animations';
import { ImageComponent } from './image/image.component';
import { ColorpickerComponent } from './colorpicker/colorpicker.component';

@NgModule({
  declarations: [
    AppComponent,
    ClockComponent,
    SwirlComponent,
    DrawComponent,
    ImageComponent,
    ColorpickerComponent
  ],
  imports: [
    BrowserModule,
    ColorPickerModule,
    NoopAnimationsModule,
    AppRoutingModule,
    HttpClientModule,
    MatSliderModule,
    MatButtonModule,
    MatCheckboxModule,
    MatExpansionModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
