#pragma once

#include <Arduino.h>

namespace qlocktoo {

typedef enum {
    UNKNOWN,
    NO_WIFI,
    OTA_UPDATE,
    CLOCK,
    SWIRL,
    TEXT,
    IMAGE,
    ERROR,
    DEBUG,
    XMAS,
    SNOW
} Mode;

}