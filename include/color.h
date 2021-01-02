#pragma once

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

class RGBW {
    public:
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t w = 0;
        uint8_t brightness = 255;

        RGBW() {};
        RGBW(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), w(0) {};
        RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) : r(r), g(g), b(b), w(0), brightness(brightness) {};

        void setColor(uint8_t r, uint8_t g, uint8_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->w = 0;
        };

        // Adafruit compatible color
        uint32_t getColor() {
            return Adafruit_NeoPixel::Color(
                brightness*brightness*r/65025,
                brightness*brightness*g/65025,
                brightness*brightness*b/65025,
                brightness*brightness*w/65025);
        };
};