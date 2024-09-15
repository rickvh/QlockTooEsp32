#include "utils/stringutils.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <Arduino.h>

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

