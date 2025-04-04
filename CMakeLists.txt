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

add_executable(AlphaDeepChess 
src/main.cpp
src/board/board.cpp
src/uci/uci.cpp
src/utilities/perft.cpp
src/search/history.cpp
src/utilities/transposition_table.cpp
src/move_generator/precomputed_move_data.cpp
src/utilities/coordinates.cpp
)

# Add basic algorithm source files
set(BASIC_SOURCES
    src/evaluation/evaluation_dynamic.cpp
    # src/evaluation/evaluation_gpu.cpp # For future purposes
    src/search/search_transposition_table.cpp
    # src/search/search_multithread.cpp
    src/move_ordering/move_ordering_MVV_LVA.cpp
    src/move_generator/move_generator_magic_bitboards.cpp
)

# For future purposes
# # Check if evaluation_gpu.cpp is included
# if ("src/evaluation/evaluation_gpu.cpp" IN_LIST BASIC_SOURCES)
#     target_compile_definitions(AlphaDeepChess PRIVATE EVALUATION_GPU)

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
#     target_link_directories(AlphaDeepChess PRIVATE ${JULIA_LIB_DIR})
#     target_link_libraries(AlphaDeepChess PRIVATE ${JULIA_LIBRARY})
#     target_compile_options(AlphaDeepChess PRIVATE -std=c++20 -static-libstdc++)

#     set(OUTPUT_PARENT_DIR "$<TARGET_FILE_DIR:AlphaDeepChess>/..")

#     # Copy bin directory to one level up
#     add_custom_command(TARGET AlphaDeepChess POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_PARENT_DIR}/bin"
#         COMMAND ${CMAKE_COMMAND} -E copy_directory "${JULIA_BIN_DIR}" "${OUTPUT_PARENT_DIR}/bin"
#     )

#     # Copy lib directory to one level up
#     add_custom_command(TARGET AlphaDeepChess POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_PARENT_DIR}/lib"
#         COMMAND ${CMAKE_COMMAND} -E copy_directory "${JULIA_LIB_DIR}" "${OUTPUT_PARENT_DIR}/lib"
#     )

#     add_custom_command(TARGET AlphaDeepChess POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${JULIA_BIN_DIR}"
#         $<TARGET_FILE_DIR:AlphaDeepChess>
#     )
# endif()

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
endif()
