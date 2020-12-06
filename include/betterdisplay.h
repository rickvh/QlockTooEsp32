#pragma once

#define DISPLAY_WIDTH   11
#define DISPLAY_HEIGHT  10


#include <Adafruit_NeoPixel.h>

/**
 * Display driver, which essentially wraps the NeoPixel driver with a few addons:
 * - supports X,Y coördinates.
 * - handles displays with custom ledstrip wirings
 */
// extern unsigned int display_buffer[DISPLAY_WIDTH - 1][DISPLAY_HEIGHT - 1];

/**
 * Flush buffer to display (this actually updates the display)
 */
void flush();


class Display : public Adafruit_NeoPixel {
    private:
        // Mapping from x,y coordinates to the actual led-index
        uint8_t pixelmapping[DISPLAY_WIDTH][DISPLAY_HEIGHT];

    public:
        

        Display(const uint16_t gpio_pin, );

        void begin();


        /*!
            @brief   Convert separate red, green, blue and white values into a
                    single "packed" 32-bit WRGB color.
            @param   r  Red brightness, 0 to 255.
            @param   g  Green brightness, 0 to 255.
            @param   b  Blue brightness, 0 to 255.
            @param   w  White brightness, 0 to 255.
            @return  32-bit packed WRGB value, which can then be assigned to a
                    variable for later use or passed to the setPixelColor()
                    function. Packed WRGB format is predictable, regardless of
                    LED strand color order.
        */
        static uint32_t   Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
            return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
        }
        // static uint32_t   ColorHSV(uint16_t hue, uint8_t sat=255, uint8_t val=255);


        void setPixel(const uint8_t x, const uint8_t y, const uint32_t c);

        uint8_t getWidth() { return DISPLAY_WIDTH; };
        uint8_t getHeight() { return DISPLAY_HEIGHT; };

        void show();
};
