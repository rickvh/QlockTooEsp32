import { Component, OnInit } from '@angular/core';
import { Image, RGBW } from './image';

@Component({
  selector: 'app-image',
  templateUrl: './image.component.html',
  styleUrls: ['./image.component.scss'],
  inputs: ['image'],
  outputs: ['image']
})
export class ImageComponent implements OnInit {
  image: Image = new Image();

  constructor() {
  }


  ngOnInit(): void {
  }

  getColor(x: number, y: number) : RGBW {
    return this.image.getColor(x, y);
  }

  onPixelClick(x: number, y: number) {
    console.log("Pixel clicked: ", x, ", ", y);
    this.image.setColor(x, y, {red: 255, green: 0, blue: 0, white: 0});
  }

  clear(): void {
    this.image = new Image();
  }


}

