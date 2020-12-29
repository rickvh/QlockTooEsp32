import { Component, OnInit } from '@angular/core';
import { Image, RGBW } from './image';

@Component({
  selector: 'app-image',
  templateUrl: './image.component.html',
  styleUrls: ['./image.component.scss'],
  inputs: ['image', 'brushColor'],
  outputs: ['image']
})
export class ImageComponent implements OnInit {
  image: Image = new Image();
  brushColor: RGBW = new RGBW(0, 255, 0);

  constructor() {
  }


  ngOnInit(): void {
  }

  getColor(x: number, y: number) : RGBW {
    return this.image.getColor(x, y);
  }

  getCssColor(x: number, y: number) : any {
    let color = this.image.getColor(x, y);
    let styles = {
      'background-color': `rgb(${color.red}, ${color.green}, ${color.blue})`
    };
    return styles;
  }

  onPixelClick(x: number, y: number) {
    console.log("Pixel clicked: ", x, ", ", y);
    this.image.setColor(x, y, this.brushColor);
  }

  clear(): void {
    this.image = new Image();
  }
}
