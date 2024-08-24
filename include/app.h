#pragma once

#include "mode.h"
#include <string>

namespace qlocktoo {
class App {
   private:
    bool initialized;

   protected:
    App() : initialized(false) {};
    virtual void setup() = 0;
    virtual void loop() = 0;

   public:
    virtual ~App() {};
    virtual void stop() {};
    virtual void handle();
    virtual std::string name() = 0;
};
}