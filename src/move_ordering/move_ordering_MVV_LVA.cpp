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

    const Piece origin_piece = board.get_piece(move.square_from());
    const uint32_t origin_value = raw_value(origin_piece);
    const PieceType promotion_piece = move.promotion_piece();
    const bool is_promotion = move.type() == MoveType::PROMOTION;

    uint32_t score = 0;
    bool is_capture = false;
    Piece victim_piece;

    // Determine if the move is a capture and get the victim piece
    if (move.type() == MoveType::EN_PASSANT) {
        is_capture = true;
        // Calculate victim square for en passant
        Square victim_square(move.square_from().row(), move.square_to().col());
        victim_piece = board.get_piece(victim_square);
    }
    else {
        victim_piece = board.get_piece(move.square_to());
        is_capture = (victim_piece != Piece::EMPTY) || (move.type() == MoveType::EN_PASSANT);
    }

    if (is_capture) {
        const uint32_t victim_value = raw_value(victim_piece);
        const uint32_t aggressor_value = origin_value;
        const uint32_t mvv_lva = victim_value * 1000 + (1000 - aggressor_value);
        score = mvv_lva * 10000;   // Ensure captures are prioritized first

        // Add promotion bonus
        if (is_promotion) {
            const uint32_t promotion_value = raw_value(promotion_piece);
            score += promotion_value * 100;
        }
    }
    else if (is_promotion) {
        // Promotion without capture
        const uint32_t promotion_value = raw_value(promotion_piece);
        score = 9000 + promotion_value * 100;
    }
    else {
        // Non-capture, non-promotion moves ordered by origin piece value
        score = origin_value;
    }

    return score;
}