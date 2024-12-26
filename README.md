# AlphaDeepChess
AlphaDeepChess, desarrollo de un motor de ajedrez basado en podas alfa-beta

## Getting Started

These instructions will help you set up and build the project on your local machine.

### Linux

1. **Install git and clone the repository**:

    ```bash
    sudo apt-get install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **Install g++ and cmake**:

    ```bash
    sudo apt-get install g++
    sudo apt-get install cmake
    ```

3. **move to the project folder and run the setup script for cmake**:

    ```bash
    cd ./AlphaDeepChess
    sudo chmod +x setup_linux.sh
    ./setupLinux.sh
    ```

### Windows

1. **Install git and clone the repository**:

    ```bash
    install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. ** OPTION 1 : use Visual Studio (EASY)**:

    ```bash
    you can directly run the project from Visual Studio IDE
    ```

3. ** OPTION 2 : download g++(mingw) and cmake (HARD)**:

    ```bash
    [CMake](https://cmake.org/download/)
    [Mingw](https://www.mingw-w64.org/downloads/)

    configure the PATH enviroment variable
    configure cmake build folder
    ```

### MacOS

1. **Install homebrew, git and clone the repository**:

    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    brew install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **Install g++ and cmake**:

    ```bash
    brew install gcc
    brew install cmake
    ```

3. **move to the project folder and run the setup script for cmake**:

    ```bash
    cd ./AlphaDeepChess
    chmod +x setup_linux.sh
    ./setupLinux.sh
    ```

# Doxygen Commenting Guidelines

Standard Doxygen commenting format to be followed for documenting functions, structs, classes, and enums in the project.

## Header (.hpp) File Comment Template


```cpp
#pragma once

/**
 * @file template.hpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```
## Source (.cpp) File Comment Template


```cpp
/**
 * @file template.cpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```

## Function Comment Template

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

## Class Comment Template

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

## Struct Comment Template

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

## Enum Comment Template

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