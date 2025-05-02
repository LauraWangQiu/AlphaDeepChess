# AlphaDeepChess

AlphaDeepChess, developing a chess engine based in alpha-beta pruning.

## Getting Started

These instructions will help you set up and build the project on your local machine.

### Linux

1. **Download and install git and clone the repository**:

    ```bash
    sudo apt-get install git
    git clone https://github.com/LauraWangQiu/AlphaDeepChess.git
    ```

2. **Install g++ and cmake**:

    ```bash
    sudo apt-get install g++
    sudo apt-get install cmake
    ```

3. **Go to the project folder and run the setup script for cmake**:

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

2. **Download g++(mingw) and cmake**:

    [Mingw](https://www.mingw-w64.org/downloads/)  
    [CMake](https://cmake.org/download/)  

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

2. **Install g++ and cmake**:

    ```bash
    brew install gcc
    brew install cmake

    # PowerShell installation
    # brew install --cask powershell
    ```

3. **Go to the project folder and run the setup script for cmake**:

    ```bash
    cd ./AlphaDeepChess
    chmod +x setup_linux.sh
    ./setupLinux.sh
    ```

## Profiling (only in Linux)

1. **Profile analysis with perf linux tool**:

    ```bash
    cd ./AlphaDeepChess
    chmod +x profile.sh
    ./profile.sh
    ```

## Comparing engines with CuteChess

To compare engines, we will use the `cutechess` tool. This tool allows us to run matches between two engines and get the results in a `.pgn` file and the final positions in a `epd` file. We will compare it with the `stockfish` engine.

> We encourage you to take a look to the documentation of [CuteChess](https://github.com/cutechess/cutechess/wiki/Building-from-source) to know how to download and build it from source if you want to use the latest version or use the graphical interface.

In an automated way, we created a script to download `cutechess` and `stockfish` portable version. To run it:

```bash
python .\setupComparator.py
```

There should be a new directory called `enginesComparator` with the `cutechess` and `stockfish` portable version.

```txt
/enginesComparator
|-- /cutechess
|   |-- cutechess-cli
|   |-- ...
|-- /stockfish
|   |-- /stockfish
|       |-- stockfish_X_x86_64
|       |-- ...
```

To compare the engines, we will use the `cutechess-cli` tool. We will run a match between `stockfish` and our engine. To do so, we will use the `compareEngines.py`.

```bash
python .\compareEngines.py -buildType [debug (default=release)] -games [num_games (default=10)] -tc [time_control (default=INF)] -st [search_time (default=INF)] -timemargin [time_margin (default=500ms)] -depth [depth (default=INF)] -concurrency [concurrency (default=0)]
```

### Tests

To run the tests, we will use the `cutechess-cli` tool. We will run a match between `stockfish` and our engine. To do so, we will use the `tests.py`.

```bash
python .\tests.py
```

## Doxygen Commenting Guidelines

Standard Doxygen commenting format to be followed for documenting functions, structs, classes, and enums in the project.

### Header (.hpp) File Comment Template

```
#pragma once

/**
 * @file template.hpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```

### Source (.cpp) File Comment Template

```
/**
 * @file template.cpp
 * @brief Brief description of the file.
 *
 * Detailed description of the file.
 */
```

### Function Comment Template

```
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

```
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

```
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

```
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

## Visualizing the board

Execute [AlphaDeepChessGUI.py](AlphaDeepChessGUI.py) to send commands as with the UCI and visualize the chessboard.
