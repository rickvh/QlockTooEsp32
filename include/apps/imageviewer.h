#pragma once

#include "app.h"
#include "Arduino.h"
#include "display.h"

namespace qlocktoo {
class Imageviewer : public App {
   private:
    static constexpr const char* LOG_TAG = "imageviewer";
    Image image;

   protected:
    void setup() override;
    void loop() override;

   public:
    Imageviewer(Image image) : App(), image(image) {}
    std::string name() override;
};
}