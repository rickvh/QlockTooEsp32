import IroColorValue from '@jaames/iro';
import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';


@Component({
  selector: 'app-clock',
  templateUrl: './clock.component.html',
  styleUrls: ['./clock.component.scss']
})
export class ClockComponent implements OnInit {
  busy = false;
  mode: String = '';
  cssColor: string = '';

  itIsColor: IroColorValue.Color = new IroColorValue.Color('#ff00ff');
  wordsColor: IroColorValue.Color = new IroColorValue.Color('#9addaf');
  hourColor: IroColorValue.Color = new IroColorValue.Color('#00dddd');

  constructor(private http: HttpClient) { }

  ngOnInit(): void {
    this.submitToClock();
  }

  private submitToClock() {
    if (!this.busy) {
      this.busy = true;
      this.http.post('/api/clock', {
        colorItIs: this.itIsColor.hsv,
        colorWords: this.wordsColor.hsv,
        colorHour: this.hourColor.hsv
      }).subscribe(data => {
        this.busy = false;
        console.log(data);
      },
      error => {
        this.busy = false;
        console.log(error);
      });
    }
  }

  colorChanged(): void {
    this.submitToClock();
  }
}

// TODO: moet deze gebruikt worden ?!
export interface ClockConfig {
  itIs: {
    enabled: boolean,
    color: string | null
  }
}
