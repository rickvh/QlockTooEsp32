#pragma once
#include "display.h"

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
        static constexpr const char* CONFIGFILE = "/settings.cfg";
        static void printConfig();
    
    public:
        // Holds the configuration of QlockToo. Configuration is loaded upon class initialisation.
        static Config CONFIG;
        static bool connectedToWifi;
        static void save();
        static void init();
};
}
