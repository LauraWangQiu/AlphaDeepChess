/**
 * @file evaluation_dynamic.cpp
 * @brief evaluation services implementation.
 *
 * chess position dynamic evaluation implementation.
 * 
 * https://www.chessprogramming.org/Evaluation
 * https://www.chessprogramming.org/Simplified_Evaluation_Function
 * https://www.chessprogramming.org/Tapered_Eval
 * 
 */
#include "evaluation.hpp"
#include "move_list.hpp"
#include "move_generator.hpp"
#include "precomputed_eval_data.hpp"
#include "precomputed_move_data.hpp"
#include "bit_utilities.hpp"

static constexpr inline int calculate_middlegame_percentage(const Board& board);

/** 
 * @brief evaluate_position
 *
 * Evaluate chess position.
 *  
 * @note None.
 * 
 * @param[in] board board to evaluate.
 *
 * @returns
 *  - (0) if position is evaluated as equal.
 *  - (+) if position is evaluated as white is better.
 *  - (-) if position is evaluated as black is better.
 */
int evaluate_position(Board& board)
{
    board.update_attacks_bb();

    int middlegame_eval = 0;
    int endgame_eval = 0;

    const int middlegame_percentage = calculate_middlegame_percentage(board);
    const int endgame_percentage = 24 - calculate_middlegame_percentage(board);

    uint64_t pieces = board.get_bitboard_all();

    while (pieces) {

        const Square square(pop_lsb(pieces));
        const Piece piece = board.get_piece(square);
        const ChessColor color = get_color(piece);
        const int piece_raw_value = raw_value(piece);
        const int bonus_middlegame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_MIDDLEGAME>(piece, square);
        const int bonus_endgame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_ENDGAME>(piece, square);

        const int piece_value_middlegame = piece_raw_value + bonus_middlegame;
        const int piece_value_endgame = piece_raw_value + bonus_endgame;

        middlegame_eval += is_white(color) ? piece_value_middlegame : -piece_value_middlegame;
        endgame_eval += is_white(color) ? piece_value_endgame : -piece_value_endgame;
    }
    const int blended_eval = (middlegame_eval * middlegame_percentage + endgame_eval * endgame_percentage) / 24;

    return blended_eval;
}

/**
 * @brief Calculate the middlegame percentage (0 = endgame, 24 = middlegame)
 * 
 * minor piece weight : 1
 * rook weight : 2
 * queen weight : 4
 * 
 * @param[in] board Chess position
 * @returns int
 *  - (24) for full middlegame (8 minor pieces, 4 rooks and 2 queens)
 *  - (0) for full endgame
 */
static constexpr inline int calculate_middlegame_percentage(const Board& board)
{
    const int queens = board.get_piece_counter(Piece::W_QUEEN) + board.get_piece_counter(Piece::B_QUEEN);

    const int rooks = board.get_piece_counter(Piece::W_ROOK) + board.get_piece_counter(Piece::B_ROOK);

    const int minor_pieces = board.get_piece_counter(Piece::W_KNIGHT) + board.get_piece_counter(Piece::B_KNIGHT) +
        board.get_piece_counter(Piece::W_BISHOP) + board.get_piece_counter(Piece::B_BISHOP);

    const int game_phase = minor_pieces + 2 * rooks + 4 * queens;

    return std::min(game_phase, 24);   // max gamephase is 24
}