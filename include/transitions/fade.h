#pragma once

#include <memory>
#include "image.h"
#include "transitions/transition.h"

namespace qlocktoo {
    class Fade : public Transition {
        private:
            static constexpr const char* LOG_TAG = "fade";
            bool done = false;
            int frame = 0;

        public:
            Fade(std::unique_ptr<Image> from, std::unique_ptr<Image> to) : Transition(std::move(from), std::move(to)) {};
            void update() override;
            bool isDone() override { return done; }
    };
}
