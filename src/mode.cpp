#include "mode.h"

namespace qlocktoo {

const char *modeToString(Mode mode) {
    return mode_str[(int)mode];
}
}