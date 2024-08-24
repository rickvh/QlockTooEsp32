#include "apps/imageviewer.h"
#include <SPIFFS.h>
#include <algorithm>
#include <array>
#include <string>
#include "display.h"

namespace qlocktoo {
std::string Imageviewer::name() {
    return "Imageviewer";
}

void Imageviewer::setup() {
    for (uint8_t y = 0; y < Display::HEIGHT; y++) {
        for (uint8_t x = 0; x < Display::WIDTH; x++) {
            auto color = image->getColor(x, y);
            Display::drawPixel(x, y, color);
        }
    }
}

void Imageviewer::loop() {
    delay(300);
}
}  // namespace qlocktoo