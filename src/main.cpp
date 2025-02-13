#include "uci.hpp"

int main() {
    Uci uci;
    uci.loop();

   return 0; 
}

// #include <CL/cl.h>
// #include <iostream>

// int main() {
//     cl_uint num_platforms;
//     cl_int err = clGetPlatformIDs(0, nullptr, &num_platforms);
//     if (err != CL_SUCCESS || num_platforms == 0) {
//         std::cerr << "OpenCL not found." << std::endl;
//         return 1;
//     }
//     std::cout << "OpenCL is available. Number of platforms: " << num_platforms << std::endl;
//     return 0;
// }
