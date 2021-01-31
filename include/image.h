#pragma once

#include <pgmspace.h>

#include <array>
#include <string>

#include "app.h"
#include "display.h"

namespace qlocktoo {
const static uint8_t PROGMEM IMG_WIFI_FRAME_1[] = {
    14, 18, 24, 27, 30, 35, 38, 41, 47, 49, 51, 60, 71, 82, 93, 103, 104, 105};
const static uint8_t PROGMEM IMG_WIFI_FRAME_2[] = {
    27, 38, 49, 60, 71, 82, 93, 103, 104, 105, 12, 13, 23, 34, 45, 46, 19, 20, 31, 42, 52, 53};
const static uint8_t PROGMEM IMG_WIFI_FRAME_3[] = {
    27, 1, 9, 11, 21, 22, 27, 32, 33, 38, 43, 44, 49, 54, 56, 60, 64, 71, 82, 93, 103, 104, 105};

const static uint8_t PROGMEM IMG_XMAS_TREE_GREEN[] = {
    16, 26, 27, 28, 38, 47, 48, 49, 50, 51, 60, 68, 69, 70, 71, 72, 73, 74, 82, 89, 90, 91, 92, 93, 94, 95, 96, 97};
const static uint8_t PROGMEM IMG_SNOWMAN_WHITE[] = {
    26, 27, 28, 37, 38, 39, 59, 60, 61, 69, 70, 71, 72, 73, 80, 81, 82, 83, 84, 91, 92, 93, 94, 95, 103, 104, 105};
const static uint8_t PROGMEM IMG_SNOWMAN_RED[] = {
    47, 48, 49, 50, 57};
const static uint8_t PROGMEM IMG_SNOWMAN_BLUE[] = {
    5, 15, 16, 17};
const static uint8_t PROGMEM IMG_ERROR[] = {
    4, 5, 15, 16, 26, 27, 37, 38, 48, 49, 59, 60, 92, 93, 103, 104};

class Image : public App {
   private:
    const static uint8_t WIDTH = Display::WIDTH;
    const static uint8_t HEIGHT = Display::HEIGHT;
    std::array<NeoGrbwFeature::ColorObject, WIDTH * HEIGHT> pixels;

    // App implementation
    void setup();
    void loop();

   public:
    enum Preset {
        ERROR,
        WIFI1,
        WIFI2,
        WIFI3,
        XMAS_TREE,
        SNOWMAN
    };

    Image();
    // Image(RemoteDebug &debug);
    Image(Preset file);
    // Image(RemoteDebug &debug, Preset file);
    void readFile(std::string filename);
    NeoGrbwFeature::ColorObject getColor(uint8_t x, uint8_t y);
    uint32_t getRawColor(uint8_t x, uint8_t y);

    void show();
};
}