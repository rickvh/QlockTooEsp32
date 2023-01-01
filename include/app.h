#pragma once

#include "control.h"

namespace qlocktoo {
class App {
   private:
    bool initialized;
    qlocktoo::Mode app;

   protected:
    App(Mode app) : initialized(false), app(app) {};
    virtual void setup() = 0;
    virtual void loop() = 0;

   public:
    virtual ~App() {};
    Mode getApp() const { return app; };
    void handle();
    volatile bool canTerminate;
};
}