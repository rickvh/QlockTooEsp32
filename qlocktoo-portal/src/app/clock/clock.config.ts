import IroColorValue from '@jaames/iro';

export enum ClockMode {
  digital = "d",
  analog = "a"
}

export interface ClockConfig {
  colorItIs: IroColorValue.Color,
  colorWords: IroColorValue.Color,
  colorHour: IroColorValue.Color,
  clockMode?: ClockMode
  minutesEnabled?: boolean
}
