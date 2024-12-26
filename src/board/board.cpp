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
    game_state.clean();

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
        const Col rook_end_col = (end_square.col() == COL_G) ? COL_F : COL_D;

        const Square rook_origin_square(origin_square.row(), rook_origin_col);
        const Square rook_end_square(origin_square.row(), rook_end_col);

        put_piece(origin_piece, end_square);
        put_piece(get_piece(rook_origin_square), rook_end_square);
        remove_piece(origin_square);
        remove_piece(rook_origin_square);
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

    // update castling rights

    if (origin_piece == Piece::W_KING) {
        game_state.set_castle_king_white(false);
        game_state.set_castle_queen_white(false);
    }
    else if (origin_piece == Piece::B_KING) {
        game_state.set_castle_king_black(false);
        game_state.set_castle_queen_black(false);
    }
    else if (origin_piece == Piece::W_ROOK) {
        if (origin_square == Square::SQ_A1) {
            game_state.set_castle_queen_white(false);
        }
        else if (origin_square == Square::SQ_H1) {
            game_state.set_castle_king_white(false);
        }
    }
    else if (origin_piece == Piece::B_ROOK) {
        if (origin_square == Square::SQ_A8) {
            game_state.set_castle_queen_black(false);
        }
        else if (origin_square == Square::SQ_H8) {
            game_state.set_castle_king_black(false);
        }
    }
    // if move is not a capture last capture piece will be Piece::Empty.
    if (move_type != MoveType::EN_PASSANT) {
        game_state.set_last_captured_piece(piece_to_pieceType(end_piece));
    }
    else {
        game_state.set_last_captured_piece(PieceType::PAWN);
    }

    // if move is double push pawn then update the square where enPassant is avaliable

    const bool is_pawn_move = (piece_to_pieceType(origin_piece) == PieceType::PAWN);
    const uint32_t row_diff = abs((int32_t)origin_square.row() - (int32_t)end_square.row());
    const bool is_move_double_push = is_pawn_move && row_diff == 2U;

    if (is_move_double_push) {
        const Piece left_piece =
            end_square.west().is_valid() ? get_piece(end_square.west()) : Piece::EMPTY;
        const Piece right_piece =
            end_square.east().is_valid() ? get_piece(end_square.east()) : Piece::EMPTY;
        const Piece enemy_pawn =
            get_color(origin_piece) == ChessColor::WHITE ? Piece::B_PAWN : Piece::W_PAWN;

        // check if en passant is really possible in the position
        if (left_piece == enemy_pawn || right_piece == enemy_pawn) {
            const Row en_passant_row = get_color(origin_piece) == ChessColor::WHITE ? ROW_5 : ROW_4;
            game_state.set_en_passant_square(Square(en_passant_row, end_square.col()));
        }
        else {
            game_state.set_en_passant_square(Square::SQ_INVALID);
        }
    }
    else {
        game_state.set_en_passant_square(Square::SQ_INVALID);
    }

    // increment the move counter
    const uint64_t next_move_number = game_state.side_to_move() == ChessColor::BLACK
        ? game_state.move_number() + 1ULL
        : game_state.move_number();

    game_state.set_move_number(next_move_number);

    // increment 50 move rule if the move is not a pawn move or is not a capture move

    const bool is_move_capture = end_piece != Piece::EMPTY || move.type() == MoveType::EN_PASSANT;

    const uint32_t new_50_rule_move_counter =
        is_move_capture || piece_to_pieceType(origin_piece) == PieceType::PAWN
        ? 0UL
        : game_state.fifty_move_rule_counter() + 1UL;

    game_state.set_fifty_move_rule_counter(new_50_rule_move_counter);

    // change side to move
    const ChessColor next_side_to_move =
        game_state.side_to_move() == ChessColor::WHITE ? ChessColor::BLACK : ChessColor::WHITE;

    game_state.set_side_to_move(next_side_to_move);
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
        const Col rook_end_col = (end_square.col() == COL_G) ? COL_F : COL_D;

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
    game_state.set_castle_king_white(false);
    game_state.set_castle_queen_white(false);
    game_state.set_castle_king_black(false);
    game_state.set_castle_queen_black(false);

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

        game_state.set_en_passant_square(Square(row, col));
        check_and_modify_en_passant_rule();
    }

    // 5-6. fifty move rule counter and fullmove number

    uint64_t moveNumber = 0ULL;
    uint32_t fifty_move_rule_counter = 0UL;

    ss >> std::skipws >> fifty_move_rule_counter >> moveNumber;

    game_state.set_fifty_move_rule_counter(fifty_move_rule_counter);
    game_state.set_move_number(moveNumber);
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

    for (Row row = ROW_8; is_valid_row(row); row--) {
        for (Col col = COL_A; col <= COL_H; col++) {
            emptyCounter = 0;
            while (col <= COL_H && is_empty(Square(row, col))) {
                emptyCounter++;
                col++;
            };

            if (emptyCounter) fen << emptyCounter;

            if (col <= 7) fen << (piece_to_char(get_piece(Square(row, col))));
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
    if (game_state.castle_queen_black()) {
        fen << 'q';
        any_castle_avaliable = true;
    }

    if (!any_castle_avaliable) {
        fen << '-';
    }

    if (game_state.en_passant_square().is_valid()) {
        fen << " " + game_state.en_passant_square().to_string() + " ";
    }
    else {
        fen << " - ";
    }

    const uint32_t fifty_move_rule_counter = game_state.fifty_move_rule_counter();
    const uint32_t move_number = game_state.move_number();

    fen << fifty_move_rule_counter << " " << move_number;

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

    for (Row row = ROW_8; is_valid_row(row); row--) {
        for (Col col = COL_A; col<=COL_H; col++) {
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

    Square enPassantSquare = game_state.en_passant_square();
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
            game_state.set_en_passant_square(Square::SQ_INVALID);
        }
    }
}