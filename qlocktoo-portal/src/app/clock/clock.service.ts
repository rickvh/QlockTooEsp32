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
    console.info('send config to Qlock');
    console.log(config.colorItIs.hsv);

    this.post('clock', {
      colorItIs: {
        h: config.colorItIs.hsv.h!! / 360,
        // s: config.colorItIs.hsv.s!! / 100,
        s: 1,
        v: config.colorItIs.hsv.v!! / 100,
      },
      colorWords: {
        h: config.colorWords.hsv.h!! / 360,
        // s: config.colorWords.hsv.s!! / 100,
        s: 1,
        v: config.colorWords.hsv.v!! / 100,
      },
      colorHour: {
        h: config.colorHour.hsv.h!! / 360,
        // s: config.colorHour.hsv.s!! / 100,
        s: 1,
        v: config.colorHour.hsv.v!! / 100,
      }
    });
  }
}
