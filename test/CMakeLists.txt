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
    ../src/utilities/transposition_table.cpp
    ../src/search/history.cpp
    ../src/move_generator/precomputed_move_data.cpp
    ../src/utilities/coordinates.cpp
)

# Add basic algorithm source files
set(BASIC_SOURCES
    ../src/evaluation/evaluation_dynamic.cpp
    ../src/search/search_transposition_table.cpp
    ../src/move_ordering/move_ordering_MVV_LVA.cpp 
    ../src/move_generator/move_generator_magic_bitboards.cpp
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

# Compilation settings for Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(testsAlphaDeepChess PRIVATE _DEBUG)
    target_compile_options(testsAlphaDeepChess PRIVATE 
        -g                               # Generate debugging information
        -O0                              # No optimization
        -Wall                            # Enable basic warnings
        -Wextra                          # Enable additional warnings
        -Wpedantic                       # Enable strict standard warnings
    )
endif()

# Compilation settings for Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(testsAlphaDeepChess PRIVATE _RELEASE NDEBUG)
    target_compile_options(testsAlphaDeepChess PRIVATE 
        #-O3                   
        -Ofast                # Aggressive optimization
        -march=native         # CPU-specific instructions
        -mtune=native         # Tune for current CPU
        -flto=auto            # Link-time optimization with parallel jobs
        -funroll-loops        # Unroll loops
        -fomit-frame-pointer  # Free up a register
        #-fno-exceptions       # Disable exceptions
        -fno-rtti             # Disable RTTI
        -mbmi2                # for pext instruction
    )
endif()