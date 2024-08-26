#pragma once

#include "image.h"

namespace qlocktoo {
    class Transition {
        protected:
            Transition(Image from, Image to) : from(from), to(to) {};
            Image from;
            Image to;
        
        public:
            virtual void update() = 0;
            virtual bool isDone() = 0;
    };
}
