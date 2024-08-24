#include "app.h"
#include <string>

using namespace qlocktoo;

void App::handle() {
    if (! initialized) {
        setup();
        initialized = true;
    }

    loop();
}