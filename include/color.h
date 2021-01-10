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
        RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) : r(r), g(g), b(b), w(w) {};
        RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t brightness) : r(r), g(g), b(b), w(w), brightness(brightness) {};

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

        // Returns the Red component of a 32-bit color
    // uint8_t Red(uint32_t color)
    // {
    //     return (color >> 16) & 0xFF;
    // }
 
    // // Returns the Green component of a 32-bit color
    // uint8_t Green(uint32_t color)
    // {
    //     return (color >> 8) & 0xFF;
    // }
 
    // // Returns the Blue component of a 32-bit color
    // uint8_t Blue(uint32_t color)
    // {
    //     return color & 0xFF;
    // }

    // // Return color, dimmed by 75% (used by scanner)
    // uint32_t DimColor(uint32_t color)
    // {
    //     uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    //     return dimColor;
    // }
};