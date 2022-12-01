import { NetworkService } from './network.service';
import { Component, OnInit } from '@angular/core';
import { UntypedFormControl, UntypedFormBuilder, UntypedFormGroup, Validators } from '@angular/forms';
import { NetworkConfig } from './network.config';


@Component({
  selector: 'app-network',
  templateUrl: './network.component.html',
  styleUrls: ['./network.component.scss']
})
export class NetworkComponent implements OnInit {
  config: NetworkConfig;
  public readonly form: UntypedFormGroup;

  onFormSubmit(): void {
    let config = this.form.getRawValue();
    this.networkService.saveConfig(config);
    this.form.markAsPristine();
  }

  constructor(private readonly formBuilder: UntypedFormBuilder, private networkService: NetworkService) {
    this.form = this.formBuilder.group({
      ssid: new UntypedFormControl('', [Validators.required]),
      password: new UntypedFormControl(),
      hostname: new UntypedFormControl('', [Validators.required])
    });
    this.config = {
      hostname: '',
      ssid: '',
      password: '',
      connectedToWifi: true
    };
  }

  ngOnInit(): void {
    this.networkService.getConfig().subscribe((config) => {
      this.config = config;
      this.form.patchValue(this.config);
    });
  }

  private submitToClock() {
    this.networkService.saveConfig(this.config);
  }
}
