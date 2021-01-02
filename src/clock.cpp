#include "clock.h"

#include <vector>
// #include <TimeLib.h>
// #include "WifiConfig.h"
#include "color.h"
#include "time.h"
#include <WiFi.h>
#include "RemoteDebugger.h"

#define NTP_TIMEOUT 1500
#define SHOW_TIME_PERIOD 50


// Use time or LDR light sensor for auto brightness adjustment.
#define TIME_BRIGHTNESS
//#define LDR_BRIGHTNESS


#define R_VALUE_2         255 //200
#define G_VALUE_2         0 //200
#define B_VALUE_2         0 //0
#define W_VALUE_2         0

#define MIN_BRIGHTNESS  15
#define MAX_BRIGHTNESS  140
#define BRIGHTNESS      255 // legacy, keep at 255
int     lastBrightness  =  MIN_BRIGHTNESS;
int     dayHour         = 8; // Start increasing brightness
int     nightHour       = 17; // Start decreasing brightness

#define SYNC_INTERVAL   1200

#define NUM_LEDS        110


// bool        autoDST         = true;

// IPAddress   timeServerIP; // time.nist.gov NTP server address
// const char* ntpServerName   = "nl.pool.ntp.org";
// byte        packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// WiFiUDP     Udp;
// unsigned int localPort = 8888;

unsigned long   lastdisplayupdate   = 0;

uint8_t targetlevels[NUM_LEDS];
uint8_t currentlevels[NUM_LEDS];

bool isDST(int d, int m, int y);
bool isDSTSwitchDay(int d, int m, int y);
void updateBrightness();
int timeBrightness();
void showKlok(Adafruit_NeoPixel &display);

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
#define VIJF  13
#define TIEN  14
#define KWART 15
#define VOOR1  16
#define OVER1  17
#define HALF  18
#define UUR   19
#define VOOR2 20
#define OVER2 21

#define PIEK 0
#define BOOM 1
#define STAM 2


std::vector<std::vector<int>> ledsbyword = {
    {0,37,38,36,39}, // HET IS
    {17,19,54},            // een
    {58,89,95,57},      // twee
    {91,93,55,92},      // drie
    {96,88,59,97},      // vier
    {87,60,98,86},         // vijf
    {61,99,85},         // zes
    {102,65,82,101,64},   // zeven
    {80,66,103,79},      // acht
    {100,63,84,107,62},   // negen
    {67,109,78,68},      // tien
    {108,77,69},         // elf
    {73,81,72,74,104,71},// twaalf
    {35,40,3,34},    // VIJF
    {30,43,6,31},    // TIEN
    {9,46,27,10,47}, // KWART
    {41,4,33,26},    // VOOR1
    {7,44,29,8},    // OVER1
    {22,14,50,23},    // HALF
    {70,76,106},      // UUR
    {51,15,21,52},          // VOOR2
    {12,48,25,11}          // OVER2
};




void Clock::begin() {
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
    if ((millis() - lastdisplayupdate) <= SHOW_TIME_PERIOD) {
        return;
    }
    lastdisplayupdate = millis();
    

    #ifdef TIME_BRIGHTNESS
        display.setBrightness(timeBrightness());
    #endif
    #ifdef LDR_BRIGHTNESS
        updateBrightness();
    #endif

    if(!getLocalTime(&this->currentTime)){
        debugD("Failed to obtain time");
    }
    // char str_date[256];
    // strftime(str_date, sizeof(str_date), "%A, %d %B %Y", &currentTime);
    // debugD("Time: %s", str_date);


    // int imageCount = second() / 15;

    // if (hour() >= 22 || hour() <= 7)
    //   imageCount = 0;

    // if (imageCount == 1) {
    //   showImage(0);
    // } else if (imageCount == 3) {
    //   showImage(1);
    // } else {
      showKlok(display);
    // }
    
    
    // Update LEDs
}

void Clock::showKlok(Adafruit_NeoPixel &display) {
      // calculate target brightnesses:
    int current_hourword = hour();
    if(current_hourword>12) current_hourword = current_hourword - 12; // 12 hour clock, where 12 stays 12 and 13 becomes one
    if(current_hourword==0) current_hourword = 12;            // 0 is also called 12

    int next_hourword = hour()+1;
    if(next_hourword>12) next_hourword = next_hourword - 12;      // 12 hour clock, where 12 stays 12 and 13 becomes one
    if(next_hourword==0) next_hourword = 12;              // 0 is also called 12


    int speed = 2;
  
  
    // move current brightness towards target brightness:
    for(int i=0;i<NUM_LEDS;++i) {
        if(currentlevels[i] < targetlevels[i]) {
            currentlevels[i] = std::min(BRIGHTNESS,currentlevels[i]+speed);
        }
        if(currentlevels[i] > targetlevels[i]) {
            currentlevels[i] = std::max(0,currentlevels[i]-speed);
        }

        // output the value to led: according to the function x^2/255 to compensate for the perceived brightness of leds which is not linear
        auto color = this->config.colorWords;
        color.brightness = currentlevels[i];
        display.setPixelColor(i, color.getColor());
    }


    for(int i=0;i<NUM_LEDS;i++) {
        targetlevels[i] = 0;
    }


    // RVH: Custom debug optie
    //targetlevels[selectedLed] =  255;

    for(int l : ledsbyword[HETIS]) { targetlevels[l] = 255; }
    switch((minute()%60)/5) {
        case 0:
            for(int l : ledsbyword[current_hourword])   { targetlevels[l] = 255; }
            for(int l : ledsbyword[UUR])        { targetlevels[l] = 255; }
            break;
        case 1:
            for(int l : ledsbyword[VIJF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[OVER1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[current_hourword]) { targetlevels[l] = 255; }
            break;
        case 2:
            for(int l : ledsbyword[TIEN])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[OVER2])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[current_hourword])   { targetlevels[l] = 255; }
            break;
        case 3:
            for(int l : ledsbyword[KWART])        { targetlevels[l] = 255; }
            for(int l : ledsbyword[OVER2])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[current_hourword])   { targetlevels[l] = 255; }
            break;
        case 4:
            for(int l : ledsbyword[TIEN])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[VOOR1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[HALF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 5:
            for(int l : ledsbyword[VIJF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[VOOR1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[HALF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 6:
            for(int l : ledsbyword[HALF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 7:
            for(int l : ledsbyword[VIJF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[OVER1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[HALF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 8:
            for(int l : ledsbyword[TIEN])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[OVER1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[HALF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 9:
            for(int l : ledsbyword[KWART])        { targetlevels[l] = 255; }
            for(int l : ledsbyword[VOOR2])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 10:
            for(int l : ledsbyword[TIEN])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[VOOR1])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
        case 11:
            for(int l : ledsbyword[VIJF])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[VOOR2])         { targetlevels[l] = 255; }
            for(int l : ledsbyword[next_hourword])    { targetlevels[l] = 255; }
            break;
    }
    

    // FIX 2e kleur
    for(int uur = 1; uur <=12; uur++) {
        for(int l : ledsbyword[uur]) {
            if (currentlevels[l] > 0) {
                auto color = this->config.colorHour;
                color.brightness = currentlevels[l];
                display.setPixelColor(l, color.getColor());
            }
        }
    }

    // FIX 'het is'
     for(int l : ledsbyword[0]) {
        if (currentlevels[l] > 0) {
            auto color = this->config.colorItIs;
            color.brightness = currentlevels[l];
            display.setPixelColor(l, color.getColor());
        }
    }
}


int Clock::timeBrightness() {
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