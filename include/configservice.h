#pragma once
#include "display.h"

namespace qlocktoo {

typedef struct {
    HsbColor colorItIs;
    HsbColor colorWords;
    HsbColor colorHour;
} ClockConfig;

typedef struct {
    std::string hostname;
} NetworkConfig;

typedef struct {
    ClockConfig clockConfig;
    NetworkConfig networkConfig;
} Config;


class ConfigService {
    private:
        static constexpr const char* CONFIGFILE = "/settings.cfg";
    
    public:
        // Holds the configuration of QlockToo. Configuration is loaded upon class initialisation.
        static Config CONFIG;
        static void save();
        static void init();
};
}
