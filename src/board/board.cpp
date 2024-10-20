/**
 * @file board.cpp
 * @brief Board implementations.
 *
 */

#include "board.hpp"

#include <sstream>
#include <stdexcept>
/**
 * @brief put_piece
 * 
 * Add piece to the board
 * 
 * @note piece and square must be valid.
 * 
 * @param[in] piece
 * @param[in] square
 * 
 */
void Board::put_piece(Piece piece, Square square)
{
    uint64_t mask = square.mask();

    // first remove the previous piece
    bitboard_piece[static_cast<int>(get_piece(square))] &= ~mask;

    // place the new piece
    bitboard_piece[static_cast<int>(piece)] |= mask;
    array_piece[square] = piece;

    if (get_color(piece) == ChessColor::WHITE) {
        bitboard_white |= mask;
        bitboard_black &= ~mask;
    }
    else {
        bitboard_black |= mask;
        bitboard_white &= ~mask;
    }

    bitboard_all |= mask;
}

/**
 * @brief remove_piece
 * 
 * Remove piece from the board
 * 
 * @note square must be valid.
 * 
 * @param[in] square
 * 
 */
void Board::remove_piece(Square square)
{
    uint64_t mask = square.mask();

    bitboard_piece[static_cast<int>(get_piece(square))] &= ~mask;
    array_piece[square] = Piece::EMPTY;
    bitboard_all &= ~mask;
    bitboard_white &= ~mask;
    bitboard_black &= ~mask;
}

/**
 * @brief clean
 * 
 * Remove all pieces on the board.
 * 
 */
void Board::clean()
{
    bitboard_all = 0U;
    bitboard_black = 0U;
    bitboard_white = 0U;

    for (int piece = 0; piece < NUM_CHESS_PIECES; piece++) {
        bitboard_piece[piece] = 0U;
    }

    for (int square = 0; square < NUM_SQUARES; square++) {
        array_piece[square] = Piece::EMPTY;
    }
}

/**
 * @brief make_move
 * 
 * Make the move in the board.
 * 
 * @note If the move is not valid in the position the game will be corrupted.
 * 
 * @param[in] move chess move.
 * 
 */
void Board::make_move(Move move)
{
    const Square origin_square = move.square_from();
    const Square end_square = move.square_to();

    const Piece origin_piece = get_piece(origin_square);
    const Piece end_piece = get_piece(end_square);
    const MoveType move_type = move.type();

    /** Update the board pieces */

    if (move_type == MoveType::NORMAL) {
        remove_piece(end_square);
        put_piece(origin_piece, end_square);
        remove_piece(origin_square);
    }
    else if (move_type == MoveType::CASTLING) {

        const Col rook_origin_col = (end_square.col() == COL_G) ? COL_H : COL_A;
        const Col rook_end_col = (end_square.col() == COL_G) ? COL_D : COL_F;

        const Square rook_origin_square(origin_square.row(), rook_origin_col);
        const Square rook_end_square(origin_square.row(), rook_end_col);

        put_piece(origin_piece, end_square);
        put_piece(get_piece(rook_origin_square), rook_end_square);
        remove_piece(origin_square);
        remove_piece(rook_origin_square);

        /**  update the game state regarding to castling */
        if (move == Move::castle_white_king()) {
            game_state.set_castle_king_white(false);
        }
        else if (move == Move::castle_black_king()) {
            game_state.set_castle_king_black(false);
        }
        else if (move == Move::castle_white_queen()) {
            game_state.set_castle_queen_white(false);
        }
        else {
            game_state.set_castle_queen_black(false);
        }
    }
    else if (move_type == MoveType::EN_PASSANT) {

        const Square enemy_square(origin_square.row(), end_square.col());
        remove_piece(enemy_square);
        put_piece(origin_piece, end_square);
        remove_piece(origin_square);
    }
    else if (move_type == MoveType::PROMOTION) {
        const PieceType promotion_piece = move.promotion_piece();
        remove_piece(end_square);
        put_piece(create_piece(promotion_piece, get_color(origin_piece)), end_square);
        remove_piece(origin_square);
    }

    /**  update the game state */

    // if move is not a capture last capture piece will be Piece::Empty.
    if (move_type != MoveType::EN_PASSANT) {
        game_state.set_last_captured_piece(piece_to_PieceType(end_piece));
    }
    else {
        game_state.set_last_captured_piece(PieceType::PAWN);
    }

    // increment the move counter
    game_state.set_move_number(game_state.move_number() + 1UL);

    // in black turn (move counter * 2) and in white (move counter * 2 + 1)
    game_state.set_half_move(!game_state.half_move());

    // increment 50 move rule if the move is not a pawn move or is not a capture move
    if (!is_empty(end_square) && piece_to_PieceType(origin_piece) != PieceType::PAWN) {
        game_state.set_fifty_move_rule_counter(game_state.fifty_move_rule_counter() + 1U);
    }
}

/**
 * @brief unmake_move
 * 
 * unmake the move in the board, restoring the previous game state.
 * 
 * @note If the move was not valid in the position the game will be corrupted.
 * 
 * @param[in] move chess move.
 * @param[in] previous_state previous game state.
 * 
 */
void Board::unmake_move(Move move, GameState previous_state)
{
    const Square origin_square = move.square_from();
    const Square end_square = move.square_to();

    const Piece end_piece = get_piece(end_square);
    const MoveType move_type = move.type();

    if (move_type == MoveType::NORMAL) {

        put_piece(end_piece, origin_square);

        // if last move was not a capture captured piece will be EMPTY.
        const PieceType captured_piece = game_state.last_captured_piece();
        put_piece(create_piece(captured_piece, game_state.side_to_move()), end_square);
    }
    else if (move_type == MoveType::CASTLING) {

        const Col rook_origin_col = (end_square.col() == COL_G) ? COL_H : COL_A;
        const Col rook_end_col = (end_square.col() == COL_G) ? COL_D : COL_F;

        const Square rook_origin_square(origin_square.row(), rook_origin_col);
        const Square rook_end_square(origin_square.row(), rook_end_col);

        put_piece(end_piece, origin_square);
        put_piece(get_piece(rook_end_square), rook_origin_square);
        remove_piece(end_square);
        remove_piece(rook_end_square);
    }
    else if (move_type == MoveType::EN_PASSANT) {
        const Square enemy_square(origin_square.row(), end_square.col());

        put_piece(create_piece(PieceType::PAWN, game_state.side_to_move()), enemy_square);
        put_piece(end_piece, origin_square);
        remove_piece(end_square);
    }
    else if (move_type == MoveType::PROMOTION) {
        const PieceType promotion_piece = move.promotion_piece();
        put_piece(create_piece(promotion_piece, get_color(end_piece)), origin_square);

        // if last move was not a capture captured piece will be EMPTY.
        const PieceType captured_piece = game_state.last_captured_piece();
        put_piece(create_piece(captured_piece, game_state.side_to_move()), end_square);
    }

    game_state = previous_state;
}

/**
 * @brief load_fen
 * 
 * Set the position represented as fen on the chess board.
 * 
 *  https://www.chess.com/terms/fen-chess
 *  https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 * 
 * @param[in] fen Chess position in fen string format
 * 
 */
void Board::load_fen(const std::string& fen)
{
    Board::clean();

    Row row = ROW_8;
    Col col = COL_A;
    char token;
    Piece piece;
    std::istringstream ss(fen);

    ss >> std::noskipws;

    // 1. Piece placement
    while ((ss >> token) && !isspace(token)) {
        if (isdigit(token)) {
            col += (token - '0');   // Advance the given number of columns
        }
        else if (token == '/') {
            row -= 1;   // Advance to the next row
            col = COL_A;
        }
        else if ((piece = char_to_piece(token)) != Piece::EMPTY) {
            put_piece(piece, Square(row, col));
            col++;
        }
    }

    // 2. Active color
    ss >> token;

    game_state.set_side_to_move(token == 'w' ? ChessColor::WHITE : ChessColor::BLACK);

    ss >> token;

    // 3. Castling availability.
    while ((ss >> token) && !isspace(token)) {
        if (token == 'K') {
            game_state.set_castle_king_white(true);
        }
        else if (token == 'Q') {
            game_state.set_castle_queen_white(true);
        }
        else if (token == 'k') {
            game_state.set_castle_king_black(true);
        }
        else if (token == 'q') {
            game_state.set_castle_queen_black(true);
        }
    }

    check_and_modify_castle_rights();

    // 4. En passant square.

    ss >> std::skipws >> token;
    if (token != '-') {
        col = static_cast<Col>(token - 'a');
        ss >> token;
        row = static_cast<Row>(token - '1');

        game_state.set_en_passsant_square(Square(row, col));
        check_and_modify_en_passant_rule();
    }

    // 5-6. Halfmove clock and fullmove number
    uint32_t moveNumber, halfmove;
    ss >> std::skipws >> halfmove >> moveNumber;
    game_state.set_move_number(moveNumber);

    //half move counter could only be (2 * moveCounter) or (2 * moveCounter + 1).
    // we put half move bit to 1 if halfmove == moveNumber * 2U, else to 0.
    game_state.set_half_move(halfmove != moveNumber * 2U);
}

/**
 * @brief fen
 * 
 * Returns fen representation of the position
 * 
 *  https://www.chess.com/terms/fen-chess
 *  https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 * 
 * @return std::string fen
 * 
 */
std::string Board::fen() const
{
    int emptyCounter;
    std::ostringstream fen;

    for (Row row = ROW_8; row >= ROW_1; row--) {
        for (Col col = COL_A; col <= COL_H; col++) {
            emptyCounter = 0;
            while (col <= COL_H && is_empty(Square(row, col))) {
                emptyCounter++;
                col++;
            };

            if (emptyCounter) fen << emptyCounter;

            if (col <= 7) fen << (Square(row, col));
        }

        if (row > 0) fen << '/';
    }

    fen << (game_state.side_to_move() == ChessColor::WHITE ? " w " : " b ");

    bool any_castle_avaliable = false;
    if (game_state.castle_king_white()) {
        fen << 'K';
        any_castle_avaliable = true;
    }
    if (game_state.castle_queen_white()) {
        fen << 'Q';
        any_castle_avaliable = true;
    }
    if (game_state.castle_king_black()) {
        fen << 'k';
        any_castle_avaliable = true;
    }
    if (game_state.castle_king_black()) {
        fen << 'q';
        any_castle_avaliable = true;
    }

    if (!any_castle_avaliable) {
        fen << '-';
    }

    if (game_state.en_passsant_square().is_valid()) {
        fen << " " + game_state.en_passsant_square().to_string() + " ";
    }
    else {
        fen << " - ";
    }

    fen << game_state.half_move() << " " << game_state.move_number();

    return fen.str();
}

/**
 * @brief Board
 * 
 *  Constructor of Board class.
 */
Board::Board() : bitboard_all(0), bitboard_white(0), bitboard_black(0), game_state() { clean(); }

/**
 * @brief ~Board
 * 
 *  Destructor of Board class.
 */
Board::~Board() { }

/**
 * @brief operator<<
 * 
 * Overloads the << operator to print the board
 * 
 * @param[out] os The output stream
 * @param[in] board The board to print
 * 
 */
std::ostream& operator<<(std::ostream& os, const Board& board)
{

    os << "\n +---+---+---+---+---+---+---+---+\n";

    for (Row row = ROW_8; row >= ROW_1; --row) {
        for (Col col = COL_A; col <= COL_H; ++col) {
            os << " | " << piece_to_char(board.get_piece(Square(row, col)));
        }
        os << " | " << static_cast<int>(row) + 1 << "\n +---+---+---+---+---+---+---+---+\n";
    }
    os << "   a   b   c   d   e   f   g   h\n";

    os << "\n\nFen: " << board.fen();
    return os;
}


/**
 * @brief check_and_modify_castle_rights
 * 
 *  Check if the castle rights are correct and modify them if there are errors.
 * 
 */
void Board::check_and_modify_castle_rights()
{

    // Check for white king-side castling rights
    if (game_state.castle_king_white()) {

        if (get_piece({ROW_1, COL_E}) == Piece::W_KING &&
            get_piece({ROW_1, COL_H}) == Piece::W_ROOK) {
            game_state.set_castle_king_white(true);
        }
    }
    // Check for white queen-side castling rights
    if (game_state.castle_queen_white()) {

        if (get_piece({ROW_1, COL_E}) == Piece::W_KING &&
            get_piece({ROW_1, COL_A}) == Piece::W_ROOK) {
            game_state.set_castle_queen_white(true);
        }
    }

    // Check for black king-side castling rights
    if (game_state.castle_king_black()) {

        if (get_piece({ROW_8, COL_E}) == Piece::B_KING &&
            get_piece({ROW_8, COL_H}) == Piece::B_ROOK) {
            game_state.set_castle_king_black(true);
        }
    }

    // Check for black queen-side castling rights
    if (game_state.castle_queen_black()) {

        if (get_piece({ROW_8, COL_E}) == Piece::B_KING &&
            get_piece({ROW_8, COL_A}) == Piece::B_ROOK) {
            game_state.set_castle_queen_black(true);
        }
    }
}

/**
 * @brief check_and_modify_en_passant_rule
 * 
 *  Check if en passant is really possible and modify them if there are errors.
 * 
 */
void Board::check_and_modify_en_passant_rule()
{
    // En passant square will be considered only if
    // a) side to move have a pawn threatening enPassantSquare
    // b) there is an enemy pawn in front of enPassantSquare
    // c) there is no piece on enPassantSquare or behind enPassantSquare

    bool valid = false;

    Square enPassantSquare = game_state.en_passsant_square();
    if (enPassantSquare.is_valid()) {

        Col col = enPassantSquare.col();
        if (enPassantSquare.row() == ROW_6) {

            if ((get_piece({ROW_5, col - 1}) == Piece::W_PAWN ||
                 get_piece({ROW_5, col + 1}) == Piece::W_PAWN) &&
                get_piece({ROW_5, col}) == Piece::B_PAWN && is_empty(enPassantSquare) &&
                is_empty({ROW_7, col})) {
                valid = true;
            }
        }
        else if (enPassantSquare.row() == ROW_3) {
            if ((get_piece({ROW_4, col - 1}) == Piece::B_PAWN ||
                 get_piece({ROW_4, col + 1}) == Piece::B_PAWN) &&
                get_piece({ROW_5, col}) == Piece::W_PAWN && is_empty(enPassantSquare) &&
                is_empty({ROW_2, col})) {
                valid = true;
            }
        }

        if (!valid) {
            // invalid enPassant row
            game_state.set_en_passsant_square(Square::SQ_INVALID);
        }
    }
}