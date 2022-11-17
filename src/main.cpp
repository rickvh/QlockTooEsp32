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
#include "control.h"
#include "webinterface.h"
#include "swirl.h"
#include "clock.h"
#include "tz.h"
#include "image.h"
#include "animation.h"
#include "buildinformation.h"
#include <SPIFFS.h>


using namespace qlocktoo;
using namespace std;

const uint8_t   timeZone        = 1;     // Central European Time
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";

WifiManager Wifi;
Webinterface webinterface(80);

void changeMode(qlocktoo::Mode mode);
void listPartitions();
void listFiles();
void runAppTask(void * parameter);
void runImportantStuffTask(void * parameter);

TaskHandle_t currentAppTask = NULL;
App* currentApp = NULL;
qlocktoo::Mode currentMode = Mode::Unknown;

/**
 * FreeRTOS stuff
 */
QueueHandle_t xChangeAppQueue = NULL;
QueueHandle_t xWifiConfigChangedQueue = NULL;
QueueHandle_t xClockConfigQueue = NULL;


void setupWifi() {
  Serial.println("Connecting to Wifi...");
  // Serial.println("Connecting to Wifi...");
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  
  // set mode to CONNECTING_WIFI
  // check if wifi config availiable
  // yes: start connecting

  // listeners
  //  - wifi connected: set mode TO CLOCK
  //  - wifi lost / failed: set mode NO_WIFI

  // start AP
  // - wifi config received:
  //   - stop AP
  //   - setupWifi()

  // WiFi.printDiag(Serial);
  // WiFi.onEvent(std::bind(&WiFiManager::WiFiEvent,this,_1,_2));


  // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // debugE("Connection Failed! Rebooting...");
    // delay(5000);
    // ESP.restart();
  // }

  Wifi.begin();
  Serial.printf("Wifi connected: %s\n", WiFi.localIP());

  #if defined USE_MDNS && defined HOST_NAME
	if (MDNS.begin(HOST_NAME)) {
		Serial.printf("* MDNS responder started. Hostname -> ", HOST_NAME);
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("telnet", "tcp", 23);
	}
  #else
  Serial.println("mDNS disabled");
  #endif
}

void setupNTP() {
  // const long  gmtOffset_sec = 3600;
  // const int   daylightOffset_sec = 3600;
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  configTzTime(getTimezone().c_str(), "pool.ntp.org");
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
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      changeMode(Mode::Error);
      delay(5000);
    });

  ArduinoOTA.begin();
}

void setupDisplay() {
  Display::begin();
}

// void processCmdRemoteDebug() {
//   using namespace qlocktoo;

//   vector<string> tokens = split(Debug.getLastCommand().c_str(), ' ');
//   if (! tokens.size()) return;

//   uint8_t index = 0;
//   string const& cmd = tokens[index++];

//   if (cmd == "swirl") {
//     changeMode(Mode::Swirl);
//     Serial.println("* Mode set to SWIRL");
//   } else if (cmd == "mem") {
//     auto free = xPortGetFreeHeapSize();
//     Serial.printf("Free memory: %u\n", free);
//   } else if (cmd == "img") {
//     if (tokens.size() != 2) {
//       Serial.println("Command \'img\' requires 1 parameter: [xmas, snow, wifi]");
//       return;
//     }

//     if (tokens[1] == "xmas") {
//       changeMode(Mode::Xmas);
//       Serial.println("* Image set to XMAS");
//     } else if (tokens[1] == "snow") {
//       changeMode(Mode::Snow);
//       Serial.println("* Image set to SNOW");
//     } else if (tokens[1] == "wifi") {
//       changeMode(Mode::OTAinProgress);
//       Serial.println("* Image set to WIFI");
//     } else {
//       return;
//     }
//   }
//   else if (cmd == "text") {
//     changeMode(Mode::Text);
//     Serial.println("* Mode set to TEXT");
//   } else if (cmd == "clock") {
//     changeMode(Mode::Clock);
//     Serial.println("* Mode set to CLOCK");
//   } else if (cmd == "ls") {
//     Serial.println("Partitions:");
//     listPartitions();
//     Serial.println("Files:");
//     listFiles();
//   } else if (cmd == "version") {
//     Serial.printlf("%s\n", build_str);
//   }
// }


void setupLogging() {

  // Debug.begin("qlocktoo"); // Initialize the WiFi server
	// Debug.setResetCmdEnabled(true); // Enable the reset command
  // // Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	// Debug.showColors(true); // Colors

	// String helpCmd = "clear - Clear display\n";
  // helpCmd.concat("red - Set red color");
	// helpCmd.concat("rgb <r,g,b> - Set RGB value");

	// Debug.setHelpProjectsCmds(helpCmd);
	// Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
	// Debug.initDebugger(debugGetDebuggerEnabled, debugHandleDebugger, debugGetHelpDebugger, debugProcessCmdDebugger); // Set the callbacks

	// debugInitDebugger(&Debug); // Init the debugger

  // if (debugAddFunctionVoid("benchInt", &benchInt) >= 0) {
  // debugSetLastFunctionDescription("To run a benchmark of integers");
// }

}

void changeMode(Mode mode) {
  if (currentMode == mode) {
    return;
  }
  currentMode = mode;

  if (currentApp) {
    while (!currentApp->canTerminate) {
      Serial.println("wait till termination is safe...");
      delay(50);
    }
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
    case Mode::Swirl:
      currentApp = new Swirl();
      break;
    default:
      return;
  }

  //debugI("Start new app: %s", currentApp->getApp());
  Serial.println("Start new app");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting QlockToo");
  Serial.printf("%s\n", build_str);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS cannot be opened");
  };

  xChangeAppQueue = xQueueCreate(1, sizeof(Mode));
  xWifiConfigChangedQueue = xQueueCreate(1, sizeof(NetworkConfig));
  xClockConfigQueue = xQueueCreate(1, sizeof(ClockConfig));
  Serial.println("Queues created");

  Serial.println("setup persistable configuration");
  ConfigService::init();
  Serial.println("setup Wifi");
  setupWifi();
  Serial.println("setup OTA");
  setupOTA();
  Serial.println("setup Logging");
  setupLogging();
  Serial.println("setup LED Display");
  setupDisplay();
  Serial.println("setup NTP");
  setupNTP();
  Serial.println("setup Webinterface");
  webinterface.begin();
  
  

  xTaskCreatePinnedToCore(runImportantStuffTask, "OTA_and_Debug", 8192, NULL, 2, &currentAppTask, 0);

  // Start in Clock mode
  changeMode(Mode::Clock);
  xTaskCreatePinnedToCore(runAppTask, "App", 8192, NULL, 1, &currentAppTask, 1);
}

// Arduino loop. Most features are implemented as RTOS tasks and are therefore not handled inside this loop.
void loop() {
  delay(300);

  Mode newMode;
  if (xQueueReceive(xChangeAppQueue, &newMode, pdMS_TO_TICKS(300)) == pdTRUE) {
    Serial.println("Mode changed");
    changeMode(newMode);
  }

  NetworkConfig networkConfig;
  if (xQueueReceive(xWifiConfigChangedQueue, &networkConfig, 0) == pdTRUE) {
    Serial.println("Wifi settings updated");
    Serial.printf("SSID: %s\n", (char*) networkConfig.ssid);
    Serial.printf("PWD: %s\n", (char*) networkConfig.password);
    Wifi.updateConfig(networkConfig);
  }
};

void runImportantStuffTask(void * parameter) {
  for (;;) {
    ArduinoOTA.handle();
    delay(300);
  }
}


void runAppTask(void * parameter) {
  for (;;) {
    if (currentApp) {
      currentApp->handle();
    }

    // For safety reasons - make sure there's at least a chance for FreeRTOS to switch tasks.
    delay(10);
  }
}

void listPartitions(void)
{
  size_t ul;
  esp_partition_iterator_t _mypartiterator;
  const esp_partition_t *_mypart;
  ul = spi_flash_get_chip_size();
  Serial.printf("Flash chip size: %u\n", ul);
  Serial.println("Partiton table:");
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      Serial.printf("%x - %x - %x - %x - %s - %i\n", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      Serial.printf("%x - %x - %x - %x - %s - %i\n", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
}

void listFiles() {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while(file){
    Serial.printf("File: %s\n", file.name());
 
    file = root.openNextFile();
  }
}
