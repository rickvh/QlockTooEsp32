#pragma once

namespace qlocktoo {

enum class Mode {
    // don't use these directly to set a mode. Should be refactored.
    Unknown,
    Image,
    Animation,
    Text,
    
    // actual modes
    WifiConnecting,
    WifiSetupRequired,
    OTAinProgress,
    Clock,
    Swirl,
    Ledtest,
    Xmas,
    Snow,
    Error
};

static const char *mode_str[] =
    { 
        "Unknown", "Image", "Animation", "Text",
        "WifiConnecting", "WifiSetupRequired", "OTAinProgress", "Clock", "Swirl", "Ledtest", "Snow", "Error"
    };

const char *modeToString(Mode mode);
}