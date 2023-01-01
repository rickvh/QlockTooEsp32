#pragma once
#include "display.h"
#include "Ticker.h"

using namespace std;

namespace qlocktoo {

typedef struct {
    HsbColor colorItIs;
    HsbColor colorWords;
    HsbColor colorHour;
} ClockConfig;

typedef struct {
    char hostname[32];
    char ssid[32];
    char password[64];
    bool connectedToWifi;
} NetworkConfig;

typedef struct {
    ClockConfig clockConfig;
    NetworkConfig networkConfig;
} Config;


class ConfigService {
    private:
        static constexpr const char* LOG_TAG = "configservice";
        static constexpr const char* CONFIGFILE = "/settings.cfg";
        static const uint16_t MIN_INTERVAL_SECONDS = 60;
        static void printConfig();
        static void saveInternally();
        static Ticker flashTicker;
    
    public:
        // Holds the configuration of QlockToo. Configuration is loaded upon class initialisation.
        static Config CONFIG;
        static bool connectedToWifi;
        /**
         * Store config in flash, while also taking care not to write too frequently to flash memory in order to minimize wear leveling. After each save action, the next save action
         * is hold back x seconds before it's actually being written to flash again. The values to store are evaluated at the moment
         * when the write-to-flash eventually happens.
        */
        static void saveEventually();
        static void init();
};
}
