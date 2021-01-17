#pragma once

#include <memory>

#include "app.h"
#include "control.h"
#include "image.h"

namespace qlocktoo {
class Animation : public App {
   public:
    Mode getApp();

   private:
    void setup();
    void loop();

    Image::Preset currentImage;
    std::shared_ptr<Image> currentFrame = NULL;
};
}  // namespace qlocktoo