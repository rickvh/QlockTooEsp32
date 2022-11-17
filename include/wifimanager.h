#pragma once

#include <WiFi.h>
#include "configservice.h"

extern QueueHandle_t xChangeAppQueue;
extern QueueHandle_t xWifiConfigChangedQueue;

namespace qlocktoo {
    class WifiManager {
        private:
            const char* AP_SSID = "Qlocktoo";
            const char* AP_PASSWORD = "qlocktoo";
            
            wifi_event_id_t wm_event_id;
            uint8_t reconnectCount;
            void WiFiEvent(arduino_event_id_t event, arduino_event_info_t info);
            void connectToWifi();
            void showWifiAnimation();
            void showSetupRequired();
            void showClock();

        public:
            WifiManager();
            ~WifiManager();
            void begin();
            void startAP();
            
            void updateConfig(NetworkConfig &config);
    };
}