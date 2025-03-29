#pragma once

/**
 * @file precomputed_data.hpp
 * @brief precomputed data.
 *
 * precomputed data
 * 
 */
#include "square.hpp"
#include <array>
#include <cassert>

/**
 * @brief PrecomputedData
 *
 * class with the precomputed data between chess pieces
 * 
 */
class PrecomputedData
{
public:
    PrecomputedData() = delete;

    ~PrecomputedData() = delete;

    /**
     * @brief get_distance_chebyshev
     * 
     * Get the distance between two squares using Chebyshev's Distance.
     * https://en.wikipedia.org/wiki/Chebyshev_distance
     * 
     * @note None.
     * 
     * @param[in] square1 First square to calculate distance
     * @param[in] square2 Second square to calculate distance
     * 
     * @return chebyshevDistance(square1, square2)
     */
    static inline float get_distance_chebyshev(Square square1, Square square2)
    {
        assert(square1.is_valid() && square2.is_valid());
        return CHEBYSHEV_DISTANCE[square1][square2];
    }

    /**
     * @brief get_distance_manhattan 
     * 
     * Get the distance between two squares using Manhattan's Distance.
     * https://simple.wikipedia.org/wiki/Manhattan_distance
     * 
     * @note None.
     * 
     * @param[in] square1 First square to calculate distance
     * @param[in] square2 Second square to calculate distance
     * 
     * @return manhattanDistance(square1, square2)
     */
    static inline float get_distance_manhattan(Square square1, Square square2)
    {
        assert(square1.is_valid() && square2.is_valid());
        return MANHATTAN_DISTANCE[square1][square2];
    }

    /**
     * @brief get_safety_table 
     * 
     * Get the safety table value for a number of pieces.
     * 
     * @note None.
     * 
     * @param[in] number_pieces number of pieces to get the safety value.
     * 
     * @return valueOfAttack(number_pieces)
     */
    static inline float get_safety_table(int number_pieces)
    {
        assert(number_pieces >= 0 && number_pieces < 100);
        return SAFETY_TABLE[number_pieces];
    }

    /**
     * @brief get_pawn_piece_square_table 
     * 
     * Get the pawn piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the pawn.
     * 
     * @return PAWN_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_pawn_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? PAWN_PIECE_SQUARE_TABLE[int(square)] : PAWN_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_rook_piece_square_table 
     * 
     * Get the rook piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the rook.
     * 
     * @return ROOK_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_rook_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? ROOK_PIECE_SQUARE_TABLE[int(square)] : ROOK_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_knight_piece_square_table 
     * 
     * Get the knight piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the knight.
     * 
     * @return KNIGHT_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_knight_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? KNIGHT_PIECE_SQUARE_TABLE[int(square)] : KNIGHT_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_bishop_piece_square_table 
     * 
     * Get the bishop piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the bishop.
     * 
     * @return BISHOP_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_bishop_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? BISHOP_PIECE_SQUARE_TABLE[int(square)] : BISHOP_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_queen_piece_square_table 
     * 
     * Get the queen piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the queen.
     * 
     * @return QUEEN_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_queen_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? QUEEN_PIECE_SQUARE_TABLE[int(square)] : QUEEN_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_king_middle_game_piece_square_table 
     * 
     * Get the king middle game piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the king.
     * 
     * @return KING_MIDDLE_GAME_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_king_middle_game_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[int(square)] : KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[63 - int(square)];
    }

    /**
     * @brief get_king_end_game_piece_square_table 
     * 
     * Get the king end game piece-square table value from a square.
     * 
     * @note None.
     * 
     * @param[in] square square to get the value.
     * @param[in] color color of the king.
     * 
     * @return KING_END_GAME_PIECE_SQUARE_TABLE(square)
     */
    static inline float get_king_end_game_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        return (color == ChessColor::WHITE) ? KING_END_GAME_PIECE_SQUARE_TABLE[int(square)] : KING_END_GAME_PIECE_SQUARE_TABLE[63 - int(square)];
    }

private:
    static constexpr void initialize_distance_matrices();

    static float CHEBYSHEV_DISTANCE[64][64];
    static float MANHATTAN_DISTANCE[64][64];

    static const int SAFETY_TABLE[100];

    static const int PAWN_PIECE_SQUARE_TABLE[64];
    static const int ROOK_PIECE_SQUARE_TABLE[64];
    static const int KNIGHT_PIECE_SQUARE_TABLE[64];
    static const int BISHOP_PIECE_SQUARE_TABLE[64];
    static const int QUEEN_PIECE_SQUARE_TABLE[64];
    static const int KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[64];
    static const int KING_END_GAME_PIECE_SQUARE_TABLE[64];

    static const bool initialized;
};

float PrecomputedData::CHEBYSHEV_DISTANCE[64][64];
float PrecomputedData::MANHATTAN_DISTANCE[64][64];

// https://www.chessprogramming.org/King_Safety
const int PrecomputedData::SAFETY_TABLE[100] =
{
    0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
  68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
 260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
 494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
 500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};

// https://www.chessprogramming.org/Simplified_Evaluation_Function
const int PrecomputedData::PAWN_PIECE_SQUARE_TABLE[64] =
{
     0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};
const int PrecomputedData::ROOK_PIECE_SQUARE_TABLE[64] =
{
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};
const int PrecomputedData::KNIGHT_PIECE_SQUARE_TABLE[64] =
{
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};
const int PrecomputedData::BISHOP_PIECE_SQUARE_TABLE[64] =
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};
const int PrecomputedData::QUEEN_PIECE_SQUARE_TABLE[64] =
{
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
const int PrecomputedData::KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[64] =
{
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};
const int PrecomputedData::KING_END_GAME_PIECE_SQUARE_TABLE[64] =
{
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

const bool PrecomputedData::initialized = []() {
    PrecomputedData::initialize_distance_matrices();
    return true;
}();

constexpr void PrecomputedData::initialize_distance_matrices()
{
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            const Square sq1 = Square(i);
            const Square sq2 = Square(j);
            CHEBYSHEV_DISTANCE[i][j] = std::abs(int(sq1.row()) - int(sq2.row())) + std::abs(int(sq1.col()) - int(sq2.col()));
            MANHATTAN_DISTANCE[i][j] = std::max(std::abs(int(sq1.row()) - int(sq2.row())), std::abs(int(sq1.col()) - int(sq2.col())));
        }
    }
}
