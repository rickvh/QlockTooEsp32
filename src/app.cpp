#include "app.h"

namespace qlocktoo {
    void App::handle() {
        if (! initialized) {
            setup();
            initialized = true;
        }

        loop();
    }
}