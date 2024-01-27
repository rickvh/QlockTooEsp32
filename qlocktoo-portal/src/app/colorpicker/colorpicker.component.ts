import { Component, ElementRef, EventEmitter, Input, OnInit, Output } from '@angular/core';
import iro from '@jaames/iro';
import IroColorValue from '@jaames/iro';


@Component({
  selector: 'app-colorpicker',
  template: ''
})
export class ColorpickerComponent implements OnInit {
  private colorpicker: any;
  private _color = new IroColorValue.Color('#ff0000');

  @Input()
  set color(value: IroColorValue.Color) {
    this._color = value;
    if (this.colorpicker) {
      // clone color
      const hsv = this._color.hsv;
      this.colorpicker.color.hsv = { h: hsv.h, s: hsv.s, v: hsv.v };
    }
  }

  get color(): IroColorValue.Color {
    return this._color;
  }

  @Output()
  colorChange = new EventEmitter();

  @Output()
  colorChanged = new EventEmitter();

  constructor(private el: ElementRef) {
    this.color = new IroColorValue.Color('#000');
  }

  ngOnInit(){
    let $this= this;
    this.colorpicker = iro.ColorPicker(this.el.nativeElement, {
      color: this.color,
      wheelLightness: false
    });
    this.colorpicker.on('input:change', function(color: IroColorValue.Color) {
      $this.colorChange.emit(color);
      $this.colorChanged.emit(color);
    });
  }
}
