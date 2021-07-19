import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { NetworkConfig } from './network.config';
import { Observable, EMPTY } from 'rxjs';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class NetworkService {
  constructor(private http: HttpClient) {
  }

  public getConfig(): Observable<NetworkConfig> {
    console.info('get config from Qlock');

    return this.http.get<NetworkConfig>('/api/network', {}).pipe(
      map((data: NetworkConfig) => {
        console.log('networkconfig from Qlock received');
        return data;
      }));
  }

  public saveConfig(config: NetworkConfig) {
    console.info('send config to Qlock');
    console.log(config.hostname);
    console.log(config.ssid);

    this.http.post('/api/network', config).subscribe(data => {
      console.log(data);
    },
    error => {
      console.log(error);
    });
  }
}
