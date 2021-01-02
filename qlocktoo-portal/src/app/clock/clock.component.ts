import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { ColorPickerControl, ColorsTable } from '@iplab/ngx-color-picker';

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

  constructor(private http: HttpClient) { }

  public colorPickerControl = new ColorPickerControl()
    .setValueFrom(ColorsTable.yellowgreen)
    .showPresets()
    .hideAlphaChannel();

  ngOnInit(): void {
    this.http.post('/api/clock', { color: this.color }).subscribe(data => {
      console.log(data);
    })
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
