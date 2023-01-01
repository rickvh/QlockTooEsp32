#include "app.h"

using namespace qlocktoo;

void App::handle() {
    if (! initialized) {
        setup();
        initialized = true;
    }

    loop();
}