import { UrlBuilder } from './url-builder.service';
import { Constants } from './../config/constants';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class ApiService {
  private busy = false;

  constructor(private http: HttpClient, private settings: Constants) { }

  private createUrl(
    action: string
  ): string {
    const urlBuilder: UrlBuilder = new UrlBuilder(
      this.settings.API_ENDPOINT,
      action
    );
    return urlBuilder.toString();
  }

  protected get(action: string, options?: any) {
    if (this.busy) {
      return;
    }

    this.busy = true;
    const url = this.createUrl(action);

    return this.http.get(url, options).subscribe(data => {
        this.busy = false;
        console.log(data);
      },
      error => {
        this.busy = false;
        console.log(error);
      });
  }

  protected post(action: string, body: any, options?: any) {
    if (this.busy) {
      return;
    }

    this.busy = true;
    const url = this.createUrl(action);

    return this.http.post(url, body, options).subscribe(data => {
        this.busy = false;
        console.log(data);
      },
      error => {
        this.busy = false;
        console.log(error);
      });
  }
}
