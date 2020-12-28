#pragma once

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

typedef struct RGBW {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t brightness = 255;

    // RGBW() : r(0), g(0), b(0), w(0) {}
    RGBW() {}
    RGBW(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};
    RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) : r(r), g(g), b(b), brightness(brightness) {};

    // Adafruit compatible color
    uint32_t getColor() {
        return Adafruit_NeoPixel::Color(
            brightness*brightness*r/65025,
            brightness*brightness*g/65025,
            brightness*brightness*b/65025,
            brightness*brightness*w/65025);
    };
} RGBW;