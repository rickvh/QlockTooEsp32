#include "animation.h"
#include "image.h"

namespace qlocktoo {
Animation::Animation(Preset preset) : App(Mode::Animation) {
    switch(preset) {
        case Preset::Wifi:
            // frames/pu
            // currentImage = Image::Preset::WIFI1;
        break;
    }
}

void Animation::setup() {
    // currentImage = Image::Preset::WIFI1;
}

void Animation::loop() {
    // switch (currentImage) {
    //     case Image::Preset::WIFI1:
    //         currentImage = Image::Preset::WIFI2;
    //         break;
    //     case Image::Preset::WIFI2:
    //         currentImage = Image::Preset::WIFI3;
    //         break;
    //     case Image::Preset::WIFI3:
    //         currentImage = Image::Preset::WIFI1;
    //         break;
    //     default:
    //         currentImage = Image::Preset::ERROR;
    //         break;
    // }
    // currentFrame = std::shared_ptr<Image>(new Image(currentImage));
    // if (currentFrame.get()) {
    //     currentFrame.get()->show();
    // }

    delay(300);
}
}  // namespace qlocktoo