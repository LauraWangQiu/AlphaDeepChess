#!/bin/bash

# Check if the script is being executed at the same level as CMakeLists.txt
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: This script must be executed in the same directory level as CMakeLists.txt."
    exit 1
fi

# Create and configure the build directory
mkdir -p build
cd build
cmake ..

cd ..

# Check if there is a CMakeLists.txt file inside the test folder
if [ ! -f "test/CMakeLists.txt" ]; then
    echo "Error: The test folder must contain a CMakeLists.txt file."
    exit 1
fi

# Create and configure the test build directory
cd test
mkdir -p build
cd build
cmake ..