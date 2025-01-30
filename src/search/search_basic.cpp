/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 */

#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_list.hpp"

#include <limits>

// Constants for infinity
const int INF = std::numeric_limits<int>::max();

int alpha_beta(Board& board, uint32_t depth, int alpha, int beta, bool is_maximizing_player, std::atomic<bool>& stop);

/**
 * @brief search_best_move
 * 
 * Calculate the best legal move in the chess position.
 * 
 * @note The function is prepared to be called in a separated thread, you could use stop signal.
 * 
 * @param[in] board chess position.
 * @param[in] max_depth use INFINITE_SEARCH_DEPTH_VALUE to do an infinite search.
 * @param[in] stop signal to stop the search.
 * 
 * @return 
 *  - move, best move found in the position.
 *  - move.none() if no move was found.
 */
Move search_best_move(Board& board, uint32_t max_depth, std::atomic<bool>& stop) {
    Move best_move = Move::null();
    int best_value = -INF;
    int alpha = -INF;
    int beta = INF;

    MoveList legal_moves;
    generate_legal_moves(legal_moves, board);
    const GameState game_state = board.state();

    for (int i = 0; i < legal_moves.size(); i++) {
        board.make_move(legal_moves[i]);
        int move_value = alpha_beta(board, max_depth - 1, alpha, beta, false, stop);
        board.unmake_move(legal_moves[i], game_state);

        if (move_value > best_value) {
            best_value = move_value;
            best_move = legal_moves[i];
        }

        if (stop) {
            break;
        }
    }

    return best_move;
}

int alpha_beta(Board& board, uint32_t depth, int alpha, int beta, bool is_maximizing_player, std::atomic<bool>& stop) {
    if (depth == 0 || stop) {
        return evaluate_position(board);
    }

    MoveList legal_moves;
    generate_legal_moves(legal_moves, board);
    if (legal_moves.size() == 0) {
        return evaluate_position(board);
    }

    const GameState game_state = board.state();

    if (is_maximizing_player) {
        int max_eval = -INF;

        for (int i = 0; i < legal_moves.size(); i++) {
            board.make_move(legal_moves[i]);
            int eval = alpha_beta(board, depth - 1, alpha, beta, false, stop);
            board.unmake_move(legal_moves[i], game_state);

            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Poda beta
            }
        }

        return max_eval;
    } else {
        int min_eval = INF;

        for (int i = 0; i < legal_moves.size(); i++) {
            board.make_move(legal_moves[i]);
            int eval = alpha_beta(board, depth - 1, alpha, beta, true, stop);
            board.unmake_move(legal_moves[i], game_state);

            min_eval = std::min(min_eval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Poda alfa
            }
        }

        return min_eval;
    }
}
