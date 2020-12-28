#pragma once

#include <Adafruit_NeoMatrix.h>
#include "RemoteDebugger.h"
#include "color.h"

typedef struct {
    RGBW colorItIs;
    RGBW colorWords;
    RGBW colorHour;
} ClockConfig;


class Clock {
    private:
        // current time
        Adafruit_NeoMatrix &display;
        ClockConfig config;
        RemoteDebug &Debug;
        struct tm currentTime;
        void showKlok(Adafruit_NeoPixel &display);
        int timeBrightness();
        uint8_t hour();
        uint8_t minute();

    public:
        Clock(Adafruit_NeoMatrix &display, RemoteDebug &debug) : display(display), Debug(debug) {};
        void begin();
        void update();
        void applyConfig(ClockConfig &config);
        //~Clock();
};