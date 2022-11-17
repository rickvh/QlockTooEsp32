#include "display.h"
#include <NeoPixelBrightnessBus.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif

namespace qlocktoo {
    static uint16_t getLedByCoordinate(uint16_t x, uint16_t y) {
        #ifdef BOARD_FREESTYLE
        static uint8_t mapping[11][10] = {
            {73, 80, 102, 96, 58, 51, 22, 7, 30, 0},
            {81, 66, 65, 88, 89, 15, 14, 44, 43, 37},
            {72, 103, 82, 59, 95, 21, 50, 29, 6, 38},
            {74, 79, 101, 97, 57, 52, 23, 8, 31, 1},
            {104, 67, 64, 87, 90, 16, 13, 45, 42, 36},
            {71, 109, 83, 60, 94, 20, 49, 28, 5, 39},
            {75, 78, 100, 98, 56, 53, 24, 9, 32, 2},
            {105, 68, 63, 86, 91, 17, 12, 46, 41, 35},
            {70, 108, 84, 61, 93, 19, 48, 27, 4, 40},
            {76, 77, 107, 99, 55, 54, 25, 10, 33, 3},
            {106, 69, 62, 85, 92, 18, 11, 47, 26, 34}};
            
            return mapping[x][9 - y];
        #endif
        #ifdef BOARD_WS2811
            return y * 10 + x + 1; 
        #endif
        
    }


    NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> Display::realDisplay(110, LEDSTRIP_PIN);
    bool Display::initialized = false;

    void Display::begin() {
        if (!initialized) {
            RgbColor dark(2, 2, 0);
            realDisplay.Begin();
            // realDisplay.SetBrightness(255);
            realDisplay.ClearTo(dark);
            realDisplay.Show();
            // realDisplay.setRemapFunction(getLedByCoordinate);
            // realDisplay.Begin begin(LEDSTRIP_PIN, WIDTH * HEIGHT);
            initialized = true;
        }
    }

    void Display::drawPixel(int16_t x, int16_t y, NeoGrbwFeature::ColorObject c) {
        drawPixel(getLedByCoordinate(x, y), c);
    };

    void Display::drawPixel(int16_t index, NeoGrbwFeature::ColorObject c) {
        realDisplay.SetPixelColor(index, c);
    }

    void Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, NeoGrbwFeature::ColorObject color) {
        int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            _swap_int16_t(x0, y0);
            _swap_int16_t(x1, y1);
        }

        if (x0 > x1) {
            _swap_int16_t(x0, x1);
            _swap_int16_t(y0, y1);
        }

        int16_t dx, dy;
        dx = x1 - x0;
        dy = abs(y1 - y0);

        int16_t err = dx / 2;
        int16_t ystep;

        if (y0 < y1) {
            ystep = 1;
        } else {
            ystep = -1;
        }

        for (; x0 <= x1; x0++) {
            if (steep) {
                drawPixel(y0, x0, color);
            } else {
                drawPixel(x0, y0, color);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
    }
}