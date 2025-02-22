/**
 * @file cpp
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
    assert(is_valid_piece(piece));
    assert(square.is_valid());

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
    assert(square.is_valid());

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
 * Remove all pieces on the 
 * 
 */
void Board::clean()
{
    game_state.clean();

    bitboard_all = 0ULL;
    bitboard_black = 0ULL;
    bitboard_white = 0ULL;

    for (int piece = 0; piece < NUM_CHESS_PIECES; piece++) {
        bitboard_piece[piece] = 0ULL;
    }

    for (int square = 0; square < NUM_SQUARES; square++) {
        array_piece[square] = Piece::EMPTY;
    }
}

/**
 * @brief make_move
 * 
 * Make the move in the 
 * 
 * @note If the move is not valid in the position the game will be corrupted.
 * 
 * @param[in] move chess move.
 * 
 */
void Board::make_move(Move move)
{
    assert(move.is_valid());

    const Square origin_square = move.square_from();
    const Square end_square = move.square_to();
    const Piece origin_piece = get_piece(origin_square);
    const Piece end_piece = get_piece(end_square);

    switch (move.type()) {
    case MoveType::NORMAL: make_normal_move(move); break;
    case MoveType::CASTLING: make_castling_move(move); break;
    case MoveType::EN_PASSANT: make_enPassant_move(move); break;
    case MoveType::PROMOTION: make_promotion_move(move); break;
    default: break;
    }


    // increment the move counter
    if (game_state.side_to_move() == ChessColor::BLACK) {
        game_state.set_move_number(game_state.move_number() + 1ULL);
    }

    // increment 50 move rule if the move is not a pawn move or is not a capture move

    const bool is_move_capture = end_piece != Piece::EMPTY || move.type() == MoveType::EN_PASSANT;
    const bool is_pawn_move = piece_to_pieceType(origin_piece) == PieceType::PAWN;
    const uint8_t new_50_rule_move_counter =
        is_move_capture || is_pawn_move ? 0U : game_state.fifty_move_rule_counter() + 1U;
    game_state.set_fifty_move_rule_counter(new_50_rule_move_counter);

    // change side to move
    game_state.set_side_to_move(opposite_color(game_state.side_to_move()));

    // check if castling is still available
    check_and_modify_castle_rights();
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
    assert(move.is_valid());

    switch (move.type()) {
    case MoveType::NORMAL: unmake_normal_move(move); break;
    case MoveType::CASTLING: unmake_castling_move(move); break;
    case MoveType::EN_PASSANT: unmake_enPassant_move(move); break;
    case MoveType::PROMOTION: unmake_promotion_move(move); break;
    default: break;
    }

    game_state = previous_state;
}

/**
 * @brief load_fen
 * 
 * Set the position represented as fen on the chess 
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
        Col col = COL_A;
        while (is_valid_col(col)) {
            emptyCounter = 0;
            while (is_valid_col(col) && is_empty(Square(row, col))) {
                emptyCounter++;
                col++;
            };

            if (emptyCounter) fen << emptyCounter;

            if (is_valid_col(col)) {
                fen << piece_to_char(get_piece(Square(row, col)));
                col++;
            }
        }

        if (row > 0) fen << '/';
    }

    fen << (game_state.side_to_move() == ChessColor::WHITE ? " w " : " b ");

    bool any_castle_available = false;
    if (game_state.castle_king_white()) {
        fen << 'K';
        any_castle_available = true;
    }
    if (game_state.castle_queen_white()) {
        fen << 'Q';
        any_castle_available = true;
    }
    if (game_state.castle_king_black()) {
        fen << 'k';
        any_castle_available = true;
    }
    if (game_state.castle_queen_black()) {
        fen << 'q';
        any_castle_available = true;
    }

    if (!any_castle_available) {
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
Board::Board() : bitboard_all(0ULL), bitboard_white(0ULL), bitboard_black(0ULL), game_state()
{
    clean();
}

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

    for (Row row = ROW_8; is_valid_row(row); --row) {
        for (Col col = COL_A; is_valid_col(col); ++col) {
            os << " | " << piece_to_char(board.get_piece(Square(row, col)));
        }
        os << " | " << static_cast<int>(row) + 1 << "\n +---+---+---+---+---+---+---+---+\n";
    }
    os << "   a   b   c   d   e   f   g   h\n";

    os << "\n\nFen: " << board.fen();
    return os;
}

void Board::make_normal_move(Move normal_move)
{
    assert(normal_move.is_valid());
    assert(normal_move.type() == MoveType::NORMAL);
    assert(!is_empty(normal_move.square_from()));

    const Square origin_square(normal_move.square_from());
    const Square end_square(normal_move.square_to());
    const Piece origin_piece = get_piece(origin_square);
    const Piece end_piece = get_piece(end_square);

    put_piece(origin_piece, end_square);
    remove_piece(origin_square);

    // captured piece will be Empty if move was not a capture
    game_state.set_last_captured_piece(piece_to_pieceType(end_piece));
    //en Passant will be invalid


    // if move is double push pawn then update the square where enPassant is available

    const bool is_pawn_move = (piece_to_pieceType(origin_piece) == PieceType::PAWN);
    const bool is_move_double_push = is_pawn_move &&
        ((origin_square.row() == ROW_7 && end_square.row() == ROW_5) ||
         (origin_square.row() == ROW_2 && end_square.row() == ROW_4));

    if (is_move_double_push) {
        const Square eps(origin_square.row() == ROW_2 ? ROW_3 : ROW_6, end_square.col());
        game_state.set_en_passant_square(eps);
        check_and_modify_en_passant_rule();
    }
    else {
        game_state.set_en_passant_square(Square::SQ_INVALID);
    }
}

void Board::unmake_normal_move(Move normal_move)
{
    assert(normal_move.is_valid());
    assert(normal_move.type() == MoveType::NORMAL);
    assert(!is_empty(normal_move.square_to()));

    const Square origin_square(normal_move.square_from());
    const Square end_square(normal_move.square_to());
    const Piece moved_piece = get_piece(end_square);
    const PieceType captured_piece_type = game_state.last_captured_piece();
    const ChessColor captured_piece_color = opposite_color(get_color(moved_piece));
    const Piece captured_piece = create_piece(captured_piece_type, captured_piece_color);

    put_piece(moved_piece, origin_square);

    // captured piece will be Empty if move was not a capture
    put_piece(captured_piece, end_square);
}

void Board::make_promotion_move(Move promotion_move)
{
    assert(promotion_move.is_valid());
    assert(promotion_move.type() == MoveType::PROMOTION);
    assert(is_valid_pieceType(promotion_move.promotion_piece()));
    assert(!is_empty(promotion_move.square_from()));

    const Square origin_square(promotion_move.square_from());
    const Square end_square(promotion_move.square_to());
    const Piece moved_piece = get_piece(origin_square);
    const Piece end_piece = get_piece(end_square);

    const PieceType promo_piece_type = promotion_move.promotion_piece();
    const ChessColor promo_piece_color = get_color(moved_piece);
    const Piece promotion_piece = create_piece(promo_piece_type, promo_piece_color);

    remove_piece(origin_square);
    put_piece(promotion_piece, end_square);

    // captured piece will be Empty if move was not a capture
    game_state.set_last_captured_piece(piece_to_pieceType(end_piece));
    //en Passant will be invalid
    game_state.set_en_passant_square(Square::SQ_INVALID);
}

void Board::unmake_promotion_move(Move promotion_move)
{
    assert(promotion_move.is_valid());
    assert(promotion_move.type() == MoveType::PROMOTION);
    assert(is_valid_pieceType(promotion_move.promotion_piece()));
    assert(!is_empty(promotion_move.square_to()));
    assert(is_empty(promotion_move.square_from()));

    const Square origin_square(promotion_move.square_from());
    const Square end_square(promotion_move.square_to());
    const ChessColor moved_pawn_color = get_color(get_piece(end_square));
    const PieceType captured_piece_type = game_state.last_captured_piece();
    const ChessColor captured_piece_color = opposite_color(moved_pawn_color);
    const Piece captured_piece = create_piece(captured_piece_type, captured_piece_color);

    put_piece(create_piece(PieceType::PAWN, moved_pawn_color), origin_square);

    // captured piece will be Empty if move was not a capture
    put_piece(captured_piece, end_square);
}

void Board::make_castling_move(Move castling_move)
{
    assert(castling_move.is_valid());
    assert(castling_move.type() == MoveType::CASTLING);

    if (castling_move == Move::castle_white_king()) {

        assert(game_state.castle_king_white());
        assert(get_piece(Square::SQ_E1) == Piece::W_KING);
        assert(get_piece(Square::SQ_H1) == Piece::W_ROOK);
        assert(is_empty(Square::SQ_F1) && is_empty(Square::SQ_G1));

        put_piece(Piece::W_KING, Square::SQ_G1);
        put_piece(Piece::W_ROOK, Square::SQ_F1);
        remove_piece(Square::SQ_E1);
        remove_piece(Square::SQ_H1);

        game_state.set_castle_king_white(false);
        game_state.set_castle_queen_white(false);
        game_state.set_castled_white(true);
    }
    else if (castling_move == Move::castle_black_king()) {

        assert(game_state.castle_king_black());
        assert(get_piece(Square::SQ_E8) == Piece::B_KING);
        assert(get_piece(Square::SQ_H8) == Piece::B_ROOK);
        assert(is_empty(Square::SQ_F8) && is_empty(Square::SQ_G8));

        put_piece(Piece::B_KING, Square::SQ_G8);
        put_piece(Piece::B_ROOK, Square::SQ_F8);
        remove_piece(Square::SQ_E8);
        remove_piece(Square::SQ_H8);

        game_state.set_castle_king_black(false);
        game_state.set_castle_queen_black(false);
        game_state.set_castled_black(true);
    }
    else if (castling_move == Move::castle_white_queen()) {

        assert(game_state.castle_queen_white());
        assert(get_piece(Square::SQ_E1) == Piece::W_KING);
        assert(get_piece(Square::SQ_A1) == Piece::W_ROOK);
        assert(is_empty(Square::SQ_D1) && is_empty(Square::SQ_C1) && is_empty(Square::SQ_B1));

        put_piece(Piece::W_KING, Square::SQ_C1);
        put_piece(Piece::W_ROOK, Square::SQ_D1);
        remove_piece(Square::SQ_E1);
        remove_piece(Square::SQ_A1);

        game_state.set_castle_queen_white(false);
        game_state.set_castle_king_white(false);
        game_state.set_castled_white(true);
    }
    else if (castling_move == Move::castle_black_queen()) {

        assert(game_state.castle_queen_black());
        assert(get_piece(Square::SQ_E8) == Piece::B_KING);
        assert(get_piece(Square::SQ_A8) == Piece::B_ROOK);
        assert(is_empty(Square::SQ_D8) && is_empty(Square::SQ_C8) && is_empty(Square::SQ_B8));

        put_piece(Piece::B_KING, Square::SQ_C8);
        put_piece(Piece::B_ROOK, Square::SQ_D8);
        remove_piece(Square::SQ_E8);
        remove_piece(Square::SQ_A8);

        game_state.set_castle_queen_black(false);
        game_state.set_castle_king_black(false);
        game_state.set_castled_black(true);
    }

    game_state.set_last_captured_piece(PieceType::EMPTY);

    //en Passant will be invalid
    game_state.set_en_passant_square(Square::SQ_INVALID);
}

void Board::unmake_castling_move(Move castling_move)
{
    assert(castling_move.is_valid());
    assert(castling_move.type() == MoveType::CASTLING);

    if (castling_move == Move::castle_white_king()) {

        assert(get_piece(Square::SQ_G1) == Piece::W_KING);
        assert(get_piece(Square::SQ_F1) == Piece::W_ROOK);

        put_piece(Piece::W_KING, Square::SQ_E1);
        put_piece(Piece::W_ROOK, Square::SQ_H1);
        remove_piece(Square::SQ_G1);
        remove_piece(Square::SQ_F1);
    }
    else if (castling_move == Move::castle_black_king()) {

        assert(get_piece(Square::SQ_G8) == Piece::B_KING);
        assert(get_piece(Square::SQ_F8) == Piece::B_ROOK);

        put_piece(Piece::B_KING, Square::SQ_E8);
        put_piece(Piece::B_ROOK, Square::SQ_H8);
        remove_piece(Square::SQ_G8);
        remove_piece(Square::SQ_F8);
    }
    else if (castling_move == Move::castle_white_queen()) {

        assert(get_piece(Square::SQ_C1) == Piece::W_KING);
        assert(get_piece(Square::SQ_D1) == Piece::W_ROOK);

        put_piece(Piece::W_KING, Square::SQ_E1);
        put_piece(Piece::W_ROOK, Square::SQ_A1);
        remove_piece(Square::SQ_C1);
        remove_piece(Square::SQ_D1);
    }
    else if (castling_move == Move::castle_black_queen()) {

        assert(get_piece(Square::SQ_C8) == Piece::B_KING);
        assert(get_piece(Square::SQ_D8) == Piece::B_ROOK);

        put_piece(Piece::B_KING, Square::SQ_E8);
        put_piece(Piece::B_ROOK, Square::SQ_A8);
        remove_piece(Square::SQ_C8);
        remove_piece(Square::SQ_D8);
    }
}

void Board::make_enPassant_move(Move enPassant_move)
{
    assert(enPassant_move.is_valid());
    assert(enPassant_move.type() == MoveType::EN_PASSANT);
    assert(game_state.en_passant_square().is_valid());
    assert(game_state.en_passant_square() == enPassant_move.square_to());
    assert(is_empty(enPassant_move.square_to()));

    const Square origin_square(enPassant_move.square_from());
    const Square end_square(enPassant_move.square_to());
    const Square captured_pawn_square(origin_square.row(), end_square.col());
    const Piece attacker_pawn_piece(get_piece(origin_square));

    assert(piece_to_pieceType(get_piece(origin_square)) == PieceType::PAWN);
    assert(piece_to_pieceType(get_piece(captured_pawn_square)) == PieceType::PAWN);


    put_piece(attacker_pawn_piece, end_square);
    remove_piece(origin_square);
    remove_piece(captured_pawn_square);

    game_state.set_last_captured_piece(PieceType::PAWN);

    //en Passant will be invalid
    game_state.set_en_passant_square(Square::SQ_INVALID);
}

void Board::unmake_enPassant_move(Move enPassant_move)
{
    assert(enPassant_move.is_valid());
    assert(enPassant_move.type() == MoveType::EN_PASSANT);
    assert(piece_to_pieceType(get_piece(enPassant_move.square_to())) == PieceType::PAWN);

    const Square origin_square(enPassant_move.square_from());
    const Square end_square(enPassant_move.square_to());
    const Square captured_pawn_square(origin_square.row(), end_square.col());

    const Piece attacker_pawn_piece(get_piece(end_square));
    const ChessColor captured_pawn_color =
        get_color(attacker_pawn_piece) == ChessColor::WHITE ? ChessColor::BLACK : ChessColor::WHITE;

    const Piece captured_pawn_piece = create_piece(PieceType::PAWN, captured_pawn_color);


    put_piece(attacker_pawn_piece, origin_square);
    put_piece(captured_pawn_piece, captured_pawn_square);
    remove_piece(end_square);
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

        if (get_piece({ROW_1, COL_E}) != Piece::W_KING ||
            get_piece({ROW_1, COL_H}) != Piece::W_ROOK) {
            game_state.set_castle_king_white(false);
        }
    }
    // Check for white queen-side castling rights
    if (game_state.castle_queen_white()) {

        if (get_piece({ROW_1, COL_E}) != Piece::W_KING ||
            get_piece({ROW_1, COL_A}) != Piece::W_ROOK) {
            game_state.set_castle_queen_white(false);
        }
    }

    // Check for black king-side castling rights
    if (game_state.castle_king_black()) {

        if (get_piece({ROW_8, COL_E}) != Piece::B_KING ||
            get_piece({ROW_8, COL_H}) != Piece::B_ROOK) {
            game_state.set_castle_king_black(false);
        }
    }

    // Check for black queen-side castling rights
    if (game_state.castle_queen_black()) {

        if (get_piece({ROW_8, COL_E}) != Piece::B_KING ||
            get_piece({ROW_8, COL_A}) != Piece::B_ROOK) {
            game_state.set_castle_queen_black(false);
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
    // En passant square will be considered only if:
    // a) Side to move has a pawn threatening enPassantSquare
    // b) There is an enemy pawn in front of enPassantSquare
    // c) There is no piece on enPassantSquare or behind it

    // Get the en passant square (EPS) set by the previous move.
    const Square eps = game_state.en_passant_square();
    if (!eps.is_valid()) {
        return;
    }

    const Square pawn_pushed_sq(eps.row() == ROW_6 ? ROW_5 : ROW_4, eps.col());
    const Square pawn_pushed_origin_sq(eps.row() == ROW_6 ? ROW_7 : ROW_2, eps.col());
    const Square pawn_attacker1_sq = pawn_pushed_sq.east();
    const Square pawn_attacker2_sq = pawn_pushed_sq.west();

    // c) There is no piece on enPassantSquare or behind it
    if (!is_empty(eps) || !is_empty(pawn_pushed_origin_sq)) {
        game_state.set_en_passant_square(Square::SQ_INVALID);
        return;
    }

    const Piece capturable_pawn = eps.row() == ROW_6 ? Piece::B_PAWN : Piece::W_PAWN;
    const Piece attacker_pawn = eps.row() == ROW_6 ? Piece::W_PAWN : Piece::B_PAWN;

    // b) There is an enemy pawn in front of enPassantSquare
    if (get_piece(pawn_pushed_sq) != capturable_pawn) {
        game_state.set_en_passant_square(Square::SQ_INVALID);
        return;
    }

    // a) Side to move has a pawn threatening enPassantSquar
    bool has_pawn_attacker = false;
    if (pawn_attacker1_sq.is_valid() && get_piece(pawn_attacker1_sq) == attacker_pawn) {
        has_pawn_attacker = true;
    }
    if (pawn_attacker2_sq.is_valid() && get_piece(pawn_attacker2_sq) == attacker_pawn) {
        has_pawn_attacker = true;
    }

    game_state.set_en_passant_square(has_pawn_attacker ? eps : Square::SQ_INVALID);
}