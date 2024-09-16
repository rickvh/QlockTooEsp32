#pragma once

#include <list>
#include "transition.h"
#include "snake.h"

using std::string;

namespace qlocktoo {    
class SnakeTransition : public Transition {
    private:
        static constexpr const char* LOG_TAG = "snakeTransition";
        Image from;
        Image to;
        Snake snake;
        std::list<Pixel> pixelsToAdd, pixelsToRemove, pixelsThatDontChange;
        std::list<Pixel> getObstacles() const;
        void logPixels(std::list<qlocktoo::Pixel> pixels);

        template<typename S>
        auto selectRandom(const S &set) {
            auto random = rand() % set.size();
            auto it = std::begin(set);
            std::advance(it, random);
            return *it;
        }

    public:
        SnakeTransition(Image from, Image to);
        void update() override;
        bool isDone() override;
};
}