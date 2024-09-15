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
            
            return mapping[x][y];

#endif
#ifdef BOARD_WS2811
            if (y%2 == 0) {
                return (Display::HEIGHT - y) * Display::WIDTH - x - 1; 
            } else {
                return (Display::HEIGHT - 1 - y) * Display::WIDTH + x; 
            }
#endif
    }
    
    NeoPixelBus<COLOR_FEATURE, NEOPIXEL_METHOD> Display::realDisplay(LEDS_IN_LEDSTRIP, LEDSTRIP_PIN);
    bool Display::initialized = false;

    void Display::begin() {
        if (!initialized) {
            RgbColor initColor(2, 2, 0);
            realDisplay.Begin();
            realDisplay.ClearTo(initColor);
            realDisplay.Show();
            initialized = true;

#ifdef BOARD_FREESTYLE
            /**
             * 'Freestyle' hardware layout has non-addressable led's indicating 1 to 4 minutes after the displayed text.
             */
            pinMode(MINUTE_1_PIN, OUTPUT);
            pinMode(MINUTE_2_PIN, OUTPUT);
            pinMode(MINUTE_3_PIN, OUTPUT);
            pinMode(MINUTE_4_PIN, OUTPUT);
            digitalWrite(MINUTE_1_PIN, LOW);
            digitalWrite(MINUTE_2_PIN, LOW);
            digitalWrite(MINUTE_3_PIN, LOW);
            digitalWrite(MINUTE_4_PIN, LOW);
#endif            
        }
    }

    void Display::drawPixel(int16_t x, int16_t y, NeoGrbwFeature::ColorObject c) {
        drawPixel(getLedByCoordinate(x, y), c);
    };

    void Display::drawPixel(int16_t index, NeoGrbwFeature::ColorObject c) {
#ifdef BOARD_WS2811
        realDisplay.SetPixelColor(index, RgbColor(
            max(c.R, c.W) / 4,
            max(c.G, c.W) / 4,
            max(c.B, c.W) / 4
        ));
#else
        realDisplay.SetPixelColor(index, c);
#endif
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

    void Display::drawImage(Image &image) {
        for (uint8_t y = 0; y < Display::HEIGHT; y++) {
            for (uint8_t x = 0; x < Display::WIDTH; x++) {
                drawPixel(x, y, image.getColor({x, y}));
            }
        }
    }

    void Display::writeMinute1(boolean enabled) {
        #ifdef BOARD_FREESTYLE
        digitalWrite(MINUTE_1_PIN, enabled);
        #endif
        #ifdef BOARD_WS2811
        drawPixel(HEIGHT*WIDTH + 0, HsbColor(0.0f, 0.0f, enabled ? (float)getbrightness() / 255.0f : 0.0f));
        #endif
    }

    void Display::writeMinute2(boolean enabled) {
        #ifdef BOARD_FREESTYLE
        digitalWrite(MINUTE_2_PIN, enabled);
        #endif
        #ifdef BOARD_WS2811
        drawPixel(HEIGHT*WIDTH + 1, HsbColor(0.0f, 0.0f, enabled ? (float)getbrightness() / 255.0f : 0.0f));
        #endif
    }

    void Display::writeMinute3(boolean enabled) {
        #ifdef BOARD_FREESTYLE
        digitalWrite(MINUTE_3_PIN, enabled);
        #endif
        #ifdef BOARD_WS2811
        drawPixel(HEIGHT*WIDTH + 2, HsbColor(0.0f, 0.0f, enabled ? (float)getbrightness() / 255.0f : 0.0f));
        #endif
    }

    void Display::writeMinute4(boolean enabled) {
        #ifdef BOARD_FREESTYLE
        digitalWrite(MINUTE_4_PIN, enabled);
        #endif
        #ifdef BOARD_WS2811
        drawPixel(HEIGHT*WIDTH + 3, HsbColor(0.0f, 0.0f, enabled ? (float)getbrightness() / 255.0f : 0.0f));
        #endif
    }
}