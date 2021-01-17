#pragma once

#include <Arduino.h>

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

        uint32_t getColor()
        {
            uint32_t color = 0;

            color |= ((uint32_t)g << 24);
            color |= ((uint32_t)r << 16);
            color |= ((uint32_t)b << 8);
            color |= (uint32_t)w;

            return color;
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