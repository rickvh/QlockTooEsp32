import { Component, ElementRef, EventEmitter, Input, OnInit, Output } from '@angular/core';
import iro from '@jaames/iro';
import IroColorValue from '@jaames/iro';


@Component({
  selector: 'app-colorpicker',
  template: ''
})
export class ColorpickerComponent implements OnInit {
  private colorpicker: any;

  constructor(private el: ElementRef) {
      this.color = new IroColorValue.Color('#ff0000');
  }
  @Input()
  color: IroColorValue.Color;

  @Output()
  colorChange = new EventEmitter();

  @Output()
  colorChanged = new EventEmitter();

  ngOnInit(){
    let $this= this;
    this.colorpicker = iro.ColorPicker(this.el.nativeElement, { color: this.color });
    this.colorpicker.on('input:change', function(color: IroColorValue.Color) {
      $this.colorChange.emit(color);
      $this.colorChanged.emit(color);
    });
  }
}
