#include "configservice.h"
#include <SPIFFS.h>

namespace qlocktoo {
Config ConfigService::CONFIG;

void ConfigService::save() {
    if (!SPIFFS.begin()) {
        Serial.println("Filesystem cannot be opened");
        return;
    }

    File configfile = SPIFFS.open(CONFIGFILE, "w");
    if (!configfile) {
        Serial.println("Failed to save configfile");
    }

    configfile.write((unsigned char*) &CONFIG, sizeof(Config));

    configfile.close();
    SPIFFS.end();

    Serial.println("Config saved to filesystem");
}

void ConfigService::init() {
    if (!SPIFFS.begin(true) || !SPIFFS.exists(CONFIGFILE)) {
        Serial.println("No configfile found...Initializing defaults");
        CONFIG = Config();
        return;
    }

    File configfile = SPIFFS.open(CONFIGFILE);
    if (!configfile) {
        Serial.println("Failed to open configfile");
    }

    unsigned char * buffer = (unsigned char *) &CONFIG;
    for(uint16_t i = 0; i < sizeof(Config); i++){
        buffer[i] = configfile.read();
    }
            
    configfile.close();
    SPIFFS.end();

    Serial.println("Config loaded from filesystem");
}
}