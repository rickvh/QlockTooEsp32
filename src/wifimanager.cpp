#include "wifimanager.h"
#include <WiFi.h>
#include "control.h"
#include "configservice.h"

namespace qlocktoo {
    const String TAG = "WIFI::";
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
        Serial.printf("%sConnecting to previously stored Wifi-network: %s\n", TAG.c_str(), (char*) config->ssid);
        reconnectCount = 0;
        WiFi.mode(WIFI_MODE_STA);
        WiFi.enableSTA(true);
        if (WiFi.begin((char*) config->ssid, (char*) config->password) == WL_CONNECT_FAILED) {
            startAP();
        };
    }

    void WifiManager::startAP() {
        Serial.println(TAG + "Starting AP");
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
        if (config.ssid != currentConfig->ssid || config.password != currentConfig->password) {
            Serial.println("Wifi settings changed");
            strcpy(currentConfig->ssid, config.ssid);
            strcpy(currentConfig->password, config.password);
            ConfigService::save();
            connectToWifi();
        } else {
            Serial.println("Wifi settings unchanged");
        }
    }

    void WifiManager::WiFiEvent(WiFiEvent_t event, system_event_info_t info) {
        switch (event) {
            case SYSTEM_EVENT_STA_DISCONNECTED:
                Serial.println(TAG + "STA Disconnected: " + info.disconnected.reason);
                reconnectCount++;
                Serial.printf("Retry (%u/10)\n", reconnectCount);
                WiFi.reconnect();
                if (reconnectCount == 10) {
                    startAP();
                    showSetupNeeded();
                } else {
                    showWifiAnimation();
                }
                break;
            case SYSTEM_EVENT_STA_CONNECTED:
                Serial.println(TAG + "STA Connected");
                reconnectCount = 0;
                showClock();
                break;
            case SYSTEM_EVENT_WIFI_READY:
                Serial.println(TAG + "ready");
                break;
            case SYSTEM_EVENT_STA_GOT_IP:
                Serial.println(TAG + "STA Got IP: " + WiFi.localIP().toString());
                WiFi.enableAP(false);
                break;
            case SYSTEM_EVENT_STA_START:
                Serial.println(TAG + "STA Start");
                break;
            default:
                Serial.println(TAG + "event " + event);
                break;
        }
    }

    void WifiManager::showWifiAnimation() {
        auto newMode = Mode::WifiConnecting;
        xQueueSend(xChangeAppQueue, &newMode, 0);
    }

    void WifiManager::showSetupNeeded() {
        auto newMode = Mode::WifiSetupNeeded;
        xQueueSend(xChangeAppQueue, &newMode, 0);
    }

    void WifiManager::showClock() {
        auto newMode = Mode::Clock;
        xQueueSend(xChangeAppQueue, &newMode, 0);
    }
}