#pragma once

#include "app.h"
#include "control.h"

namespace qlocktoo {
class Swirl : public App {
   public:
    Mode getApp();

   private:
    void setup();
    void loop();
};
}  // namespace qlocktoo