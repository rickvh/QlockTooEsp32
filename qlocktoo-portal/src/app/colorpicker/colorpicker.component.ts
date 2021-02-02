import { Component, ElementRef, EventEmitter, Input, OnInit, Output } from '@angular/core';
import iro from '@jaames/iro';


@Component({
  selector: 'app-colorpicker',
  template: ''
})
export class ColorpickerComponent implements OnInit {
  private colorpicker: any;

  constructor(private el: ElementRef) {
      this.color = '';
  }
  @Input()
  color: string;

  @Output()
  colorChange = new EventEmitter();

  ngOnInit(){
    // this.jqElement = $(this.el.nativeElement);
    // this.jqElement.fullCalendar(this.options);
    console.log(this.color);
    this.colorpicker = iro.ColorPicker(this.el.nativeElement, { color: '#ffff00' })
  }

  // ngOnChanges(changes:{[k:string]:SimpleChange}){
    // if(changes['options']&&!changes['options'].firstChange) // do not apply options on first change (jquery plugin not applied)
      // this.jqElement.fullCalendar("option",this.options);
  // }

  ngOnDestroy(){
    // this.jqElement.fullCalendar("destroy");
  }

}
