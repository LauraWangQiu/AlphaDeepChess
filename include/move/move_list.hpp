#pragma once

/**
 * @file move_list.hpp
 * @brief Move list types and utilities declaration.
 *
 * 
 */

#include "move.hpp"


/**
 * @brief MAX_CHESS_MOVES (218)
 *  
 *   max number of moves found in a chess position is 218.
 *   https://www.chessprogramming.org/Encoding_Moves#MoveIndex
 * 
 */
constexpr const int MAX_CHESS_MOVES = 256;

/**
 * @brief MoveList
 * 
 * Represents a chess move list.
 * 
 */
class MoveList
{
public:
    MoveList() : nMoves(0) { }

    // store a move in the list
    constexpr inline void add(Move move) { moves[nMoves++] = move; }

    // empty the list
    constexpr inline void clear() { nMoves = 0; }

    // return the number of moves stored
    constexpr inline int size() const { return nMoves; }

    // return the move in the pos index, index should be valid ( 0 <= index< nMoves)
    constexpr inline Move get(int index) const { return moves[index]; }

    /*
     *   Return string representation of all moves in the list E.g :
     *   e2e4:
     *   b1b2:
     */
    inline std::string to_string() const
    {
        std::string s = "";
        for (int i = 0; i < nMoves; i++) {
            s += moves[i].to_string() + ":\n";
        }
        return s;
    }

private:
    Move moves[MAX_CHESS_MOVES];
    int nMoves;
};