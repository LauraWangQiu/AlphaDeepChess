cmake_minimum_required(VERSION 3.10)
project(AlphaDeepChess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
include_directories(include/board)
include_directories(include/move)
include_directories(include/utilities)
include_directories(include/search)
include_directories(include/evaluation)
include_directories(include/uci)
include_directories(include/move_ordering)
include_directories(include/move_generator)

#find_package(OpenCL REQUIRED)

message(STATUS "OpenCL include directory: ${OpenCL_INCLUDE_DIR}")
message(STATUS "OpenCL library path: ${OpenCL_LIBRARY}")

add_executable(AlphaDeepChess 
src/main.cpp
src/board/board.cpp
src/uci/uci.cpp
src/utilities/perft.cpp
)

target_include_directories(AlphaDeepChess PRIVATE ${OpenCL_INCLUDE_DIR})
target_link_libraries(AlphaDeepChess PRIVATE ${OpenCL_LIBRARY})

# Add basic algorithm source files
set(BASIC_SOURCES
    src/evaluation/evaluation_basic2.cpp
    src/search/search_basic.cpp
    src/move_ordering/move_ordering_basic.cpp
    src/move_generator/move_generator_basic.cpp
)

# Add advanced algorithm source files
set(ADVANCED_SOURCES

)

# Check for user-specified algorithm flag
if (ALGO_TYPE STREQUAL "advanced")
    message(STATUS "Using advanced algorithms.")
    target_sources(AlphaDeepChess PRIVATE ${ADVANCED_SOURCES})
    target_compile_definitions(AlphaDeepChess PRIVATE ALGO_ADVANCED)
else()
    message(STATUS "Using basic algorithms (default).")
    target_sources(AlphaDeepChess PRIVATE ${BASIC_SOURCES})
    target_compile_definitions(AlphaDeepChess PRIVATE ALGO_BASIC)
endif()

# Compilation settings for Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(AlphaDeepChess PRIVATE _DEBUG)
    target_compile_options(AlphaDeepChess PRIVATE 
        -g                               # Generate debugging information
        -O0                              # No optimization
        -Wall                            # Enable basic warnings
        -Wextra                          # Enable additional warnings
        -Wpedantic                       # Enable strict standard warnings
    )
endif()

# Compilation settings for Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(AlphaDeepChess PRIVATE _RELEASE NDEBUG)
    target_compile_options(AlphaDeepChess PRIVATE 
        -O3                              # No optimization
        #-Ofast                          # Maximum and aggressive optimization
        -march=native                   # Enables the use of machine architecture-specific instructions
        -mtune=native                   # Optimize for current CPU
    )
endif()
