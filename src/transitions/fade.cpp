#include "transitions/fade.h"
#include "display.h"
#include "image.h"
#include "math.h"

using namespace qlocktoo;

void Fade::update() {
    frame++;
    ESP_LOGD(LOG_TAG, "show frame %u", frame);

    Display::clear();
    bool differenceBetweenCurrentAndTarget = false;
    for (uint8_t y = 0; y < Display::HEIGHT; y++) {
        for (uint8_t x = 0; x < Display::WIDTH; x++) {
            auto currentColor = HsbColor(current.getColor({x, y}));

            auto targetColor = HsbColor(to.getColor({x, y}));
            float currentHue = currentColor.H;
            float currentSaturation = currentColor.S;
            float currentBrightness = currentColor.B;
            float targetHue = targetColor.H;
            float targetSaturation = targetColor.S;
            float targetBrightness = targetColor.B;
    
            if (currentHue != targetHue || currentSaturation != targetSaturation || currentBrightness != targetBrightness) {
                differenceBetweenCurrentAndTarget = true;
            }

            auto color = HsbColor(
                getFadedValue(currentHue, targetHue),
                getFadedValue(currentSaturation, targetSaturation),
                getFadedValue(currentBrightness, targetBrightness)
            );
            current.setColor({x, y}, color);
            Display::drawPixel(x, y, color);
        }
        delay(10); // Warning: you may adjust this value, but keep it with at least value '0' in order to allow RTOS to switch tasks to prevent a watchdog reset.
    }
    Display::show();
    done = !differenceBetweenCurrentAndTarget;
}

float Fade::getFadedValue(float currentValue, float targetValue) {
    if (fabs(currentValue - targetValue) < STEP) {
        return targetValue;
    }
    if (currentValue < targetValue) {
        return currentValue + STEP;
    } else {
        return currentValue - STEP;
    }
}

void Fade::showCurrentFrame() {
    for (uint8_t y = 0; y < Display::HEIGHT; y++) {
        for (uint8_t x = 0; x < Display::WIDTH; x++) {
            auto color = current.getColor({x, y});
            Display::drawPixel(x, y, color);
        }
    }
    Display::show();
}
