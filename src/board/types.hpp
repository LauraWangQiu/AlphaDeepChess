#pragma once

/**
 * @brief Represents all the rows of a chess board from ROW_1 to ROW_8
 */
enum Row : uint8_t { ROW_1 = 0, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8 };

/**
 * @brief Represents all the columns of a chess board from COL_A to COL_H
 */
enum Column : uint8_t { COL_A = 0, COL_B, COL_C, COL_D, COL_E, COL_F, COL_G, COL_H };

/**
 * @brief Returns if the coordinate is valid (between 0 and 7)
 * @param row The row coordinate
 * @param col The column coordinate
 * @return true if the coordinate is valid
 * @return false if the coordinate is not valid
 */
constexpr inline bool validCoord(int row, int col) {
    return row >= ROW_1 && row <= ROW_8 && col >= COL_A && col <= COL_H;
}

/**
 * @brief WHITE = 0  
 * @brief BLACK = 1
 */
enum class Color { WHITE = 0, BLACK = 1 };

/**
 * @brief W_PAWN = 0,  
 * @brief W_KNIGHT = 1,  
 * @brief W_BISHOP = 2,  
 * @brief W_ROOK = 3,  
 * @brief W_QUEEN = 4,  
 * @brief W_KING = 5,  
 * @brief B_PAWN = 6,  
 * @brief B_KNIGHT = 7,  
 * @brief B_BISHOP = 8,  
 * @brief B_ROOK = 9,  
 * @brief B_QUEEN = 10,  
 * @brief B_KING = 11,  
 * @brief EMPTY = 12  
 * @brief MAX_SIZE = 13
*/
enum class Piece {
    W_PAWN = 0,
    W_KNIGHT = 1,
    W_BISHOP = 2,
    W_ROOK = 3,
    W_QUEEN = 4,
    W_KING = 5,
    B_PAWN = 6,
    B_KNIGHT = 7,
    B_BISHOP = 8,
    B_ROOK = 9,
    B_QUEEN = 10,
    B_KING = 11,
    EMPTY = 12,
    MAX_SIZE = 13   // Don't remove this line
};

static constexpr char pieceRepresentation[static_cast<int>(Piece::MAX_SIZE)] = {
    'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '};

/**
 * @brief Returns the character piece representation  
 * @brief {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '}
 */
constexpr inline char pieceToChar(Piece piece) {
    return pieceRepresentation[static_cast<int>(piece)];
}

/**
 * @brief Returns the Piece enum from the character representation
 * @param pieceChar {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '}
 * @return Piece enum
 */
constexpr inline Piece charToPiece(char pieceChar) {
    switch (pieceChar) {
    case 'P': return Piece::W_PAWN;
    case 'N': return Piece::W_KNIGHT;
    case 'B': return Piece::W_BISHOP;
    case 'R': return Piece::W_ROOK;
    case 'Q': return Piece::W_QUEEN;
    case 'K': return Piece::W_KING;
    case 'p': return Piece::B_PAWN;
    case 'n': return Piece::B_KNIGHT;
    case 'b': return Piece::B_BISHOP;
    case 'r': return Piece::B_ROOK;
    case 'q': return Piece::B_QUEEN;
    case 'k': return Piece::B_KING;
    default: return Piece::EMPTY;
    }
}

/**
 * @brief Returns the color of the piece
 * @param piece The piece
 * @return Color of the piece
 */
constexpr inline Color color(Piece piece) {
    if (static_cast<int>(piece) <= 5) return Color::WHITE;
    return Color::BLACK;
}
