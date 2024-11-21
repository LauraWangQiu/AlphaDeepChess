#pragma once

/**
 * @file col.hpp
 * @brief Board chess Column types and utilities declaration.
 *
 */

/**
 * @brief Col
 * 
 * Represents all the columns of a chess board from COL_A(0) to COL_H(7)
 * 
 */
enum Col : int
{
    COL_A = 0,
    COL_B,
    COL_C,
    COL_D,
    COL_E,
    COL_F,
    COL_G,
    COL_H,
    COL_INVALID
};

/**
 * @brief col_to_char(const Col col)
 * 
 * column conversion to char.
 * 
 * @note col must be valid
 * 
 * @param[in] col The col value
 * 
 * @return 'a','b','c','d','e','f','g','h'
 */
constexpr inline char col_to_char(const Col col)
{
    return 'a' + static_cast<int>(col);
}

/**
 * @brief operator+(Col, int)
 * 
 * Addition operator overload
 * 
 * @param[in] col The col value
 * @param[in] value The integer to add
 * 
 * @return (Col)(col + value)
 */
constexpr inline Col operator+(Col col, int value)
{
    return static_cast<Col>(static_cast<int>(col) + value);
}

/**
 * @brief operator-(Col, int)
 * 
 * Subtraction operator overload
 * 
 * @param[in] col The col value
 * @param[in] value The integer to subtract
 * 
 * @return  (Col)(col - value)
 */
constexpr inline Col operator-(Col col, int value)
{
    return static_cast<Col>(static_cast<int>(col) - value);
}


/**
 * @brief operator++(Col&)
 * Pre-increment operator overload for Col
 * 
 * @param[in,out] col
 * 
 * @return col + 1
 */
constexpr inline Col& operator++(Col& col)
{
    col = static_cast<Col>(static_cast<int>(col) + 1);

    return col;
}

/**
 * @brief operator++(Col&,int)
 * Post-increment operator overload for Col
 * 
 * @param[in,out] col
 * 
 * @return col(old value)
 */
constexpr inline Col operator++(Col& col, int)
{
    Col old = col;
    ++col;
    return old;
}

/**
 * @brief operator--(Col&)
 * Pre-decrement operator overload for Col
 * 
 * @param[in,out] col
 * 
 * @return col - 1
 */
constexpr inline Col& operator--(Col& col)
{

    col = static_cast<Col>(static_cast<int>(col) - 1);
    return col;
}

/**
 * @brief operator--(Col&,int)
 * Post-decrement operator overload for Col
 * 
 * @param[in,out] col
 * 
 * @return col(old value)
 */
constexpr inline Col operator--(Col& col, int)
{
    Col old = col;
    --col;
    return old;
}

/**
 * @brief operator+=(Col&, int)
 * 
 * Addition and assignment operator overload
 * 
 * @param[in,out] col The col value
 * @param[in] value The integer to add
 * 
 * @return pointer to (Col)(col + value)
 */
constexpr inline Col& operator+=(Col& col, int value)
{
    col = static_cast<Col>(static_cast<int>(col) + value);
    return col;
}

/**
 * @brief operator-=(Col&, int)
 * 
 * Subtraction and assignment operator overload
 * 
 * @param[in,out] col The col value
 * @param[in] value The integer to subtract
 * 
 * @return pointer to (Col)(col - value)
 */
constexpr inline Col& operator-=(Col& col, int value)
{
    col = static_cast<Col>(static_cast<int>(col) - value);
    return col;
}