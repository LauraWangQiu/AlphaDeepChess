#pragma once
#include <cstdint>
#include <types.hpp>

/**
* @brief Handles the game board  
* @brief Uses bitboards to represent the board state
*/
class Board {
private:
    uint64_t allBB;     // Bitboard for all pieces
    uint64_t whiteBB;   // Bitboard for white pieces
    uint64_t blackBB;   // Bitboard for black pieces

    uint64_t piecesBB[static_cast<int>(Piece::MAX_SIZE) - 2];   // Bitboards for each piece type

public:
    /**
     * @brief Construct a new Board object
     */
    Board();
    /**
     * @brief Destroy the Board object
     */
    ~Board();

    /**
     * @brief Overloads the << operator to print the board
     * @param os The output stream
     * @param board The board to print
     */
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

    /**
     * @brief Get the all pieces bitboard
     * @return All pieces bitboard
     */
    inline uint64_t getAllBB() const { return allBB; }
    /**
     * @brief Set the all pieces bitboard
     * @param allBB The all pieces bitboard
     */
    inline void setAllBB(uint64_t allBB) { this->allBB = allBB; }

    /**
     * @brief Get the white pieces bitboard
     * @return White pieces bitboard
     */
    inline uint64_t getWhiteBB() const { return whiteBB; }
    /**
     * @brief Set the white pieces bitboard
     * @param whiteBB The white pieces bitboard
     */
    inline void setWhiteBB(uint64_t whiteBB) { this->whiteBB = whiteBB; }

    /**
     * @brief Get the black pieces bitboard
     * @return Black pieces bitboard
     */
    inline uint64_t getBlackBB() const { return blackBB; }
    /**
     * @brief Set the black pieces bitboard
     * @param blackBB The black pieces bitboard
     */
    inline void setBlackBB(uint64_t blackBB) { this->blackBB = blackBB; }

    /**
     * @brief Get all the bitboards of each piece
     * @return All the bitboards of each piece
     */
    inline uint64_t* getPiecesBB() { return piecesBB; }
    /**
     * @brief Get the bitboard for a specific piece type
     * @param piece The piece type
     * @return The bitboard for the piece type
     */
    inline uint64_t getPieceBB(Piece piece) const { return piecesBB[static_cast<size_t>(piece)]; }
    /**
     * @brief Set the bitboard for a specific piece type
     * @param piece The piece type
     * @param bb The bitboard for the piece type
     */
    inline void setPieceBB(Piece piece, uint64_t bb) { piecesBB[static_cast<size_t>(piece)] = bb; }
};
