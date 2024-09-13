#include "wifimanager.h"
#include <WiFi.h>
#include "configservice.h"
#include "mode.h"

using namespace qlocktoo;

WifiManager::WifiManager() {
}

WifiManager::~WifiManager() {
    WiFi.removeEvent(wm_event_id);
}

void WifiManager::begin() {
    using namespace std::placeholders;
    WiFi.disconnect(true); // for stability
    delay(1000);
    WiFi.onEvent(std::bind(&WifiManager::WiFiEvent, this, _1, _2));
    connectToWifi();
}

void WifiManager::connectToWifi() {
    NetworkConfig* config = &ConfigService::CONFIG.networkConfig;
    ESP_LOGI(LOG_TAG, "Connecting to previously stored Wifi-network: %s", (char*) config->ssid);
    reconnectCount = 0;
    WiFi.mode(WIFI_MODE_STA);
    WiFi.enableSTA(true);
    if (WiFi.begin((char*) config->ssid, (char*) config->password) == WL_CONNECT_FAILED) {
        startAP();
    };
}

void WifiManager::startAP() {
    ESP_LOGI(LOG_TAG, "Starting AP");
    WiFi.mode(WIFI_MODE_AP);
    delay(5000); // delay for stability  
    WiFi.enableAP(true);
    const IPAddress ip(192, 168, 1, 1);
    const IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(ip, ip, subnet);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    showWifiAnimation();
}

void WifiManager::updateConfig(NetworkConfig &config) {
    NetworkConfig* currentConfig = &ConfigService::CONFIG.networkConfig;
    if (config.hostname != currentConfig->hostname) {
        ESP_LOGI(LOG_TAG, "Hostname changed");
        strcpy(currentConfig->hostname, config.hostname);
        ConfigService::saveEventually();
    }
    if (config.ssid != currentConfig->ssid || (strlen(config.password) > 0 && config.password != currentConfig->password)) {
        ESP_LOGI(LOG_TAG, "Wifi settings changed");
        strcpy(currentConfig->ssid, config.ssid);
        strcpy(currentConfig->password, config.password);
        ConfigService::saveEventually();
        connectToWifi();
    } else {
        ESP_LOGI(LOG_TAG, "Wifi settings unchanged");
    }
}

void WifiManager::WiFiEvent(arduino_event_id_t event, arduino_event_info_t info) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            ESP_LOGI(LOG_TAG, "STA Disconnected: %u", info.wifi_sta_disconnected.reason);
            ConfigService::connectedToWifi = false;
            reconnectCount++;
            ESP_LOGI(LOG_TAG, "Retry (%u)", reconnectCount);
            WiFi.reconnect();
            showWifiAnimation();
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            ESP_LOGI(LOG_TAG, "STA Connected");
            ConfigService::connectedToWifi = true;
            reconnectCount = 0;
            break;
        case ARDUINO_EVENT_WIFI_READY:
            ESP_LOGI(LOG_TAG, "Ready");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            ESP_LOGI(LOG_TAG, "STA Got IP: %s", WiFi.localIP().toString());
            WiFi.enableAP(false);
            showClock();
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            ESP_LOGI(LOG_TAG, "STA Start");
            break;
        default:
            ESP_LOGI(LOG_TAG, "event %u", event);
            break;
    }
}

void WifiManager::showWifiAnimation() {
    auto newMode = Mode::WifiConnecting;
    xQueueOverwrite(xChangeAppQueue, &newMode);
}

void WifiManager::showSetupRequired() {
    auto newMode = Mode::WifiSetupRequired;
    xQueueOverwrite(xChangeAppQueue, &newMode);
}

void WifiManager::showClock() {
    auto newMode = Mode::Clock;
    xQueueOverwrite(xChangeAppQueue, &newMode);
}