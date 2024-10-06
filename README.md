# AlphaDeepChess
AlphaDeepChess, desarrollo de un motor de ajedrez basado en podas alfa-beta



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