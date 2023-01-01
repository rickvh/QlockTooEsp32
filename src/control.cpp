#include "control.h"

const char *qlocktoo::toString(Mode mode)
{
    return enum_str[(int)mode];
}
