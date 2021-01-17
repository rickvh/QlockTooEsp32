#include "clock.h"

#include <vector>
// #include <TimeLib.h>
// #include "WifiConfig.h"
#include <WiFi.h>

#include "RemoteDebugger.h"
#include "color.h"
#include "time.h"

#define NTP_TIMEOUT 1500

#define R_VALUE_2 255  //200
#define G_VALUE_2 0    //200
#define B_VALUE_2 0    //0
#define W_VALUE_2 0

#define BRIGHTNESS 255  // legacy, keep at 255

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

#define PIEK 0
#define BOOM 1
#define STAM 2

namespace qlocktoo {
void Clock::begin() {
    debugI("begin()");

    display.setRemapFunction(remapPixels);

    // TODO: ledstrip pin global? Of als deze lib het goed doet gewoon eenmalig global het display initialiseren...
    display.begin(13, 110);

    //init and get the time
    // TODO: verplaatsen naar main

    // const long  gmtOffset_sec = 3600;
    // const int   daylightOffset_sec = 3600;
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    // debugI("Clock::begin()");
}

void Clock::applyConfig(ClockConfig &config) {
    this->config = config;
}

void Clock::update() {
    if (!getLocalTime(&this->currentTime)) {
        debugD("Failed to obtain time");
    }

    showKlok();
}

void Clock::showKlok() {
    // calculate target brightnesses:
    int current_hourword = hour();
    if (current_hourword > 12) current_hourword = current_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (current_hourword == 0) current_hourword = 12;                     // 0 is also called 12

    int next_hourword = hour() + 1;
    if (next_hourword > 12) next_hourword = next_hourword - 12;  // 12 hour clock, where 12 stays 12 and 13 becomes one
    if (next_hourword == 0) next_hourword = 12;                  // 0 is also called 12

    int speed = 2;

    // move current brightness towards target brightness:
    for (int i = 0; i < NUM_LEDS; ++i) {
        if (currentlevels[i] < targetlevels[i]) {
            currentlevels[i] = std::min(BRIGHTNESS, currentlevels[i] + speed);
        }
        if (currentlevels[i] > targetlevels[i]) {
            currentlevels[i] = std::max(0, currentlevels[i] - speed);
        }

        // output the value to led: according to the function x^2/255 to compensate for the perceived brightness of leds which is not linear
        auto color = this->config.colorWords;
        color.brightness = currentlevels[i];
        display.color32(i, color.getColor());
    }

    for (int i = 0; i < NUM_LEDS; i++) {
        targetlevels[i] = 0;
    }

    // RVH: Custom debug optie
    //targetlevels[selectedLed] =  255;

    for (int l : ledsbyword[HETIS]) {
        targetlevels[l] = 255;
    }
    switch ((minute() % 60) / 5) {
        case 0:
            for (int l : ledsbyword[current_hourword]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[UUR]) {
                targetlevels[l] = 255;
            }
            break;
        case 1:
            for (int l : ledsbyword[VIJF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[OVER1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[current_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 2:
            for (int l : ledsbyword[TIEN]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[OVER2]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[current_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 3:
            for (int l : ledsbyword[KWART]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[OVER2]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[current_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 4:
            for (int l : ledsbyword[TIEN]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[VOOR1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[HALF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 5:
            for (int l : ledsbyword[VIJF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[VOOR1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[HALF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 6:
            for (int l : ledsbyword[HALF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 7:
            for (int l : ledsbyword[VIJF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[OVER1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[HALF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 8:
            for (int l : ledsbyword[TIEN]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[OVER1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[HALF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 9:
            for (int l : ledsbyword[KWART]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[VOOR2]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 10:
            for (int l : ledsbyword[TIEN]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[VOOR1]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
        case 11:
            for (int l : ledsbyword[VIJF]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[VOOR2]) {
                targetlevels[l] = 255;
            }
            for (int l : ledsbyword[next_hourword]) {
                targetlevels[l] = 255;
            }
            break;
    }

    // FIX 2e kleur
    for (int uur = 1; uur <= 12; uur++) {
        for (int l : ledsbyword[uur]) {
            if (currentlevels[l] > 0) {
                auto color = this->config.colorHour;
                color.brightness = currentlevels[l];
                display.color32(l, color.getColor());
            }
        }
    }

    // FIX 'het is'
    for (int l : ledsbyword[0]) {
        if (currentlevels[l] > 0) {
            auto color = this->config.colorItIs;
            color.brightness = currentlevels[l];
            display.color32(l, color.getColor());
        }
    }

    display.show();
}

uint8_t Clock::timeBrightness() {
    if (hour() > dayHour && hour() < nightHour) {
        return MAX_BRIGHTNESS;
    } else if (hour() < dayHour || hour() > nightHour) {
        return MIN_BRIGHTNESS;
    } else if (hour() == dayHour) {
        return constrain(
            map(minute(), 0, 29, MIN_BRIGHTNESS, MAX_BRIGHTNESS),
            MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    } else if (hour() == nightHour) {
        return constrain(
            map(minute(), 0, 29, MAX_BRIGHTNESS, MIN_BRIGHTNESS),
            MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
    return 0;
}

uint8_t Clock::hour() {
    return this->currentTime.tm_hour;
}

uint8_t Clock::minute() {
    return this->currentTime.tm_min;
}
}  // namespace qlocktoo