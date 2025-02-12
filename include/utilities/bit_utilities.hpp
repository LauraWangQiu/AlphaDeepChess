#pragma once

/**
 * @file bit_utilities.hpp
 * @brief bit manipulation utilities.
 *
 */

#include <bit>
#include <cstdint>
#include <cassert>

/**
 * @brief lsb(uint64_t bits)
 * 
 * Returns the index of the least significant bit (LSB) that is set.
 * 
 * @note bits should be non-zero.
 * 
 * @param[in] bits The bitboard to analyze.
 * 
 * @return Index of the least significant set bit.
 */
constexpr inline uint8_t lsb(uint64_t bits)
{
    assert(bits);
    return std::countr_zero(bits);
}

/**
 * @brief msb(uint64_t bits)
 * 
 * Returns the index of the most significant bit (MSB) that is set.
 * 
 * @note bits should be non-zero.
 * 
 * @param[in] bits The bitboard to analyze.
 * 
 * @return Index of the most significant set bit.
 */
constexpr inline uint8_t msb(uint64_t bits)
{
    assert(bits);
    return 63 - std::countl_zero(bits);  // 63 - leading zeros gives the MSB index
}

/**
 * @brief pop_lsb(uint64_t& bits)
 * 
 * return the least significant bit that is 1 and clears it.
 * 
 * @note bits should be non zero
 * 
 * @param[in,out] bits bits to operate, bits &= bits - 1;
 * 
 * @return lsb(bits)
 */
constexpr inline uint8_t pop_lsb(uint64_t& bits)
{
    assert(bits);

    uint8_t lsb = std::countr_zero(bits);   // Get index of LSB
    bits &= bits - 1;                       // Clear the LSB
    return lsb;
}

/**
 * @brief pop_msb(uint64_t& bits)
 * 
 * Returns the index of the most significant bit that is set, then clears it.
 * 
 * @note bits should be non-zero.
 * 
 * @param[in,out] bits Bitboard to modify (MSB will be cleared).
 * 
 * @return Index of the most significant set bit.
 */
constexpr inline uint8_t pop_msb(uint64_t& bits)
{
    assert(bits);

    uint8_t msb_index = 63 - std::countl_zero(bits);
    bits &= ~(1ULL << msb_index);  // Clears the MSB
    return msb_index;
}