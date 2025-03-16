/**
 * @file perft.cpp
 * @brief perft services.
 *
 * perft test types and utilities declaration.
 * 
 * https://www.chessprogramming.org/Perft
 * 
 */

#include "perft.hpp"
#include "board.hpp"
#include "transposition_table.hpp"
#include "move_generator.hpp"
#include "zobrist.hpp"
#include <chrono>

// store node result in transposition table
static inline void store_nodes_in_tt(uint64_t zobrist_key, uint8_t depth, int nodes);

// return false if no entry in transposition table
static inline bool get_nodes_in_tt(uint64_t zobrist_key, uint8_t depth, int& nodest);

/**
 * @brief perft_recursive
 * 
 * Count all the nodes of the position until the maximum depth.
 * 
 * @param[in,out] board The output stream
 * @param[in] depth maximum depth to reach.
 * @param[in] use_tt use of transposition table to speed up process
 * 
 * @return counted nodes.
 * 
 */
static uint64_t perft_recursive(Board& board, uint8_t depth, bool use_tt);

/**
 * @brief perft
 * 
 * Debugging function to walk the move generation tree 
 * of strictly legal moves to count all the leaf nodes of a certain depth,
 * which can be compared to predetermined values and used to isolate bugs. In perft,
 * nodes are only counted at the end after the last makemove. Thus "higher" terminal nodes
 * (e.g. mate or stalemate) are not counted, instead the number of move paths of a certain depth.
 * 
 * @note Perft ignores draws by repetition, by the fifty-move rule and by insufficient material.
 * By recording the amount of time taken for each iteration, it's possible to compare the 
 * performance of different move generators or the same generator on different machines.
 * 
 * @param[in] FEN position to study.
 * @param[in] depth depth to reach.
 * @param[out] moveNodeList legal moves and their number of nodes
 * @param[out] time ms passed to complete the perft test.
 * @param[in] use_tt use of transposition table to speed up process
 * 
 */
void perft(const std::string& FEN, uint64_t depth, MoveNodesList& moveNodeList, int64_t& time, bool use_tt)
{
    Board board;
    board.load_fen(FEN);

    MoveList moves;
    moveNodeList.reserve(moves.size());

    generate_legal_moves(moves, board);

    for (int i = 0; i < moves.size(); i++) {
        moveNodeList.emplace_back(moves[i], 0ULL);
    }

    auto start = std::chrono::high_resolution_clock::now();


    const GameState game_state = board.state();

    for (int i = 0; i < moves.size(); i++) {
        board.make_move(moves[i]);
        moveNodeList[i].second = perft_recursive(board, depth - 1, use_tt);
        board.unmake_move(moves[i], game_state);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    time = duration.count();
}

/**
 * @brief perft_recursive
 * 
 * Count all the nodes of the position until the maximum depth.
 * 
 * @param[in,out] board The output stream
 * @param[in] depth maximum depth to reach.
 * @param[in] use_tt use of transposition table to speed up process
 * 
 * @return counted nodes.
 * 
 */
static uint64_t perft_recursive(Board& board, uint8_t depth, bool use_tt)
{
    assert(board.state().get_zobrist_key() == Zobrist::hash(board));

    if (depth == 0) {
        return 1ULL;
    }

    const GameState game_state = board.state();
    const uint64_t zobrist_key = game_state.get_zobrist_key();

    if (use_tt) {
        int nodes_tt = 0;

        if (get_nodes_in_tt(zobrist_key, depth, nodes_tt)) {

            return uint64_t(nodes_tt);
        }
    }

    uint64_t nodes = 0ULL;
    MoveList moves;
    generate_legal_moves(moves, board);


    for (int i = 0; i < moves.size(); i++) {
        board.make_move(moves[i]);
        nodes += perft_recursive(board, depth - 1, use_tt);
        board.unmake_move(moves[i], game_state);
    }

    if (use_tt) {
        store_nodes_in_tt(zobrist_key, depth, int(nodes));
    }

    return nodes;
}

// store node result in transposition table
static inline void store_nodes_in_tt(uint64_t zobrist_key, uint8_t depth, int nodes)
{
    const TranspositionTable::Entry entry = TranspositionTable::get_entry(zobrist_key);

    if (entry.is_valid() && depth <= entry.depth) {
        return;
    }

    TranspositionTable::store_entry(zobrist_key, nodes, Move(2U), TranspositionTable::NodeType::EXACT, depth);
}

// return false if no entry in transposition table
static inline bool get_nodes_in_tt(uint64_t zobrist_key, uint8_t depth, int& nodes)
{
    const TranspositionTable::Entry entry = TranspositionTable::get_entry(zobrist_key);

    nodes = entry.evaluation;   // we store the nodes in the evaluation field

    return entry.is_valid() && entry.key == zobrist_key && entry.depth == depth;
}