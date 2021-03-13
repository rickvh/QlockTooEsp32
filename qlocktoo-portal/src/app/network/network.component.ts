import { NetworkService } from './network.service';
import { Component, OnInit } from '@angular/core';
import { FormControl, FormBuilder, FormGroup, Validators } from '@angular/forms';
import { NetworkConfig } from './network.config';


@Component({
  selector: 'app-network',
  templateUrl: './network.component.html',
  styleUrls: ['./network.component.scss']
})
export class NetworkComponent implements OnInit {
  config: NetworkConfig;
  public readonly form: FormGroup;

  onFormSubmit(): void {
    console.log('SSID:' + this.form.get('ssid')!!.value);
    console.log('PWD:' + this.form.get('password')!!.value);
    this.form.getRawValue();
    let config = this.form.getRawValue();
    this.networkService.saveConfig(config);
  }

  constructor(private readonly formBuilder: FormBuilder, private networkService: NetworkService) {
    this.form = this.formBuilder.group({
      ssid: new FormControl('', [Validators.required]),
      password: new FormControl('', [Validators.required]),
      hostname: new FormControl('', [Validators.required])
    });
    this.config = networkService.getConfig();
    this.form.patchValue(this.config);
  }

  ngOnInit(): void {
    this.submitToClock();
  }

  onColorChanged(): void {
    this.submitToClock();
  }

  private submitToClock() {
    this.networkService.saveConfig(this.config);
  }
}
