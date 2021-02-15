import { Injectable } from '@angular/core';

@Injectable()
export class Constants {
    public readonly API_ENDPOINT: string = 'http://192.168.0.108/api';
    public readonly API_MOCK_ENDPOINT: string = 'mock-domain/api';
}
