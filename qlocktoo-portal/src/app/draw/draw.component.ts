import { RGBW } from './../image/image';
import { Component, OnInit } from '@angular/core';
import { ColorPickerControl, ColorsTable } from '@iplab/ngx-color-picker';
import { Image } from '../image/image';

@Component({
  selector: 'app-draw',
  templateUrl: './draw.component.html',
  styleUrls: ['./draw.component.scss'],
  inputs: ['image']
})
export class DrawComponent implements OnInit {
  color: string = "";
  image: Image;

  constructor() {
    this.image = new Image();
  }

  public colorPickerControl = new ColorPickerControl()
    .setValueFrom(ColorsTable.yellowgreen)
    .showPresets()
    .hideAlphaChannel();

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
