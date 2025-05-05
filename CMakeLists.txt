cmake_minimum_required(VERSION 3.10)
project(AlphaDeepChess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported!")
endif()

include_directories(include)
include_directories(include/board)
include_directories(include/move)
include_directories(include/utilities)
include_directories(include/search)
include_directories(include/evaluation)
include_directories(include/uci)
include_directories(include/move_ordering)
include_directories(include/move_generator)

if(DEFINED EXECUTABLE_NAME)
    set(EXECUTABLE_OUTPUT_NAME ${EXECUTABLE_NAME})
else()
    set(EXECUTABLE_OUTPUT_NAME "AlphaDeepChess")
endif()

add_executable(${EXECUTABLE_OUTPUT_NAME}
src/main.cpp
src/board/board.cpp
src/uci/uci.cpp
src/utilities/perft.cpp
src/search/history.cpp
src/utilities/transposition_table.cpp
src/move_generator/precomputed_move_data.cpp
src/utilities/coordinates.cpp
)

list(APPEND BASIC_SOURCES src/move_ordering/move_ordering_MVV_LVA.cpp)

# Add basic algorithm source files
option(USE_EVALUATION_DYNAMIC "Use evaluation_dynamic.cpp" ON)
option(USE_EVALUATION_SAFETY_MOBILITY "Use evaluation_safety_mobility.cpp" OFF)

option(USE_MOVE_GENERATOR_BASIC "Use move_generator_basic.cpp" OFF)
option(USE_MOVE_GENERATOR_MAGIC_BITBOARDS "Use move_generator_magic_bitboards.cpp" ON)

option(USE_SEARCH_BASIC "Use search_basic.cpp" OFF)
option(USE_SEARCH_MULTITHREAD "Use search_multithread.cpp" OFF)
option(USE_SEARCH_TRANSPOSITION_TABLE "Use search_transposition_table.cpp" OFF)
option(USE_SEARCH_TT_REDUCTIONS "Use search_tt_reductions.cpp" ON)

if (USE_EVALUATION_DYNAMIC)
    message(STATUS "Using evaluation_dynamic.cpp")
    list(APPEND BASIC_SOURCES src/evaluation/evaluation_dynamic.cpp)
endif()

if (USE_EVALUATION_SAFETY_MOBILITY)
    message(STATUS "Using evaluation_safety_mobility.cpp")
    list(APPEND BASIC_SOURCES src/evaluation/evaluation_safety_mobility.cpp)
endif()

if (USE_MOVE_GENERATOR_BASIC)
    message(STATUS "Using move_generator_basic.cpp")
    list(APPEND BASIC_SOURCES src/move_generator/move_generator_basic.cpp)
endif()

if (USE_MOVE_GENERATOR_MAGIC_BITBOARDS)
    message(STATUS "Using move_generator_magic_bitboards.cpp")
    list(APPEND BASIC_SOURCES src/move_generator/move_generator_magic_bitboards.cpp)
endif()

if (USE_SEARCH_BASIC)
    message(STATUS "Using search_basic.cpp")
    list(APPEND BASIC_SOURCES src/search/search_basic.cpp)
endif()

if (USE_SEARCH_MULTITHREAD)
    message(STATUS "Using search_multithread.cpp")
    list(APPEND BASIC_SOURCES src/search/search_multithread.cpp)
endif()

if (USE_SEARCH_TRANSPOSITION_TABLE)
    message(STATUS "Using search_transposition_table.cpp")
    list(APPEND BASIC_SOURCES src/search/search_transposition_table.cpp)
endif()

if (USE_SEARCH_TT_REDUCTIONS)
    message(STATUS "Using search_tt_reductions.cpp")
    list(APPEND BASIC_SOURCES src/search/search_tt_reductions.cpp)
endif()

target_sources(${EXECUTABLE_OUTPUT_NAME} PRIVATE ${BASIC_SOURCES})

# Compilation settings for Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${EXECUTABLE_OUTPUT_NAME} PRIVATE _DEBUG)
    target_compile_options(${EXECUTABLE_OUTPUT_NAME} PRIVATE 
        -g                               # Generate debugging information
        -O0                              # No optimization
        -Wall                            # Enable basic warnings
        -Wextra                          # Enable additional warnings
        -Wpedantic                       # Enable strict standard warnings
    )
endif()

# Compilation settings for Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${EXECUTABLE_OUTPUT_NAME} PRIVATE _RELEASE NDEBUG)
    set(RELEASE_FLAGS
        -Ofast                # Aggressive optimization
        -march=native         # CPU-specific instructions
        -mtune=native         # Tune for current CPU
        -flto=auto            # Link-time optimization with parallel jobs
        -funroll-loops        # Unroll loops
        -fomit-frame-pointer  # Free up a register
        -fno-rtti             # Disable RTTI
    )

    if(COMPILER_SUPPORTS_BMI2)
        list(APPEND RELEASE_FLAGS -mbmi2)  # for pext instruction
        message(STATUS "Adding -mbmi2 flag in Release mode")
    else()
        message(STATUS "Compiler does not support -mbmi2; not adding it")
    endif()

    target_compile_options(${EXECUTABLE_OUTPUT_NAME} PRIVATE ${RELEASE_FLAGS})
endif()
