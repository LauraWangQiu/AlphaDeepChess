#pragma once

/**
 * @file diagonal.hpp
 * @brief Diagonal types and utilities declaration.
 *
 */

#include <cstdint>

/**
 * @brief Diagonal
 * 
 * Represents all the diagonals of a chess board
 * 
 */
enum Diagonal : int
{
    DIAGONAL_0_H1_H1 = 0,
    DIAGONAL_1_G1_H2,
    DIAGONAL_2_F1_H3,
    DIAGONAL_3_E1_H4,
    DIAGONAL_4_D1_H5,
    DIAGONAL_5_C1_H6,
    DIAGONAL_6_B1_H7,
    DIAGONAL_7_A1_H8,
    DIAGONAL_8_A2_G8,
    DIAGONAL_9_A3_F8,
    DIAGONAL_10_A4_E8,
    DIAGONAL_11_A5_D8,
    DIAGONAL_12_A6_C8,
    DIAGONAL_13_A7_B8,
    DIAGONAL_14_A8_A8,
    DIAGONAL_INVALID
};

constexpr uint64_t DIAGONAL_0_MASK = 0x0000000000000080ULL;
constexpr uint64_t DIAGONAL_1_MASK = 0x0000000000008040ULL;
constexpr uint64_t DIAGONAL_2_MASK = 0x0000000000804020ULL;
constexpr uint64_t DIAGONAL_3_MASK = 0x0000000080402010ULL;
constexpr uint64_t DIAGONAL_4_MASK = 0x0000008040201008ULL;
constexpr uint64_t DIAGONAL_5_MASK = 0x0000804020100804ULL;
constexpr uint64_t DIAGONAL_6_MASK = 0x0080402010080402ULL;
constexpr uint64_t DIAGONAL_7_MASK = 0x8040201008040201ULL;
constexpr uint64_t DIAGONAL_8_MASK = 0x4020100804020100ULL;
constexpr uint64_t DIAGONAL_9_MASK = 0x2010080402010000ULL;
constexpr uint64_t DIAGONAL_10_MASK = 0x1008040201000000ULL;
constexpr uint64_t DIAGONAL_11_MASK = 0x0804020100000000ULL;
constexpr uint64_t DIAGONAL_12_MASK = 0x0402010000000000ULL;
constexpr uint64_t DIAGONAL_13_MASK = 0x0201000000000000ULL;
constexpr uint64_t DIAGONAL_14_MASK = 0x0100000000000000ULL;

static constexpr uint64_t DIAGONAL_MASKS[15] = {
    DIAGONAL_0_MASK,  DIAGONAL_1_MASK,  DIAGONAL_2_MASK,  DIAGONAL_3_MASK,  DIAGONAL_4_MASK,
    DIAGONAL_5_MASK,  DIAGONAL_6_MASK,  DIAGONAL_7_MASK,  DIAGONAL_8_MASK,  DIAGONAL_9_MASK,
    DIAGONAL_10_MASK, DIAGONAL_11_MASK, DIAGONAL_12_MASK, DIAGONAL_13_MASK, DIAGONAL_14_MASK};

/**
 * @brief AntiDiagonal
 * 
 * Represents all the anti-diagonals of a chess board
 * 
 */
enum AntiDiagonal : int
{
    ANTIDIAGONAL_0_A1_A1 = 0,
    ANTIDIAGONAL_1_B1_A2,
    ANTIDIAGONAL_2_C1_A3,
    ANTIDIAGONAL_3_D1_A4,
    ANTIDIAGONAL_4_E1_A5,
    ANTIDIAGONAL_5_F1_A6,
    ANTIDIAGONAL_6_G1_A7,
    ANTIDIAGONAL_7_H1_A8,
    ANTIDIAGONAL_8_H2_B8,
    ANTIDIAGONAL_9_H3_C8,
    ANTIDIAGONAL_10_H4_D8,
    ANTIDIAGONAL_11_H5_E8,
    ANTIDIAGONAL_12_H6_F8,
    ANTIDIAGONAL_13_H7_G8,
    ANTIDIAGONAL_14_H8_H8,
    ANTIDIAGONAL_INVALID
};

constexpr uint64_t ANTIDIAGONAL_0_MASK = 0x0000000000000001ULL;
constexpr uint64_t ANTIDIAGONAL_1_MASK = 0x0000000000000102ULL;
constexpr uint64_t ANTIDIAGONAL_2_MASK = 0x0000000000010204ULL;
constexpr uint64_t ANTIDIAGONAL_3_MASK = 0x0000000001020408ULL;
constexpr uint64_t ANTIDIAGONAL_4_MASK = 0x0000000102040810ULL;
constexpr uint64_t ANTIDIAGONAL_5_MASK = 0x0000010204081020ULL;
constexpr uint64_t ANTIDIAGONAL_6_MASK = 0x0001020408102040ULL;
constexpr uint64_t ANTIDIAGONAL_7_MASK = 0x0102040810204080ULL;
constexpr uint64_t ANTIDIAGONAL_8_MASK = 0x0204081020408000ULL;
constexpr uint64_t ANTIDIAGONAL_9_MASK = 0x0408102040800000ULL;
constexpr uint64_t ANTIDIAGONAL_10_MASK = 0x0810204080000000ULL;
constexpr uint64_t ANTIDIAGONAL_11_MASK = 0x1020408000000000ULL;
constexpr uint64_t ANTIDIAGONAL_12_MASK = 0x2040800000000000ULL;
constexpr uint64_t ANTIDIAGONAL_13_MASK = 0x4080000000000000ULL;
constexpr uint64_t ANTIDIAGONAL_14_MASK = 0x8000000000000000ULL;

static constexpr uint64_t ANTIDIAGONAL_MASKS[15] = {
    ANTIDIAGONAL_0_MASK,  ANTIDIAGONAL_1_MASK,  ANTIDIAGONAL_2_MASK,  ANTIDIAGONAL_3_MASK,
    ANTIDIAGONAL_4_MASK,  ANTIDIAGONAL_5_MASK,  ANTIDIAGONAL_6_MASK,  ANTIDIAGONAL_7_MASK,
    ANTIDIAGONAL_8_MASK,  ANTIDIAGONAL_9_MASK,  ANTIDIAGONAL_10_MASK, ANTIDIAGONAL_11_MASK,
    ANTIDIAGONAL_12_MASK, ANTIDIAGONAL_13_MASK, ANTIDIAGONAL_14_MASK};


/**
 * @brief get_diagonal_mask
 * 
 * calculates the mask with all 1 in the selected diagonal
 * 
 * @note out of bounds will result in mask 0
 * 
 * @param[in] diagonal The selected diagonal
 * 
 * @return DIAGONAL_MASKS[diagonal]
 */
constexpr inline uint64_t get_diagonal_mask(Diagonal diagonal)
{
    return (0 <= diagonal && diagonal < 15) ? DIAGONAL_MASKS[diagonal] : 0ULL;
}

/**
 * @brief get_antidiagonal_mask
 * 
 * calculates the mask with all 1 in the selected antidiagonal
 * 
 * @note out of bounds will result in mask 0
 * 
 * @param[in] antidiagonal The selected antidiagonal
 * 
 * @return ANTIDIAGONAL_MASKS[antidiagonal]
 */
constexpr inline uint64_t get_antidiagonal_mask(AntiDiagonal antidiagonal)
{
    return (0 <= antidiagonal && antidiagonal < 15) ? ANTIDIAGONAL_MASKS[antidiagonal] : 0ULL;
}