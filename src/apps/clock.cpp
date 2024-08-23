#include "apps/clock.h"
#include "transitions/slideshow.h"
#include <memory>
#include <vector>
#include <WiFi.h>
#include "time.h"

#define NTP_TIMEOUT 1500

using namespace qlocktoo;

void Clock::setup() {
    ESP_LOGI(LOG_TAG, "Clock setup");
    Display::begin();
    Display::clear();
}

std::unique_ptr<Image> Clock::getImageFromTime(const tm &time) {
    // TODO
}


void Clock::loop() {
    // update time
    if (!getLocalTime(&this->currentTime)) {
        ESP_LOGI(LOG_TAG, "Failed to obtain time");
        return;
    }

    // if time has advanced 5 minutes
    // if (previousTime.tm_min % 5 != 0 && currentTime.tm_min % 5 == 0) {
    if (previousTime.tm_min != currentTime.tm_min) {
        ESP_LOGD(LOG_TAG, "Activate transition");
        //transition = std::unique_ptr<Transition>(new Slideshow(getImageFromTime(previousTime), getImageFromTime(currentTime)));
        transition = std::unique_ptr<Transition>(new Slideshow(std::unique_ptr<Image>(new Image(Image::Preset::Snowman)), std::unique_ptr<Image>(new Image(Image::Preset::XmasTree))));
    }
    previousTime = currentTime; 


    if (transition.get() != nullptr && !transition->isDone()) {
        transition->update();
        return;
    }


    // show current time
    Display::clear();

    int currentHourWord = currentTime.tm_hour;
    if (currentHourWord > 12) currentHourWord = currentHourWord - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (currentHourWord == 0) currentHourWord = 12;                     // 0 is also called 12

    int nextHourWord = currentTime.tm_hour + 1;
    if (nextHourWord > 12) nextHourWord = nextHourWord - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (nextHourWord == 0) nextHourWord = 12;                  // 0 is also called 12
    
    const ClockConfig &config = ConfigService::CONFIG.clockConfig;
    setColor(ledsByWord[HETIS], config.colorItIs);
    switch ((currentTime.tm_min % 60) / 5) {
        case 0:
            setColor(ledsByWord[currentHourWord], config.colorHour);
            setColor(ledsByWord[UUR], config.colorWords);
            break;
        case 1:
            setColor(ledsByWord[VIJF], config.colorWords);
            setColor(ledsByWord[OVER1], config.colorWords);
            setColor(ledsByWord[currentHourWord], config.colorHour);
            break;
        case 2:
            setColor(ledsByWord[TIEN], config.colorWords);
            setColor(ledsByWord[OVER2], config.colorWords);
            setColor(ledsByWord[currentHourWord], config.colorHour);
            break;
        case 3:
            setColor(ledsByWord[KWART], config.colorWords);
            setColor(ledsByWord[OVER2], config.colorWords);
            setColor(ledsByWord[currentHourWord], config.colorHour);
            break;
        case 4:
            setColor(ledsByWord[TIEN], config.colorWords);
            setColor(ledsByWord[VOOR1], config.colorWords);
            setColor(ledsByWord[HALF], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 5:
            setColor(ledsByWord[VIJF], config.colorWords);
            setColor(ledsByWord[VOOR1], config.colorWords);
            setColor(ledsByWord[HALF], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 6:
            setColor(ledsByWord[HALF], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 7:
            setColor(ledsByWord[VIJF], config.colorWords);
            setColor(ledsByWord[OVER1], config.colorWords);
            setColor(ledsByWord[HALF], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 8:
            setColor(ledsByWord[TIEN], config.colorWords);
            setColor(ledsByWord[OVER1], config.colorWords);
            setColor(ledsByWord[HALF], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 9:
            setColor(ledsByWord[KWART], config.colorWords);
            setColor(ledsByWord[VOOR2], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 10:
            setColor(ledsByWord[TIEN], config.colorWords);
            setColor(ledsByWord[VOOR1], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
        case 11:
            setColor(ledsByWord[VIJF], config.colorWords);
            setColor(ledsByWord[VOOR2], config.colorWords);
            setColor(ledsByWord[nextHourWord], config.colorHour);
            break;
    }
    Display::show();

    // Display minutes 1-4 
    uint8_t minutesAfterFive = currentTime.tm_min % 5;
    Display::writeMinute1(minutesAfterFive > 0);
    Display::writeMinute2(minutesAfterFive > 1);
    Display::writeMinute3(minutesAfterFive > 2);
    Display::writeMinute4(minutesAfterFive > 3);
    
    delay(1000);
}

void Clock::setColor(const std::vector<uint8_t> &leds, HsbColor color) {
    for (uint8_t led : leds) {
        Display::drawPixel(led % Display::WIDTH, led / Display::WIDTH, color);
    }
}