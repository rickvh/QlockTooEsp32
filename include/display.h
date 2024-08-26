#pragma once

#include "Arduino.h"
#include <NeoPixelBrightnessBus.h>

namespace qlocktoo {
#ifdef BOARD_WS2811
    #define COLOR_FEATURE NeoGrbFeature
    #define NEOPIXEL_METHOD NeoWs2811Method
#else
    #define COLOR_FEATURE NeoGrbwFeature
    #define NEOPIXEL_METHOD NeoSk6812Method
#endif

class Display {
   private:
    const static uint8_t LEDSTRIP_PIN = 13;
    const static uint8_t MINUTE_1_PIN = 23;
    const static uint8_t MINUTE_2_PIN = 3;
    const static uint8_t MINUTE_3_PIN = 1;
    const static uint8_t MINUTE_4_PIN = 22;
    static bool initialized;
    static NeoPixelBus<COLOR_FEATURE, NEOPIXEL_METHOD> realDisplay;
    static void drawPixel(int16_t index, NeoGrbwFeature::ColorObject);

   public:
    const static uint8_t WIDTH = 11;
    const static uint8_t HEIGHT = 10;
    static void begin();
    static uint8_t setbrightness(uint8_t brightness) { return brightness; }; // TODO
    static uint8_t getbrightness() { return 255; }; // TODO
    static void clear() { realDisplay.ClearTo(HsbColor(0.0f, 0.0f, 0.0f)); };
    static void show() { realDisplay.Show(); };
    static void drawPixel(int16_t x, int16_t y, NeoGrbwFeature::ColorObject color);
    static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, NeoGrbwFeature::ColorObject color);
    /**
     * Depending on the hardware this will provide access to the leds indication individual minutes.
    */
    static void writeMinute1(boolean enabled);
    static void writeMinute2(boolean enabled);
    static void writeMinute3(boolean enabled);
    static void writeMinute4(boolean enabled);
};
}  // namespace qlocktoo