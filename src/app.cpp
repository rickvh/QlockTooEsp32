#include "app.h"

using namespace qlocktoo;

void App::handle() {
    if (! initialized) {
        setup();
        initialized = true;
    }

    // TODO: thread-safe
    canTerminate = false;
    loop();
    canTerminate = true;
}