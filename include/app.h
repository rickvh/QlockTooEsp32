#pragma once

#include "control.h"

namespace qlocktoo {
class App {
   private:
    bool initialized;

   protected:
    virtual void setup();
    virtual void loop();

   public:
    App() : initialized(false){};
    virtual Mode getApp();
    void handle();
};
}  // namespace qlocktoo