#pragma once

/**
 * @file row.hpp
 * @brief Board chess Rows types and utilities declaration.
 *
 */


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
