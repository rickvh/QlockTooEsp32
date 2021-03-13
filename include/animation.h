#pragma once

#include <memory>
#include <vector>
#include "app.h"
#include "control.h"
#include "image.h"

using namespace std;

namespace qlocktoo {
class Animation : public App {
   private:
    void setup();
    void loop();

    vector<Image::Preset> frames;
    shared_ptr<Image> currentFrame = NULL;

   public:
    enum class Preset {
        Wifi
    };

    Animation(Preset animation);
};
}