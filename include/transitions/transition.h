#pragma once

#include "image.h"

namespace qlocktoo {
    class Transition {
        protected:
            Transition() {};
        
        public:
            virtual void update() = 0;
            virtual bool isDone() = 0;
    };
}
