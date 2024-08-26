#pragma once

namespace qlocktoo {

enum class Mode {
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
        "WifiConnecting", "WifiSetupRequired", "OTAinProgress", "Clock",
        "Swirl", "Ledtest", "Xmas", "Snow", "Error"
    };

const char *modeToString(Mode mode);
}