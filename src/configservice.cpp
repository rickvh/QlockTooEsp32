#include "configservice.h"
#include <SPIFFS.h>

namespace qlocktoo {
ConfigService::ConfigService() {
    reload();
}

void ConfigService::save() {
    if (!SPIFFS.begin()) {
        Serial.println("Filesystem cannot be opened");
        return;
    }

    File configfile = SPIFFS.open(CONFIGFILE);
    if (!configfile) {
        Serial.println("Failed to save configfile");
    }

    size_t bytes = configfile.write((unsigned char*) &CONFIG, sizeof(Config));

    configfile.close();
    SPIFFS.end();
}

void ConfigService::reload() {
    if (!SPIFFS.begin() || !SPIFFS.exists(CONFIGFILE)) {
        Serial.println("No configfile found...Initializing defaults");
        CONFIG = Config();
        return;
    }

    File configfile = SPIFFS.open(CONFIGFILE);
    if (!configfile) {
        Serial.println("Failed to open configfile");
    }

    unsigned char * buffer = (unsigned char *) &this->CONFIG;
    for(uint16_t i = 0; i < sizeof(Config); i++){
        buffer[i] = configfile.read();
    }
            
    configfile.close();
    SPIFFS.end();
}
}