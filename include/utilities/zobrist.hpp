#pragma once

/**
 * @file zobrist.hpp
 * @brief Zobrist hashing utilities declaration.
 *
 * https://en.wikipedia.org/wiki/Zobrist_hashing
 * 
 * https://www.chessprogramming.org/Zobrist_Hashing
 * 
 */

 #include "board.hpp"
#include <random>

/**
 * @brief Zobrist
 *
 * Zobrist hash class, used to get the hash key of a chess position
 * 
 */
class Zobrist
{

    /**
     * @brief Zobrist()
     * 
     * constructor of the Zobrist hash.
     * 
     * @note initialize the uniform random numbers
     * 
     */
    Zobrist()
    {

    }

    /**
     * @brief hash(const Board&)
     * 
     * @param[in] position board containing the chess position
     * 
     * @return the hash key of the chess position
     * 
     */
    uint64_t hash(const Board& position)
    {   
        uint64_t hash = 0ULL;
        return hash;
    }
};