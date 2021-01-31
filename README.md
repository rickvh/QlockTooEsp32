= Setup development environment =

* Install VSCode
TODO
 - PlatformIO extension
 - 

* Install Angular CLI
To install the Angular CLI, open a terminal window and run the following command:

npm install -g @angular/cli

*


= API =
POST /api/clock
{
    "colorItIs": {
        "r": <0-255>,
        "g": <0-255>,
        "b": <0-255>,
        "w": <0-255>
    },
    "colorWords": {
        "r": <0-255>,
        "g": <0-255>,
        "b": <0-255>,
        "w": <0-255>
    },
    "colorHour": {
        "r": <0-255>,
        "g": <0-255>,
        "b": <0-255>,
        "w": <0-255>
    }
}


FAQ:
- error during compiling. Error in timelib include:  ...::tm not found
  -> fix: Delete file: <project-fir>/.pio/libdeps/esp32dev/Time/Time.h
- .pio/libdeps/esp32dev/ESPAsyncUDP/src/AsyncUDP.cpp:5:28: fatal error: user_interface.h: No such file or directory
  -> fix: <project-dir>/.pio/libdeps/esp32dev/ESPAsyncUDP/src/AsyncUDP.cpp -> remove line '#include "user_interface.h"'