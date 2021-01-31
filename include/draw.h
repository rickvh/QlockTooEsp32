#pragma once

#include "app.h"
#include "control.h"
#include "Arduino.h"
#include "display.h"
#include "image.h"
#include "RemoteDebugger.h"

extern QueueHandle_t xDrawQueue;

namespace qlocktoo {

class Draw : public App {
   public:
    Draw(RemoteDebug &debug) : App(Mode::DRAW), Debug(debug) {
        image = new Image(Debug);
    }
    ~Draw() {
        delete image;
    };

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
    Image *image = NULL;
    RemoteDebug &Debug;
};
}  // namespace qlocktoo