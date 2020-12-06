#pragma once

#include "betterdisplay.h"

// /*
//  * Display heeft nodig:
//  * - breedte
//  * - hoogte
//  * - data-pin
//  * - type (RGBW)
//  * - mapping (x,y naar echte pixel)
//  * 
//  * 
//  * 
//  * 
//  * 
//  * 
//  */

Adafruit_NeoPixel m_ledstrip;

Display::Display(uint16_t gpio_pin) {
    //m_ledstrip = Adafruit_NeoPixel(DISPLAY_WIDTH * DISPLAY_HEIGHT, gpio_pin, NEO_GRBW + NEO_KHZ800);
    m_ledstrip = Adafruit_NeoPixel(110, gpio_pin, NEO_GRBW + NEO_KHZ800);
}

// Adafruit_NeoPixel* Display::ledstrip() {
    // return &m_ledstrip;
// }


void Display::begin() {
    m_ledstrip.begin();
    m_ledstrip.setBrightness(50);
    m_ledstrip.show(); // Initialize all pixels to 'off'
}


void Display::setPixel(const uint8_t x, const uint8_t y, const uint32_t c) {
    
}

void Display::show() {
    m_ledstrip.show();
}