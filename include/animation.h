#pragma once

#include <memory>
#include <vector>
#include <string>
#include "app.h"
#include "image.h"

using namespace std;

namespace qlocktoo {
class Animation : public App {
   private:
    void setup();
    void loop();

    vector<Image::Preset> frames;
    uint8_t currentFrame;

   protected:
    std::string name() override;

   public:
    enum class Preset {
        Wifi
    };

    Animation(Preset animation);
};
}