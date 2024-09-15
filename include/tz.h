#pragma once

#include <pgmspace.h>
#include <string>

const std::string temp_fixed_tz PROGMEM = "CET-1CEST,M3.5.0,M10.5.0/3";

typedef struct {
    std::string zone;
    std::string info;
} zoneinfo_t;

const zoneinfo_t TIMEZONES[] PROGMEM = {
        {"Europe/Amsterdam", "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Greenwich", "GMT0"}
    };

void setTimezone(const std::string &zone);

std::string getTimezone();
