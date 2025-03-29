#pragma once

/**
 * @file precomputed_data.hpp
 * @brief precomputed data.
 *
 * precomputed data
 * 
 */
#include "square.hpp"
#include "piece.hpp"
#include <array>
#include <cassert>

/**
 * @brief PrecomputedEvalData
 *
 * class with the precomputed data between chess pieces
 * 
 */
class PrecomputedEvalData
{
public:
    PrecomputedEvalData() = delete;

    ~PrecomputedEvalData() = delete;

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
    static inline int get_distance_chebyshev(Square square1, Square square2)
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
    static inline int get_distance_manhattan(Square square1, Square square2)
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
    static inline int get_safety_table(int number_pieces)
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
    static inline int get_pawn_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return PAWN_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_rook_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return ROOK_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_knight_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return KNIGHT_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_bishop_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return BISHOP_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_queen_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return QUEEN_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_king_middle_game_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[index_sq];
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
    static inline int get_king_end_game_piece_square_table(Square square, ChessColor color)
    {
        assert(square.is_valid());
        assert(is_valid_color(color));

        const int index_sq = (color == ChessColor::WHITE) ? 64 - square.value() : square.value();
        return KING_END_GAME_PIECE_SQUARE_TABLE[index_sq];
    }

private:
    static inline const std::array<std::array<int, 64>, 64> init_chebyshev_distance()
    {
        std::array<std::array<int, 64>, 64> distances;

        for (Square sq1 = Square::SQ_A1; sq1.is_valid(); sq1++) {
            for (Square sq2 = Square::SQ_A1; sq2.is_valid(); sq2++) {
                distances[sq1][sq2] =
                    std::abs(int(sq1.row()) - int(sq2.row())) + std::abs(int(sq1.col()) - int(sq2.col()));
            }
        }

        return distances;
    };

    static inline const std::array<std::array<int, 64>, 64> init_manhattan_distance()
    {
        std::array<std::array<int, 64>, 64> distances;

        for (Square sq1 = Square::SQ_A1; sq1.is_valid(); sq1++) {
            for (Square sq2 = Square::SQ_A1; sq2.is_valid(); sq2++) {
                distances[sq1][sq2] =
                    std::max(std::abs(int(sq1.row()) - int(sq2.row())), std::abs(int(sq1.col()) - int(sq2.col())));
            }
        }

        return distances;
    };


    static inline const std::array<std::array<int, 64>, 64> CHEBYSHEV_DISTANCE = init_chebyshev_distance();
    static inline const std::array<std::array<int, 64>, 64> MANHATTAN_DISTANCE = init_manhattan_distance();

    // clang-format off

    // https://www.chessprogramming.org/King_Safety
    static constexpr int SAFETY_TABLE[100] = {
        0,   0,   1,   2,   3,   5,   7,   9,   12,  15,  18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
        68,  75,  82,  85,  89,  97,  105, 113, 122, 131, 140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
        260, 272, 283, 295, 307, 319, 330, 342, 354, 366, 377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
        494, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};

    // https://www.chessprogramming.org/Simplified_Evaluation_Function
    static constexpr int PAWN_PIECE_SQUARE_TABLE[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        50,  50,  50,  50,  50,  50,  50,  50,
        10,  10,  20,  30,  30,  20,  10,  10,
        5,   5,   10,  25,  25,  10,  5,   5,
        0,   0,   0,   20,  20,  0,   0,   0,
        5,   -5,  -10, 0,   0,   -10, -5,  5,
        5,   10,  10,  -20, -20, 10,  10,  5,
        0,   0,   0,   0,   0,   0,   0,   0
    };

    static constexpr int ROOK_PIECE_SQUARE_TABLE[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        5,   10,  10,  10,  10,  10,  10,  5,
        -5,  0,   0,   0,   0,   0,   0,   -5,
        -5,  0,   0,   0,   0,   0,   0,   -5,
        -5,  0,   0,   0,   0,   0,   0,   -5,
        -5,  0,   0,   0,   0,   0,   0,   -5,
        -5,  0,   0,   0,   0,   0,   0,   -5,
        0,   0,   0,   5,   5,   0,   0,   0
    };

    static constexpr int KNIGHT_PIECE_SQUARE_TABLE[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0,   0,   0,   0,   -20, -40,
        -30, 0,   10,  15,  15,  10,  0,   -30,
        -30, 5,   15,  20,  20,  15,  5,   -30,
        -30, 0,   15,  20,  20,  15,  0,   -30,
        -30, 5,   10,  15,  15,  10,  5,   -30,
        -40, -20, 0,   5,   5,   0,   -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    };

    static constexpr int BISHOP_PIECE_SQUARE_TABLE[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 0,   0,   0,   0,   0,   0,   -10,
        -10, 0,   5,   10,  10,  5,   0,   -10,
        -10, 5,   5,   10,  10,  5,   5,   -10,
        -10, 0,   10,  10,  10,  10,  0,   -10,
        -10, 10,  10,  10,  10,  10,  10,  -10,
        -10, 5,   0,   0,   0,   0,   5,   -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    };
    static constexpr int QUEEN_PIECE_SQUARE_TABLE[64] = {
        -20, -10, -10, -5,  -5,  -10, -10, -20,
        -10, 0,   0,   0,   0,   0,   0,   -10,
        -10, 0,   5,   5,   5,   5,   0,   -10,
        -5,  0,   5,   5,   5,   5,   0,   -5,
        0,   0,   5,   5,   5,   5,   0,   -5,
        -10, 5,   5,   5,   5,   5,   0,   -10, 
        -10, 0,   5,   0,   0,   0,   0,   -10, 
        -20, -10, -10, -5,  -5,  -10, -10, -20
    };
    static constexpr int KING_MIDDLE_GAME_PIECE_SQUARE_TABLE[64] = {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20,  20,  0,   0,   0,   0,   20,  20,
        20,  30,  10,  0,   0,   10,  30,  20
    };
    static constexpr int KING_END_GAME_PIECE_SQUARE_TABLE[64] = {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, 0,   0,   -10, -20, -30,
        -30, -10, 20,  30,  30,  20,  -10, -30,
        -30, -10, 30,  40,  40,  30,  -10, -30,
        -30, -10, 30,  40,  40,  30,  -10, -30,
        -30, -10, 20,  30,  30,  20,  -10, -30,
        -30, -30, 0,   0,   0,   0,   -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
    };

    // clang-format on
};
