#pragma once

/**
 * @file precomputed_move_data.hpp
 * @brief precomputed move data services.
 *
 * precomputed move data
 * 
 */

#include "square.hpp"
#include "magic_numbers.hpp"
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
     * @return ROOK_MOVES[square][blockers]
     */
    static inline uint64_t rookMoves(Square square, uint64_t blockers)
    {
        assert(square.is_valid());
        assert((blockers & ~(ROOK_ATTACKS[square])) == 0ULL);
        const uint64_t index = (blockers * rook_magic(square)) >> (64 - rook_occupancy_number(square));
        assert(index < 4096);
        return ROOK_MOVES[square][index];
    }

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
     * @return BISHOP_MOVES[square][blockers]
     */
    static inline uint64_t bishopMoves(Square square, uint64_t blockers)
    {
        assert(square.is_valid());
        assert((blockers & ~(BISHOP_ATTACKS[square])) == 0ULL);
        const uint64_t index = (blockers * bishop_magic(square)) >> (64 - bishop_occupancy_number(square));
        assert(index < 512);
        return BISHOP_MOVES[square][index];
    }

    /**
     * @brief queenMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note blockers bitboard cannot contain a piece outside the movement mask
     * 
     * @param[in] square queen square
     * @param[in] orthogonal_blockers bitboard of pieces that block the queen movement vertically and horizontlly
     * @param[in] diagonal_blockers bitboard of pieces that block the queen movement in diagonal
     * 
     * @return rookMoves(square, ortogonal_blockers) | bishopMoves(square, diagonal_blockers)
     */
    static inline uint64_t queenMoves(Square square, uint64_t orthogonal_blockers, uint64_t diagonal_blockers)
    {
        assert(square.is_valid());
        assert((orthogonal_blockers & ~(ROOK_ATTACKS[square])) == 0ULL);
        assert((diagonal_blockers & ~(BISHOP_ATTACKS[square])) == 0ULL);
        return rookMoves(square, orthogonal_blockers) | bishopMoves(square, diagonal_blockers);
    }

    /**
     * @brief kingAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return KING_ATTACKS[square]
     */
    static inline uint64_t kingAttacks(Square square)
    {
        assert(square.is_valid());
        return KING_ATTACKS[square];
    }

    /**
     * @brief knightAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return KNIGHT_ATTACKS[square]
     */
    static inline uint64_t knightAttacks(Square square)
    {
        assert(square.is_valid());
        return KNIGHT_ATTACKS[square];
    }

    /**
     * @brief whitePawnAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return WHITE_PAWN_ATTACKS[square]
     */
    static inline uint64_t whitePawnAttacks(Square square)
    {
        assert(square.is_valid());
        return WHITE_PAWN_ATTACKS[square];
    }

    /**
     * @brief blackPawnAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return BLACK_PAWN_ATTACKS[square]
     */
    static inline uint64_t blackPawnAttacks(Square square)
    {
        assert(square.is_valid());
        return BLACK_PAWN_ATTACKS[square];
    }

    /**
     * @brief rookAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return ROOK_ATTACKS[square]
     */
    static inline uint64_t rookAttacks(Square square)
    {
        assert(square.is_valid());
        return ROOK_ATTACKS[square];
    }

    /**
     * @brief bishopAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return BISHOP_ATTACKS[square]
     */
    static inline uint64_t bishopAttacks(Square square)
    {
        assert(square.is_valid());
        return BISHOP_ATTACKS[square];
    }

    /**
     * @brief queenAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid
     * 
     * @param[in] square The selected square
     * 
     * @return  ROOK_ATTACKS[square] | BISHOP_ATTACKS[square]
     */
    static inline uint64_t queenAttacks(Square square)
    {
        assert(square.is_valid());
        return ROOK_ATTACKS[square] | BISHOP_ATTACKS[square];
    }

private:
    static const std::array<uint64_t, 64> WHITE_PAWN_ATTACKS;
    static const std::array<uint64_t, 64> BLACK_PAWN_ATTACKS;
    static const std::array<uint64_t, 64> KING_ATTACKS;
    static const std::array<uint64_t, 64> KNIGHT_ATTACKS;
    static const std::array<uint64_t, 64> BISHOP_ATTACKS;
    static const std::array<uint64_t, 64> ROOK_ATTACKS;

    /**
     * @brief ROOK_MOVES
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static const std::array<std::array<uint64_t, 512>, 64> BISHOP_MOVES;


    /**
     * @brief BISHOP_MOVES
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static const std::array<std::array<uint64_t, 4096>, 64> ROOK_MOVES;
};
