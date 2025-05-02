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

/**
 * @brief Mask for diagonal 0 (H1-H1).
 *
 * This constant represents a bitboard mask for diagonal 0 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 0.
 */
constexpr uint64_t DIAGONAL_0_MASK = 0x0000000000000080ULL;

/**
 * @brief Mask for diagonal 1 (G1-H2).
 *
 * This constant represents a bitboard mask for diagonal 1 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 1.
 */
constexpr uint64_t DIAGONAL_1_MASK = 0x0000000000008040ULL;

/**
 * @brief Mask for diagonal 2 (F1-H3).
 *
 * This constant represents a bitboard mask for diagonal 2 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 2.
 */
constexpr uint64_t DIAGONAL_2_MASK = 0x0000000000804020ULL;

/**
 * @brief Mask for diagonal 3 (E1-H4).
 *
 * This constant represents a bitboard mask for diagonal 3 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 3.
 */
constexpr uint64_t DIAGONAL_3_MASK = 0x0000000080402010ULL;

/**
 * @brief Mask for diagonal 4 (D1-H5).
 *
 * This constant represents a bitboard mask for diagonal 4 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 4.
 */
constexpr uint64_t DIAGONAL_4_MASK = 0x0000008040201008ULL;

/**
 * @brief Mask for diagonal 5 (C1-H6).
 *
 * This constant represents a bitboard mask for diagonal 5 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 5.
 */
constexpr uint64_t DIAGONAL_5_MASK = 0x0000804020100804ULL;

/**
 * @brief Mask for diagonal 6 (B1-H7).
 *
 * This constant represents a bitboard mask for diagonal 6 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 6.
 */
constexpr uint64_t DIAGONAL_6_MASK = 0x0080402010080402ULL;

/**
 * @brief Mask for diagonal 7 (A1-H8).
 *
 * This constant represents a bitboard mask for diagonal 7 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 7.
 */
constexpr uint64_t DIAGONAL_7_MASK = 0x8040201008040201ULL;

/**
 * @brief Mask for diagonal 8 (A2-G8).
 *
 * This constant represents a bitboard mask for diagonal 8 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 8.
 */
constexpr uint64_t DIAGONAL_8_MASK = 0x4020100804020100ULL;

/**
 * @brief Mask for diagonal 9 (A3-F8).
 *
 * This constant represents a bitboard mask for diagonal 9 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 9.
 */
constexpr uint64_t DIAGONAL_9_MASK = 0x2010080402010000ULL;

/**
 * @brief Mask for diagonal 10 (A4-E8).
 *
 * This constant represents a bitboard mask for diagonal 10 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 10.
 */
constexpr uint64_t DIAGONAL_10_MASK = 0x1008040201000000ULL;

/**
 * @brief Mask for diagonal 11 (A5-D8).
 *
 * This constant represents a bitboard mask for diagonal 11 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 11.
 */
constexpr uint64_t DIAGONAL_11_MASK = 0x0804020100000000ULL;

/**
 * @brief Mask for diagonal 12 (A6-C8).
 *
 * This constant represents a bitboard mask for diagonal 12 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 12.
 */
constexpr uint64_t DIAGONAL_12_MASK = 0x0402010000000000ULL;

/**
 * @brief Mask for diagonal 13 (A7-B8).
 *
 * This constant represents a bitboard mask for diagonal 13 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 13.
 */
constexpr uint64_t DIAGONAL_13_MASK = 0x0201000000000000ULL;

/**
 * @brief Mask for diagonal 14 (A8-A8).
 *
 * This constant represents a bitboard mask for diagonal 14 of the chessboard.
 * Each bit in the mask corresponds to a square in diagonal 14.
 */
constexpr uint64_t DIAGONAL_14_MASK = 0x0100000000000000ULL;

static constexpr uint64_t DIAGONAL_MASKS[15] = {
    DIAGONAL_0_MASK,  DIAGONAL_1_MASK,  DIAGONAL_2_MASK,  DIAGONAL_3_MASK,  DIAGONAL_4_MASK,
    DIAGONAL_5_MASK,  DIAGONAL_6_MASK,  DIAGONAL_7_MASK,  DIAGONAL_8_MASK,  DIAGONAL_9_MASK,
    DIAGONAL_10_MASK, DIAGONAL_11_MASK, DIAGONAL_12_MASK, DIAGONAL_13_MASK, DIAGONAL_14_MASK};  // Masks for each diagonal

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

/**
 * @brief Mask for antidiagonal 0 (A1-A1).
 *
 * This constant represents a bitboard mask for antidiagonal 0 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 0.
 */
constexpr uint64_t ANTIDIAGONAL_0_MASK = 0x0000000000000001ULL;

/**
 * @brief Mask for antidiagonal 1 (B1-A2).
 *
 * This constant represents a bitboard mask for antidiagonal 1 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 1.
 */
constexpr uint64_t ANTIDIAGONAL_1_MASK = 0x0000000000000102ULL;

/**
 * @brief Mask for antidiagonal 2 (C1-A3).
 *
 * This constant represents a bitboard mask for antidiagonal 2 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 2.
 */
constexpr uint64_t ANTIDIAGONAL_2_MASK = 0x0000000000010204ULL;

/**
 * @brief Mask for antidiagonal 3 (D1-A4).
 *
 * This constant represents a bitboard mask for antidiagonal 3 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 3.
 */
constexpr uint64_t ANTIDIAGONAL_3_MASK = 0x0000000001020408ULL;

/**
 * @brief Mask for antidiagonal 4 (E1-A5).
 *
 * This constant represents a bitboard mask for antidiagonal 4 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 4.
 */
constexpr uint64_t ANTIDIAGONAL_4_MASK = 0x0000000102040810ULL;

/**
 * @brief Mask for antidiagonal 5 (F1-A6).
 *
 * This constant represents a bitboard mask for antidiagonal 5 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 5.
 */
constexpr uint64_t ANTIDIAGONAL_5_MASK = 0x0000010204081020ULL;

/**
 * @brief Mask for antidiagonal 6 (G1-A7).
 *
 * This constant represents a bitboard mask for antidiagonal 6 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 6.
 */
constexpr uint64_t ANTIDIAGONAL_6_MASK = 0x0001020408102040ULL;

/**
 * @brief Mask for antidiagonal 7 (H1-A8).
 *
 * This constant represents a bitboard mask for antidiagonal 7 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 7.
 */
constexpr uint64_t ANTIDIAGONAL_7_MASK = 0x0102040810204080ULL;

/**
 * @brief Mask for antidiagonal 8 (H2-B8).
 *
 * This constant represents a bitboard mask for antidiagonal 8 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 8.
 */
constexpr uint64_t ANTIDIAGONAL_8_MASK = 0x0204081020408000ULL;

/**
 * @brief Mask for antidiagonal 9 (H3-C8).
 *
 * This constant represents a bitboard mask for antidiagonal 9 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 9.
 */
constexpr uint64_t ANTIDIAGONAL_9_MASK = 0x0408102040800000ULL;

/**
 * @brief Mask for antidiagonal 10 (H4-D8).
 *
 * This constant represents a bitboard mask for antidiagonal 10 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 10.
 */
constexpr uint64_t ANTIDIAGONAL_10_MASK = 0x0810204080000000ULL;

/**
 * @brief Mask for antidiagonal 11 (H5-E8).
 *
 * This constant represents a bitboard mask for antidiagonal 11 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 11.
 */
constexpr uint64_t ANTIDIAGONAL_11_MASK = 0x1020408000000000ULL;

/**
 * @brief Mask for antidiagonal 12 (H6-F8).
 *
 * This constant represents a bitboard mask for antidiagonal 12 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 12.
 */
constexpr uint64_t ANTIDIAGONAL_12_MASK = 0x2040800000000000ULL;

/**
 * @brief Mask for antidiagonal 13 (H7-G8).
 *
 * This constant represents a bitboard mask for antidiagonal 13 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 13.
 */
constexpr uint64_t ANTIDIAGONAL_13_MASK = 0x4080000000000000ULL;

/**
 * @brief Mask for antidiagonal 14 (H8-H8).
 *
 * This constant represents a bitboard mask for antidiagonal 14 of the chessboard.
 * Each bit in the mask corresponds to a square in antidiagonal 14.
 */
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