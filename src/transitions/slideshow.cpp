#include "transitions/slideshow.h"
#include <memory>
#include "display.h"
#include "image.h"

using namespace qlocktoo;

void Slideshow::update() {
    frame++;
    ESP_LOGD(LOG_TAG, "show frame %u", frame);
    Display::clear();

    switch (frame) {
        case 1:
            from->show();
            break;
        case 2:
            to->show();
            done = true;
            break;
        default:
            break;
    }

    delay(5000);
}
