#pragma once

/**
 * @file col.hpp
 * @brief Board chess Column types and utilities declaration.
 *
 */

#include <cstdint>
#include <cassert>

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

constexpr uint64_t COL_A_MASK = 0x0101010101010101ULL;
constexpr uint64_t COL_B_MASK = COL_A_MASK << 1;
constexpr uint64_t COL_C_MASK = COL_A_MASK << 2;
constexpr uint64_t COL_D_MASK = COL_A_MASK << 3;
constexpr uint64_t COL_E_MASK = COL_A_MASK << 4;
constexpr uint64_t COL_F_MASK = COL_A_MASK << 5;
constexpr uint64_t COL_G_MASK = COL_A_MASK << 6;
constexpr uint64_t COL_H_MASK = COL_A_MASK << 7;

static constexpr uint64_t COL_MASKS[8] = {COL_A_MASK, COL_B_MASK, COL_C_MASK, COL_D_MASK,
                                          COL_E_MASK, COL_F_MASK, COL_G_MASK, COL_H_MASK};

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
    assert(is_valid_col(col));
    return 'a' + static_cast<int>(col);
}

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

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
    assert(is_valid_col(col));

    const Col sub = static_cast<Col>(static_cast<int>(col) - value);
    col = is_valid_col(sub) ? sub : COL_INVALID;
    return col;
}


/**
 * @brief get_col_mask
 * 
 * calculates the mask with all 1 in the selected col
 * 
 * @note out of bounds will result in mask 0
 * 
 * @param[in] col The selected col
 * 
 * @return COL_MASKS[col]
 */
constexpr inline uint64_t get_col_mask(Col col)
{
    assert(is_valid_col(col));
    return is_valid_col(col) ? COL_MASKS[col] : 0ULL;
}