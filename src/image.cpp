#include "image.h"
#include <SPIFFS.h>
#include <algorithm>
#include <array>
#include <string>
#include "display.h"

namespace qlocktoo {

// TODO: dit moet in de algehele config staan
const float BRIGHTNESS = 0.75f;

Image::Image() {
    fill(HsbColor(0.0f, 1.0f, 0.0f));
}

Image::Image(Preset preset) {
    HsbColor color(0.0f, 1.0f, 0.0f);
    fill(color);
    color.B = BRIGHTNESS;

    switch (preset) {
        case Preset::Error:
            color.H = 0.0f;

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
        case Preset::Wifi1:
            color.H = 0.47f;

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
        case Preset::Wifi2:
            color.H = 0.47f;

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
        case Preset::Wifi3:
            color.H = 0.47f;

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

        case Preset::XmasTree:
            color.H = 0.24f;

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
            pixels[5] = HsbColor(0.1667f, 1.0f, BRIGHTNESS);
            pixels[104] = HsbColor(0.0f, 1.0f, BRIGHTNESS);
            break;

        case Preset::Snowman:
            color.S = 0.0f;
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
            color.S = 1.0f;
            color.H = 0.0f;
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
            color.H = 0.47f;
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
    if (!SPIFFS.exists(filename.c_str())) {
        ESP_LOGE(LOG_TAG, "File not found: %s", filename);
        return;
    }

    File file = SPIFFS.open(filename.c_str());

    uint8_t x = 0, y = 0;
    float hue = 0.0f, saturation = 0.0f, brightness = 0.0f;
    // while (file >> x >> y >> hue >> saturation >> brightness) {
        // pixels[y * WIDTH + x] = COLOR_FEATURE::ColorObject(HsbColor(hue, saturation, brightness));
    // }

    // while (file.available()) {
    //     int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
    //     buffer << file;
    //     buffer[l] = 0;
    //     if (strcmp)
    // }
}

HsbColor Image::getColor(Coordinate coordinate) {
    if (coordinate.x < 0 || coordinate.x > WIDTH || coordinate.y < 0 || coordinate.y > HEIGHT) {
        return HsbColor(0.0f, 0.0f, 0.0f);
    }
    return pixels[coordinate.y * WIDTH + coordinate.x];
}

void Image::setColor(Coordinate coordinate,HsbColor color) {
    pixels[coordinate.y * WIDTH + coordinate.x] = color;
}

void Image::fill(HsbColor color) {
    pixels.fill(color);
}

}  // namespace qlocktoo