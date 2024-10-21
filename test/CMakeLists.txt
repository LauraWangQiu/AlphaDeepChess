cmake_minimum_required(VERSION 3.10)
project(testsAlphaDeepChess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../include)
include_directories(../include/board)
include_directories(../include/move)
include_directories(../include/utilities)
include_directories(../include/search)
include_directories(../include/evaluation)
include_directories(../include/uci)
include_directories(../include/move_ordering)
include_directories(../include/move_generator)

add_executable(testsAlphaDeepChess 
    testsAlphaDeepChess.cpp
    ../src/board/board.cpp
    ../src/uci/uci.cpp
    ../src/utilities/perft.cpp
)

# Add basic algorithm source files
set(BASIC_SOURCES
    ../src/evaluation/evaluation_basic.cpp
    ../src/search/search_basic.cpp
    ../src/move_ordering/move_ordering_basic.cpp
    ../src/move_generator/move_generator_basic.cpp
)

# Add advanced algorithm source files
set(ADVANCED_SOURCES

)

# Check for user-specified algorithm flag
if (ALGO_TYPE STREQUAL "advanced")
    message(STATUS "Using advanced algorithms.")
    target_sources(testsAlphaDeepChess PRIVATE ${ADVANCED_SOURCES})
    target_compile_definitions(testsAlphaDeepChess PRIVATE ALGO_ADVANCED)
else()
    message(STATUS "Using basic algorithms (default).")
    target_sources(testsAlphaDeepChess PRIVATE ${BASIC_SOURCES})
    target_compile_definitions(testsAlphaDeepChess PRIVATE ALGO_BASIC)
endif()

target_compile_definitions(testsAlphaDeepChess PRIVATE _DEBUG)
target_compile_options(testsAlphaDeepChess PRIVATE 
    -g                               # Generate debugging information
    -O0                              # No optimization
    -Wall                            # Enable basic warnings
    -Wextra                          # Enable additional warnings
    -Wpedantic                       # Enable strict standard warnings
)