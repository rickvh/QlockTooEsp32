#include "apps/ledtest.h"
#include "display.h"

using namespace qlocktoo;

void Ledtest::setup() {
    Display::clear();
}

void Ledtest::loop() {
    hueCurrent += 0.05f;

    if (x == 0) {
        if (y == 0) {
            hueCurrent = hueRed;
        } else {
            hueCurrent = hueGreen;
        }
    }
    
    ESP_LOGI(LOG_TAG, "X: %u, Y: %u", x, y);
    Display::drawPixel(x, y, HsbColor(hueCurrent, 1.0f, 0.5f));
    Display::show();
    
    delay(2000);

    x++;
    if (x == Display::WIDTH) {
        y++;
        x = 0;

        if (y == Display::HEIGHT) {
            y = 0;
        }
    }
}