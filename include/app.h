#pragma once

#include <atomic>
#include "mode.h"

namespace qlocktoo {
class App {
   private:
    bool initialized;
    qlocktoo::Mode mode;

   protected:
    App(Mode mode) : initialized(false), mode(mode) {};
    virtual void setup() = 0;
    virtual void loop() = 0;

   public:
    virtual ~App() {};
    qlocktoo::Mode getMode() const { return mode; };
    virtual void stop() {};
    virtual void handle();
};
}