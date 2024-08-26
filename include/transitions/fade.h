#pragma once

#include "image.h"
#include "transitions/transition.h"

namespace qlocktoo {
    class Fade : public Transition {
        private:
            static constexpr const char* LOG_TAG = "fade";
            const float STEP = 0.01f;
            Image current;
            bool done = false;
            int frame = 0;
            float getFadedValue(float currentValue, float targetValue);
            void showCurrentFrame();

        public:
            Fade(Image &from, Image &to) : Transition(from, to), current(from) {};
            void update() override;
            bool isDone() override { return done; }
    };
}

