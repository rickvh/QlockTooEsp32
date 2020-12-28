import { Component, OnInit } from '@angular/core';
import { ColorPickerControl, ColorsTable } from '@iplab/ngx-color-picker';

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.scss']
})
export class TestComponent implements OnInit {

  constructor() { }

  color: string = "";

  public colorPickerControl = new ColorPickerControl()
    .setValueFrom(ColorsTable.aquamarine)
    .hidePresets()
    .hideAlphaChannel();



  ngOnInit(): void {
  }

}
