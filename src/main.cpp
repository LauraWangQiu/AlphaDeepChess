#include <loop.hpp>

int main(int argc, char** argv) {

    std::unique_ptr<Loop> loop = std::make_unique<Loop>();
    if (loop->init()) loop->run();
    else
        return 1;

    return 0;
}
