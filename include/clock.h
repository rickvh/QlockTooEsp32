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
        const static uint8_t NUM_LEDS = 110;
        const static uint8_t MIN_BRIGHTNESS = 15;
        const static uint8_t MAX_BRIGHTNESS = 140;

        uint8_t lastBrightness  =  MIN_BRIGHTNESS;
        uint8_t dayHour         = 8; // Start increasing brightness
        uint8_t nightHour       = 17; // Start decreasing brightness
        uint8_t targetlevels[NUM_LEDS];
        uint8_t currentlevels[NUM_LEDS];

        // current time
        Adafruit_NeoMatrix display;
        ClockConfig config;
        RemoteDebug &Debug;
        struct tm currentTime;
        void showKlok();
        uint8_t timeBrightness();
        uint8_t hour();
        uint8_t minute();

        static uint16_t remapPixels(uint16_t x, uint16_t y) {
            static uint8_t mapping[11][10] = {
                {73,80,102,96,58,51,22,7,30,0},
                {81,66,65,88,89,15,14,44,43,37},
                {72,103,82,59,95,21,50,29,6,38},
                {74,79,101,97,57,52,23,8,31,1},
                {104,67,64,87,90,16,13,45,42,36},
                {71,109,83,60,94,20,49,28,5,39},
                {75,78,100,98,56,53,24,9,32,2},
                {105,68,63,86,91,17,12,46,41,35},
                {70,108,84,61,93,19,48,27,4,40},
                {76,77,107,99,55,54,25,10,33,3},
                {106,69,62,85,92,18,11,47,26,34}
            };

            // debugD("Getting mapping for: %u, %u", x, y);
            return mapping[x][9-y];
        };

        const std::vector<std::vector<int>> ledsbyword = {
            {0,37,38,36,39}, // HET IS
            {17,19,54},            // een
            {58,89,95,57},      // twee
            {91,93,55,92},      // drie
            {96,88,59,97},      // vier
            {87,60,98,86},         // vijf
            {61,99,85},         // zes
            {102,65,82,101,64},   // zeven
            {80,66,103,79},      // acht
            {100,63,84,107,62},   // negen
            {67,109,78,68},      // tien
            {108,77,69},         // elf
            {73,81,72,74,104,71},// twaalf
            {35,40,3,34},    // VIJF
            {30,43,6,31},    // TIEN
            {9,46,27,10,47}, // KWART
            {41,4,33,26},    // VOOR1
            {7,44,29,8},    // OVER1
            {22,14,50,23},    // HALF
            {70,76,106},      // UUR
            {51,15,21,52},          // VOOR2
            {12,48,25,11}          // OVER2
        };

    public:
        Clock(RemoteDebug &debug) : display(Adafruit_NeoMatrix(11, 10, 13,
            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
            NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
            NEO_GRBW           + NEO_KHZ800)), Debug(debug) {};
        void begin();
        void update();
        void applyConfig(ClockConfig &config);
        //~Clock();
};