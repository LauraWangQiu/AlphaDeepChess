#pragma once

/**
 * @file precomputed_move_data.hpp
 * @brief precomputed move data services.
 *
 * precomputed move data
 * 
 */

#include "square.hpp"
#include <array>
#include <cassert>

/**
 * @brief PrecomputedMoveData
 *
 * class with the precomputed moves of chess pieces
 * 
 */
class PrecomputedMoveData
{
public:
    PrecomputedMoveData() = delete;

    ~PrecomputedMoveData() = delete;

    /**
     * @brief kingAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return KING_ATTACKS[square]
     */
    static inline uint64_t kingAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? KING_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief knightAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return KNIGHT_ATTACKS[square]
     */
    static inline uint64_t knightAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? KNIGHT_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief whitePawnAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return WHITE_PAWN_ATTACKS[square]
     */
    static inline uint64_t whitePawnAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? WHITE_PAWN_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief blackPawnAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return BLACK_PAWN_ATTACKS[square]
     */
    static inline uint64_t blackPawnAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? BLACK_PAWN_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief rookAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return ROOK_ATTACKS[square]
     */
    static inline uint64_t rookAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? ROOK_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief bishopAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return BISHOP_ATTACKS[square]
     */
    static inline uint64_t bishopAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? BISHOP_ATTACKS[square] : 0ULL;
    }

    /**
     * @brief queenAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note out of bounds will result in mask 0
     * 
     * @param[in] square The selected square
     * 
     * @return QUEEN_ATTACKS[square]
     */
    static inline uint64_t queenAttacks(Square square)
    {
        assert(square.is_valid());
        return square.is_valid() ? QUEEN_ATTACKS[square] : 0ULL;
    }

private:
    static constexpr std::array<uint64_t, 64> initializeWhitePawnAttacks();
    static constexpr std::array<uint64_t, 64> initializeBlackPawnAttacks();
    static constexpr std::array<uint64_t, 64> initializeKingAttacks();
    static constexpr std::array<uint64_t, 64> initializeKnightAttacks();
    static constexpr std::array<uint64_t, 64> initializeRookAttacks();
    static constexpr std::array<uint64_t, 64> initializeBishopAttacks();
    static constexpr std::array<uint64_t, 64> initializeQueenAttacks();

    static std::array<uint64_t, 64> WHITE_PAWN_ATTACKS;
    static std::array<uint64_t, 64> BLACK_PAWN_ATTACKS;
    static std::array<uint64_t, 64> KING_ATTACKS;
    static std::array<uint64_t, 64> KNIGHT_ATTACKS;
    static std::array<uint64_t, 64> BISHOP_ATTACKS;
    static std::array<uint64_t, 64> ROOK_ATTACKS;
    static std::array<uint64_t, 64> QUEEN_ATTACKS;
};

std::array<uint64_t, 64> PrecomputedMoveData::WHITE_PAWN_ATTACKS = initializeWhitePawnAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::BLACK_PAWN_ATTACKS = initializeBlackPawnAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::KING_ATTACKS = initializeKingAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::KNIGHT_ATTACKS = initializeKnightAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::BISHOP_ATTACKS = initializeBishopAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::ROOK_ATTACKS = initializeRookAttacks();
std::array<uint64_t, 64> PrecomputedMoveData::QUEEN_ATTACKS = initializeQueenAttacks();

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeKingAttacks()
{
    std::array<uint64_t, 64> KING_ATTACKS {};

    const int dirs[8][2] = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};


    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square king_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    KING_ATTACKS[king_square] |= attack_square.mask();
                }
            }
        }
    }
    return KING_ATTACKS;
}


constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeKnightAttacks()
{
    std::array<uint64_t, 64> KNIGHT_ATTACKS {};

    const int dirs[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square knight_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {
                    KNIGHT_ATTACKS[knight_square] |= attack_square.mask();
                }
            }
        }
    }
    return KNIGHT_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeWhitePawnAttacks()
{
    std::array<uint64_t, 64> WHITE_PAWN_ATTACKS {};

    const int dirs[2][2] = {{1, 1}, {1, -1}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square pawn_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    WHITE_PAWN_ATTACKS[pawn_square] |= attack_square.mask();
                }
            }
        }
    }
    return WHITE_PAWN_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeBlackPawnAttacks()
{
    std::array<uint64_t, 64> BLACK_PAWN_ATTACKS {};

    const int dirs[2][2] = {{-1, -1}, {-1, +1}};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square pawn_square = Square(row, col);

            for (auto& dir : dirs) {

                const Row attack_row = row + static_cast<Row>(dir[0]);
                const Col attack_col = col + static_cast<Col>(dir[1]);

                const Square attack_square = Square(attack_row, attack_col);

                if (attack_square.is_valid()) {

                    BLACK_PAWN_ATTACKS[pawn_square] |= attack_square.mask();
                }
            }
        }
    }
    return BLACK_PAWN_ATTACKS;
}

constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeRookAttacks()
{
    std::array<uint64_t, 64> ROOK_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {
            const Square rook_square = Square(row, col);
            ROOK_ATTACKS[rook_square] = get_row_mask(row) | get_col_mask(col);
            ROOK_ATTACKS[rook_square] &= ~rook_square.mask();   // Remove the piece itself
        }
    }
    return ROOK_ATTACKS;
}
constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeBishopAttacks()
{
    std::array<uint64_t, 64> BISHOP_ATTACKS {};

    for (Row row = ROW_1; is_valid_row(row); row++) {
        for (Col col = COL_A; is_valid_col(col); col++) {

            const Square bishop_square = Square(row, col);

            const Diagonal bishop_diagonal = bishop_square.diagonal();
            const AntiDiagonal bishop_antidiagonal = bishop_square.antidiagonal();

            BISHOP_ATTACKS[bishop_square] = get_diagonal_mask(bishop_diagonal);
            BISHOP_ATTACKS[bishop_square] |= get_antidiagonal_mask(bishop_antidiagonal);

            BISHOP_ATTACKS[bishop_square] &= ~bishop_square.mask();   // Remove the piece itself
        }
    }
    return BISHOP_ATTACKS;
}
constexpr std::array<uint64_t, 64> PrecomputedMoveData::initializeQueenAttacks()
{
    std::array<uint64_t, 64> QUEEN_ATTACKS {};

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        QUEEN_ATTACKS[sq] = ROOK_ATTACKS[sq] | BISHOP_ATTACKS[sq];
    }
    return QUEEN_ATTACKS;
}