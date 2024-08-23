#pragma once

#include <functional>

namespace qlocktoo {
struct Pixel
{
    unsigned int x, y;

    Pixel() = default;
    Pixel(unsigned int x, unsigned int y) : x(x), y(y) {}

    Pixel& operator=(const Pixel& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    bool operator==(const Pixel& p) const
    {
        return (x == p.x && y == p.y);
    }

    bool operator!=(const Pixel& p) const
    {
        return !(*this == p);
    }

    unsigned int manhattanDistanceTo(const Pixel& other) const {
        return abs(int(x) - int(other.x)) + abs(int(y) - int(other.y));
    }

};
}

template<> struct std::hash<qlocktoo::Pixel> {
    size_t operator()(const qlocktoo::Pixel &p) const {
        return hash<unsigned int>()(p.x) ^ hash<unsigned int>()(p.y);
    }
};
