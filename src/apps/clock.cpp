#include "apps/clock.h"
#include <vector>
#include <WiFi.h>
#include "time.h"

#define NTP_TIMEOUT 1500

using namespace qlocktoo;
/*
   HET IS X UUR
   HET IS VIJF OVER X
   HET IS TIEN OVER X
   HET IS KWART OVER X
   HET IS TIEN VOOR HALF (X+1)
   HET IS VIJF VOOR HALF (X+1)
   HET IS HALF (X+1)
   HET IS VIJF OVER HALF (X+1)
   HET IS TIEN OVER HALF (X+1)
   HET IS KWART VOOR (X+1)
   HET IS TIEN VOOR (X+1)
   HET IS VIJF VOOR (X+1)
   HET IS (X+1) UUR
   ...
*/


void Clock::setup() {
    ESP_LOGI(LOG_TAG, "Clock setup");
    Display::begin();
    Display::clear();
}

void Clock::loop() {
    if (!getLocalTime(&this->currentTime)) {
        ESP_LOGI(LOG_TAG, "Failed to obtain time");
        return;
    }

    uint8_t hour = currentTime.tm_hour;
    uint8_t minute = currentTime.tm_min;

    int currentHourWord = hour;
    if (currentHourWord > 12) currentHourWord = currentHourWord - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (currentHourWord == 0) currentHourWord = 12;                     // 0 is also called 12

    int nextHourWord = hour + 1;
    if (nextHourWord > 12) nextHourWord = nextHourWord - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (nextHourWord == 0) nextHourWord = 12;                  // 0 is also called 12

    Display::clear();

    const ClockConfig &config = ConfigService::CONFIG.clockConfig;
    setColor(ledsByWord[HETIS], config.colorItIs);
    switch ((minute % 60) / 5) {
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
    uint8_t minutesAfterFive = minute % 5;
    Display::writeMinute1(minutesAfterFive > 0);
    Display::writeMinute2(minutesAfterFive > 1);
    Display::writeMinute3(minutesAfterFive > 2);
    Display::writeMinute4(minutesAfterFive > 3);
    
    delay(1000);
}

void Clock::setColor(const std::vector<uint8_t> leds, HsbColor color) {
    for (uint8_t led : leds) {
        Display::drawPixel(led % Display::WIDTH, led / Display::WIDTH, color);
    }
}