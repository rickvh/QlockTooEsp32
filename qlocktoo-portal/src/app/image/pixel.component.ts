import { Component, EventEmitter } from '@angular/core';
import { RGBW } from './image';

@Component({
  selector: 'app-pixel',
  templateUrl: './pixel.component.html',
  styleUrls: ['./pixel.component.scss'],
  inputs: ['color', 'x', 'y'],
  outputs: ['click']
})
export class PixelComponent {
  color: RGBW = new RGBW();
  x: number = -1;
  y: number = -1;

  click: EventEmitter<PixelComponent> = new EventEmitter<PixelComponent>();



  constructor() {
  }

  getCssColor() : any {
    let color = this.color;
    let styles = {
      'background-color': `rgb(${color.red}, ${color.green}, ${color.blue})`
    };
    return styles;
  }

  onClick() {
    console.log(`Pixel clicked: ${this.x}, ${this.y}`);
    this.click.emit(this);
  }
}
