export class Image {
    public static WIDTH = 11;
    public static HEIGHT = 10;

    grid: RGBW[][];

    constructor() {
      this.grid = new Array(Image.WIDTH)
            .fill(false)
            .map(() => new Array(Image.HEIGHT)
            .fill({
                red: 100,
                green: 0,
                blue: 0,
                white: 0
            }));
      console.log(this.grid);
    }

    // getRgbValue(color: RGBW) : string {
    //   return `rgb(${color.red}, ${color.green}, ${color.blue})`;
    // }

    getColor(x: number, y: number): RGBW {
      if (x < 0 || x >= Image.WIDTH || y < 0 || y >= Image.HEIGHT) {
        console.error("x/y out of range", x, y);
        return new RGBW();
      }

      if (this.grid[x] == undefined) {
        debugger;
      }
      return this.grid[x][y];
    }

    setColor(x: number, y: number, color: RGBW) {
      if (x < 0 || x >= Image.WIDTH || y < 0 || y >= Image.HEIGHT) {
        console.error("x/y out of range ff", x, y);
        return;
      }

      this.grid[x][y] = color;
    }
}


export class RGBW {
    red: number;
    green: number;
    blue: number;
    white: number;


    constructor(red: number = 0, green: number = 0, blue: number = 0) {
      this.red = red;
      this.green = green;
      this.blue = blue;
      this.white = 0;
    }
  }
