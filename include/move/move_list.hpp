#pragma once

/**
 * @file move_list.hpp
 * @brief Move list types and utilities declaration.
 *
 * 
 */

#include "move.hpp"
#include <cassert>

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
    /**
     * @brief MoveList
     * 
     * MoveList constructor.
     * 
     */
    MoveList() : num_moves(0) { }

    /**
     * @brief add
     * 
     * Add a move to the list.
     * 
     * @note this dont check if the capacity is exceeded (MAX_CHESS_MOVES)
     * 
     * @param[in] move move to store.
     * 
     */
    constexpr inline void add(Move move) { moves[num_moves++] = move; }

    /**
     * @brief clear
     * 
     * Empty the list.
     * 
     */
    constexpr inline void clear() { num_moves = 0; }

    /**
     * @brief add
     * 
     * return the number of moves stored.
     * 
     * @return num_moves.
     * 
     */
    constexpr inline int size() const { return num_moves; }

    /**
     * @brief get
     * 
     * return the move in the pos index, 
     * 
     * @note index should be valid (0 <= index < num_moves)
     * 
     * @param[in] index index of the move in the array.
     * 
     * @return moves[index]
     * 
     */
    constexpr inline Move get(int index) const
    {
        assert(0 <= index && index < num_moves);
        return moves[index];
    }

    /**
     * @brief to_string
     * 
     * calculates the string representation of the move list. 
     * 
     * @return (i = 0; i < num_moves)"moves[i].to_string():\n"
     * 
     */
    inline std::string to_string() const
    {
        std::string s = "";
        for (int i = 0; i < num_moves; i++) {
            s += moves[i].to_string() + ":\n";
        }
        return s;
    }

    /**
     * @brief Move& operator[](index)
     * 
     * Overload array operator for non-const access.
     * 
     * @return moves[index]
     * 
     */
    constexpr inline Move& operator[](int index)
    {
        assert(0 <= index && index < num_moves);
        return moves[index];
    }

    /**
     * @brief const Move& operator[](index)
     * 
     * Overload array operator for const access.
     * 
     * @return moves[index]
     * 
     */
    constexpr inline const Move& operator[](int index) const
    {
        assert(0 <= index && index < num_moves);
        return moves[index];
    }

    /**
     * @brief begin
     * 
     * Returns an iterator to the beginning of the move list.
     * 
     * @return Iterator to the first move.
     * 
     */
    constexpr inline Move* begin() { return moves; }

    /**
     * @brief end
     * 
     * Returns an iterator to the end of the move list.
     * 
     * @return Iterator to one past the last move.
     * 
     */
    constexpr inline Move* end() { return moves + num_moves; }

private:
    /**
     * @brief moves
     * 
     * @note max capacity : MAX_CHESS_MOVES
     * 
     * move list
     * 
     */
    Move moves[MAX_CHESS_MOVES];

    /**
     * @brief num_moves
     * 
     * number of moves stored in the list.
     * 
     */
    int num_moves;
};