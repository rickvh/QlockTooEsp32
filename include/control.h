#pragma once

#include <Arduino.h>

namespace qlocktoo {

typedef enum {
    NOT_SET,
    NO_WIFI,
    WIFI_ANIMATION,
    CLOCK,
    SWIRL,
    TEXT,
    IMAGE,
    ERROR,
    DEBUG,
    XMAS,
    SNOW,
    DRAW
} Mode;

}