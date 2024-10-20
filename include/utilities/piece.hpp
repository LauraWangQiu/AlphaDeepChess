#pragma once

/**
 * @file piece.hpp
 * @brief Piece types and utilities declaration.
 *
 * 
 */

/**
 * @brief WHITE = 0  
 * @brief BLACK = 1
 */
enum class ChessColor
{
    WHITE = 0,
    BLACK = 1
};

/**
 * @brief Piece
 * 
 * @note Do not change the assigned values, they are used in other functions.
 * 
 * W_PAWN = 0,  
 * W_KNIGHT = 1,  
 * W_BISHOP = 2,  
 * W_ROOK = 3,  
 * W_QUEEN = 4,  
 * W_KING = 5,  
 * B_PAWN = 6,  
 * B_KNIGHT = 7,  
 * B_BISHOP = 8,  
 * B_ROOK = 9,  
 * B_QUEEN = 10,  
 * B_KING = 11,  
 * EMPTY = 12  
 * NUM_PIECES = 13
*/
enum class Piece : int
{
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
    NUM_PIECES = 13
};

/*
 * @brief PieceType
 * 
 * @note Do not change the assigned values, they are used in other functions.
 * 
 *  KNIGHT = 1,
 *  BISHOP = 2,
 *  ROOK = 3,
 *  QUEEN = 4,
 *  KING = 5,
 *  EMPTY = 6
 *
 * 
 */
enum class PieceType : int
{
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    EMPTY = 6
};

/**
 * @brief NUM_CHESS_PIECES (13)
 * 
 * Number of piece types in chess, including the empty piece.
 * 
 */
constexpr const int NUM_CHESS_PIECES = static_cast<int>(Piece::NUM_PIECES);

static constexpr char pieceRepresentation[NUM_CHESS_PIECES] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p',
                                                               'n', 'b', 'r', 'q', 'k', ' '};

/**
 * @brief pieceToChar
 * 
 * Returns the character piece representation  
 * 
 * @param[in] piece piece to convert to char.
 * 
 * @return {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '}
 * 
 */
constexpr inline char piece_to_char(Piece piece)
{
    return pieceRepresentation[static_cast<int>(piece)];
}

/**
 * @brief char_to_piece
 * 
 * Returns the Piece enum from the character representation
 * 
 * @param piece_char {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '}
 * 
 * @return
 * - Piece::W_PAWN if piece_char = 'P'.
 * - Piece::W_BISHOP if piece_char = 'B'.
 * - Piece::W_KNIGHT if piece_char = 'N'.
 * - Piece::W_ROOK if piece_char = 'R'.
 * - Piece::W_QUEEN if piece_char = 'Q'.
 * - Piece::W_KING if piece_char = 'K'.
 * - Piece::B_PAWN if piece_char = 'p'.
 * - Piece::B_KNIGHT if piece_char = 'n'.
 * - Piece::B_BISHOP if piece_char = 'b'.
 * - Piece::B_ROOK if piece_char = 'r'.
 * - Piece::B_QUEEN if piece_char = 'q'.
 * - Piece::B_KING if piece_char = 'k'.
 * - Piece::EMPTY if other char.
 * 
 */
constexpr inline Piece char_to_piece(char piece_char)
{
    switch (piece_char) {
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
 * @brief char_to_pieceType
 * 
 * Returns the PieceType enum from the character representation
 * 
 * @note capital letters will be converted to lowercase.
 * 
 * @param piece_char {'p', 'n', 'b', 'r', 'q', 'k', ' '}
 * 
 * @return
 * - PieceType::PAWN if piece_char = 'p'.
 * - PieceType::KNIGHT if piece_char = 'n'.
 * - PieceType::BISHOP if piece_char = 'b'.
 * - PieceType::ROOK if piece_char = 'r'.
 * - PieceType::QUEEN if piece_char = 'q'.
 * - PieceType::KING if piece_char = 'k'.
 * - PieceType::EMPTY if other char.
 * 
 */
constexpr inline PieceType char_to_pieceType(char pieceType_char)
{
    switch (tolower(pieceType_char)) {
    case 'p': return PieceType::PAWN;
    case 'n': return PieceType::KNIGHT;
    case 'b': return PieceType::BISHOP;
    case 'r': return PieceType::ROOK;
    case 'q': return PieceType::QUEEN;
    case 'k': return PieceType::KING;
    default: return PieceType::EMPTY;
    }
}

/**
 * @brief pieceType_to_char
 * 
 * Returns the character piece type representation  
 * 
 * @param[in] piece_type piece to convert to char.
 * 
 * @return {'n', 'b', 'r', 'q', 'k', ' '}
 * 
 */
constexpr inline char pieceType_to_char(PieceType piece_type)
{
    return pieceRepresentation[static_cast<int>(piece_type) + 6];
}

/**
 * @brief get_color 
 * 
 * Returns the color of the piece
 * 
 * @param piece selected piece.
 * @return
 *  - Color::WHITE if piece is white
 *  - Color::BLACK if piece is black
 */
constexpr inline ChessColor get_color(Piece piece)
{
    // depends on the int value of the Piece enum
    return static_cast<int>(piece) <= 5 ? ChessColor::WHITE : ChessColor::BLACK;
}

/**
 * @brief piece_to_PieceType 
 * 
 * Convert from Piece to PieceType.
 * E.g : Piece::W_QUEEN -> PieceType::QUEEN.
 * 
 * @param[in] piece piece to convert.
 * 
 * @return PieceType
 * 
 */
constexpr inline PieceType piece_to_PieceType(Piece piece)
{
    // the white and black pieces have a difference of +6 int value
    // E.g  WKnight = 1, BKnight = 7
    // if piece is black then we substract 6 to convert to white.
    const uint8_t type_int = static_cast<int>(piece);
    return static_cast<PieceType>(type_int - (6 * (type_int >= 6)));
}

/**
 * @brief create_piece 
 * 
 * Create piece by piece type and color.
 * 
 * @param[in] type selected piece.
 * @param[in] color selected color.
 * 
 * @return Piece
 * 
 */
constexpr inline Piece create_piece(PieceType type, ChessColor color)
{
    // the white and black pieces have a difference of +6 int value
    // E.g  WKnight = 1, BKnight = 7
    return static_cast<Piece>(static_cast<int>(type) + (6 * static_cast<int>(color)));
}