import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { ClockConfig } from './clock.config';
import { Observable, EMPTY } from 'rxjs';
import { map } from 'rxjs/operators';
import IroColorValue from '@jaames/iro';

@Injectable({
  providedIn: 'root'
})
export class ClockService {
  private busy = false;
  private serializer : ClockConfigSerializer;

  constructor(private http: HttpClient) {
    this.serializer  = new ClockConfigSerializer();
  }

  public getConfig(): Observable<ClockConfig> {
    console.info('get config from Qlock');
    if (this.busy) {
      return EMPTY;
    }
    this.busy = true;

    return this.http.get<ClockConfig>('/api/clock', {}).pipe(
      map((data: any) => {
        console.log('config from Qlock received');
        this.busy = false;
        return this.serializer.fromJson(data);
      }));
  }

  public saveConfig(config: ClockConfig) {
    console.info('send config to Qlock');
    if (this.busy) {
      return;
    }
    this.busy = true;

    this.http.post('/api/clock', this.serializer.toJson(config)).subscribe(data => {
      this.busy = false;
      console.log(data);
    },
    error => {
      this.busy = false;
      console.log(error);
    });
  }

  public getDefault(): ClockConfig {
    return {
      colorItIs: new IroColorValue.Color('#000'),
      colorWords: new IroColorValue.Color('#000'),
      colorHour: new IroColorValue.Color('#000')
    };
  }
}

class ClockConfigSerializer {
  fromJson(json: any): ClockConfig {
    return {
      colorItIs: new IroColorValue.Color({
        h: json.colorItIs.h * 360,
        s: json.colorItIs.s * 100,
        v: json.colorItIs.v * 100,
      }),
      colorWords: new IroColorValue.Color({
        h: json.colorWords.h * 360,
        s: json.colorWords.s * 100,
        v: json.colorWords.v * 100,
      }),
      colorHour: new IroColorValue.Color({
        h: json.colorHour.h * 360,
        s: json.colorHour.s * 100,
        v: json.colorHour.v * 100,
      }),
    };
  }

  toJson(config: ClockConfig): any {
    return {
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
    }
  }
}
