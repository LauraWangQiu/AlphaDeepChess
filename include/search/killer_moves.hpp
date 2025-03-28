#pragma once

/**
 * @file killer_moves.hpp
 * @brief killer moves utilities declaration.
 * 
 * https://www.chessprogramming.org/Killer_Heuristic
 * https://rustic-chess.org/search/ordering/killers.html
 * 
 * A Killer Move is a quiet move which caused a search cutoff in a sibling node, 
 * or any other earlier branch in the tree with the same ply distance to the root.
 * There is a high probability that this moves are the best in the position, 
 * so is good to promote them in the move ordering.
 * 
 */

#include "search_utils.hpp"
#include <cstring>

/**
 * @brief KillerMoves
 *
 * Killer move class, used to store two killer for each depth ply in the search
 * 
 */
class KillerMoves
{

public:
    /**
     * @brief store a killer move at a given search ply
     * 
     * @note Max capacity of 2 moves per ply
     * 
     * @param[in] ply search ply
     * @param[in] killer_move killer move to store
     *   
     */
    static inline void store_killer(uint32_t ply, Move killer_move)
    {
        assert(killer_move.is_valid());
        assert(ply < KILLERS_MAX_SIZE);

        if (killer_move == killers[ply][0] || killer_move == killers[ply][1]) {
            return;   // move was previously stored return
        }
        // if index 1 is occupied then store in index 2, if 2 is occupied overwrite 2.
        // (1 is the older, 2 is the Least Recently Stored)
        const int free_index = killers[ply][0] == Move::null() ? 0 : 1;
        killers[ply][free_index] = killer_move;
    }

    /**
     * @brief retrieve the first killer move stored at the selected ply.
     * 
     * @param[in] ply search ply
     *   
     * @return Move
     * @retval Move valid if there is a killer move stored
     * @retval Move::null() if no killer move stored
     * 
     */
    static inline Move get_killer_1(uint32_t ply)
    {
        assert(ply < KILLERS_MAX_SIZE);
        return killers[ply][0];
    }

    /**
     * @brief retrieve the second killer move stored at the selected ply.
     * 
     * @param[in] ply search ply
     *   
     * @return Move
     * @retval Move valid if there is a killer move stored
     * @retval Move::null() if no killer move stored
     * 
     */
    static inline Move get_killer_2(uint32_t ply)
    {
        assert(ply < KILLERS_MAX_SIZE);
        return killers[ply][1];
    }

    /**
     * @brief clear()
     * 
     * remove all killer moves
     *       
     */
    static inline void clear() { std::memset(killers, 0, sizeof(killers)); }

    KillerMoves() = delete;
    ~KillerMoves() = delete;

private:
    static inline const uint32_t KILLERS_MAX_SIZE = INF_DEPTH;
    static inline Move killers[KILLERS_MAX_SIZE][2];
};
