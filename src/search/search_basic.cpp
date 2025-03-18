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

#include <limits>
#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"

const int INF = std::numeric_limits<int>::max();
const int INMEDIATE_MATE_SCORE = 32000;
const int MATE_THRESHOLD = INMEDIATE_MATE_SCORE - 1000U;

static std::atomic<bool> stop;
static Move bestMoveFound;
static int bestEvalFound;
static Move bestMoveInIteration;
static int bestEvalInIteration;

static inline void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, std::chrono::steady_clock::time_point start_time, uint32_t time_limit);
static int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta, std::chrono::steady_clock::time_point start_time, uint32_t time_limit);
static int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta, std::chrono::steady_clock::time_point start_time, uint32_t time_limit);
static int quiescence_maximize_white(Board& board, int ply, int alpha, int beta);
static int quiescence_minimize_black(Board& board, int ply, int alpha, int beta);
static void insert_new_result(SearchResults& searchResults, int depth, int evaluation, Move move);

/**
 * @brief search_stop
 * 
 * @note this method is thread safe
 * 
 * stop the search process
 * 
 */
void search_stop() { stop.store(true); }

/**
 * @brief is_search_running
 * 
 * @note this method is thread safe
 * 
 * @return True if the search is running (stop is false)
 * 
 */
bool is_search_running() { return !stop.load(); }

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
void search_best_move(SearchResults& searchResults, Board board, int32_t max_depth, uint32_t wtime, uint32_t btime, uint32_t movestogo)
{
    const ChessColor side_to_move = board.state().side_to_move();

    bestEvalFound = side_to_move == ChessColor::WHITE ? -INF : +INF;
    bestMoveFound = Move::null();
    searchResults.depthReached = 0;
    stop = false;

    uint32_t time_limit;
    if (movestogo == INFINITE_MOVES_TO_GO) {
        time_limit = std::numeric_limits<uint32_t>::max();
    } else if (movestogo > 0) {
        time_limit = (side_to_move == ChessColor::WHITE ? wtime : btime) / movestogo;
    } else {
        // Sudden death
        time_limit = (side_to_move == ChessColor::WHITE ? wtime : btime);
    }

    auto start_time = std::chrono::steady_clock::now();

    iterative_deepening(searchResults, board, max_depth, start_time, time_limit);

    stop = true;

    {
        //notify the reader thread that search has stopped
        std::lock_guard<std::mutex> lock(searchResults.mtx_data_available_cv);
        searchResults.data_available_cv.notify_one();
    }
}

void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, std::chrono::steady_clock::time_point start_time, uint32_t time_limit)
{

    const ChessColor side_to_move = board.state().side_to_move();

    for (int depth = 1; depth <= max_depth; depth++) {

        bestMoveInIteration = Move::null();
        bestEvalInIteration = side_to_move == ChessColor::WHITE ? -INF : +INF;

        side_to_move == ChessColor::WHITE ? alpha_beta_maximize_white(board, depth, 0, -INF, +INF, start_time, time_limit)
                                          : alpha_beta_minimize_black(board, depth, 0, -INF, +INF, start_time, time_limit);

        if (stop) {
            break;
        }

        bestMoveFound = bestMoveInIteration;
        bestEvalFound = bestEvalInIteration;

        assert(bestMoveFound.is_valid());

        insert_new_result(searchResults, depth, bestEvalFound, bestMoveFound);

        if (abs(bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }
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
 * @param[in] start_time start time of the search
 * @param[in] time_limit time limit for the search
 * 
 * @return best score possible for white in this node (maximum score possible)
 * 
 */
int alpha_beta_maximize_white(Board& board, int depth, int ply, int alpha, int beta, std::chrono::steady_clock::time_point start_time, uint32_t time_limit)
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

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed_time >= time_limit) {
            stop = true;
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_minimize_black(board, depth - 1, ply + 1, alpha, beta, start_time, time_limit);
        board.unmake_move(moves[i], game_state);

        if (ply == 0 && evaluation > bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
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
 * @param[in] start_time start time of the search
 * @param[in] time_limit time limit for the search
 * 
 * @return best score possible for black in this node (minimum score possible)
 * 
 */
int alpha_beta_minimize_black(Board& board, int depth, int ply, int alpha, int beta, std::chrono::steady_clock::time_point start_time, uint32_t time_limit)
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

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        if (elapsed_time >= time_limit) {
            stop = true;
            return 0;
        }

        board.make_move(moves[i]);
        int evaluation = alpha_beta_maximize_white(board, depth - 1, ply + 1, alpha, beta, start_time, time_limit);
        board.unmake_move(moves[i], game_state);

        if (ply == 0 && evaluation < bestEvalInIteration) {
            // if we are in the root node update the best move
            bestEvalInIteration = evaluation;
            bestMoveInIteration = moves[i];
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
    int max_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_minimize_black(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        max_evaluation = std::max(max_evaluation, eval);
        alpha = std::max(alpha, eval);

        if (max_evaluation >= beta) {
            break;   // Beta cutoff
        }
    }

    return max_evaluation;
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
    int min_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {
        board.make_move(capture_moves[i]);
        int eval = quiescence_maximize_white(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        min_evaluation = std::min(min_evaluation, eval);
        beta = std::min(beta, eval);

        if (min_evaluation <= alpha) {
            break;   // Alpha cutoff
        }
    }

    return min_evaluation;
}

static inline void insert_new_result(SearchResults& searchResults, int depth, int evaluation, Move move)
{
    assert(searchResults.depthReached < INFINITE_DEPTH);
    searchResults.results[searchResults.depthReached].depth = depth;
    searchResults.results[searchResults.depthReached].evaluation = evaluation;
    searchResults.results[searchResults.depthReached].bestMove_data = move.raw_data();
    searchResults.depthReached++;

    {
        std::lock_guard<std::mutex> lock(searchResults.mtx_data_available_cv);
        searchResults.data_available_cv.notify_one();
    }
}