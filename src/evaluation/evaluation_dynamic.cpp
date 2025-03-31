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
#include "bit_utilities.hpp"
#include <algorithm>

constexpr inline int calculate_game_phase_percentage(const Board& board);

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
int evaluate_position(const Board& board)
{
    int middlegame_eval = 0;
    int endgame_eval = 0;

    const int game_phase_percentage = calculate_game_phase_percentage(board);

    uint64_t pieces = board.get_bitboard_all();

    while (pieces) {

        const Square square(pop_lsb(pieces));
        const Piece piece = board.get_piece(square);
        const ChessColor piece_color = get_color(piece);
        const int piece_raw_value = raw_value(piece);
        const int bonus_middlegame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_MIDDLEGAME>(piece, square);
        const int bonus_endgame = PrecomputedEvalData::get_piece_square_table<PST_TYPE_ENDGAME>(piece, square);

        const int piece_value_middlegame = piece_raw_value + bonus_middlegame;
        const int piece_value_endgame = piece_raw_value + bonus_endgame;

        middlegame_eval += is_white(piece_color) ? piece_value_middlegame : -piece_value_middlegame;
        endgame_eval += is_white(piece_color) ? piece_value_endgame : -piece_value_endgame;
    }
    // Blend evaluations
    return (middlegame_eval * game_phase_percentage + endgame_eval * (100 - game_phase_percentage)) / 100;
}

/** 
 * @brief calculate the game phase percentage 
 *   
 * @param[in] board chess position
 *
 * @returns int
 *  - (100) middlegame
 *  - (0) endgame
 */
constexpr inline int calculate_game_phase_percentage(const Board& board)
{
    const uint64_t queens = board.get_bitboard_piece(Piece::W_QUEEN) | board.get_bitboard_piece(Piece::B_QUEEN);

    const uint64_t pieces = board.get_bitboard_all();

    const int num_pieces = number_of_1_bits(pieces);
    const int num_queens = number_of_1_bits(queens);

    // Calculate game phase percentage (0-100)
    constexpr int MAX_PIECES = 32;
    constexpr int ENDGAME_THRESHOLD = 10;   // less that 10 pieces is considered endgame
    constexpr int PIECE_PHASE_DENOM = MAX_PIECES - ENDGAME_THRESHOLD;

    const int piece_phase = 100 * (num_pieces - ENDGAME_THRESHOLD) / PIECE_PHASE_DENOM;   // 10 pieces = 0%, 32 = 100%
    const int queen_phase = 100 * num_queens / 2;   // 0 queens = 0%, 2 queens = 100%

    // Blend factors with queen presence having more weight
    const int game_phase_percentage = (3 * queen_phase + piece_phase) / 4;

    return std::clamp(game_phase_percentage, 0, 100);
}