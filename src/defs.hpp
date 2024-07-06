#ifdef _DEBUG
#include <iostream>
#endif

#define VALUESTR(value) ((value) ? "true" : "false")

static void log(const char* message) {
#ifdef _DEBUG
    std::cout << message << std::endl;
#endif
}
