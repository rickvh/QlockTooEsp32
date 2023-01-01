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
        ESP_LOGE(LOG_TAG, "Failed to save config to filesystem");
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
        ESP_LOGI(LOG_TAG, "No configfile found...Initializing defaults");
        CONFIG = Config();
        return;
    }

    auto configfile = SPIFFS.open(CONFIGFILE);
    if (!configfile) {
        ESP_LOGE(LOG_TAG, "Failed to open configfile");
    }

    // check if filesize matches structsize
    if (sizeof(Config) == configfile.size()) {
        unsigned char * buffer = (unsigned char *) &CONFIG;
        for(uint16_t i = 0; i < sizeof(Config); i++){
            buffer[i] = configfile.read();
        }
        ESP_LOGI(LOG_TAG, "Config loaded from filesystem");
    } else {
        ESP_LOGE(LOG_TAG, "Config corrupt...Initializing defaults");
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
    ESP_LOGI(LOG_TAG, "Hostname: %s", (char*) CONFIG.networkConfig.hostname);
    ESP_LOGI(LOG_TAG, "AP SSID: %s", (char*) CONFIG.networkConfig.ssid);
    ESP_LOGI(LOG_TAG, "AP password: %s", (char*) CONFIG.networkConfig.password);
    ESP_LOGI(LOG_TAG, "Clock color ItIs (H, S, B): %f, %f, %f", CONFIG.clockConfig.colorItIs.H, CONFIG.clockConfig.colorItIs.S, CONFIG.clockConfig.colorItIs.B);
    ESP_LOGI(LOG_TAG, "Clock color Words (H, S, B): %f, %f, %f", CONFIG.clockConfig.colorWords.H, CONFIG.clockConfig.colorWords.S, CONFIG.clockConfig.colorWords.B);
    ESP_LOGI(LOG_TAG, "Clock color Hour (H, S, B): %f, %f, %f", CONFIG.clockConfig.colorHour.H, CONFIG.clockConfig.colorHour.S, CONFIG.clockConfig.colorHour.B);
}