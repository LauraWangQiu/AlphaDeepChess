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
constexpr inline char col_to_char(const Col col) { return 'a' + static_cast<int>(col); }

/**
 * @brief is_valid_col 
 * 
 * Returns if col is valid.
 * 
 * @param[in] col The column coordinate
 * 
 * @return
 * - TRUE if col >= COL_A && col <= COL_H.
 * - FALSE if col is invalid.
 * 
 */
constexpr inline bool is_valid_col(Col col) { return col >= COL_A && col <= COL_H; }

/**
 * @brief operator+(Col, int)
 * 
 * Addition operator overload
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in] col The col value
 * @param[in] value The integer to add
 * 
 * @return (Col)(col + value)
 */
constexpr inline Col operator+(Col col, int value)
{
    const Col sum = static_cast<Col>(static_cast<int>(col) + value);
    return is_valid_col(sum) ? sum : COL_INVALID;
}

/**
 * @brief operator-(Col, int)
 * 
 * Subtraction operator overload
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in] col The col value
 * @param[in] value The integer to subtract
 * 
 * @return  (Col)(col - value)
 */
constexpr inline Col operator-(Col col, int value)
{
    const Col sub = static_cast<Col>(static_cast<int>(col) - value);
    return is_valid_col(sub) ? sub : COL_INVALID;
}


/**
 * @brief operator++(Col&)
 * Pre-increment operator overload for Col
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col
 * 
 * @return col + 1
 */
constexpr inline Col& operator++(Col& col)
{
    const Col sum = static_cast<Col>(static_cast<int>(col) + 1);
    col = is_valid_col(sum) ? sum : COL_INVALID;
    return col;
}

/**
 * @brief operator++(Col&,int)
 * Post-increment operator overload for Col
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col
 * 
 * @return col(old value)
 */
constexpr inline Col operator++(Col& col, int)
{
    Col old = col;
    const Col sum = static_cast<Col>(static_cast<int>(col) + 1);
    col = is_valid_col(sum) ? sum : COL_INVALID;
    return old;
}

/**
 * @brief operator--(Col&)
 * Pre-decrement operator overload for Col
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col
 * 
 * @return col - 1
 */
constexpr inline Col& operator--(Col& col)
{
    const Col sub = static_cast<Col>(static_cast<int>(col) - 1);
    col = is_valid_col(sub) ? sub : COL_INVALID;
    return col;
}

/**
 * @brief operator--(Col&,int)
 * Post-decrement operator overload for Col
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col
 * 
 * @return col(old value)
 */
constexpr inline Col operator--(Col& col, int)
{
    Col old = col;
    const Col sub = static_cast<Col>(static_cast<int>(col) - 1);
    col = is_valid_col(sub) ? sub : COL_INVALID;
    return old;
}

/**
 * @brief operator+=(Col&, int)
 * 
 * Addition and assignment operator overload
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col The col value
 * @param[in] value The integer to add
 * 
 * @return pointer to (Col)(col + value)
 */
constexpr inline Col& operator+=(Col& col, int value)
{
    const Col sum = static_cast<Col>(static_cast<int>(col) + value);
    col = is_valid_col(sum) ? sum : COL_INVALID;
    return col;
}

/**
 * @brief operator-=(Col&, int)
 * 
 * Subtraction and assignment operator overload
 * 
 * @note out of bounds will result in COL_INVALID
 * 
 * @param[in,out] col The col value
 * @param[in] value The integer to subtract
 * 
 * @return pointer to (Col)(col - value)
 */
constexpr inline Col& operator-=(Col& col, int value)
{
    const Col sub = static_cast<Col>(static_cast<int>(col) - value);
    col = is_valid_col(sub) ? sub : COL_INVALID;
    return col;
}