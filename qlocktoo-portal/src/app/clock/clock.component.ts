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
  color = {
    red: 20,
    green: 40,
    blue: 100
  }
  
  constructor(private http: HttpClient) { }

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
