#include "transitions/fade.h"
#include <memory>
#include "display.h"
#include "image.h"
#include "apps/imageviewer.h"

using namespace qlocktoo;

void Fade::update() {
    frame++;
    ESP_LOGD(LOG_TAG, "show frame %u", frame);
    Display::clear();

    switch (frame) {
        case 1:
            // TODO
            break;
        case 2:
            // TODO
            done = true;
            break;
        default:
            break;
    }


    delay(5000);
}
