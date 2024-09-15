#pragma once

// #include <functional>
#include "NeoPixelBus.h"
#include "coordinate.h"

namespace qlocktoo {
struct Pixel
{
    Coordinate coordinate;
    HsbColor color;

    Pixel() = default;
    Pixel(Coordinate coordinate, HsbColor color) : coordinate(coordinate), color(color) {}

    Pixel& operator=(const Pixel& pixel)
    {
        coordinate = pixel.coordinate;
        color = pixel.color;
        return *this;
    }

    bool operator==(const Pixel& pixel) const
    {
        return (coordinate == pixel.coordinate &&
            color.H == pixel.color.H &&
            color.S == pixel.color.S &&
            color.B == pixel.color.B);
    }

    bool operator==(const Coordinate& other) const
    {
        return coordinate == other;
    }

    bool operator!=(const Pixel& pixel) const
    {
        return !(*this == pixel);
    }

    bool operator<(const Pixel& other) const {
        return coordinate.y < other.coordinate.y && coordinate.x < other.coordinate.x;
    }
};
}