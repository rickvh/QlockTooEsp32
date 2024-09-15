#include <algorithm>
#include <memory>
#include <list>
#include <set>
#include <random>
#include "display.h"
#include "transitions/snaketransition.h"

using std::string;
using std::set;
using namespace qlocktoo;

SnakeTransition::SnakeTransition(Image from, Image to) : Transition(),
        from(from),
        to(to)
{
    // set_difference(from. ->begin(), from->end(), to->begin(), to->end(), std::inserter(pixelsToRemove, pixelsToRemove.begin()), WordComparator());
    // set_difference(to->begin(), to->end(), from->begin(), from->end(), std::inserter(pixelsToAdd, pixelsToAdd.begin()), WordComparator());
    // set_intersection(to->begin(), to->end(), from->begin(), from->end(), std::inserter(pixelsThatDontChange, wordsToAdd.begin()), WordComparator());

    Pixel snakePixel = selectRandom(pixelsToRemove);
    pixelsToRemove.erase(snakePixel);
    
    set<Pixel> targets;
    for (auto const &pixel: pixelsToRemove) {
        targets.insert(pixel);
    }
    snake = Snake(snakePixel, targets, getObstacles());
}


void SnakeTransition::update() {
    snake.move();
    delay(500);
}

std::set<Pixel> SnakeTransition::getObstacles() const {
    auto obstacles = std::set<Pixel>();
    for(auto& pixel : pixelsThatDontChange) {
        obstacles.insert(pixel);
    }
    return obstacles;
}

bool SnakeTransition::isDone() {
    return snake.isDone();
}