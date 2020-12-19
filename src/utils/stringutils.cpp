#include "utils/stringutils.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <Arduino.h>

using namespace std;

template <typename Out>
void split(const string &s, char delim, Out result) {
    istringstream iss(s);
    string item;
    while (getline(iss, item, delim)) {
        *result++ = item;
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

