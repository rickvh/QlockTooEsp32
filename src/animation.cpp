#include "animation.h"

#include "control.h"
#include "image.h"

namespace qlocktoo {

Mode Animation::getApp() {
    return Mode::WIFI_ANIMATION;
}

void Animation::setup() {
    currentImage = Image::Preset::WIFI1;
}

void Animation::loop() {
    switch (currentImage) {
        case Image::Preset::WIFI1:
            currentImage = Image::Preset::WIFI2;
            break;
        case Image::Preset::WIFI2:
            currentImage = Image::Preset::WIFI3;
            break;
        case Image::Preset::WIFI3:
            currentImage = Image::Preset::WIFI1;
            break;
        default:
            currentImage = Image::Preset::ERROR;
            break;
    }
    currentFrame = std::shared_ptr<Image>(new Image(currentImage));
    currentFrame.get()->show();

    delay(300);
}
}  // namespace qlocktoo