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
    this.config = this.clockService.getDefault();
    clockService.getConfig().subscribe((config) => this.config = config);
  }

  ngOnInit(): void {
    this.submitToClock();
  }

  onColorChanged(): void {
    this.submitToClock();
  }

  private submitToClock() {
    if (this.config) {
      this.clockService.saveConfig(this.config);
    }
  }
}
