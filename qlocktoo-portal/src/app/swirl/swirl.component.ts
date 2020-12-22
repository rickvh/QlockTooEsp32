import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-swirl',
  templateUrl: './swirl.component.html',
  styleUrls: ['./swirl.component.scss']
})
export class SwirlComponent implements OnInit {

  constructor(private http: HttpClient) { }

  ngOnInit(): void {
    this.http.post('/api/swirl', { }).subscribe(data => {
      console.log(data);
    })
  }

}
