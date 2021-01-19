#pragma once

#include "app.h"
#include "control.h"
#include "Arduino.h"
#include "display.h"

namespace qlocktoo {
class Swirl : public App {
   public:
    Swirl() : App(Mode::SWIRL) {}

   private:
    void setup();
    void loop();

    const uint8_t width = Display::WIDTH;
    const uint8_t height = Display::HEIGHT;
    uint8_t x_start = 1;
    uint8_t y_start = 1;
    uint8_t speed = 50, acc = 50, dir = 1;
    uint16_t hue = 0;
    uint8_t saturation = 255, brightness = 255;
};
}  // namespace qlocktoo