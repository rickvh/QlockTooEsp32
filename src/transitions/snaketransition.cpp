#include "transitions/snaketransition.h"
#include <algorithm>
#include <memory>
#include <random>
#include "display.h"

using namespace qlocktoo;

void SnakeTransition::logPixels(std::list<qlocktoo::Pixel> pixels) {
    for (auto pixel : pixels) {
        ESP_LOGD(LOG_TAG, "(%u, %u) - %f, %f, %f", pixel.coordinate.x, pixel.coordinate.y, pixel.color.H, pixel.color.S, pixel.color.B);
    }
}

SnakeTransition::SnakeTransition(Image from, Image to) : Transition(),
        from(from),
        to(to)
{
    pixelsToRemove = from.getPixelsThatSatisfy(to, [](const HsbColor& fromColor, const HsbColor& toColor) {
        return fromColor.B > 0.0f && toColor.B == 0.0f;
    });
    ESP_LOGD(LOG_TAG, "pixelsToRemove: %u", pixelsToRemove.size());
    logPixels(pixelsToRemove);
    ESP_LOGD(LOG_TAG, "\n");

    pixelsToAdd = from.getPixelsThatSatisfy(to, [](const HsbColor& fromColor, const HsbColor& toColor) {
        return fromColor.B == 0.0f && toColor.B > 0.0f;
    });
    ESP_LOGD(LOG_TAG, "pixelsToAdd: %u", pixelsToAdd.size());
    logPixels(pixelsToAdd);
    ESP_LOGD(LOG_TAG, "\n");

    pixelsThatDontChange = from.getPixelsThatSatisfy(to, [](const HsbColor& fromColor, const HsbColor& toColor) {
        // return fromColor.H == toColor.H && fromColor.S == toColor.S && fromColor.B == toColor.B;
        return fromColor.B > 0.0f && toColor.B > 0.0f;
    });
    ESP_LOGD(LOG_TAG, "pixelsThatDontChange: %u", pixelsThatDontChange.size());
    // logPixels(pixelsThatDontChange);
    ESP_LOGD(LOG_TAG, "\n");

    Pixel snakePixel;
    if (pixelsToRemove.size() != 0) {
        snakePixel = selectRandom(pixelsToRemove);
        
        pixelsToRemove.remove(snakePixel);
    } else {
        ESP_LOGE(LOG_TAG, "No pixels to eat. Snake will start at 0,0 to prevent a crash.");
    }
    ESP_LOGD(LOG_TAG, "Snakepixel set to (%u, %u)", snakePixel.coordinate.x, snakePixel.coordinate.y);
    
    
    std::list<Pixel> targets;
    for (auto const &pixel: pixelsToRemove) {
        targets.push_back(pixel);
    }
    snake = Snake(snakePixel, targets, getObstacles());
}


void SnakeTransition::update() {
    snake.move();
    delay(500);
}

std::list<Pixel> SnakeTransition::getObstacles() const {
    auto obstacles = std::list<Pixel>();
    for(auto& pixel : pixelsThatDontChange) {
        obstacles.push_back(pixel);
    }
    return obstacles;
}

bool SnakeTransition::isDone() {
    return snake.isDone();
}