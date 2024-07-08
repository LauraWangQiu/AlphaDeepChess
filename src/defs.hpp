#define ALPHADEEPCHESS_VERSION "1.0.0"

#ifdef _DEBUG
#include <iostream>
#endif

#define VALUESTR(value) ((value) ? "true" : "false")

static void debuglog(const char* message) {
#ifdef _DEBUG
    std::cout << message << std::endl;
#endif
}
