#pragma once

#include "app.h"
#include "Arduino.h"
#include "display.h"

namespace qlocktoo {
class Ledtest : public App {
   private:
    static constexpr const char* LOG_TAG = "ledtest";

    const uint8_t width = Display::WIDTH;
    const uint8_t height = Display::HEIGHT;
    uint8_t x = 0;
    uint8_t y = 0;

    float hueRed = 0.0f;
    float hueGreen = 0.25f;
    float hueCurrent = 0.0f;

   protected:
    void setup() override;
    void loop() override;

   public:
    Ledtest() : App() {}
    std::string name() override;
};
}