/**
 * @file move_ordering_basic.cpp
 * @brief move ordering services.
 *
 * chess move ordering implementation. 
 * 
 */

#include "move_ordering.hpp"
#include <cassert>

/**
 * @brief move_value
 * 
 * Get move value.
 * 
 * @param[in] board chess position.
 * @param[in] move move.
 * 
 * @return move value = origin_piece_raw_value + bonus_capture + bonus_promotion
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
    // bubblesort, the best move is the one with the piece of higher value,
    // bonus points if move is capture or promotion.

    int n = moves.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (move_value(moves.get(j), board) < move_value(moves.get(j + 1), board)) {

                Move temp = moves.get(j);
                moves.get(j) = moves.get(j + 1);
                moves.get(j + 1) = temp;
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}


/**
 * @brief move_value
 * 
 * Get move value.
 * 
 * @param[in] board chess position.
 * @param[in] move move.
 * 
 * @return move value = origin_piece_raw_value + bonus_capture + bonus_promotion
 * 
 */
static uint32_t move_value(const Move& move, const Board& board)
{
    assert(move.is_valid());

    const Piece origin_piece = board.get_piece(move.square_from());
    const uint32_t origin_piece_value = raw_value(origin_piece);
    const PieceType promotion_piece = move.promotion_piece();

    const uint32_t bonus_capture =
        move.type() == MoveType::EN_PASSANT || !board.is_empty(move.square_to()) ? 10U : 0U;

    const uint32_t bonus_promotion =
        move.type() == MoveType::PROMOTION ? raw_value(promotion_piece) : 0U;

    return origin_piece_value + bonus_capture + bonus_promotion;
}