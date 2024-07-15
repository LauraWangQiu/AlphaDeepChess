#include <loop.hpp>
#include <defs.hpp>

int main(int argc, char** argv) {

    Loop loop;
    if (loop.init()) loop.run();
    else {
        debugerror("Failed to initialize loop");
        return 1;
    }

    return 0;
}
