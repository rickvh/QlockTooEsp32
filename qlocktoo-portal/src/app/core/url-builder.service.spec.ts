import { TestBed } from '@angular/core/testing';

import { UrlBuilder } from './url-builder.service';

describe('UrlBuilderService', () => {
  let service: UrlBuilder;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(UrlBuilder);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
