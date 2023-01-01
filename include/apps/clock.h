#pragma once

#include <vector>
#include "app.h"
#include "display.h"
#include "configservice.h"

namespace qlocktoo {
class Clock : public App {
   private:
    static constexpr const char* LOG_TAG = "clock";
    const uint8_t HETIS = 0;
    const uint8_t VIJF = 13;
    const uint8_t TIEN = 14;
    const uint8_t KWART = 15;
    const uint8_t VOOR1 = 16;
    const uint8_t OVER1 = 17;
    const uint8_t HALF = 18;
    const uint8_t UUR = 19;
    const uint8_t VOOR2 = 20;
    const uint8_t OVER2 = 21;

    struct tm currentTime;
    uint8_t timeBrightness();
    void setColor(const std::vector<uint8_t> leds, HsbColor color);

    // Every pixel is calculated as: pixel = y * displaywidth + x. Where x and y are zero-based, starting in the top-left corner.
    const std::vector<std::vector<uint8_t>> ledsbyword = {
        {0, 1, 2, 4, 5},                // HET IS
        {51, 52, 53},                   // een
        {55, 56, 57, 58},               // twee
        {62, 63, 64, 65},               // drie
        {66, 67, 68, 69},               // vier
        {70, 71, 72, 73},               // vijf
        {74, 75, 76},                   // zes
        {77, 78, 79, 80, 81},           // zeven
        {88, 89, 90, 91},               // acht
        {83, 84, 85, 86, 87},           // negen
        {92, 93, 94, 95},               // tien
        {96, 97, 98},                   // elf
        {99, 100, 101, 102, 103, 104},  // twaalf
        {7, 8, 9, 10},                  // VIJF
        {11, 12, 13, 14},               // TIEN
        {28, 29, 30, 31, 32},           // KWART
        {18, 19, 20, 21},               // VOOR1
        {22, 23, 24, 25},               // OVER1
        {33, 34, 35, 36},               // HALF
        {107, 108, 109},                // UUR
        {44, 45, 46, 47},               // VOOR2
        {40, 41, 42, 43}                // OVER2
    };

   protected:
    void setup() override;
    void loop() override;

   public:
    Clock() : App(Mode::Clock) {};
};
}