#include "image.h"
#include <array>
#include <string>
#include <algorithm>
#include <SPIFFS.h>

Image::Image() {
    pixels.fill(RGBW());
}

Image::Image(Preset preset) {
    RGBW color;

    switch(preset) {
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
            pixels[5] = RGBW(200,200,0);
            pixels[104] = RGBW(200,0,0);
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

    uint8_t x, y, red, green, blue, white;
    while (file >> x >> y >> red >> green >> blue >> white)
    {
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

// Adafruit compatible color
uint32_t Image::getRawColor(uint8_t x, uint8_t y) {
    RGBW color = getColor(x, y);
    return Adafruit_NeoPixel::Color(
        color.brightness*color.brightness*color.r/65025,
        color.brightness*color.brightness*color.g/65025,
        color.brightness*color.brightness*color.b/65025,
        color.brightness*color.brightness*color.w/65025);
};
