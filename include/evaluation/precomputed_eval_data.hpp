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

constexpr bool PST_TYPE_MIDDLEGAME = false;
constexpr bool PST_TYPE_ENDGAME = true;

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
     * @brief get piece value in the PST (piece square table)
     * 
     * @tparam searchType [PST_TYPE_MIDDLEGAME, PST_TYPE_ENDGAME]
     * @param[in] piece piece to get the value
     * @param[in] square piece square
     * 
     * @return int PIECE_SQUARE_TABLE[piece][square]
     */
    template<bool PST_TYPE>
    static inline int get_piece_square_table(Piece piece, Square square)
    {
        assert(PST_TYPE == PST_TYPE_MIDDLEGAME || PST_TYPE == PST_TYPE_ENDGAME);

        assert(square.is_valid());
        assert(is_valid_piece(piece));

        const ChessColor color = get_color(piece);
        const PieceType piece_type = piece_to_pieceType(piece);

        assert(piece_type != PieceType::EMPTY);
        assert(is_valid_color(color));

        const int index_sq = is_white(color) ? 63 - square.value() : square.value();

        if constexpr (PST_TYPE == PST_TYPE_MIDDLEGAME) {
            return PIECE_SQUARE_TABLE[static_cast<int>(piece_type)][index_sq];
        }
        else {
            return PIECE_ENDGAME_SQUARE_TABLE[static_cast<int>(piece_type)][index_sq];
        }
    }

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

    static inline uint64_t get_king_danger_zone(Square king_sq)
    {
        assert(king_sq.is_valid());
        return KING_DANGER_ZONE[king_sq];
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

private:
    static inline const std::array<std::array<int, 64>, 64> init_chebyshev_distance()
    {
        std::array<std::array<int, 64>, 64> distances;

        for (Square sq1 = Square::A1; sq1.is_valid(); sq1++) {
            for (Square sq2 = Square::A1; sq2.is_valid(); sq2++) {
                distances[sq1][sq2] =
                    std::max(std::abs(int(sq1.row()) - int(sq2.row())), std::abs(int(sq1.col()) - int(sq2.col())));
            }
        }

        return distances;
    };

    static inline const std::array<std::array<int, 64>, 64> init_manhattan_distance()
    {
        std::array<std::array<int, 64>, 64> distances;

        for (Square sq1 = Square::A1; sq1.is_valid(); sq1++) {
            for (Square sq2 = Square::A1; sq2.is_valid(); sq2++) {
                distances[sq1][sq2] =
                    std::abs(int(sq1.row()) - int(sq2.row())) + std::abs(int(sq1.col()) - int(sq2.col()));
            }
        }

        return distances;
    };

    static inline const std::array<uint64_t, 64> init_king_danger_zone()
    {
        std::array<uint64_t, 64> zone;

        for (Square sq = Square::A1; sq.is_valid(); sq++) {
            const Row king_row = sq.row();
            const Col king_col = sq.col();
            zone[sq] = sq.mask();

            const Row row_plus_1 = king_row + 1;
            const Row row_plus_2 = king_row + 2;
            const Row row_plus_3 = king_row + 3;
            const Row row_minus_1 = king_row - 1;
            const Row row_minus_2 = king_row - 2;
            const Row row_minus_3 = king_row - 3;

            const Col col_plus_1 = king_col + 1;
            const Col col_plus_2 = king_col + 2;
            const Col col_plus_3 = king_col + 3;
            const Col col_minus_1 = king_col - 1;
            const Col col_minus_2 = king_col - 2;
            const Col col_minus_3 = king_col - 3;

            zone[sq] |= is_valid_row(row_plus_1) ? get_row_mask(row_plus_1) : 0ULL;
            zone[sq] |= is_valid_row(row_plus_2) ? get_row_mask(row_plus_2) : 0ULL;
            zone[sq] |= is_valid_row(row_plus_3) ? get_row_mask(row_plus_3) : 0ULL;
            zone[sq] |= is_valid_row(row_minus_1) ? get_row_mask(row_minus_1) : 0ULL;
            zone[sq] |= is_valid_row(row_minus_2) ? get_row_mask(row_minus_2) : 0ULL;
            zone[sq] |= is_valid_row(row_minus_3) ? get_row_mask(row_minus_3) : 0ULL;

            zone[sq] |= is_valid_col(col_plus_1) ? get_col_mask(col_plus_1) : 0ULL;
            zone[sq] |= is_valid_col(col_plus_2) ? get_col_mask(col_plus_2) : 0ULL;
            zone[sq] |= is_valid_col(col_plus_3) ? get_col_mask(col_plus_3) : 0ULL;
            zone[sq] |= is_valid_col(col_minus_1) ? get_col_mask(col_minus_1) : 0ULL;
            zone[sq] |= is_valid_col(col_minus_2) ? get_col_mask(col_minus_2) : 0ULL;
            zone[sq] |= is_valid_col(col_minus_3) ? get_col_mask(col_minus_3) : 0ULL;
        }

        return zone;
    };

    static inline const std::array<std::array<int, 64>, 64> CHEBYSHEV_DISTANCE = init_chebyshev_distance();
    static inline const std::array<std::array<int, 64>, 64> MANHATTAN_DISTANCE = init_manhattan_distance();
    static inline const std::array<uint64_t, 64> KING_DANGER_ZONE = init_king_danger_zone();

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
    static constexpr int KING_PIECE_SQUARE_TABLE[64] = {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20,  20,  0,   0,   0,   0,   20,  20,
        20,  30,  10,  0,   0,   10,  30,  20
    };

    static constexpr int PAWN_ENDGAME_SQUARE_TABLE[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        80,  80,  80,  80,  80,  80,  80,  80,
        50,  50,  50,  50,  50,  50,  50,  50,
        30,  30,  30,  30,  30,  30,  30,  30,
        20,  20,  20,  20,  20,  20,  20,  20,
        10,  10,  10,  10,  10,  10,  10,  10,
        10,  10,  10,  10,  10,  10,  10,  10,
        0,   0,   0,   0,   0,   0,   0,   0
   };

    static constexpr int KING_ENDGAME_SQUARE_TABLE[64] = {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, 0,   0,   -10, -20, -30,
        -30, -10, 20,  30,  30,  20,  -10, -30,
        -30, -10, 30,  40,  40,  30,  -10, -30,
        -30, -10, 30,  40,  40,  30,  -10, -30,
        -30, -10, 20,  30,  30,  20,  -10, -30,
        -30, -30, 0,   0,   0,   0,   -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
    };

    /**
     * @brief lookup table for each middlegame PST indexed by PieceType
     * 
     * 0 : PAWN_PIECE_SQUARE_TABLE
     * 1 : KNIGHT_PIECE_SQUARE_TABLE
     * 2 : BISHOP_PIECE_SQUARE_TABLE
     * 3 : ROOK_PIECE_SQUARE_TABLE
     * 4 : QUEEN_PIECE_SQUARE_TABLE
     * 5 : KING_PIECE_SQUARE_TABLE
     */
    static constexpr const int* PIECE_SQUARE_TABLE[NUM_CHESS_PIECE_TYPES - 1] = {
        PAWN_PIECE_SQUARE_TABLE,
        KNIGHT_PIECE_SQUARE_TABLE,
        BISHOP_PIECE_SQUARE_TABLE,
        ROOK_PIECE_SQUARE_TABLE, 
        QUEEN_PIECE_SQUARE_TABLE,
        KING_PIECE_SQUARE_TABLE
    };

    /**
     * @brief lookup table for each endgame PST indexed by PieceType
     * 
     * 0 : PAWN_PIECE_SQUARE_TABLE
     * 1 : KNIGHT_PIECE_SQUARE_TABLE
     * 2 : BISHOP_PIECE_SQUARE_TABLE
     * 3 : ROOK_PIECE_SQUARE_TABLE
     * 4 : QUEEN_PIECE_SQUARE_TABLE
     * 5 : KING_ENDGAME_SQUARE_TABLE
     */
    static constexpr const int* PIECE_ENDGAME_SQUARE_TABLE[NUM_CHESS_PIECE_TYPES - 1] = {
        PAWN_ENDGAME_SQUARE_TABLE,
        KNIGHT_PIECE_SQUARE_TABLE,
        BISHOP_PIECE_SQUARE_TABLE,
        ROOK_PIECE_SQUARE_TABLE, 
        QUEEN_PIECE_SQUARE_TABLE,
        KING_ENDGAME_SQUARE_TABLE
    };

    // clang-format on
};
