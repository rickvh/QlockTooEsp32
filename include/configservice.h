#pragma once
#include "display.h"

namespace qlocktoo {

typedef struct {
    HsbColor colorItIs;
    HsbColor colorWords;
    HsbColor colorHour;
} ClockConfig;

typedef struct {
    char* hostname;
} NetworkConfig;

typedef struct {
    ClockConfig clockConfig;
    NetworkConfig networkConfig;
} Config;


class ConfigService {
    private:
        const char* CONFIGFILE = "settings.cfg";
    
    public:
        ConfigService();
        // Holds the configuration of QlockToo. Configuration is loaded upon class initialisation.
        Config CONFIG;
        void save();
        void reload();
};
}
