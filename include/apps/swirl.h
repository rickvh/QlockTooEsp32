#pragma once

#include "app.h"
#include "Arduino.h"
#include "display.h"

namespace qlocktoo {
class Swirl : public App {
   private:
    const uint8_t width = Display::WIDTH;
    const uint8_t height = Display::HEIGHT;
    uint8_t x_start = 1;
    uint8_t y_start = 1;
    uint8_t speed = 50, acc = 50, dir = 1;
    float hue = 0.0f;
    float saturation = 1.0f, brightness = 1.0f;

   protected:
    void setup() override;
    void loop() override;

   public:
    Swirl() : App() {}
};
}