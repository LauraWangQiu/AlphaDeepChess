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
#include <array>

/**
 * @brief Zobrist
 *
 * Zobrist hash class, used to get the hash key of a chess position
 * 
 */
class Zobrist
{
public:

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
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        // Initialize piece table
        for (auto& row : piece_table)
        {
            for (auto& elem : row)
            {
                elem = dis(gen);
            }
        }

        // Initialize castling rights
        for (auto& elem : castling)
        {
            elem = dis(gen);
        }

        // Initialize en passant files
        for (auto& elem : en_passant)
        {
            elem = dis(gen);
        }

        // Initialize black to move
        black_to_move = dis(gen);
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

        for (Square square = Square::SQ_A1; square.is_valid(); square++)
        {
            const Piece piece = position.get_piece(square);
            if (piece != Piece::EMPTY)
            {
                hash ^= piece_table[static_cast<int>(piece)][square];
            }
        }

        if (position.state().side_to_move() == ChessColor::BLACK)
        {
            hash ^= black_to_move;
        }

        if (position.state().castle_king_white())
        {
            hash ^= castling[0];
        }
        if (position.state().castle_queen_white())
        {
            hash ^= castling[1];
        }
        if (position.state().castle_king_black())
        {
            hash ^= castling[2];
        }
        if (position.state().castle_queen_black())
        {
            hash ^= castling[3];
        }

        const Square eps_square = position.state().en_passant_square();
        if (eps_square.is_valid())
        {
            hash ^= en_passant[static_cast<int>(eps_square.col())];
        }

        return hash;
    }
private:

    // Random numbers for each piece (12 types) on each square (64 squares)
    std::array<std::array<uint64_t, 64>, 12> piece_table;
    // Random numbers for castling rights (WK, WQ, BK, BQ)
    std::array<uint64_t, 4> castling;
    // Random numbers for en passant files (a-h)
    std::array<uint64_t, 8> en_passant;
    // Random number for black to move
    uint64_t black_to_move;
};