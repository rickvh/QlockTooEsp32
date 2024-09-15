#pragma once

namespace qlocktoo {
    struct Coordinate {
        uint8_t x;
        uint8_t y;

        Coordinate() : x(0), y(0) {}
        Coordinate(uint8_t x, uint8_t y) : x(x), y(y) {}

        bool operator==(const Coordinate& other) const {
           return x == other.x && y == other.y;
        }

        bool operator!=(const Coordinate& other) const
        {
            return !(*this == other);
        }

        unsigned int manhattanDistanceTo(const Coordinate& other) const {
            return abs(int(x) - int(other.x)) + abs(int(y) - int(other.y));
        }
    };
}

namespace std {
    template<>
    struct hash<qlocktoo::Coordinate> {
        std::size_t operator()(const qlocktoo::Coordinate& coordinate) const {
            return std::hash<uint8_t>()(coordinate.x) ^ (std::hash<uint8_t>()(coordinate.y) << 1);
        }
    };
}