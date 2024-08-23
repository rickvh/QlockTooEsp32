#pragma once

#include <memory>
#include <vector>
#include "image.h"
#include "word.h"
#include "transitions/transition.h"

namespace qlocktoo {
enum class SnakeState {
    IDENTIFY,
    MOVING_TO_TARGET
};

class Snake : public Transition {
    private:
        SnakeState state;
        std::vector<Word> targets;


        static constexpr const char* LOG_TAG = "snake";
        bool done = false;
        int delayBetweenMoves = 5000; //ms
        int frame = 0;
        void setup();

    public:
        Snake(std::unique_ptr<Image> from, std::unique_ptr<Image> to) : Transition(std::move(from), std::move(to)) { setup(); };
        void update() override;
        bool isDone() override { return done; }
};
}

