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
        WiFi.disconnect(true);
        delay(1000);
        WiFi.onEvent(std::bind(&WifiManager::WiFiEvent, this, _1, _2));
        connectToWifi();
    }

    void WifiManager::connectToWifi() {
        // Check if there's some valid configuration
        NetworkConfig* config = &ConfigService::CONFIG.networkConfig;

        // if (strlen(config->ssid.empty() || config->password.empty()) {
        //     Serial.println(TAG + "No config found");
        //     startAP();
        //     return;
        // }
        // startAP();

        // Serial.printf("testvalue: %u\n", config->corruptme);
        // Serial.printf("SSID: %s\n", (char*) config->ssid);
        // Serial.printf("password: %s\n", (char*) config->password);
        Serial.printf("%sConnecting to previously stored Wifi-network: %s\n", TAG.c_str(), (char*) config->ssid);
        if (WiFi.begin((char*) config->ssid, (char*) config->password) == WL_CONNECT_FAILED) {
            // startAP();
        }
    }

    void WifiManager::startAP() {
        Serial.println(TAG + "Starting AP");
        WiFi.mode(WIFI_MODE_AP);
        delay(5000);
        WiFi.enableAP(true);
        const IPAddress ip(192, 168, 1, 1);
        const IPAddress subnet(255, 255, 255, 0);
        WiFi.softAPConfig(ip, ip, subnet);
        WiFi.softAP(AP_SSID, AP_PASSWORD);


        Serial.println("===");
        delay(5000);
        WiFi.softAP(AP_SSID, AP_PASSWORD);
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
                    // WiFi.reconnect();
                // } else {
                    // WiFi.enableSTA(false);
                if (reconnectCount == 10) {
                    startAP();
                }
                break;
            case SYSTEM_EVENT_STA_CONNECTED:
                Serial.println(TAG + "STA Connected");
                reconnectCount = 0;
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

            //0 SYSTEM_EVENT_WIFI_READY,           /**< ESP32 WiFi ready */
            //1 SYSTEM_EVENT_SCAN_DONE,                /**< ESP32 finish scanning AP */
            //2 SYSTEM_EVENT_STA_START,                /**< ESP32 station start */
            //3 SYSTEM_EVENT_STA_STOP,                 /**< ESP32 station stop */
            //4 SYSTEM_EVENT_STA_CONNECTED,            /**< ESP32 station connected to AP */
            //5 SYSTEM_EVENT_STA_DISCONNECTED,         /**< ESP32 station disconnected from AP */
            //6 SYSTEM_EVENT_STA_AUTHMODE_CHANGE,      /**< the auth mode of AP connected by ESP32 station changed */
            //7 SYSTEM_EVENT_STA_GOT_IP,               /**< ESP32 station got IP from connected AP */
            //8 SYSTEM_EVENT_STA_LOST_IP,              /**< ESP32 station lost IP and the IP is reset to 0 */
            //9 SYSTEM_EVENT_STA_WPS_ER_SUCCESS,       /**< ESP32 station wps succeeds in enrollee mode */
            //10 SYSTEM_EVENT_STA_WPS_ER_FAILED,        /**< ESP32 station wps fails in enrollee mode */
            //11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,       /**< ESP32 station wps timeout in enrollee mode */
            //12 SYSTEM_EVENT_STA_WPS_ER_PIN,           /**< ESP32 station wps pin code in enrollee mode */
            //13 SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP,   /*!< ESP32 station wps overlap in enrollee mode */
            //14 SYSTEM_EVENT_AP_START,                 /**< ESP32 soft-AP start */
            //15 SYSTEM_EVENT_AP_STOP,                  /**< ESP32 soft-AP stop */
            //16 SYSTEM_EVENT_AP_STACONNECTED,          /**< a station connected to ESP32 soft-AP */
            //17 SYSTEM_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from ESP32 soft-AP */
            //18 SYSTEM_EVENT_AP_STAIPASSIGNED,         /**< ESP32 soft-AP assign an IP to a connected station */
            //19 SYSTEM_EVENT_AP_PROBEREQRECVED,        /**< Receive probe request packet in soft-AP interface */
            // SYSTEM_EVENT_GOT_IP6,                  /**< ESP32 station or ap or ethernet interface v6IP addr is preferred */
            // SYSTEM_EVENT_ETH_START,                /**< ESP32 ethernet start */
            // SYSTEM_EVENT_ETH_STOP,                 /**< ESP32 ethernet stop */
            // SYSTEM_EVENT_ETH_CONNECTED,            /**< ESP32 ethernet phy link up */
            // SYSTEM_EVENT_ETH_DISCONNECTED,         /**< ESP32 ethernet phy link down */
            // SYSTEM_EVENT_ETH_GOT_IP,               /**< ESP32 ethernet got IP from connected AP */
        }
    }



}