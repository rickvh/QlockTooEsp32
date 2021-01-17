#pragma once

#include "control.h"

namespace qlocktoo {
class App {
   public:
    virtual Mode getApp();
    void handle();

   protected:
    virtual void setup();
    virtual void loop();

   private:
    bool initialized;
};
}  // namespace qlocktoo