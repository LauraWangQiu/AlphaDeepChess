cmake_minimum_required(VERSION 3.10)
project(AlphaDeepChess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
include_directories(src/board)

add_executable(AlphaDeepChess 
src/main.cpp
src/loop.cpp
src/board/board.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(AlphaDeepChess PRIVATE _DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(AlphaDeepChess PRIVATE _RELEASE)
endif()

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)
