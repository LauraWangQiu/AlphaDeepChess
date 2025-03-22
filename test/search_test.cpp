#include "search.hpp"
#include "search_utils.hpp"
#include "move_generator.hpp"
#include "evaluation.hpp"
#include "test_utils.hpp"
#include "history.hpp"

static void search_compare_with_minimax_test(const std::string& fen, const int depth);

template<SearchType searchType>
static int minimax(Board& board, int depth, int ply, Move& best_global_move, int& best_global_eval);
template<SearchType searchType>
static int quiescence_minimax(Board& board, int ply, Move& best_global_move, int& best_global_eval);

void search_test()
{
    std::cout << "---------search test---------\n\n";

    const std::string fens[5] = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                                 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                                 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                                 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                                 "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"};

    for (const std::string& fen : fens) {
        for (int depth = 1; depth <= 3; depth++) {
            search_compare_with_minimax_test(fen, depth);
        }
    }
}

static void search_compare_with_minimax_test(const std::string& fen, const int depth)
{
    const std::string test_name = "search_compare_with_minimax_test_depth: " + std::to_string(depth);
    SearchResults searchResults;
    Board board;
    board.load_fen(fen);

    const ChessColor side_to_move = board.state().side_to_move();

    search_best_move(searchResults, board, depth);

    Move best_minimax_move = Move::null();
    int best_minimax_eval = side_to_move == ChessColor::WHITE ? -INF_EVAL : +INF_EVAL;

    History::clear();

    side_to_move == ChessColor::WHITE ? minimax<MAXIMIZE_WHITE>(board, depth, 0, best_minimax_move, best_minimax_eval)
                                      : minimax<MINIMIZE_BLACK>(board, depth, 0, best_minimax_move, best_minimax_eval);

    Move best_search_move(searchResults.results[depth - 1].bestMove_data);
    int best_search_eval = searchResults.results[depth - 1].evaluation;

    if (best_search_move != best_minimax_move) {
        PRINT_TEST_FAILED(test_name, "best_search_move != best_minimax_move");
    }
    if (best_search_eval != best_minimax_eval) {
        PRINT_TEST_FAILED(test_name, "best_search_eval != best_minimax_eval");
    }
}

template<SearchType searchType>
static int minimax(Board& board, int depth, int ply, Move& best_global_move, int& best_global_eval)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    const uint64_t zobrist_key = board.state().get_zobrist_key();

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
    else if (isStaleMate || History::threefold_repetition_detected(zobrist_key)) {
        return 0;
    }

    else if (depth == 0) {
        return quiescence_minimax<searchType>(board, ply, best_global_move, best_global_eval);
    }

    int final_node_evaluation = MAXIMIZING_WHITE ? -INF_EVAL : +INF_EVAL;

    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        History::push_position(zobrist_key);
        board.make_move(moves[i]);
        int eval = minimax<nextSearchType>(board, depth - 1, ply + 1, best_global_move, best_global_eval);
        board.unmake_move(moves[i], game_state);
        History::pop_position();

        if constexpr (MAXIMIZING_WHITE) {

            if (ply == 0 && eval > best_global_eval) {
                best_global_eval = eval;
                best_global_move = moves[i];   // if we are in the root node update the best move
            }

            final_node_evaluation = std::max(final_node_evaluation, eval);
        }
        else if (MINIMIZING_BLACK) {

            if (ply == 0 && eval < best_global_eval) {
                best_global_eval = eval;
                best_global_move = moves[i];   // if we are in the root node update the best move
            }

            final_node_evaluation = std::min(final_node_evaluation, eval);
        }
    }
    return final_node_evaluation;
}

template<SearchType searchType>
static int quiescence_minimax(Board& board, int ply, Move& best_global_move, int& best_global_eval)
{
    constexpr bool MAXIMIZING_WHITE = searchType == MAXIMIZE_WHITE;
    constexpr bool MINIMIZING_BLACK = searchType == MINIMIZE_BLACK;

    const uint64_t zobrist_key = board.state().get_zobrist_key();

    if (History::threefold_repetition_detected(zobrist_key)) {
        return 0;
    }

    int static_evaluation = evaluate_position(board);

    if (ply >= MAX_PLY) {
        return static_evaluation;
    }

    MoveList capture_moves;
    generate_legal_moves<ONLY_CAPTURES>(capture_moves, board);

    if (capture_moves.size() == 0) {
        return static_evaluation;   // No captures: return static evaluation
    }

    const GameState game_state = board.state();
    int final_node_evaluation = static_evaluation;

    for (int i = 0; i < capture_moves.size(); i++) {

        constexpr SearchType nextSearchType = MAXIMIZING_WHITE ? MINIMIZE_BLACK : MAXIMIZE_WHITE;

        History::push_position(zobrist_key);
        board.make_move(capture_moves[i]);
        int eval = quiescence_minimax<nextSearchType>(board, ply + 1, best_global_move, best_global_eval);
        board.unmake_move(capture_moves[i], game_state);
        History::pop_position();

        if constexpr (MAXIMIZING_WHITE) {
            final_node_evaluation = std::max(final_node_evaluation, eval);
        }
        else if constexpr (MINIMIZING_BLACK) {
            final_node_evaluation = std::min(final_node_evaluation, eval);
        }
    }

    return final_node_evaluation;
}