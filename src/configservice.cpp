#include "configservice.h"
#include <SPIFFS.h>

namespace qlocktoo {
Config ConfigService::CONFIG;

void ConfigService::save() {
    File configfile = SPIFFS.open(CONFIGFILE, "w");
    if (!configfile) {
        Serial.println("Failed to save configfile");
    }

    configfile.write((unsigned char*) &CONFIG, sizeof(Config));
    configfile.close();

    Serial.println("Config saved to filesystem");
}

void ConfigService::init() {
    if (!SPIFFS.exists(CONFIGFILE)) {
        Serial.println("No configfile found...Initializing defaults");
        CONFIG = Config();
        return;
    }

    File configfile = SPIFFS.open(CONFIGFILE);
    if (!configfile) {
        Serial.println("Failed to open configfile");
    }

    // check if filesize matches structsize
    if (sizeof(Config) == configfile.size()) {
        unsigned char * buffer = (unsigned char *) &CONFIG;
        for(uint16_t i = 0; i < sizeof(Config); i++){
            buffer[i] = configfile.read();
        }
        Serial.println("Config loaded from filesystem");
    } else {
        Serial.println("Config corrupt...Initializing defaults");
        CONFIG = Config();
        strcpy(CONFIG.networkConfig.hostname, "qlocktoo");
        strcpy(CONFIG.networkConfig.ssid, "");
        strcpy(CONFIG.networkConfig.password, "");
    }

// reset anyway, as we're still trying to fix some issues
// CONFIG = Config();

    // configfile.close();
    printConfig();
}

void ConfigService::printConfig() {
    Serial.println("Configuration:");
    Serial.printf("- Hostname: %s\n", (char*) CONFIG.networkConfig.hostname);
    Serial.printf("- AP SSID: %s\n", (char*) CONFIG.networkConfig.ssid);
    Serial.printf("- AP password: %s\n", (char*) CONFIG.networkConfig.password);
}
}