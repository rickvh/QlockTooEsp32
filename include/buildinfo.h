#pragma once

#define ST(A) #A
#define STR(A) ST(A)


// VERSION_INFO must be set by precompiler
namespace qlocktoo {
    class BuildInfo {
        public:
            static const char* version;
    };
}