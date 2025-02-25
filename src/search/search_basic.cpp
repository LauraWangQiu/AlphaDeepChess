/**
 * @file search_basic.cpp
 * @brief search services.
 *
 * chess search basic implementation. 
 * 
 * https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
 * https://www.chessprogramming.org/Alpha-Beta
 * https://www.chessprogramming.org/Quiescence_Search
 * 
 */

#include <iostream>
#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"

const int INF = std::numeric_limits<int>::max();
const int INMEDIATE_MATE_SCORE = 32000;
const int MATE_THRESHOLD = INMEDIATE_MATE_SCORE - 1000U;

std::atomic<bool> stop;
Move bestMoveFound;
int bestEvalFound;
Move bestMoveInIteration;
int bestEvalInIteration;
bool hasSearchedAtLeastOneMove;

static void iterative_deepening(Board& board, int max_depth);
static int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta);
static int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta);
static int quiescence_maximize_white(Board& board, int ply, int alpha, int beta);
static int quiescence_minimize_black(Board& board, int ply, int alpha, int beta);


/**
 * @brief search_stop
 * 
 * stop the search process, this method is thread safe
 * 
 */
void search_stop() { stop.store(true); }

/**
 * @brief search_best_move
 * 
 * Calculate the best legal move in the chess position.
 * 
 * @note This method is thread safe
 * 
 * @param[in] board chess position.
 * @param[in] max_depth maximum depth of search, default value is INFINITE_DEPTH
 * 
 * @return best move found in the position.
 * 
 */
Move search_best_move(Board board, int32_t max_depth)
{
    const ChessColor side_to_move = board.state().side_to_move();

    bestEvalFound = side_to_move == ChessColor::WHITE ? -INF : +INF;
    bestMoveFound = Move::null();

    iterative_deepening(board, max_depth);

    // not enough time to find a move, select random move
    if (!bestMoveFound.is_valid()) {
        MoveList moves;
        generate_legal_moves(moves, board);
        bestMoveFound = moves[0];
        std::cout << "random move selected " << bestMoveFound.to_string() << "\n";
    }
    return bestMoveFound;
}

void iterative_deepening(Board& board, int max_depth)
{

    stop = false;

    const ChessColor side_to_move = board.state().side_to_move();

    for (int depth = 1; depth <= max_depth; depth++) {
        hasSearchedAtLeastOneMove = false;
        bestMoveInIteration = Move::null();
        bestEvalInIteration = side_to_move == ChessColor::WHITE ? -INF : +INF;

        side_to_move == ChessColor::WHITE ? alpha_beta_maximize_white(board, depth, 0, -INF, +INF)
                                          : alpha_beta_minimize_black(board, depth, 0, -INF, +INF);

        if (stop) {
            if (hasSearchedAtLeastOneMove) {
                bestMoveFound = bestMoveInIteration;
                bestEvalFound = bestEvalInIteration;
            }
            break;
        }

        bestMoveFound = bestMoveInIteration;
        bestEvalFound = bestEvalInIteration;

        if (abs(bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }


        std::cout << "info depth " << depth << " score " << bestEvalFound << " bestMove " << bestMoveFound.to_string()
                  << "\n";
    }
}

/**
 * @brief alpha_beta_maximize_white
 * 
 * Alpha beta white to move node maximizing the evaluation
 * 
 * @param[in] board chess position.
 * @param[in] depth current depth in the tree
 * @param[in] ply   current ply in the tree (use to track checkMate score)
 * @param[in] alpha maximum value that the maximizing player(white) can guarantee
 * @param[in] beta  minimum value that the minimizing player(black) can guarantee
 * 
 * @return best score possible for white in this node (maximum score possible)
 * 
 */
int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        return -(MATE_IN_ONE_SCORE - ply);   // white is in checkmate
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_maximize_white(board, ply, alpha, beta);
    }

    int max_evaluation = -INF;
    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (stop) {
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_minimize_black(board, depth - 1, ply + 1, alpha, beta);
        board.unmake_move(moves[i], game_state);

        if (ply == 0 && evaluation > bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
            hasSearchedAtLeastOneMove = true;
        }

        max_evaluation = std::max(max_evaluation, evaluation);
        alpha = std::max(alpha, evaluation);

        if (max_evaluation >= beta) {
            break;   // beta cutoff
        }
    }

    return max_evaluation;
}

/**
 * @brief alpha_beta_minimize_black
 * 
 * Alpha beta black to move node minimizing the evaluation
 * 
 * @param[in] board chess position.
 * @param[in] depth current depth in the tree
 * @param[in] ply   current ply in the tree
 * @param[in] alpha maximum value that the maximizing player(white) can guarantee
 * @param[in] beta  minimum value that the minimizing player(black) can guarantee
 * 
 * @return best score possible for black in this node (minimum score possible)
 * 
 */
int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        return MATE_IN_ONE_SCORE - ply;   // black is in checkmate
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_minimize_black(board, ply, alpha, beta);
    }

    int min_evaluation = +INF;
    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (stop) {
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_maximize_white(board, depth - 1, ply + 1, alpha, beta);
        board.unmake_move(moves[i], game_state);

        if (ply == 0 && evaluation < bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
            hasSearchedAtLeastOneMove = true;
        }

        min_evaluation = std::min(min_evaluation, evaluation);
        beta = std::min(beta, evaluation);

        if (min_evaluation <= alpha) {
            break;   // alpha cutoff
        }
    }

    return min_evaluation;
}

/**
 * @brief quiescence_maximize_white
 * 
 * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
 * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
 * middle of a piece exchange the evaluation is corrupted.
 * 
 * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
 * the static evaluation of the position (stand_pat)
 * 
 * @param[in] board chess position.
 * @param[in] ply   current ply in the tree
 * @param[in] alpha maximum value that the maximizing player(white) can guarantee
 * @param[in] beta  minimum value that the minimizing player(black) can guarantee
 * 
 * @return best score possible for white in this node (maximum score possible)
 * 
 */
int quiescence_maximize_white(Board& board, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        return -(MATE_IN_ONE_SCORE - ply);
    }
    else if (isStaleMate) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);

    if (static_evaluation >= beta) {
        return beta;   // beta cutoff
    }
    alpha = std::max(alpha, static_evaluation);

    MoveList capture_moves;
    for (int i = 0; i < moves.size(); i++) {
        if (board.move_is_capture(moves[i])) {
            capture_moves.add(moves[i]);
        }
    }

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int max_eval = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_minimize_black(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        max_eval = std::max(max_eval, eval);
        alpha = std::max(alpha, eval);

        if (alpha >= beta) {
            break;   // Beta cutoff
        }
    }

    return max_eval;
}

/**
 * @brief quiescence_maximize_white
 * 
 * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
 * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
 * middle of a piece exchange the evaluation is corrupted.
 * 
 * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
 * the static evaluation of the position (stand_pat)
 * 
 * @param[in] board chess position.
 * @param[in] ply   current ply in the tree
 * @param[in] alpha maximum value that the maximizing player(white) can guarantee
 * @param[in] beta  minimum value that the minimizing player(black) can guarantee
 * 
 * @return best score possible for black in this node (minimum score possible)
 * 
 */
int quiescence_minimize_black(Board& board, int ply, int alpha, int beta)
{
    MoveList moves;
    bool isCheckMate, isStaleMate = false;
    generate_legal_moves(moves, board, &isCheckMate, &isStaleMate);

    if (isCheckMate) {
        return MATE_IN_ONE_SCORE - ply;
    }
    else if (isStaleMate) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);
    if (static_evaluation <= alpha) {
        return alpha;   // Alpha cutoff
    }
    beta = std::min(beta, static_evaluation);

    MoveList capture_moves;
    for (int i = 0; i < moves.size(); i++) {
        if (board.move_is_capture(moves[i])) {
            capture_moves.add(moves[i]);
        }
    }

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int min_eval = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_maximize_white(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        min_eval = std::min(min_eval, eval);
        beta = std::min(beta, eval);

        if (beta <= alpha) {
            break;   // Alpha cutoff
        }
    }

    return min_eval;
}