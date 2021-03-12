#pragma once

#include <WiFi.h>
#include "configservice.h"

extern QueueHandle_t xWifiConfigChangedQueue;

namespace qlocktoo {
    class WifiManager {
        private:
            const char* AP_SSID = "Qlocktoo";
            const char* AP_PASSWORD = "qlocktoo";
            
            wifi_event_id_t wm_event_id;
            void WiFiEvent(WiFiEvent_t event, system_event_info_t info);
            void connectToWifi();

        public:
            WifiManager();
            ~WifiManager();
            void begin();
            void startAP();
            
            void updateConfig(NetworkConfig &config);
    };
}