import { ApiService } from '../core/api-service.service';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Constants } from '../config/constants';
import { NetworkConfig } from './network.config';

@Injectable({
  providedIn: 'root'
})
export class NetworkService extends ApiService {

  constructor(http: HttpClient, settings: Constants) {
    super(http, settings);
  }

  public getConfig(): NetworkConfig {
    console.info('get config from Qlock');
    return {
      hostname: '',
      ssid: '',
      password: ''
    }

    // return this.get('clock', {}).;
  }

  public saveConfig(config: NetworkConfig) {
    console.info('send config to Qlock');
    console.log(config.hostname);
    console.log(config.ssid);

    this.post('network', config);
  }
}
