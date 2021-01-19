#include "image.h"
#include <SPIFFS.h>
#include <algorithm>
#include <array>
#include <string>
#include "display.h"
#include "RemoteDebugger.h"

namespace qlocktoo {
Image::Image(RemoteDebug &debug) : App(Mode::IMAGE), Debug(debug) {
    pixels.fill(RGBW());
}

Image::Image(RemoteDebug &debug, Preset preset) : App(Mode::IMAGE), Debug(debug) {
    Debug = debug;
    RGBW color;

    switch (preset) {
        case ERROR:
            color.r = 200;

            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_ERROR) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            break;
        case WIFI1:
            color.b = 200;

            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_WIFI_FRAME_1) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            break;
        case WIFI2:
            color.b = 200;

            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_WIFI_FRAME_2) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            break;
        case WIFI3:
            color.b = 200;

            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_WIFI_FRAME_3) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            break;

        case XMAS_TREE:
            color.g = 200;

            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_XMAS_TREE_GREEN) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            pixels[5] = RGBW(200, 200, 0);
            pixels[104] = RGBW(200, 0, 0);
            break;

        case SNOWMAN:
            color.w = 200;
            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_SNOWMAN_WHITE) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            color.w = 0;
            color.r = 200;
            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_SNOWMAN_RED) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            color.r = 0;
            color.b = 200;
            for (uint8_t y = 0; y < HEIGHT; y++) {
                for (uint8_t x = 0; x < WIDTH; x++) {
                    uint8_t index = y * WIDTH + x;
                    for (const auto& pixel : IMG_SNOWMAN_BLUE) {
                        if (index == pixel) {
                            pixels[index] = color;
                            break;
                        }
                    }
                }
            }
            break;

        default:
            break;
    }
}

void Image::readFile(std::string filename) {
    if (!SPIFFS.begin() || !SPIFFS.exists(filename.c_str())) {
        return;
    }

    File file = SPIFFS.open(filename.c_str());

    uint8_t x = 0, y = 0, red = 0, green = 0, blue = 0, white = 0;
    while (file >> x >> y >> red >> green >> blue >> white) {
        pixels[y * WIDTH + x] = RGBW(red, green, blue, white);
    }

    // while (file.available()) {
    //     int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
    //     buffer << file;
    //     buffer[l] = 0;
    //     if (strcmp)
    // }
    SPIFFS.end();
}

RGBW Image::getColor(uint8_t x, uint8_t y) {
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
        return RGBW();
    }
    return pixels[y * WIDTH + x];
}

// TODO: kan weg??
// Adafruit compatible color
uint32_t Image::getRawColor(uint8_t x, uint8_t y) {
    return getColor(x, y).getColor();
};

void Image::show() {
    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
            auto color = getColor(x, y).getColor();
            Display::drawPixel(x, y, color);
        }
    }
    Display::show();
}

void Image::setup() {
    Display::begin();
    show();
    debugI("Image-setup");
}

void Image::loop() {
    // Since there's really nothing to do, we'll suspend ourself.
    // vTaskSuspend(NULL);
    delay(3000);
    debugI("Image-loop");
    // show();
    // Display::begin();
}
}  // namespace qlocktoo