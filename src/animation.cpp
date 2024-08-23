#include "animation.h"
#include "image.h"

using namespace qlocktoo;

Animation::Animation(Preset preset) : App() {
    switch(preset) {
        case Preset::Wifi:
            frames.push_back(Image::Preset::Wifi1);
            frames.push_back(Image::Preset::Wifi2);
            frames.push_back(Image::Preset::Wifi3);
        break;
    }
}

void Animation::setup() {
    // NOOP
}

void Animation::loop() {
    if (currentFrame < frames.size() - 1)
    {
        currentFrame++;
    } else {
        currentFrame = 0;
    }
    Image image(frames.at(currentFrame));
    //Imageviewer viewer(image);
    delay(300);
}