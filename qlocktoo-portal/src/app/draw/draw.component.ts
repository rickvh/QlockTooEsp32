import IroColorValue from '@jaames/iro';
import { RGBW } from './../image/image';
import { Component, OnInit } from '@angular/core';
import { Image } from '../image/image';

@Component({
  selector: 'app-draw',
  templateUrl: './draw.component.html',
  styleUrls: ['./draw.component.scss'],
  inputs: ['image']
})
export class DrawComponent implements OnInit {
  color: IroColorValue.Color;
  image: Image;

  constructor() {
    this.image = new Image();
    this.color = new IroColorValue.Color('#00')
  }

  clear() {
    this.image = new Image();
  }

  ngOnInit(): void {
  }

  hexToColor(hex: string): RGBW {
    var result = /^rgba?\((\d+),\s*(\d+),\s*(\d+)(?:,\s*(\d+(?:\.\d+)?))?\)$/i.exec(hex);
    return result ? new RGBW(
        parseInt(result[1]),
        parseInt(result[2]),
        parseInt(result[3])) : new RGBW();
  }

}
