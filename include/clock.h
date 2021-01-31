#pragma once

#include "RemoteDebugger.h"
#include "app.h"
#include "display.h"
#include "control.h"

extern QueueHandle_t xClockConfigQueue;

namespace qlocktoo {
typedef struct {
    NeoGrbwFeature::ColorObject colorItIs;
    NeoGrbwFeature::ColorObject colorWords;
    NeoGrbwFeature::ColorObject colorHour;
} ClockConfig;

class Clock : public App {
   private:
    const static uint8_t MIN_BRIGHTNESS = 15;
    const static uint8_t MAX_BRIGHTNESS = 140;
    const static uint8_t dayHour = 8;     // Start increasing brightness
    const static uint8_t nightHour = 17;  // Start decreasing brightness

    ClockConfig config;
    RemoteDebug &Debug;
    struct tm currentTime;
    uint8_t timeBrightness();
    void setColor(const std::vector<int> leds, NeoGrbwFeature::ColorObject color);
    void handleConfigQueue();
    const std::vector<std::vector<int>> ledsbyword = {
        {0, 37, 38, 36, 39},        // HET IS
        {17, 19, 54},               // een
        {58, 89, 95, 57},           // twee
        {91, 93, 55, 92},           // drie
        {96, 88, 59, 97},           // vier
        {87, 60, 98, 86},           // vijf
        {61, 99, 85},               // zes
        {102, 65, 82, 101, 64},     // zeven
        {80, 66, 103, 79},          // acht
        {100, 63, 84, 107, 62},     // negen
        {67, 109, 78, 68},          // tien
        {108, 77, 69},              // elf
        {73, 81, 72, 74, 104, 71},  // twaalf
        {35, 40, 3, 34},            // VIJF
        {30, 43, 6, 31},            // TIEN
        {9, 46, 27, 10, 47},        // KWART
        {41, 4, 33, 26},            // VOOR1
        {7, 44, 29, 8},             // OVER1
        {22, 14, 50, 23},           // HALF
        {70, 76, 106},              // UUR
        {51, 15, 21, 52},           // VOOR2
        {12, 48, 25, 11}            // OVER2
    };

   public:
    Clock(RemoteDebug &debug) : App(Mode::CLOCK), Debug(debug) {};
    void setup();
    void loop();
    void applyConfig(ClockConfig config);
    //~Clock();
};
}