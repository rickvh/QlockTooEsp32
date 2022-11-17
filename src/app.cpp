#include "app.h"

namespace qlocktoo {
    void App::handle() {
        if (! initialized) {
            setup();
            initialized = true;
        }

        canTerminate = false;
        loop();
        canTerminate = true;
    }
}