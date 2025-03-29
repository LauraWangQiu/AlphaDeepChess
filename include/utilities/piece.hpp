#pragma once

/**
 * @file piece.hpp
 * @brief Piece types and utilities declaration.
 *
 * 
 */

#include <cassert>

/**
 * @brief WHITE = 0  
 * @brief BLACK = 1
 */
enum class ChessColor : int
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
 *  PAWN = 0,
 *  KNIGHT = 1,
 *  BISHOP = 2,
 *  ROOK = 3,
 *  QUEEN = 4,
 *  KING = 5,
 *  EMPTY = 6
 */
enum class PieceType : int
{
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    EMPTY = 6,
    NUM_PIECES = 7
};

/**
 * @brief NUM_CHESS_PIECES (13)
 * 
 * Number of different pieces in chess, including the empty piece.
 * 
 */
constexpr const int NUM_CHESS_PIECES = static_cast<int>(Piece::NUM_PIECES);

/**
 * @brief NUM_CHESS_PIECE_TYPES (7)
 * 
 * Number of different piece types in chess, including the empty piece.
 * 
 */
constexpr const int NUM_CHESS_PIECE_TYPES = static_cast<int>(PieceType::NUM_PIECES);

/**
 * @brief pieceRepresentation[NUM_CHESS_PIECES]
 * 
 * Piece char representation :
 * 
 *   W_PAWN(0) = 'P',
 *   W_KNIGHT(1) = 'N',
 *   W_BISHOP(2) = 'B',
 *   W_ROOK(3) = 'R',
 *   W_QUEEN(4) = 'Q',
 *   W_KING(5) = 'K',
 *   B_PAWN(6) = 'p',
 *   B_KNIGHT(7) = 'n',
 *   B_BISHOP(8) = 'b',
 *   B_ROOK(9) = 'r',
 *   B_QUEEN(10) = 'q',
 *   B_KING(11) = 'k',
 *   EMPTY(12) = ' ',
 * 
 */
static constexpr char pieceRepresentation[NUM_CHESS_PIECES] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p',
                                                               'n', 'b', 'r', 'q', 'k', ' '};

/**
 * @brief pieceRepresentation[NUM_CHESS_PIECES]
 * 
 * Piece raw value :
 * 
 *   W_PAWN(0) = 100,
 *   W_KNIGHT(1) = 300,
 *   W_BISHOP(2) = 310,
 *   W_ROOK(3) = 500,
 *   W_QUEEN(4) = 900,
 *   W_KING(5) = 500,
 *   B_PAWN(6) = 100,
 *   B_KNIGHT(7) = 300,
 *   B_BISHOP(8) = 310,
 *   B_ROOK(9) = 500,
 *   B_QUEEN(10) = 900,
 *   B_KING(11) = 500,
 *   EMPTY(12) = 0,
 * 
 */
static constexpr uint32_t pieceRawValue[NUM_CHESS_PIECES] = {100U, 320U, 330U, 500U, 900U, 500U, 100U,
                                                             320U, 330U, 500U, 900U, 500U, 0U};


/**
 * @brief is_valid_piece 
 * 
 * Returns if piece is valid.
 * 
 * @param[in] piece The piece
 * 
 * @return
 * - TRUE if piece >= W_PAWN && piece < NUM_PIECES
 * - FALSE if piece is invalid.
 * 
 */
constexpr inline bool is_valid_piece(Piece piece) { return piece >= Piece::W_PAWN && piece < Piece::NUM_PIECES; }

/**
 * @brief is_valid_pieceType 
 * 
 * Returns if pieceType is valid.
 * 
 * @param[in] piece_type The piece type
 * 
 * @return
 * - TRUE if piece_type >= PAWN && piece_type < EMPTY
 * - FALSE if piece_type is invalid.
 * 
 */
constexpr inline bool is_valid_pieceType(PieceType piece_type)
{
    return piece_type >= PieceType::PAWN && piece_type <= PieceType::EMPTY;
}

/**
 * @brief is_valid_color 
 * 
 * Returns if color is valid.
 * 
 * @param[in] color The color
 * 
 * @return
 * - TRUE if color == WHITE || color == BLACK
 * - FALSE if color is invalid.
 * 
 */
constexpr inline bool is_valid_color(ChessColor color)
{
    return color == ChessColor::WHITE || color == ChessColor::BLACK;
}

/**
 * @brief calculates if the color is white 
 * 
 * Returns if color is valid.
 * 
 * @param[in] color The color
 * 
 * @return bool
 * @retval TRUE if color == WHITE
 * @retval False if color == BLACK
 * 
 */
constexpr inline bool is_white(ChessColor color) { return !static_cast<bool>(color); }

/**
 * @brief opposite_color 
 * 
 * Returns the opposite color.
 * 
 * @param[in] color selected color
 * 
 * @return
 * - WHITE if color == BLACK
 * - BLACK if color == WHITE.
 * 
 */
constexpr inline ChessColor opposite_color(ChessColor color)
{
    assert(is_valid_color(color));

    return static_cast<ChessColor>(static_cast<int>(color) ^ 1);
}

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
    assert(is_valid_piece(piece));
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
    assert(is_valid_pieceType(piece_type));

    return pieceRepresentation[static_cast<int>(piece_type) + 6];
}

/**
 * @brief get_color 
 * 
 * Returns the color of the piece
 * 
 * @note Piece::EMPTY has undefined color
 * 
 * @param piece selected piece.
 * @return
 *  - Color::WHITE if piece is white
 *  - Color::BLACK if piece is black
 */
constexpr inline ChessColor get_color(Piece piece)
{
    assert(is_valid_piece(piece));

    // depends on the int value of the Piece enum
    return static_cast<ChessColor>(static_cast<int>(piece) > 5);
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
constexpr inline PieceType piece_to_pieceType(Piece piece)
{
    assert(is_valid_piece(piece));

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
    assert(is_valid_pieceType(type));
    assert(is_valid_color(color));

    // the white and black pieces have a difference of +6 int value
    // E.g  WKnight = 1, BKnight = 7
    return type == PieceType::EMPTY ? Piece::EMPTY
                                    : static_cast<Piece>(static_cast<int>(type) + (6 * static_cast<int>(color)));
}

/**
 * @brief raw_value 
 * 
 * Calculate raw calue of the piece
 * 
 * @param[in] piece selected piece.
 * 
 * @return Piece raw value
 *   W_PAWN(0) = 100,
 *   W_KNIGHT(1) = 320,
 *   W_BISHOP(2) = 330,
 *   W_ROOK(3) = 500,
 *   W_QUEEN(4) = 900,
 *   W_KING(5) = 500,
 *   B_PAWN(6) = 100,
 *   B_KNIGHT(7) = 320,
 *   B_BISHOP(8) = 330,
 *   B_ROOK(9) = 500,
 *   B_QUEEN(10) = 900,
 *   B_KING(11) = 500,
 *   EMPTY(12) = 0,
 * 
 */
constexpr inline uint32_t raw_value(Piece piece)
{
    assert(is_valid_piece(piece));
    return pieceRawValue[static_cast<int>(piece)];
}

/**
 * @brief raw_value 
 * 
 * Calculate raw calue of the piece type
 * 
 * @param[in] piece selected piece type.
 * 
 * @return Piece raw value
 *   PAWN(6) = 100,
 *   KNIGHT(7) = 320,
 *   BISHOP(8) = 330,
 *   ROOK(9) = 500,
 *   QUEEN(10) = 900,
 *   KING(11) = 500,
 *   EMPTY(12) = 0,
 */
constexpr inline uint32_t raw_value(PieceType piece)
{
    assert(is_valid_pieceType(piece));

    return pieceRawValue[6 + static_cast<int>(piece)];
}

/**
 * @brief is_slider 
 * 
 * Check if the piece is an slider (ROOK, QUEEN OR BISHOP)
 * 
 * @param[in] piece selected piece.
 * 
 * @return
 *  - TRUE if piece is BISHOP, ROOK or QUEEN..
 *  - FALSE if piece is not BISHOP, ROOK or QUEEN.
 * 
 */
constexpr inline bool is_slider(PieceType piece)
{
    assert(is_valid_pieceType(piece));

    return piece == PieceType::BISHOP || piece == PieceType::QUEEN || piece == PieceType::ROOK;
}


/**
 * @brief operator+(Piece, int)
 * 
 * Addition operator overload
 * 
 * @note out of bounds will result in Piece::Empty
 * 
 * @param[in] piece The piece value
 * @param[in] value The integer to add
 * 
 * @return (Piece)(piece + value)
 */
constexpr inline Piece operator+(Piece piece, int value)
{
    assert(is_valid_piece(piece));

    const Piece sum = static_cast<Piece>(static_cast<int>(piece) + value);
    return sum < Piece::W_PAWN || sum > Piece::EMPTY ? Piece::EMPTY : sum;
}

/**
 * @brief operator-(Piece, int)
 * 
 * Subtraction operator overload
 * 
 * @note out of bounds will result in Piece::Empty
 * 
 * @param[in] piece The piece value
 * @param[in] value The integer to subtract
 * 
 * @return  (Piece)(piece - value)
 */
constexpr inline Piece operator-(Piece piece, int value)
{
    assert(is_valid_piece(piece));

    const Piece sub = static_cast<Piece>(static_cast<int>(piece) - value);
    return sub < Piece::W_PAWN || sub > Piece::EMPTY ? Piece::EMPTY : sub;
}

/**
 * @brief operator+(PieceType, int)
 * 
 * Addition operator overload
 * 
 * @note out of bounds will result in PieceType::Empty
 * 
 * @param[in] piece_type The piece_type value
 * @param[in] value The integer to add
 * 
 * @return (PieceType)(piece_type + value)
 */
constexpr inline PieceType operator+(PieceType piece_type, int value)
{
    assert(is_valid_pieceType(piece_type));

    const PieceType sum = static_cast<PieceType>(static_cast<int>(piece_type) + value);
    return sum < PieceType::PAWN || sum > PieceType::EMPTY ? PieceType::EMPTY : sum;
}

/**
 * @brief operator-(PieceType, int)
 * 
 * Subtraction operator overload
 * 
 * @note out of bounds will result in PieceType::Empty
 * 
 * @param[in] piece_type The piece_type value
 * @param[in] value The integer to subtract
 * 
 * @return  (PieceType)(piece_type - value)
 */
constexpr inline PieceType operator-(PieceType piece_type, int value)
{
    assert(is_valid_pieceType(piece_type));

    const PieceType sub = static_cast<PieceType>(static_cast<int>(piece_type) - value);
    return sub < PieceType::PAWN || sub > PieceType::EMPTY ? PieceType::EMPTY : sub;
}