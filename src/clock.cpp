#include "clock.h"
#include <vector>
#include <WiFi.h>
#include "time.h"
#include "configservice.h"

#define NTP_TIMEOUT 1500

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

#define HETIS 0
#define VIJF 13
#define TIEN 14
#define KWART 15
#define VOOR1 16
#define OVER1 17
#define HALF 18
#define UUR 19
#define VOOR2 20
#define OVER2 21

namespace qlocktoo {
void Clock::setup() {
    Serial.println("Clock setup");
    config = &ConfigService::CONFIG.clockConfig;
    Display::begin();
    Display::clear();
}

void Clock::loop() {
    if (!getLocalTime(&this->currentTime)) {
        Serial.println("Failed to obtain time");
        return;
    }

    uint8_t hour = currentTime.tm_hour;
    uint8_t minute = currentTime.tm_min;

    int current_hourword = hour;
    if (current_hourword > 12) current_hourword = current_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (current_hourword == 0) current_hourword = 12;                     // 0 is also called 12

    int next_hourword = hour + 1;
    if (next_hourword > 12) next_hourword = next_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (next_hourword == 0) next_hourword = 12;                  // 0 is also called 12

    Display::clear();

    setColor(ledsbyword[HETIS], config->colorItIs);
    switch ((minute % 60) / 5) {
        case 0:
            setColor(ledsbyword[current_hourword], config->colorHour);
            setColor(ledsbyword[UUR], config->colorWords);
            break;
        case 1:
            setColor(ledsbyword[VIJF], config->colorWords);
            setColor(ledsbyword[OVER1], config->colorWords);
            setColor(ledsbyword[current_hourword], config->colorHour);
            break;
        case 2:
            setColor(ledsbyword[TIEN], config->colorWords);
            setColor(ledsbyword[OVER2], config->colorWords);
            setColor(ledsbyword[current_hourword], config->colorHour);
            break;
        case 3:
            setColor(ledsbyword[KWART], config->colorWords);
            setColor(ledsbyword[OVER2], config->colorWords);
            setColor(ledsbyword[current_hourword], config->colorHour);
            break;
        case 4:
            setColor(ledsbyword[TIEN], config->colorWords);
            setColor(ledsbyword[VOOR1], config->colorWords);
            setColor(ledsbyword[HALF], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 5:
            setColor(ledsbyword[VIJF], config->colorWords);
            setColor(ledsbyword[VOOR1], config->colorWords);
            setColor(ledsbyword[HALF], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 6:
            setColor(ledsbyword[HALF], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 7:
            setColor(ledsbyword[VIJF], config->colorWords);
            setColor(ledsbyword[OVER1], config->colorWords);
            setColor(ledsbyword[HALF], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 8:
            setColor(ledsbyword[TIEN], config->colorWords);
            setColor(ledsbyword[OVER1], config->colorWords);
            setColor(ledsbyword[HALF], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 9:
            setColor(ledsbyword[KWART], config->colorWords);
            setColor(ledsbyword[VOOR2], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 10:
            setColor(ledsbyword[TIEN], config->colorWords);
            setColor(ledsbyword[VOOR1], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
        case 11:
            setColor(ledsbyword[VIJF], config->colorWords);
            setColor(ledsbyword[VOOR2], config->colorWords);
            setColor(ledsbyword[next_hourword], config->colorHour);
            break;
    }

    Display::show();
    delay(1000);
}

void Clock::setColor(const std::vector<int> leds, HsbColor color) {
    for (uint8_t led : leds) {
        Display::drawPixel(led, color);
    }
}

}  // namespace qlocktoo