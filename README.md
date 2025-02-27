# AlphaDeepChess

AlphaDeepChess, desarrollo de un motor de ajedrez basado en podas alfa-beta

## Getting Started

These instructions will help you set up and build the project on your local machine.

### Linux

1. **Download and install git and clone the repository**:

    ```bash
    sudo apt-get install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **Install g++, cmake and OpenCL**:

    ```bash
    sudo apt-get install g++
    sudo apt-get install cmake
    sudo apt install ocl-icd-opencl-dev
    ```

3. **move to the project folder and run the setup script for cmake**:

    ```bash
    cd ./AlphaDeepChess
    sudo chmod +x setup_linux.sh
    ./setupLinux.sh
    ```

### Windows

1. **Download and install git and clone the repository**:

    Download and install [Git](https://git-scm.com/downloads/win)

    ```bash
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **OPTION 1 : use Visual Studio (EASY)**:

    You can directly run the project from Visual Studio IDE

3. **OPTION 2 : download g++(mingw), cmake and OpenCL (HARD)**:

    [Mingw](https://www.mingw-w64.org/downloads/)  
    [CMake](https://cmake.org/download/)  
    OpenCL:
    - [NVIDIA](https://developer.nvidia.com/cuda-downloads)
    - [AMD](https://www.amd.com/en/developer.html)
    - [Intel](https://www.intel.com/content/www/us/en/developer/articles/tool/tools-for-opencl-applications.html)

    configure the PATH enviroment variable  
    configure cmake build folder
    ```bash
    cmake ../.. -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
    ```
### MacOS

1. **Install homebrew, git and clone the repository**:

    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    brew install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **Install g++, cmake and OpenCL**:

    ```bash
    brew install gcc
    brew install cmake
    ```

    OpenCL is pre-installed on MacOS.

3. **move to the project folder and run the setup script for cmake**:

    ```bash
    cd ./AlphaDeepChess
    chmod +x setup_linux.sh
    ./setupLinux.sh
    ```

## Doxygen Commenting Guidelines

Standard Doxygen commenting format to be followed for documenting functions, structs, classes, and enums in the project.

### Header (.hpp) File Comment Template

```cpp
#pragma once

/**
 * @file template.hpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```

### Source (.cpp) File Comment Template

```cpp
/**
 * @file template.cpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```

### Function Comment Template

```cpp
/**
 * @brief function_template
 *
 * Description of the function.
 *  
 * @note Additional notes if needed.
 * 
 * @param[in] inputParam Description of the input parameter.
 * @param[out] outputParam Description of the output parameter.
 *
 * @return
 *  - 0 on success
 *  - -1 on error
 */
int function_template(int inputParam, int& outputParam);
```

### Class Comment Template

```cpp
/**
 * @brief ClassName
 *
 * Detailed description of the class.
 */
class ClassName {
public:
    /**
     * Function comment
     */
    ClassName();

    /**
     * Function comment
     */
    int methodName(int paramName);

private:
    /**
     * @brief memberVariable
     * 
     * Description of the private member.
     */
    int memberVariable;

};
```

### Struct Comment Template

```cpp
/**
 * @brief StructName
 *
 * Description.
 * 
 */
struct StructName {

    int memberVariable;
};
```

### Enum Comment Template

```cpp
/**
 * @brief EnumName.
 *
 * Description of the enum.
 */
enum class EnumName {
    VALUE1, /**< Description of Value1 */
    VALUE2, /**< Description of Value2 */
    VALUE3  /**< Description of Value3 */
};
```

### Using Python's pygame library to visualize the board from the engine

Execute [main.py](main.py) to send commands as with the UCI and visualize the chessboard.
