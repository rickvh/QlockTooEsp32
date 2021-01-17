#define VERSION         "0.1"

// #define CONFIG_FREERTOS_USE_TRACE_FACILITY              1
// #define CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS  1

#define USE_MDNS        1
#define HOST_NAME       "qlocktoo"

//#define DEBUG_DISABLED // uncomment for production release
#define WEBSOCKET_DISABLED // disbale logging via websockets
// #include "utils/split.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <RemoteDebugger.h> 
#include <string>
#include <utility>
#include "utils/stringutils.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "display.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include "ESPmDNS.h"
// #include <WiFiUdp.h>
#include "control.h"
#include "webinterface.h"
#include "swirl.h"
#include "clock.h"
#include "tz.h"
#include "image.h"

#include <SPIFFS.h>
#include "wifipassword.h"


#define NTP_TIMEOUT 1500

using namespace qlocktoo;
using namespace std;

const char* build_str = "Version: " VERSION " " __DATE__ " " __TIME__;
const bool checkModeAlreadyActive = false;

const uint8_t   timeZone        = 1;     // Central European Time
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";

RemoteDebug Debug;
Webinterface webinterface(80, Debug);
shared_ptr<Image> currentImage = shared_ptr<Image>(new Image());

void changeMode(qlocktoo::Mode mode);
void listPartitions();
void listFiles();
void handleSwirl();
void handleText();
void handleImage();
void animateWifi(void * parameter);
void showSingleImageTask(void * parameter);
void showSwirlTask(void * parameter);
void showClockTask(void * parameter);

uint8_t frame = 1;
uint8_t x_start = 1;
uint8_t y_start = 1;
uint16_t hue = 0;
uint8_t saturation = 255, brightness = 255;

uint8_t speed = 50, acc = 50, dir = 1;

int x    = 11;
int pass = 0;

// TaskHandle_t pvCreatedTask = NULL;
TaskHandle_t currentAppTask = NULL;
shared_ptr<App> currentApp = NULL;

qlocktoo::Mode currentMode = qlocktoo::NOT_SET;




void setupDisplay() {
  
  

  // display.setTextWrap(false);
  // display.setBrightness(255);
  // display.setTextColor(display.Color(0, 200, 0));


  // ledstrip.begin(LEDSTRIP_PIN, 110);
  // for(uint8_t i = 0; i < 50; i+=2) {
  //   ledstrip.color(i, 255, 255, 0, 0);
  // }
  // for(uint8_t i = 50; i < 110; i+=2) {
  //   ledstrip.color(i, 0, 0, 100, 100);
  // }
}

void setupWifi() {
  debugI("Connecting to Wifi...");
  Serial.println("Connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    debugE("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  debugI("Wifi connected");
  Serial.print("Wifi connected: ");
  Serial.println(WiFi.localIP());


  #if defined USE_MDNS && defined HOST_NAME
	if (MDNS.begin(HOST_NAME)) {
		debugI("* MDNS responder started. Hostname -> ");
		debugI(HOST_NAME);
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("telnet", "tcp", 23);
	}
  #else
  debugI("mDNS disabled");
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
      changeMode(qlocktoo::NO_WIFI);
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
      changeMode(qlocktoo::Mode::ERROR);
      delay(5000);
    });

  ArduinoOTA.begin();
}

void processCmdRemoteDebug() {
  using namespace qlocktoo;
  
  vector<string> tokens = split(Debug.getLastCommand().c_str(), ' ');
  if (! tokens.size()) return;
  
  uint8_t index = 0;
  string const& cmd = tokens[index++];
  
  if (cmd == "swirl") {
    changeMode(SWIRL);
    debugI("* Mode set to SWIRL");
  } else if (cmd == "mem") {
    auto free = xPortGetFreeHeapSize();
    debugI("Free memory: %u", free);
  } else if (cmd == "img") {
    if (tokens.size() != 2) {
      debugE("Command \'img\' requires 1 parameter: [xmas, snow, wifi]");
      return;
    }

    if (tokens[1] == "xmas") {
      currentImage = shared_ptr<Image>(new Image(Image::Preset::XMAS_TREE));
      debugI("* Image set to XMAS");
    } else if (tokens[1] == "snow") {
      currentImage = shared_ptr<Image>(new Image(Image::Preset::SNOWMAN));
      debugI("* Image set to SNOW");
    } else if (tokens[1] == "wifi") {
      currentImage = shared_ptr<Image>(new Image(Image::Preset::WIFI1));
      debugI("* Image set to WIFI");
    } else {
      return;
    }
    changeMode(IMAGE);
  }
  else if (cmd == "task") {
    if (tokens.size() != 2) {
      debugE("Command \'task\' requires 1 parameter: [animate, list, stop]");
      return;
    }
    if (tokens[1] == "animate") {
      changeMode(NO_WIFI);
    }
    if (tokens[1] == "list") {
      // NOOP
    }
    if (tokens[1] == "stop") {
      if (currentAppTask) {
        debugI("* Task deleted");
        vTaskDelete(currentAppTask);
        currentAppTask = NULL;
      } else {
        debugI("* No task running");
      }
    }
  }
  else if (cmd == "text") {
    changeMode(TEXT);
    debugI("* Mode set to TEXT");
  } else if (cmd == "clock") {
    changeMode(CLOCK);
    debugI("* Mode set to CLOCK");
  } else if (cmd == "web") {
    webinterface.test("Kiekeboe");
  } else if (cmd == "ls") {
    debugI("Partitions:");
    listPartitions();
    debugI("Files:");
    listFiles();
  } else if (cmd == "version") {
    debugI("%s", build_str);
  }
}


void setupLogging() {

  Debug.begin("qlocktoo"); // Initialize the WiFi server
	Debug.setResetCmdEnabled(true); // Enable the reset command
  // Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors

	String helpCmd = "clear - Clear display\n";
  helpCmd.concat("red - Set red color");
	helpCmd.concat("rgb <r,g,b> - Set RGB value");

	Debug.setHelpProjectsCmds(helpCmd);
	Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
	Debug.initDebugger(debugGetDebuggerEnabled, debugHandleDebugger, debugGetHelpDebugger, debugProcessCmdDebugger); // Set the callbacks

	// debugInitDebugger(&Debug); // Init the debugger

  // if (debugAddFunctionVoid("benchInt", &benchInt) >= 0) {
  // debugSetLastFunctionDescription("To run a benchmark of integers");
// }

}

void changeMode(qlocktoo::Mode mode) {
  // if (checkModeAlreadyActive && mode == currentMode && currentAppTask) {
  //   debugI("Mode already activated");
  //   return;
  // }
  
  currentMode = mode;
  switch (currentMode) {
    case CLOCK:
      // currentApp = shared_ptr<App>(new Clock());
      break;
    case NO_WIFI:
      currentApp = shared_ptr<App>(new Image(Image::Preset::WIFI1));
      break;
    case ERROR:
      currentApp = shared_ptr<App>(new Image(Image::Preset::ERROR));
      break;
    case SWIRL:
      currentApp = shared_ptr<App>(new Swirl());
      break;
    default:
      return;
  }

  debugI("* Start new App");
  // xTaskCreate(
  //   taskFunction,    // Function that should be called
  //   "App",   // Name of the task (for debugging)
  //   1000,            // Stack size (bytes)
  //   &display,            // Parameter to pass
  //   2,               // Task priority
  //   &currentAppTask             // Task handle
  // );

  // xTaskCreatePinnedToCore(taskFunction, "App", 4096, NULL, 2, &currentAppTask, 1);

}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting QlockToo");
  debugI("Booting QlockToo");
  debugI("%s", build_str);
  Serial.println("Booting QlockToo2");

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS cannot be opened");
    debugE("SPIFFS cannot be opened");
  };
  Serial.println("setup Wifi");
  setupWifi();
  Serial.println("setup OTA");
  setupOTA();
  Serial.println("setup Logging");
  setupLogging();
  Serial.println("setup Display");
  setupDisplay();
  Serial.println("setup NTP");
  setupNTP();
  Serial.println("setup Webinterface");
  webinterface.begin(&changeMode);

  // Start in Clock mode
  // changeMode(qlocktoo::CLOCK, (void *) &tmp);
}

// Arduino loop. Most features are implemented as RTOS tasks and are therefore not handled inside this loop.
void loop() {
  ArduinoOTA.handle();
  Debug.handle();
  delay(300);
};



void runAppTask(void * parameter) {
  for (;;) {
    if (currentApp) {
      currentApp.get()->handle();
    }

    // For safety reasons - make sure there's at least a chance for FreeRTOS to switch tasks.
    delay(0);
  }
}

void showClockTask(void * parameter) {
  Clock appClock(Debug);
  ClockConfig tmp;
  tmp.colorHour = tmp.colorItIs = tmp.colorWords = RGBW(0, 255,255);
  appClock.applyConfig(tmp);
  appClock.begin();

  for (;;) {
    appClock.update();
    delay(100);
  }
}

void listPartitions(void)
{
  size_t ul;
  esp_partition_iterator_t _mypartiterator;
  const esp_partition_t *_mypart;
  ul = spi_flash_get_chip_size();
  debugI("Flash chip size: %u", ul);
  debugI("Partiton table:");
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      debugI("%x - %x - %x - %x - %s - %i", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
  _mypartiterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (_mypartiterator)
  {
    do
    {
      _mypart = esp_partition_get(_mypartiterator);
      debugI("%x - %x - %x - %x - %s - %i", _mypart->type, _mypart->subtype, _mypart->address, _mypart->size, _mypart->label, _mypart->encrypted);
    } while ((_mypartiterator = esp_partition_next(_mypartiterator)));
  }
  esp_partition_iterator_release(_mypartiterator);
}

void listFiles() {
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
 
  while(file){
    debugI("File: %s", file.name());
 
    file = root.openNextFile();
  }
  SPIFFS.end();
}
