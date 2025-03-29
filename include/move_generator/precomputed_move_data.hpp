#pragma once

/**
 * @file precomputed_move_data.hpp
 * @brief precomputed move data services.
 *
 * precomputed move data with fancy magic bitboards
 * 
 */

#include "square.hpp"
#include "magic_bitboards.hpp"
#include <array>
#include <cassert>

static int constexpr ROOK_TABLE_SIZE = 4096;
static int constexpr BISHOP_TABLE_SIZE = 512;

typedef std::array<std::array<uint64_t, ROOK_TABLE_SIZE>, NUM_SQUARES> TableRookMoves;
typedef std::array<std::array<uint64_t, BISHOP_TABLE_SIZE>, NUM_SQUARES> TableBishopMoves;

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
     * @note square must be valid
     * 
     * @param[in] square rook square
     * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
     * 
     * @return ROOK_MOVES[square][index]
     */
    static inline uint64_t rookMoves(Square square, uint64_t blockers)
    {
        assert(square.is_valid());

        const uint64_t index = magic_index_rook(blockers, square, rookAttacks(square));
        assert(index < 4096);
        return ROOK_MOVES[square][index];
    }

    /**
     * @brief bishopMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note square must be valid
     * 
     * @param[in] square bishop square
     * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
     * 
     * @return BISHOP_MOVES[square][blockers]
     */
    static inline uint64_t bishopMoves(Square square, uint64_t blockers)
    {
        assert(square.is_valid());

        const uint64_t index = magic_index_bishop(blockers, square, bishopAttacks(square));
        assert(index < 512);
        return BISHOP_MOVES[square][index];
    }

    /**
     * @brief queenMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note square must be valid
     * 
     * @param[in] square queen square
     * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
     * 
     * @return rookMoves(square, blockers) | bishopMoves(square, blockers)
     */
    static inline uint64_t queenMoves(Square square, uint64_t blockers)
    {
        assert(square.is_valid());
        return rookMoves(square, blockers) | bishopMoves(square, blockers);
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
     * @brief pawnAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square and color must be valid
     * 
     * @param[in] square The selected square
     * @param[in] color The selected color
     * 
     * @return PAWN_ATTACKS[color][square]
     */
    static inline uint64_t pawnAttacks(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));
        static constexpr const uint64_t* PAWN_ATTACK[2] = {WHITE_PAWN_ATTACKS.data(), BLACK_PAWN_ATTACKS.data()};

        return PAWN_ATTACK[static_cast<int>(color)][square];
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
     * @brief BISHOP_MOVES[64][BISHOP_TABLE_SIZE]
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static const TableRookMoves ROOK_MOVES;

    /**
     * @brief ROOK_MOVES[64][ROOK_TABLE_SIZE]
     *     
     *  Lookup table for the bishop moves given
     *  the square of the bishop and the bitboard of blocker pieces.
     */
    static const TableBishopMoves BISHOP_MOVES;
};
