#pragma once

/**
 * @file row.hpp
 * @brief Board chess Rows types and utilities declaration.
 *
 */

#include <cstdint>

/**
 * @brief Row
 * 
 * Represents all the rows of a chess board from ROW_1(0) to ROW_8(7)
 * 
 */
enum Row : int
{
    ROW_1 = 0,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8,
    ROW_INVALID
};

constexpr uint64_t ROW_1_MASK = 0x00000000000000FFULL;
constexpr uint64_t ROW_2_MASK = 0x000000000000FF00ULL;
constexpr uint64_t ROW_3_MASK = 0x0000000000FF0000ULL;
constexpr uint64_t ROW_4_MASK = 0x00000000FF000000ULL;
constexpr uint64_t ROW_5_MASK = 0x000000FF00000000ULL;
constexpr uint64_t ROW_6_MASK = 0x0000FF0000000000ULL;
constexpr uint64_t ROW_7_MASK = 0x00FF000000000000ULL;
constexpr uint64_t ROW_8_MASK = 0xFF00000000000000ULL;

static constexpr uint64_t ROW_MASKS[8] = {ROW_1_MASK, ROW_2_MASK, ROW_3_MASK, ROW_4_MASK,
                                          ROW_5_MASK, ROW_6_MASK, ROW_7_MASK, ROW_8_MASK};

/**
 * @brief row_to_char(const Row row)
 * 
 * row conversion to char.
 * 
 * @note row must be valid
 * 
 * @param[in] row The row value
 * 
 * @return '1','2','3','4','5','6','7','8'
 */
constexpr inline char row_to_char(const Row row) { return '1' + static_cast<int>(row); }

/**
 * @brief is_valid_row
 * 
 * Returns if the coordinaterow is valid
 * 
 * @param[in] row The row
 * 
 * @return
 * - TRUE if row >= ROW_1 && row <= ROW_8
 * - FALSE if row is invalid.
 * 
 */
constexpr inline bool is_valid_row(Row row) { return row >= ROW_1 && row <= ROW_8; }

/**
 * @brief operator+(Row, int)
 * 
 * Addition operator overload
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in] row The row value
 * @param[in] value The integer to add
 * 
 * @return (Row)(row + value)
 */
constexpr inline Row operator+(Row row, int value)
{
    const Row sum = static_cast<Row>(static_cast<int>(row) + value);
    return is_valid_row(sum) ? sum : ROW_INVALID;
}

/**
 * @brief operator-(Row, int)
 * 
 * Subtraction operator overload
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in] row The row value
 * @param[in] value The integer to subtract
 * 
 * @return  (Row)(row - value)
 */
constexpr inline Row operator-(Row row, int value)
{
    const Row sub = static_cast<Row>(static_cast<int>(row) - value);
    return is_valid_row(sub) ? sub : ROW_INVALID;
}

/**
 * @brief operator++(Row&)
 * 
 * Pre-increment operator overload for Row
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row
 * 
 * @return row + 1
 */
constexpr inline Row& operator++(Row& row)
{
    const Row sum = static_cast<Row>(static_cast<int>(row) + 1);
    row = is_valid_row(sum) ? sum : ROW_INVALID;
    return row;
}

/**
 * @brief operator++(Row&,int)
 * Post-increment operator overload for Row
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row
 * 
 * @return row(old value)
 */
constexpr inline Row operator++(Row& row, int)
{

    Row old = row;
    const Row sum = static_cast<Row>(static_cast<int>(row) + 1);
    row = is_valid_row(sum) ? sum : ROW_INVALID;
    return old;
}

/**
 * @brief operator--(Row&)
 * Pre-decrement operator overload for Row
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row
 * 
 * @return row - 1
 */
constexpr inline Row& operator--(Row& row)
{
    const Row sub = static_cast<Row>(static_cast<int>(row) - 1);
    row = is_valid_row(sub) ? sub : ROW_INVALID;
    return row;
}

/**
 * @brief operator--(Row&,int)
 * Post-decrement operator overload for Row
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row
 * 
 * @return row(old value)
 */
constexpr inline Row operator--(Row& row, int)
{
    Row old = row;
    const Row sub = static_cast<Row>(static_cast<int>(row) - 1);
    row = is_valid_row(sub) ? sub : ROW_INVALID;
    return old;
}

/**
 * @brief operator+=(Row&, int)
 * 
 * Addition and assignment operator overload
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row The row value
 * @param[in] value The integer to add
 * 
 * @return pointer to (Row)(row + value)
 */
constexpr inline Row& operator+=(Row& row, int value)
{
    const Row sum = static_cast<Row>(static_cast<int>(row) + value);
    row = is_valid_row(sum) ? sum : ROW_INVALID;
    return row;
}

/**
 * @brief operator-=(Row&, int)
 * 
 * Subtraction and assignment operator overload
 * 
 * @note out of bounds will result in ROW_INVALID
 * 
 * @param[in,out] row The row value
 * @param[in] value The integer to subtract
 * 
 * @return pointer to (Row)(row - value)
 */
constexpr inline Row& operator-=(Row& row, int value)
{
    const Row sub = static_cast<Row>(static_cast<int>(row) - value);
    row = is_valid_row(sub) ? sub : ROW_INVALID;
    return row;
}

/**
 * @brief get_row_mask
 * 
 * calculates the mask with all 1 in the selected row
 * 
 * @note out of bounds will result in mask 0
 * 
 * @param[in] row The selected row
 * 
 * @return ROW_MASKS[row]
 */
constexpr inline uint64_t get_row_mask(Row row)
{
    return is_valid_row(row) ? ROW_MASKS[row] : 0ULL;
}