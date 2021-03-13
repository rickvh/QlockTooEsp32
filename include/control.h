#pragma once

#include <Arduino.h>

namespace qlocktoo {

enum class Mode {
    // don't use these directly to set a mode. Should be refactored.
    Unknown,
    Image,
    Animation,
    Text,
    
    // actual modes
    WifiConnecting,
    WifiSetupNeeded,
    OTAinProgress,
    Clock,
    Swirl,
    Xmas,
    Snow,
    Error
};

}