#pragma once

#include <memory>
#include "image.h"

namespace qlocktoo {
    class Transition {
        protected:
            Transition(std::unique_ptr<Image> from, std::unique_ptr<Image> to) : from(std::move(from)), to(std::move(to)) {};
            std::unique_ptr<Image> from;
            std::unique_ptr<Image> to;
        
        public:
            virtual void update() = 0;
            virtual bool isDone() = 0;
    };
}
