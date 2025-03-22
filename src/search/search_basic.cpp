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

#include "search.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "move_ordering.hpp"
#include "move_list.hpp"
#include "search_utils.hpp"

/**
 * @brief search_stop
 * 
 * @note this variable is thread safe
 * 
 * stop the search process
 * 
 */
std::atomic<bool> searchStop;

static void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, SearchContext& context);

template<SearchType searchType>
static int alpha_beta_search(Board& board, int depth, int ply, int alpha, int beta, SearchContext& context);

template<SearchType searchType>
static int quiescence_search(Board& board, int ply, int alpha, int beta);

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
void search_best_move(SearchResults& searchResults, Board board, int32_t max_depth)
{
    SearchContext context;

    const ChessColor side_to_move = board.state().side_to_move();

    context.bestEvalFound = side_to_move == ChessColor::WHITE ? -INF_EVAL : +INF_EVAL;
    context.bestMoveFound = Move::null();
    searchResults.depthReached = 0;
    searchStop = false;

    iterative_deepening(searchResults, board, max_depth, context);

    if (!context.bestMoveFound.is_valid()) {
        // if none move found choose one
        MoveList moves;
        generate_legal_moves<ALL_MOVES>(moves, board);
        context.bestMoveFound = moves[0];
        context.bestEvalFound = 0;
        const int depth = 1;
        insert_new_result(searchResults, depth, context.bestEvalFound, context.bestMoveFound);
    }

    searchStop = true;

    //notify the reader thread that search has stopped
    searchResults.data_available_cv.notify_one();
}

void iterative_deepening(SearchResults& searchResults, Board& board, int max_depth, SearchContext& context)
{

    const ChessColor side_to_move = board.state().side_to_move();

    for (int depth = 1; depth <= max_depth; depth++) {

        context.bestMoveInIteration = Move::null();
        context.bestEvalInIteration = side_to_move == ChessColor::WHITE ? -INF_EVAL : +INF_EVAL;

        side_to_move == ChessColor::WHITE
            ? alpha_beta_search<MAXIMIZE_WHITE>(board, depth, 0, -INF_EVAL, +INF_EVAL, context)
            : alpha_beta_search<MINIMIZE_BLACK>(board, depth, 0, -INF_EVAL, +INF_EVAL, context);

        if (searchStop) {
            break;
        }

        context.bestMoveFound = context.bestMoveInIteration;
        context.bestEvalFound = context.bestEvalInIteration;

        assert(context.bestMoveFound.is_valid());

        insert_new_result(searchResults, depth, context.bestEvalFound, context.bestMoveFound);

        if (abs(context.bestEvalFound) > MATE_THRESHOLD) {
            break;   // We found a checkmate, we stop because we cant find a shorter checkMate
        }
    }
}

/**
  * @brief alpha_beta_search
  * 
  * Alpha beta search.
  * 
  * @tparam searchType [MAXIMIZE_WHITE, MINIMIZE_BLACK]
  * 
  * @param[in] board chess position.
  * @param[in] depth current depth in the tree
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black (minimum score), for white (maximum score)
  * 
  */
template<SearchType searchType>
static int alpha_beta_search(Board& board, int depth, int ply, int alpha, int beta, SearchContext& context)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    MoveList moves;
    bool isCheck;
    generate_legal_moves<ALL_MOVES>(moves, board, &isCheck);
    const bool isCheckMate = isCheck && moves.size() == 0;
    const bool isStaleMate = !isCheck && moves.size() == 0;

    if (isCheckMate) {
        // we substract ply so checkMate in less moves has a higher score
        if constexpr (MAXIMIZE_WHITE) {
            return -(MATE_IN_ONE_SCORE - ply);
        }
        else if constexpr (MINIMIZE_BLACK) {
            return MATE_IN_ONE_SCORE - ply;
        }
    }
    else if (isStaleMate) {
        return 0;
    }
    else if (depth == 0) {
        return quiescence_search<searchType>(board, ply, alpha, beta);
    }

    int final_node_evaluation = MAXIMIZING_WHITE ? -INF_EVAL : +INF_EVAL;
    const GameState game_state = board.state();

    order_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {

        if (searchStop) {
            return 0;
        }
        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        board.make_move(moves[i]);
        int eval = alpha_beta_search<nextSearchType>(board, depth - 1, ply + 1, alpha, beta, context);
        board.unmake_move(moves[i], game_state);

        if constexpr (MAXIMIZING_WHITE) {
            if (ply == 0 && eval > context.bestEvalInIteration) {
                // if we are in the root node update the best move
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];
            }

            final_node_evaluation = std::max(final_node_evaluation, eval);
            alpha = std::max(alpha, eval);

            if (final_node_evaluation >= beta) {
                break;   // beta cutoff
            }
        }
        else if (MINIMIZING_BLACK) {
            if (ply == 0 && eval < context.bestEvalInIteration) {
                // if we are in the root node update the best move
                context.bestEvalInIteration = eval;
                context.bestMoveInIteration = moves[i];
            }

            final_node_evaluation = std::min(final_node_evaluation, eval);
            beta = std::min(beta, eval);

            if (final_node_evaluation <= alpha) {
                break;   // alpha cutoff
            }
        }
    }

    return final_node_evaluation;
}

/**
  * @brief quiescence_search
  * 
  * Alpha beta search only considering the capture moves, this is called when we reach the maximum depth
  * and it is paramaunt in order to avoid the 'horizon effect', for example if we stop the search in the
  * middle of a piece exchange the evaluation is corrupted.
  * 
  * The search continues until there are no more capture moves, to also implement cutoffs we first calculate
  * the static evaluation of the position (stand_pat)
  * 
  * @tparam searchType [MAXIMIZE_WHITE, MINIMIZE_BLACK]
  * 
  * @param[in] board chess position.
  * @param[in] ply   current ply in the tree
  * @param[in] alpha maximum value that the maximizing player(white) can guarantee
  * @param[in] beta  minimum value that the minimizing player(black) can guarantee
  * 
  * @return best score possible for black (minimum score possible), for white (maximum score possible)
  * 
  */
template<SearchType searchType>
static int quiescence_search(Board& board, int ply, int alpha, int beta)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    int static_evaluation = evaluate_position(board);

    if (ply >= MAX_PLY) {
        return static_evaluation;
    }

    if constexpr (MAXIMIZING_WHITE) {
        if (static_evaluation >= beta) {
            return beta;   // beta cutoff
        }
        alpha = std::max(alpha, static_evaluation);
    }
    else if constexpr (MINIMIZING_BLACK) {
        if (static_evaluation <= alpha) {
            return alpha;   // Alpha cutoff
        }
        beta = std::min(beta, static_evaluation);
    }

    MoveList capture_moves;
    generate_legal_moves<ONLY_CAPTURES>(capture_moves, board);

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    order_moves(capture_moves, board);

    const GameState game_state = board.state();
    int final_node_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {

        if (searchStop) {
            return 0;
        }

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        board.make_move(capture_moves[i]);
        int eval = quiescence_search<nextSearchType>(board, ply + 1, alpha, beta);
        board.unmake_move(capture_moves[i], game_state);

        if constexpr (MAXIMIZING_WHITE) {
            final_node_evaluation = std::max(final_node_evaluation, eval);
            alpha = std::max(alpha, eval);

            if (final_node_evaluation >= beta) {
                break;   // Beta cutoff
            }
        }
        else if constexpr (MINIMIZING_BLACK) {
            final_node_evaluation = std::min(final_node_evaluation, eval);
            beta = std::min(beta, eval);

            if (final_node_evaluation <= alpha) {
                break;   // Alpha cutoff
            }
        }
    }

    return final_node_evaluation;
}

/**
  * @brief search_stop
  * 
  * @note this method is thread safe
  * 
  * stop the search process
  * 
  */
 void search_stop() { searchStop.store(true); }

 /**
    * @brief is_search_running
    * 
    * @note this method is thread safe
    * 
    * @return True if the search is running (stop is false)
    * 
    */
 bool is_search_running() { return !searchStop.load(); }