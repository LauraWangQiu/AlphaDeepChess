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
option(USE_EVALUATION_DYNAMIC "Use evaluation_dynamic.cpp" OFF)
option(USE_EVALUATION_SAFETY_MOBILITY "Use evaluation_safety_mobility.cpp" ON)
# option(USE_EVALUATION_GPU "Use evaluation_gpu.cpp" OFF)

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


# set(BASIC_SOURCES
#     src/evaluation/evaluation_dynamic.cpp
#     # src/move_generator/move_generator_basic.cpp
#     src/move_ordering/move_ordering_MVV_LVA.cpp
#     src/move_generator/move_generator_magic_bitboards.cpp
#     # src/evaluation/evaluation_gpu.cpp # For future purposes
#     # src/search/search_basic.cpp
#     # src/search/search_multithread.cpp
#     src/search/search_transposition_table.cpp
#     # search_tt_extensions_reductions.cpp
# )

# For future purposes
# # Check if evaluation_gpu.cpp is included
# if ("src/evaluation/evaluation_gpu.cpp" IN_LIST BASIC_SOURCES)
#     target_compile_definitions(${EXECUTABLE_OUTPUT_NAME} PRIVATE EVALUATION_GPU)

#     find_program(JULIA_EXECUTABLE NAMES julia)
#     if(NOT JULIA_EXECUTABLE)
#         message(FATAL_ERROR "Julia executable not found. Please ensure Julia is installed and available in your PATH.")
#     endif()

#     execute_process(
#         COMMAND ${JULIA_EXECUTABLE} -e "print(Sys.BINDIR)"
#         OUTPUT_VARIABLE JULIA_BIN_DIR
#         OUTPUT_STRIP_TRAILING_WHITESPACE
#     )
#     if(WIN32)
#         string(REPLACE "\\bin" "" JULIA_HOME ${JULIA_BIN_DIR})
#     else()
#         string(REPLACE "/bin" "" JULIA_HOME ${JULIA_BIN_DIR})
#     endif()

#     set(JULIA_INCLUDE_DIR "${JULIA_HOME}/include/julia")
#     set(JULIA_LIB_DIR "${JULIA_HOME}/lib")

#     if(WIN32)
#         set(JULIA_LIBRARY "${JULIA_BIN_DIR}/libjulia.dll")
#     elseif(APPLE)
#         set(JULIA_LIBRARY "${JULIA_LIB_DIR}/libjulia.dylib")
#     else()
#         set(JULIA_LIBRARY "${JULIA_LIB_DIR}/libjulia.so")
#     endif()

#     if(NOT EXISTS ${JULIA_INCLUDE_DIR})
#         message(FATAL_ERROR "Julia include directory not found: ${JULIA_INCLUDE_DIR}")
#     endif()
#     if(NOT EXISTS ${JULIA_LIBRARY})
#         message(FATAL_ERROR "Julia library not found: ${JULIA_LIBRARY}")
#     endif()

#     include_directories(${JULIA_INCLUDE_DIR})
#     target_link_directories(${EXECUTABLE_OUTPUT_NAME} PRIVATE ${JULIA_LIB_DIR})
#     target_link_libraries(${EXECUTABLE_OUTPUT_NAME} PRIVATE ${JULIA_LIBRARY})
#     target_compile_options(${EXECUTABLE_OUTPUT_NAME} PRIVATE -std=c++20 -static-libstdc++)

#     set(OUTPUT_PARENT_DIR "$<TARGET_FILE_DIR:${EXECUTABLE_OUTPUT_NAME}>/..")

#     # Copy bin directory to one level up
#     add_custom_command(TARGET ${EXECUTABLE_OUTPUT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_PARENT_DIR}/bin"
#         COMMAND ${CMAKE_COMMAND} -E copy_directory "${JULIA_BIN_DIR}" "${OUTPUT_PARENT_DIR}/bin"
#     )

#     # Copy lib directory to one level up
#     add_custom_command(TARGET ${EXECUTABLE_OUTPUT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_PARENT_DIR}/lib"
#         COMMAND ${CMAKE_COMMAND} -E copy_directory "${JULIA_LIB_DIR}" "${OUTPUT_PARENT_DIR}/lib"
#     )

#     add_custom_command(TARGET ${EXECUTABLE_OUTPUT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${JULIA_BIN_DIR}"
#         $<TARGET_FILE_DIR:${EXECUTABLE_OUTPUT_NAME}>
#     )
# endif()

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
        #-O3                   
        -Ofast                # Aggressive optimization
        -march=native         # CPU-specific instructions
        -mtune=native         # Tune for current CPU
        -flto=auto            # Link-time optimization with parallel jobs
        -funroll-loops        # Unroll loops
        -fomit-frame-pointer  # Free up a register
        #-fno-exceptions       # Disable exceptions
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
