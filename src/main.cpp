#define USE_MDNS        1
#define HOST_NAME       "qlocktoo"

//#define DEBUG_DISABLED // uncomment for production release
#define WEBSOCKET_DISABLED // disbale logging via websockets

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <string>
#include <utility>
#include "utils/stringutils.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "wifimanager.h"
#include <DNSServer.h>
#include "ESPmDNS.h"
#include "mode.h"
#include "webinterface.h"
#include "apps/ledtest.h"
#include "apps/swirl.h"
#include "apps/clock.h"
#include "tz.h"
#include "image.h"
#include "animation.h"
#include "buildinfo.h"
#include <SPIFFS.h>


using namespace qlocktoo;
using namespace std;

const uint8_t   timeZone = 1;     // Central European Time
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";
static constexpr const char* LOG_TAG = "main";

void changeMode(qlocktoo::Mode mode);
void listPartitions();
void listFiles();

App* currentApp = NULL;
qlocktoo::Mode currentMode = Mode::Unknown;
WifiManager Wifi;
Webinterface webinterface(80);


/**
 * FreeRTOS stuff
 */
void runAppTask(void * parameter);
void runOtaTask(void * parameter);
TaskHandle_t otaTask = NULL;
TaskHandle_t currentAppTask = NULL;
QueueHandle_t xChangeAppQueue = NULL;
QueueHandle_t xWifiConfigChangedQueue = NULL;

void setupIO() {
  #ifdef BOARD_FREESTYLE
  /**
   * 'Freestyle' hardware layout has non-addressable led's indicating 1 to 4 minutes after the displayed text.
  */
  const int pinMinute1 = 23;
  const int pinMinute2 = 22;
  const int pinMinute3 = 1;
  const int pinMinute4 = 3;
  pinMode(pinMinute1, OUTPUT);
  pinMode(pinMinute2, OUTPUT);
  pinMode(pinMinute3, OUTPUT);
  pinMode(pinMinute4, OUTPUT);
  digitalWrite(pinMinute1, LOW);
  digitalWrite(pinMinute1, LOW);
  digitalWrite(pinMinute1, LOW);
  digitalWrite(pinMinute1, LOW);
  #endif
}

void setupWifi() {
  ESP_LOGI(LOG_TAG, "Connecting to Wifi...");
  Wifi.begin();
  ESP_LOGI(LOG_TAG, "Wifi connected: %s", WiFi.localIP().toString());

  #if defined USE_MDNS && defined HOST_NAME
	if (MDNS.begin(HOST_NAME)) {
		ESP_LOGI(LOG_TAG, "* MDNS responder started. Hostname: %s", HOST_NAME);
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("telnet", "tcp", 23);
	}
  #else
  ESP_LOGI(LOG_TAG, "mDNS disabled");
  #endif
}

void setupNTP() {
  const long  gmtOffset_sec = 3600;
  const int   daylightOffset_sec = 3600;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  configTzTime(getTimezone().c_str(), ntpServer);
}

void setupOTA() {
  // Port defaults to 3232
  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // Unmount SPIFFS
      SPIFFS.end();
      changeMode(Mode::OTAinProgress);
      ESP_LOGI(LOG_TAG, "Start updating %s", type);
    })
    .onEnd([]() {
      ESP_LOGI(LOG_TAG, "\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      ESP_LOGI(LOG_TAG, "Progress: %u%%", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      ESP_LOGE(LOG_TAG, "Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) ESP_LOGE(LOG_TAG, "Auth Failed");
      else if (error == OTA_BEGIN_ERROR) ESP_LOGE(LOG_TAG, "Begin Failed");
      else if (error == OTA_CONNECT_ERROR) ESP_LOGE(LOG_TAG, "Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) ESP_LOGE(LOG_TAG, "Receive Failed");
      else if (error == OTA_END_ERROR) ESP_LOGE(LOG_TAG, "End Failed");
      changeMode(Mode::Error);
      delay(5000);
    });

  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200);
  ESP_LOGI(LOG_TAG, "QlockToo version %s", BuildInfo::version);

  if (!SPIFFS.begin(true)) {
    ESP_LOGE(LOG_TAG, "SPIFFS cannot be opened\n");
  };

  ESP_LOGI(LOG_TAG, "Setup I/O");
  setupIO();
  ESP_LOGI(LOG_TAG, "Setup RTOS queues");
  xChangeAppQueue = xQueueCreate(1, sizeof(Mode));
  xWifiConfigChangedQueue = xQueueCreate(1, sizeof(NetworkConfig));
  ESP_LOGI(LOG_TAG, "Setup persistent configuration");
  ConfigService::init();
  ESP_LOGI(LOG_TAG, "Setup Wifi");
  setupWifi();
  ESP_LOGI(LOG_TAG, "Setup OTA");
  setupOTA();
  ESP_LOGI(LOG_TAG, "Setup LED Display");
  Display::begin();
  ESP_LOGI(LOG_TAG, "Setup NTP");
  setupNTP();
  ESP_LOGI(LOG_TAG, "Setup Webinterface");
  webinterface.begin();

  // Start OTA and Clock tasks
  xTaskCreatePinnedToCore(runOtaTask, "OTA", 8192, NULL, 2, &otaTask, 1);
  

  changeMode(Mode::Clock);
  xTaskCreatePinnedToCore(runAppTask, "App", 8192, NULL, 1, &currentAppTask, 0);
}

// Arduino loop. Most features are implemented as RTOS tasks and are therefore not handled inside this loop.
void loop() {
  delay(300);

  NetworkConfig networkConfig;
  if (xQueueReceive(xWifiConfigChangedQueue, &networkConfig, 0) == pdTRUE) {
    ESP_LOGI(LOG_TAG, "Wifi settings updated");
    ESP_LOGI(LOG_TAG, "SSID: %s", (char*) networkConfig.ssid);
    ESP_LOGD(LOG_TAG, "PWD: %s", (char*) networkConfig.password);
    Wifi.updateConfig(networkConfig);
  }
};

void runOtaTask(void * parameter) {
  for (;;) {
    ArduinoOTA.handle();
    delay(300);
  }
}


void runAppTask(void * parameter) {
  for (;;) {
    Mode newMode;
    if (xQueueReceive(xChangeAppQueue, &newMode, pdMS_TO_TICKS(300)) == pdTRUE) {
      ESP_LOGI(LOG_TAG, "Mode changed: %s -> %s", modeToString(currentMode), modeToString(newMode));
      changeMode(newMode);
    }
    
    if (currentApp) {
      currentApp->handle();
    }

    // For safety reasons - make sure there's at least a chance for FreeRTOS to switch tasks.
    delay(10);
  }
}

void changeMode(qlocktoo::Mode mode) {
  if (currentMode == mode) {
    return;
  }
  currentMode = mode;

  if (currentApp) {
    currentApp->stop();
    delete currentApp;
    currentApp = NULL;
  }

  switch (currentMode) {
    case Mode::Clock:
      currentApp = new Clock();
      break;
    case Mode::WifiConnecting:
    case Mode::OTAinProgress:
      currentApp = new Animation(Animation::Preset::Wifi);
      break;
    case Mode::Xmas:
      currentApp = new Image(Image::Preset::XmasTree);
      break;
    case Mode::Snow:
      currentApp = new Image(Image::Preset::Snowman);
      break;
    case Mode::WifiSetupRequired:
    case Mode::Error:
      currentApp = new Image(Image::Preset::Error);
      break;
    case Mode::Ledtest:
      currentApp = new Ledtest();
      break;
    case Mode::Swirl:
      currentApp = new Swirl();
      break;
    default:
      return;
  }

  ESP_LOGI(LOG_TAG, "Start new app '%s'", modeToString(currentMode));
}

void listPartitions(void)
{
  size_t ul;
  esp_partition_iterator_t _mypartiterator;
  const esp_partition_t *_mypart;
  ul = spi_flash_get_chip_size();
  ESP_LOGI(LOG_TAG, "Flash chip size: %u", ul);
  ESP_LOGI(LOG_TAG, "Partiton table:");
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      ESP_LOGI(LOG_TAG, "%x - %x - %x - %x - %s - %i", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      ESP_LOGI(LOG_TAG, "%x - %x - %x - %x - %s - %i", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
}

void listFiles() {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while(file){
    ESP_LOGI(LOG_TAG, "File: %s", file.name());
    file = root.openNextFile();
  }
}
