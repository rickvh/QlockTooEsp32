#pragma once

#include "image.h"
#include "transitions/transition.h"

namespace qlocktoo {
    class FadeTransition : public Transition {
        private:
            static constexpr const char* LOG_TAG = "fadeTransition";
            const float STEP = 0.01f;
            Image current, to;
            bool done = false;
            int frame = 0;
            float getFadedValue(float currentValue, float targetValue);
            void showCurrentFrame();

        public:
            FadeTransition(Image &from, Image &to) : Transition(), current(from), to(to) {};
            void update() override;
            bool isDone() override { return done; }
    };
}

