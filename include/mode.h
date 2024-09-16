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
    Error,
    Snake
};

static const char *mode_str[] =
    { 
        "WifiConnecting", "WifiSetupRequired", "OTAinProgress", "Clock",
        "Swirl", "Ledtest", "Xmas", "Snow", "Error", "Snake"
    };

const char *modeToString(Mode mode);
}