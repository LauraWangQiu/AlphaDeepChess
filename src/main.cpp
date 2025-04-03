#include "uci.hpp"

#ifdef EVALUATION_GPU
#include <julia.h>
#include <iostream>
#endif

int main() {
#ifdef EVALUATION_GPU
    jl_init();
    if (jl_exception_occurred()) {
        std::cout << "Error initiating Julia: " << jl_typeof_str(jl_exception_occurred()) << std::endl;
        return -1;
    }

    jl_eval_string("include(\"../../gpu/evaluation_gpu.jl\")");
    if (jl_exception_occurred()) {
        std::cout << "Error loading evaluate_gpu.jl: " << jl_typeof_str(jl_exception_occurred()) << std::endl;
        jl_atexit_hook(0);
        return -1;
    }
#endif

    Uci uci;
    uci.loop();

#ifdef EVALUATION_GPU
    jl_atexit_hook(0);
#endif

   return 0; 
}
