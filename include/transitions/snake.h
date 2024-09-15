#pragma once

#include <list>
#include <set>
#include "pixel.h"
#include "display.h"

namespace qlocktoo {
class Snake {
    private:
        static constexpr const char* LOG_TAG = "snake";
        HsbColor bodyColor;
        std::list<Pixel> body;
        std::set<Pixel> targets;
        std::set<Pixel> obstacles;
        const Pixel* currentTarget = nullptr;
        std::list<Coordinate> pathToNextTarget;
        void calculatePathToNextTarget();

    public:
        Snake() = default;
        Snake(Pixel body, const std::set<Pixel>& targets, const std::set<Pixel>& obstacles) : body({body}), targets(targets), obstacles(obstacles), bodyColor(body.color) {}

        void move();
        bool isDone() const { return targets.empty(); };
        void draw() const;
};
}