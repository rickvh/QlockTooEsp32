#pragma once

#include <list>
#include <set>
#include "pixel.h"

/**
 * Collection of adjecent/connected Pixels representing words used to display time.
*/
namespace qlocktoo {
class Word {
    public:
        std::set<unsigned int> characters;
        std::list<Pixel> getPixels() const;
        Word(const std::set<unsigned int>& src) : characters(src.begin(), src.end()) {}
};
}