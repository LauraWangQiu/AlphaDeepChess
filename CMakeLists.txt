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

add_executable(AlphaDeepChess 
src/main.cpp
src/board/board.cpp
src/uci/uci.cpp
src/evaluation/evaluation_basic.cpp
src/search/search_basic.cpp
src/move_ordering/move_ordering_basic.cpp
src/move_generator/move_generator_basic.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(AlphaDeepChess PRIVATE _DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(AlphaDeepChess PRIVATE _RELEASE)
endif()

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)
