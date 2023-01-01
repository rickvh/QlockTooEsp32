#include "configservice.h"
#include <SPIFFS.h>
#include "Ticker.h"

using namespace qlocktoo;
    
Config ConfigService::CONFIG;
bool ConfigService::connectedToWifi;
Ticker ConfigService::flashTicker;

void ConfigService::saveInternally() {
    auto configfile = SPIFFS.open(CONFIGFILE, "w");
    if (!configfile) {
        Serial.println("Failed to save configfile");
    }

    configfile.write((unsigned char*) &CONFIG, sizeof(Config));
    configfile.close();
    ESP_LOGI(LOG_TAG, "Config saved to filesystem");
}

void ConfigService::saveEventually() {
    ESP_LOGI(LOG_TAG, "Configuration scheduled to be stored in flash");
    flashTicker.once(MIN_INTERVAL_SECONDS, &saveInternally);
}

void ConfigService::init() {
    if (!SPIFFS.exists(CONFIGFILE)) {
        Serial.println("No configfile found...Initializing defaults");
        CONFIG = Config();
        return;
    }

    auto configfile = SPIFFS.open(CONFIGFILE);
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
        strcpy(CONFIG.networkConfig.hostname, "qlocktoo"); // TODO: add unique suffix based on MAC
        strcpy(CONFIG.networkConfig.ssid, "");
        strcpy(CONFIG.networkConfig.password, "");
    }

// reset anyway, as we're still trying to fix some issues
// CONFIG = Config();

    configfile.close();
    printConfig();
}

void ConfigService::printConfig() {
    Serial.println("Configuration:");
    Serial.printf("- Hostname: %s\n", (char*) CONFIG.networkConfig.hostname);
    Serial.printf("- AP SSID: %s\n", (char*) CONFIG.networkConfig.ssid);
    Serial.printf("- AP password: %s\n", (char*) CONFIG.networkConfig.password);
}
}