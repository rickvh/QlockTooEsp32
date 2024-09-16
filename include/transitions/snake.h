#pragma once

#include <list>
#include "pixel.h"
#include "display.h"

namespace qlocktoo {
class Snake {
    private:
        static constexpr const char* LOG_TAG = "snake";
        HsbColor bodyColor;
        std::list<Pixel> body;
        std::list<Pixel> targets;
        std::list<Pixel> obstacles;
        const Pixel* currentTarget = nullptr;
        std::list<Coordinate> pathToNextTarget;
        void calculatePathToNextTarget();
        boolean done = false;

    public:
        Snake() = default;
        Snake(std::list<Pixel>& body, const std::list<Pixel>& targets, const std::list<Pixel>& obstacles) : body(body), targets(targets), obstacles(obstacles), bodyColor(body.color) {}

        void move();
        bool isDone() const { return done; };
        void draw() const;
};
}