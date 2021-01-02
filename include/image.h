#pragma once

#include <pgmspace.h>
#include <array>
#include "color.h"

const static uint8_t PROGMEM IMG_WIFI[] = {
    1,9,11,14,18,21,22,24,30,32,33,35,38,41,43,44,47,49,51,54,56,60,64,71,82,93,103,104,105
};

const static uint8_t PROGMEM IMG_XMAS_TREE_GREEN[] = {
    16,26,27,28,38,47,48,49,50,51,60,68,69,70,71,72,73,74,82,89,90,91,92,93,94,95,96,97
};
const static uint8_t PROGMEM IMG_SNOWMAN_WHITE[] = {
    26,27,28,37,38,39,59,60,61,69,70,71,72,73,80,81,82,83,84,91,92,93,94,95,103,104,105
};
const static uint8_t PROGMEM IMG_SNOWMAN_RED[] = {
    47,48,49,50,57
};
const static uint8_t PROGMEM IMG_SNOWMAN_BLUE[] = {
    5,15,16,17
};



class Image {
    private:
        const static uint8_t WIDTH = 11;
        const static uint8_t HEIGHT = 10;
        std::array<RGBW, WIDTH * HEIGHT> pixels;
    
    public:
        enum Preset {
            WIFI,
            WIFI2,
            XMAS_TREE,  
            SNOWMAN
        };

        Image();
        Image(Preset file);
        RGBW getColor(uint8_t x, uint8_t y);
};
