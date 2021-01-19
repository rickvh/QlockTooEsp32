#pragma once

#include <memory>
#include "app.h"
#include "control.h"
#include "image.h"

namespace qlocktoo {
class Animation : public App {
   private:
    void setup();
    void loop();

    Image::Preset currentImage;
    std::shared_ptr<Image> currentFrame = NULL;

   public:
    Animation() : App(Mode::WIFI_ANIMATION) {}
};
}  // namespace qlocktoo