cmake_minimum_required(VERSION 3.22.1)
project(AlphaDeepChess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
include_directories(src/board)

add_executable(AlphaDeepChess 
src/main.cpp
src/board/board.cpp
)

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)
