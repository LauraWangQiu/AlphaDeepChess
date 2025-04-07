#pragma once

/**
 * @file precomputed_move_data.hpp
 * @brief precomputed move data services.
 *
 * precomputed move data with fancy magic bitboards
 * 
 */

#include "square.hpp"
#include "piece.hpp"
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
     * @brief pieceMoves
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece could move
     * 
     * @note square must be valid and piece should be not Empty
     * 
     * @param[in] square piece square
     * @param[in] piece selected piece
     * @param[in] blockers bitboard with all pieces on the board, they block the path of the piece
     * 
     * @return pieceMoves(square,blockers)
     */
    static inline uint64_t pieceMoves(Square square, Piece piece, uint64_t blockers)
    {

        using MoveGenFunc = uint64_t (*)(Square square, uint64_t blockers);

        // jump table indexed by piece type
        static constexpr MoveGenFunc MOVE_FUNC_TABLE[12] = {
            // White pieces
            [](Square s, uint64_t) { return WHITE_PAWN_ATTACKS[s]; },   // WHITE_PAWN
            [](Square s, uint64_t) { return KNIGHT_ATTACKS[s]; },       // WHITE_KNIGHT
            [](Square s, uint64_t b) { return bishopMoves(s, b); },     // WHITE_BISHOP
            [](Square s, uint64_t b) { return rookMoves(s, b); },       // WHITE_ROOK
            [](Square s, uint64_t b) { return queenMoves(s, b); },      // WHITE_QUEEN
            [](Square s, uint64_t) { return KING_ATTACKS[s]; },         // WHITE_KING
            // Black pieces
            [](Square s, uint64_t) { return BLACK_PAWN_ATTACKS[s]; },   // BLACK_PAWN
            [](Square s, uint64_t) { return KNIGHT_ATTACKS[s]; },       // BLACK_KNIGHT
            [](Square s, uint64_t b) { return bishopMoves(s, b); },     // BLACK_BISHOP
            [](Square s, uint64_t b) { return rookMoves(s, b); },       // BLACK_ROOK
            [](Square s, uint64_t b) { return queenMoves(s, b); },      // BLACK_QUEEN
            [](Square s, uint64_t) { return KING_ATTACKS[s]; }          // BLACK_KING
        };

        assert(is_valid_piece(piece) && piece != Piece::EMPTY);
        assert(square.is_valid());

        return MOVE_FUNC_TABLE[static_cast<int>(piece)](square, blockers);
    }

    /**
     * @brief pieceAttacks
     * 
     * calculates the 64 bit mask with 1 on the squares that the piece in the provided square is attacking on an empty board
     * 
     * @note square must be valid, piece must be valid and not EMPTY
     * 
     * @param[in] square The selected square
     * @param[in] square The selected piece
     * 
     * @return PIECE_ATTACKS[piece][square]
     */
    static inline uint64_t pieceAttacks(Square square, Piece piece)
    {
        assert(square.is_valid());
        assert(is_valid_piece(piece) && piece != Piece::EMPTY);

        return PIECE_ATTACKS[static_cast<int>(piece)][square];
    }

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

    /**
     * @brief calculates the bitboard of the squares in between, (sq1 and sq2 are excluded)
     * 
     * @note sq1 and sq2 must be valid
     * 
     * @param[in] sq1 First square
     * @param[in] sq2 Second square

     * @return (uint64_t) between bitboard
     */
    static inline uint64_t in_between_bitboard(Square sq1, Square sq2)
    {
        assert(sq1.is_valid());
        assert(sq2.is_valid());

        return BETWEEN_BITBOARDS[sq1][sq2];
    }

private:
    static constexpr const std::array<std::array<uint64_t, 64>, 64> init_between_bitboards();

    /**
     * @brief arrays with precomputed attacks for each [square]
     */
    static const std::array<uint64_t, 64> WHITE_PAWN_ATTACKS;
    static const std::array<uint64_t, 64> BLACK_PAWN_ATTACKS;
    static const std::array<uint64_t, 64> KING_ATTACKS;
    static const std::array<uint64_t, 64> KNIGHT_ATTACKS;
    static const std::array<uint64_t, 64> BISHOP_ATTACKS;
    static const std::array<uint64_t, 64> ROOK_ATTACKS;
    static const std::array<uint64_t, 64> QUEEN_ATTACKS;

    /*
    * 0 = WHITE_PAWN_ATTACKS
    * 1 = KNIGHT_ATTACKS
    * 2 = BISHOP_ATTACKS
    * 3 = ROOK_ATTACKS
    * 4 = QUEEN_ATTACKS
    * 5 = KING_ATTACKS
    * 6 = BLACK_PAWN_ATTACKS
    * 7 = KNIGHT_ATTACKS
    * 8 = BISHOP_ATTACKS
    * 9 = ROOK_ATTACKS
    * 10 = QUEEN_ATTACKS 
    * 11 = KING_ATTACKS 
    */
    static constexpr const uint64_t* PIECE_ATTACKS[NUM_CHESS_PIECES - 1] = {
        WHITE_PAWN_ATTACKS.data(), KNIGHT_ATTACKS.data(), BISHOP_ATTACKS.data(),     ROOK_ATTACKS.data(),
        QUEEN_ATTACKS.data(),      KING_ATTACKS.data(),   BLACK_PAWN_ATTACKS.data(), KNIGHT_ATTACKS.data(),
        BISHOP_ATTACKS.data(),     ROOK_ATTACKS.data(),   QUEEN_ATTACKS.data(),      KING_ATTACKS.data()};

    /**
     * @brief BETWEEN_BITBOARDS[64][64]
     *     
     *  Lookup table for the in between bitboard of two squares
     */
    static const std::array<std::array<uint64_t, 64>, 64> BETWEEN_BITBOARDS;

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
