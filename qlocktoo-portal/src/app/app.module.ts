import { NetworkComponent } from './network/network.component';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatLegacySliderModule as MatSliderModule } from '@angular/material/legacy-slider';
import { MatLegacyButtonModule as MatButtonModule } from '@angular/material/legacy-button';
import { MatLegacyCheckboxModule as MatCheckboxModule } from '@angular/material/legacy-checkbox';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatLegacyFormFieldModule as MatFormFieldModule } from '@angular/material/legacy-form-field';
import { MatLegacyInputModule as MatInputModule } from '@angular/material/legacy-input';
import { HttpClientModule } from '@angular/common/http';
import { MatLegacySlideToggleModule as MatSlideToggleModule } from '@angular/material/legacy-slide-toggle';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ClockComponent } from './clock/clock.component';
import { SwirlComponent } from './swirl/swirl.component';
import { DrawComponent } from './draw/draw.component';
import { NoopAnimationsModule } from '@angular/platform-browser/animations';
import { ImageComponent } from './image/image.component';
import { ColorpickerComponent } from './colorpicker/colorpicker.component';

@NgModule({
  declarations: [
    AppComponent,
    ClockComponent,
    SwirlComponent,
    DrawComponent,
    NetworkComponent,
    ImageComponent,
    ColorpickerComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    MatInputModule,
    NoopAnimationsModule,
    AppRoutingModule,
    HttpClientModule,
    MatSliderModule,
    MatButtonModule,
    MatCheckboxModule,
    MatExpansionModule,
    MatSlideToggleModule,
    MatFormFieldModule,
    ReactiveFormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
