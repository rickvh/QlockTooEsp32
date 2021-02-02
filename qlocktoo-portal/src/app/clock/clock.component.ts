import { HttpClient } from '@angular/common/http';
import { NgZone } from '@angular/core';
import { Component, OnInit } from '@angular/core';
// import { ColorPickerControl, ColorsTable } from '@iplab/ngx-color-picker';
import iro from '@jaames/iro';
// declare var iro: any;



@Component({
  selector: 'app-clock',
  templateUrl: './clock.component.html',
  styleUrls: ['./clock.component.scss']
})
export class ClockComponent implements OnInit {
  busy = false;
  mode: String = '';

  cssColor: string = '';


  color = {
    red: 20,
    green: 40,
    blue: 100
  }

  colorpicker: any;
  constructor(private http: HttpClient, private ngZone: NgZone) { }

  // public colorPickerControl = new ColorPickerControl()
    // .setValueFrom(ColorsTable.yellowgreen)
    // .showPresets()
    // .hideAlphaChannel();

  ngOnInit(): void {
    this.http.post('/api/clock', { color: this.color }).subscribe(data => {
      console.log(data);
    })


    this.colorpicker = iro.ColorPicker('#picker', {})
    // this.colorPicker = new iro.ColorPicker('#picker', {});
    // this.colorPicker.on('color:change', this.onColorChange);

    // this.colorPicker.on('color:change', (color: string, changes: any) =>  this.ngZone.run(() => this.onColorChange(color, changes)));
  }

  ngOnDestroy(): void {
    // TODO
  }

  onColorChange(color: string, changes: any) {
    // this.selectedColor = color.hexString;
    console.log(color);
  }

  colorChanged(): void {
    if (!this.busy) {
      this.busy = true;
      this.http.post('/api/clock', { color: this.color }).subscribe(data => {
        this.busy = false;
        console.log(data);
      },
      error => {
        this.busy = false;
        console.log(error);
      });
    }
  }
}

export interface ClockConfig {
  itIs: {
    enabled: boolean,
    color: string | null
  }
}
