import { ApiService } from './../core/api-service.service';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Constants } from '../config/constants';
import { ClockConfig } from './clock.config';
import IroColorValue from '@jaames/iro';

@Injectable({
  providedIn: 'root'
})
export class ClockService extends ApiService {

  constructor(http: HttpClient, settings: Constants) {
    super(http, settings);
  }

  public getConfig(): ClockConfig {
    console.info('get config from Qlock');
    return {
      colorItIs: new IroColorValue.Color('#ff00ff'),
      colorWords: new IroColorValue.Color('#9addaf'),
      colorHour: new IroColorValue.Color('#00dddd')
    }

    // return this.get('clock', {}).;
  }

  public saveConfig(config: ClockConfig) {
    // console.info('send config to Qlock');
    this.post('clock', config);
  }
}
