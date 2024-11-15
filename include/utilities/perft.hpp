#pragma once

/**
 * @file perft.hpp
 * @brief Perft services.
 *
 * perft test types and utilities declaration.
 * 
 * https://www.chessprogramming.org/Perft
 * 
 */

#include <string>
#include <cstdint>

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
 * @param[out] time ms passed to complete the perft test.
 * @param[out] nodes counted nodes.
 */
void perft(const std::string &FEN, uint64_t depth, int64_t& time, uint64_t& nodes);