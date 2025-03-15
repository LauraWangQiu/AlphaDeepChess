#!/bin/bash

# Check if the script is being executed at the same level as CMakeLists.txt
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: This script must be executed in the same directory level as CMakeLists.txt."
    exit 1
fi

# Create and configure the build debug and release directory
mkdir -p build/debug
cd build/debug
cmake ../..
cd ../..
mkdir -p build/release
cd build/release
cmake ../..
cd ../..

# Check if there is a CMakeLists.txt file inside the test folder
if [ ! -f "test/CMakeLists.txt" ]; then
    echo "Error: The test folder must contain a CMakeLists.txt file."
    exit 1
fi

# Create and configure the test build debug and release directory
cd test
mkdir -p build/debug
cd build/debug
cmake ../..
cd ../..
mkdir -p build/release
cd build/release
cmake ../..
cd ../../..