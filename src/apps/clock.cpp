#include "apps/clock.h"
#include "transitions/fade.h"
#include <memory>
#include <string>
#include <vector>
#include <WiFi.h>
#include "time.h"

#define NTP_TIMEOUT 1500

using namespace qlocktoo;

std::string Clock::name() {
    return "Clock";
}

void Clock::setup() {
    ESP_LOGI(LOG_TAG, "Clock setup");
    Display::begin();
    Display::clear();
}

void Clock::loop() {
    if (transition.get() != nullptr && !transition->isDone()) {
        transition->update();
        return;
    }

    // update time
    if (!getLocalTime(&this->currentTime)) {
        ESP_LOGI(LOG_TAG, "Failed to obtain time");
        return;
    }

    // if time has advanced 5 minutes
    // if (previousTime.tm_min % 5 != 0 && currentTime.tm_min % 5 == 0) {
    if (previousTime.tm_min != currentTime.tm_min) {
        ESP_LOGD(LOG_TAG, "Activate transition");

        tm testFrom, testTo;
        testFrom.tm_hour = 7;
        testFrom.tm_min = 0;
        testFrom.tm_isdst = testTo.tm_isdst = currentTime.tm_isdst;
        testTo.tm_hour = 7;
        testTo.tm_min = 5;
        auto previousTimeImage = getImageFromTime(testFrom);
        auto currentTimeImage = getImageFromTime(testTo);
        auto fromColor = previousTimeImage.getColor({0,7});
        auto toColor = currentTimeImage.getColor({0,7});
        ESP_LOGD(LOG_TAG, "From color (0,0): HSB %f, %f, %f", fromColor.H, fromColor.S, fromColor.B);
        ESP_LOGD(LOG_TAG, "To   color (0,0): HSB %f, %f, %f", toColor.H, toColor.S, toColor.B);

        transition = unique_ptr<Transition>(new Fade(previousTimeImage, currentTimeImage));
    }
    previousTime = currentTime; 

    // show current time
    // Display::clear();

    

    // TODO


    // Display minutes 1-4 
    // uint8_t minutesAfterFive = currentTime.tm_min % 5;
    // Display::writeMinute1(minutesAfterFive > 0);
    // Display::writeMinute2(minutesAfterFive > 1);
    // Display::writeMinute3(minutesAfterFive > 2);
    // Display::writeMinute4(minutesAfterFive > 3);
    // Display::show();
    
    delay(1000);
}

Image Clock::getImageFromTime(const tm &time) {
    ESP_LOGD(LOG_TAG, "getImageFromTime: ()");
    int currentHourWord = time.tm_hour;
    if (currentHourWord > 12) currentHourWord = currentHourWord - 12;   // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (currentHourWord == 0) currentHourWord = 12;                     // 0 is also called 12

    int nextHourWord = time.tm_hour + 1;
    if (nextHourWord > 12) nextHourWord = nextHourWord - 12;
    if (nextHourWord == 0) nextHourWord = 12;
    
    const ClockConfig &config = ConfigService::CONFIG.clockConfig;
    Image image;
    image.fill(HsbColor(config.colorWords.H, config.colorWords.S, 0.0f));
    setColor(image, HETIS, config.colorItIs);
    switch ((time.tm_min % 60) / 5) {
        case 0:
            setColor(image, GETAL[currentHourWord], config.colorHour);
            setColor(image, UUR, config.colorWords);
            break;
        case 1:
            setColor(image, VIJF, config.colorWords);
            setColor(image, OVER1, config.colorWords);
            setColor(image, GETAL[currentHourWord], config.colorHour);
            break;
        case 2:
            setColor(image, TIEN, config.colorWords);
            setColor(image, OVER2, config.colorWords);
            setColor(image, GETAL[currentHourWord], config.colorHour);
            break;
        case 3:
            setColor(image, KWART, config.colorWords);
            setColor(image, OVER2, config.colorWords);
            setColor(image, GETAL[currentHourWord], config.colorHour);
            break;
        case 4:
            setColor(image, TIEN, config.colorWords);
            setColor(image, VOOR1, config.colorWords);
            setColor(image, HALF, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 5:
            setColor(image, VIJF, config.colorWords);
            setColor(image, VOOR1, config.colorWords);
            setColor(image, HALF, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 6:
            setColor(image, HALF, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 7:
            setColor(image, VIJF, config.colorWords);
            setColor(image, OVER1, config.colorWords);
            setColor(image, HALF, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 8:
            setColor(image, TIEN, config.colorWords);
            setColor(image, OVER1, config.colorWords);
            setColor(image, HALF, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 9:
            setColor(image, KWART, config.colorWords);
            setColor(image, VOOR2, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 10:
            setColor(image, TIEN, config.colorWords);
            setColor(image, VOOR1, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
        case 11:
            setColor(image, VIJF, config.colorWords);
            setColor(image, VOOR2, config.colorWords);
            setColor(image, GETAL[nextHourWord], config.colorHour);
            break;
    }
    return image;
}

void Clock::setColor(Image &image, const std::vector<Coordinate> &coordinates, HsbColor color) {
    for (Coordinate coordinate : coordinates) {
        image.setColor(coordinate, color);
    }
}