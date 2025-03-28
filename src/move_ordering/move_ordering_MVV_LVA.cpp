/**
 * @file move_ordering_MVV_LVA.cpp
 * @brief move ordering services.
 *
 * chess move ordering implementation. 
 * MVV-LVA (Most Valuable Victim - Least Valuable Aggressor)
 * 
 * https://www.chessprogramming.org/MVV-LVA
 */

#include "move_ordering.hpp"
#include <cassert>
#include <algorithm>

/**
 * @brief move_value
 * 
 * Orders moves using MVV-LVA and promotion heuristics.
 * 
 * @param[in] move move.
 * @param[in] board chess position.
 *
 * @return move value
 * 
 */
static uint32_t move_value(const Move& move, const Board& board);

/**
 * @brief order_moves
 * 
 * Order the move list.
 * 
 * @note the first move in the list is the move with the best prospects of being the best move.
 * 
 * @param[in] board chess position.
 * @param[out] moves move list.
 * 
 */
void order_moves(MoveList& moves, const Board& board)
{
    std::sort(moves.begin(), moves.end(), [&board](const Move& move_1, const Move& move_2) {
        return move_value(move_1, board) > move_value(move_2, board);
    });
}

/**
 * @brief move_value
 * 
 * Orders moves using MVV-LVA and promotion heuristics.
 * 
 * @param[in] move move.
 * @param[in] board chess position.
 *
 * @return move value
 * 
 */
static uint32_t move_value(const Move& move, const Board& board)
{
    assert(move.is_valid());

    // precomputed move value MVV_LVA_table[victim_piece][attacker_piece]
    static constexpr int MVV_LVA_table[NUM_CHESS_PIECE_TYPES][NUM_CHESS_PIECE_TYPES] = {
        {15, 14, 13, 12, 11, 10, 0},   // victim P, attacker P, N, B, R, Q, K, EMPTY
        {25, 24, 23, 22, 21, 20, 0},   // victim N, attacker P, N, B, R, Q, K, EMPTY
        {35, 34, 33, 32, 31, 30, 0},   // victim B, attacker P, N, B, R, Q, K, EMPTY
        {45, 44, 43, 42, 41, 40, 0},   // victim R, attacker P, N, B, R, Q, K, EMPTY
        {55, 54, 53, 52, 51, 50, 0},   // victim Q, attacker P, N, B, R, Q, K, EMPTY
        {0, 0, 0, 0, 0, 0, 0},         // victim K, attacker P, N, B, R, Q, K, EMPTY
        {0, 0, 0, 0, 0, 0, 0},         // victim EMPTY, attacker P, N, B, R, Q, K, EMPTY
    };

    const Piece origin_piece = board.get_piece(move.square_from());
    const Piece end_piece = board.get_piece(move.square_to());

    const PieceType attacker = piece_to_pieceType(origin_piece);
    const PieceType victim = move.type() != MoveType::EN_PASSANT ? piece_to_pieceType(end_piece) : PieceType::PAWN;

    uint32_t score = MVV_LVA_table[static_cast<int>(victim)][static_cast<int>(attacker)];

    if (move.type() == MoveType::PROMOTION) {
        score += raw_value(move.promotion_piece());
    }

    return score;
}
