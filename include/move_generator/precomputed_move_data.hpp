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
#include <unordered_map>

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
     * @brief rookMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note blockers bitboard cannot contain a piece outside the movement mask
     * 
     * @param[in] square rook square
     * @param[in] blockers bitboard of pieces that block the rook movement
     * 
     * @return (*ROOK_MOVES[square])[blockers]
     */
    static inline uint64_t rookMoves(Square square, uint64_t blockers) { return (*ROOK_MOVES[square])[blockers]; }

    /**
     * @brief bishopMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note blockers bitboard cannot contain a piece outside the movement mask
     * 
     * @param[in] square bishop square
     * @param[in] blockers bitboard of pieces that block the bishop movement
     * 
     * @return (*BISHOP_MOVES[square])[blockers]
     */
    static inline uint64_t bishopMoves(Square square, uint64_t blockers) { return (*BISHOP_MOVES[square])[blockers]; }

    /**
     * @brief queenMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note blockers bitboard cannot contain a piece outside the movement mask
     * 
     * @param[in] square queen square
     * @param[in] ortogonal_blockers bitboard of pieces that block the queen movement vertically and horizontlly
     * @param[in] diagonal_blockers bitboard of pieces that block the queen movement in diagonal
     * 
     * @return rookMoves(square, ortogonal_blockers) | bishopMoves(square, diagonal_blockers)
     */
    static inline uint64_t queenMoves(Square square, uint64_t ortogonal_blockers, uint64_t diagonal_blockers)
    {
        return rookMoves(square, ortogonal_blockers) | bishopMoves(square, diagonal_blockers);
    }

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

    static uint64_t calculateLegalRookMoves(Square square, uint64_t blockerBB);
    static uint64_t calculateLegalBishopMoves(Square square, uint64_t blockerBB);

    static std::array<uint64_t, 64> WHITE_PAWN_ATTACKS;
    static std::array<uint64_t, 64> BLACK_PAWN_ATTACKS;
    static std::array<uint64_t, 64> KING_ATTACKS;
    static std::array<uint64_t, 64> KNIGHT_ATTACKS;
    static std::array<uint64_t, 64> BISHOP_ATTACKS;
    static std::array<uint64_t, 64> ROOK_ATTACKS;
    static std::array<uint64_t, 64> QUEEN_ATTACKS;

    /**
     * @brief ROOK_MOVES
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static std::unordered_map<uint64_t, uint64_t>* ROOK_MOVES[64];

    /**
     * @brief BISHOP_MOVES
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static std::unordered_map<uint64_t, uint64_t>* BISHOP_MOVES[64];
};