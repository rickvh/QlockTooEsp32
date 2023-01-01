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

static const char *enum_str[] =
        { 
            "Unknown", "Image", "Animation", "Text",
            "WifiConnection", "WifiSetupRequired", "OTAinProgress", "Clock", "Swirl", "Ledtest", "Snow", "Error"
        };

const char* toString(Mode mode);

}