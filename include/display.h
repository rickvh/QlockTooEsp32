#pragma once

#include "Arduino.h"
#include <NeoPixelBrightnessBus.h>
#include "image.h"

namespace qlocktoo {
#ifdef BOARD_WS2811
    #define COLOR_FEATURE NeoGrbFeature
    #define NEOPIXEL_METHOD NeoWs2811Method
    #define LEDS_IN_LEDSTRIP 114
#endif
#ifdef BOARD_FREESTYLE
    #define COLOR_FEATURE NeoGrbwFeature
    #define NEOPIXEL_METHOD NeoSk6812Method
    #define LEDS_IN_LEDSTRIP 110
#endif

class Display {
   private:
    const static uint8_t LEDSTRIP_PIN = 13;
    #ifdef BOARD_FREESTYLE
    const static uint8_t MINUTE_1_PIN = 23;
    const static uint8_t MINUTE_2_PIN = 3;
    const static uint8_t MINUTE_3_PIN = 1;
    const static uint8_t MINUTE_4_PIN = 22;
    #endif
    static bool initialized;
    static NeoPixelBus<COLOR_FEATURE, NEOPIXEL_METHOD> realDisplay;
    static void drawPixel(int16_t index, NeoGrbwFeature::ColorObject);

   public:
    const static uint8_t WIDTH = 11;
    const static uint8_t HEIGHT = 10;
    static void begin();
    static uint8_t setbrightness(uint8_t brightness) { return brightness; }; // TODO
    static uint8_t getbrightness() { return 20; }; // TODO
    static void clear() { realDisplay.ClearTo(HsbColor(0.0f, 1.0f, 0.0f)); };
    static void show() { realDisplay.Show(); };
    static void drawPixel(int16_t x, int16_t y, NeoGrbwFeature::ColorObject color);
    static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, NeoGrbwFeature::ColorObject color);
    static void drawImage(Image &image);
    static void writeMinute1(boolean enabled);
    static void writeMinute2(boolean enabled);
    static void writeMinute3(boolean enabled);
    static void writeMinute4(boolean enabled);
};
}  // namespace qlocktoo