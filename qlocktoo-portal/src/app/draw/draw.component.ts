import { Component, OnInit } from '@angular/core';
import { ColorPickerControl, ColorsTable } from '@iplab/ngx-color-picker';
import { Image } from '../image/image';

@Component({
  selector: 'app-draw',
  templateUrl: './draw.component.html',
  styleUrls: ['./draw.component.scss']
})
export class DrawComponent implements OnInit {


  color: string = "";
  image: Image;

  constructor() {
    this.image = new Image();
  }

  public colorPickerControl = new ColorPickerControl()
    .setValueFrom(ColorsTable.aquamarine)
    .hidePresets()
    .hideAlphaChannel();



  ngOnInit(): void {
  }

}
