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
     * @brief hash(const Board&)
     * 
     * @param[in] position board containing the chess position
     * 
     * @return the hash key of the chess position
     * 
     */
    static uint64_t hash(const Board& position)
    {
        init_random_numbers_only_once();   // initialize the random numbers only the first time is executed

        uint64_t hash = 0ULL;

        for (Square square = Square::SQ_A1; square.is_valid(); square++) {
            const Piece piece = position.get_piece(square);
            if (piece != Piece::EMPTY) {
                hash ^= get_seed(square, piece);
            }
        }
        const GameState state = position.state();

        const Square eps_square = state.en_passant_square();
        if (eps_square.is_valid()) {
            hash ^= get_en_passant_seed(eps_square.col());
        }

        if (state.castle_king_white()) {
            hash ^= get_king_white_castle_seed();
        }
        if (state.castle_queen_white()) {
            hash ^= get_queen_white_castle_seed();
        }
        if (state.castle_king_black()) {
            hash ^= get_king_black_castle_seed();
        }
        if (state.castle_queen_black()) {
            hash ^= get_queen_black_castle_seed();
        }

        if (state.side_to_move() == ChessColor::BLACK) {
            hash ^= get_black_to_move_seed();
        }

        return hash;
    }

    /**
     * @brief get_seed(Square, Piece)
     * 
     * @note square must be valid and piece not empty
     * 
     * @param[in] square selected square
     * @param[in] piece selected piece
     * 
     * @return square_piece_seed[square][piece]
     */
    inline static uint64_t get_seed(Square square, Piece piece)
    {
        assert(square.is_valid());
        assert(is_valid_piece(piece));
        assert(piece != Piece::EMPTY);

        return square_piece_seed[square.value()][static_cast<int>(piece)];
    }

    /**
     * @brief get_en_passant_seed(Col)
     * 
     * @note col must be valid
     * 
     * @param[in] en_passant_col en passnt where col is available
     * 
     * @return en_passant_seed[en_passant_col]
     */
    inline static uint64_t get_en_passant_seed(Col en_passant_col)
    {
        assert(is_valid_col(en_passant_col));

        return en_passant_seed[static_cast<int>(en_passant_col)];
    }

    /**
     * @brief get_king_white_castle_seed()
     * 
     * @return king_white_castle_seed
     */
    inline static uint64_t get_king_white_castle_seed() { return king_white_castle_seed; }

    /**
     * @brief get_queen_white_castle_seed()
     * 
     * @return queen_white_castle_seed
     */
    inline static uint64_t get_queen_white_castle_seed() { return queen_white_castle_seed; }

    /**
     * @brief get_king_black_castle_seed()
     * 
     * @return king_black_castle_seed
     */
    inline static uint64_t get_king_black_castle_seed() { return king_black_castle_seed; }

    /**
     * @brief get_queen_black_castle_seed()
     * 
     * @return queen_black_castle_seed
     */
    inline static uint64_t get_queen_black_castle_seed() { return queen_black_castle_seed; }

    /**
     * @brief get_black_to_move_seed()
     * 
     * @return black_to_move_seed
     */
    inline static uint64_t get_black_to_move_seed() { return black_to_move_seed; }

private:
    /**
     * @brief init_random_numbers_only_once
     * 
     * @note use for lazy initialization of the static class, is only called once and generated the random numbers
     * 
     */
    static void init_random_numbers_only_once()
    {
        static bool initialized = false;

        if (initialized) {
            return;
        }

#ifndef NDEBUG
        const uint64_t SEED = 123456789ULL;
        std::mt19937_64 gen(SEED);   // Use constant seed in debug mode
#else
        std::random_device rd;
        std::mt19937_64 gen(rd());   // Use random seed in release mode
#endif

        std::uniform_int_distribution<uint64_t> dis;

        for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
            for (int p = 0; p < 12; p++) {
                square_piece_seed[sq.value()][p] = dis(gen);
            }
        }

        for (auto& elem : en_passant_seed) {
            elem = dis(gen);
        }

        king_white_castle_seed = dis(gen);
        queen_white_castle_seed = dis(gen);
        king_black_castle_seed = dis(gen);
        queen_black_castle_seed = dis(gen);

        black_to_move_seed = dis(gen);

        initialized = true;
    }

    inline static std::array<std::array<uint64_t, 12>, 64> square_piece_seed;   // 64 squares and 12 pieces
    inline static std::array<uint64_t, 8> en_passant_seed;                      // one for each col (a-h)
    inline static uint64_t king_white_castle_seed;
    inline static uint64_t queen_white_castle_seed;
    inline static uint64_t king_black_castle_seed;
    inline static uint64_t queen_black_castle_seed;
    inline static uint64_t black_to_move_seed;
};