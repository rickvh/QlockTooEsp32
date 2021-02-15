import { ClockService } from './clock.service';
import { Component, OnInit } from '@angular/core';
import { ClockConfig } from './clock.config';


@Component({
  selector: 'app-clock',
  templateUrl: './clock.component.html',
  styleUrls: ['./clock.component.scss']
})
export class ClockComponent implements OnInit {
  config: ClockConfig;

  constructor(private clockService: ClockService) {
    this.config = clockService.getConfig();
  }

  ngOnInit(): void {
    this.submitToClock();
  }

  onColorChanged(): void {
    this.submitToClock();
  }

  private submitToClock() {
    this.clockService.saveConfig(this.config);
  }
}
