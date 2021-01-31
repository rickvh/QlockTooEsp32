#pragma once

#include "app.h"
#include "control.h"
#include "Arduino.h"
#include "display.h"
#include "image.h"

extern QueueHandle_t xDrawQueue;

namespace qlocktoo {

class Draw : public App {
   public:
    Draw() : App(Mode::DRAW) {}
    // ~Draw();

    enum Operation {
        READ,
        WRITE,
        DRAW,
        CLEAR
    };

    struct Command {
        Operation operation;
        // char* file;
        uint8_t x, y;
        uint8_t r, g, b, w;
    };

   private:
    void setup();
    void loop();

    const uint8_t width = Display::WIDTH;
    const uint8_t height = Display::HEIGHT;
    Image image;
};
}  // namespace qlocktoo