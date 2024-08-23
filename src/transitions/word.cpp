#include "transitions/word.h"
#include "transitions/pixel.h"
#include "display.h"
#include <list>

using namespace qlocktoo;

std::list<Pixel> Word::getPixels() const {
    auto pixels = std::list<Pixel>();
    for (auto& character : characters) {
        auto x = character % Display::WIDTH;
        auto y = character / Display::WIDTH;
        pixels.push_back({x, y});
    }
    return pixels;
}
