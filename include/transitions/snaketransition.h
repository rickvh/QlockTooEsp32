#pragma once

#include <set>
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
        std::set<Pixel> pixelsToAdd, pixelsToRemove, pixelsThatDontChange;
        std::set<Pixel> getObstacles() const;

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