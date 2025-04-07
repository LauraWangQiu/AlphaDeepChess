/**
 * @file cpp
 * @brief Board implementations.
 *
 */

#include "board.hpp"
#include "zobrist.hpp"
#include "bit_utilities.hpp"

#include <sstream>
#include <stdexcept>

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
    bitboard_color[static_cast<int>(ChessColor::WHITE)] = 0ULL;
    bitboard_color[static_cast<int>(ChessColor::BLACK)] = 0ULL;

    for (int piece = 0; piece < NUM_CHESS_PIECES - 1; piece++) {
        bitboard_piece[piece] = 0ULL;
    }
    bitboard_piece[static_cast<int>(Piece::EMPTY)] = 0xffffffffffffffffULL;

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

    // if en passant was valid update hash
    if (game_state.en_passant_square().is_valid()) {
        const Square eps = game_state.en_passant_square();
        game_state.xor_zobrist(Zobrist::get_en_passant_seed(eps.col()));
    }

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
    game_state.xor_zobrist(Zobrist::get_black_to_move_seed());

    // check if castling is still available
    check_and_modify_castle_rights();

    if (is_move_capture) {
        game_state.set_num_pieces(game_state.num_pieces() - 1);
        const Piece enemy_pawn = create_piece(PieceType::PAWN, opposite_color(get_color(origin_piece)));
        const Piece captured_piece = move.type() != MoveType::EN_PASSANT ? end_piece : enemy_pawn;
        game_state.set_piece_counter(captured_piece, game_state.get_piece_counter(captured_piece) - 1);
    }

    if (move.type() == MoveType::PROMOTION) {
        game_state.set_piece_counter(origin_piece, game_state.get_piece_counter(origin_piece) - 1);
        const Piece promoted_piece = create_piece(move.promotion_piece(), get_color(origin_piece));
        game_state.set_piece_counter(promoted_piece, game_state.get_piece_counter(promoted_piece) + 1);
    }

    game_state.set_attacks_updated(false);

    assert(game_state.num_pieces() == number_of_1_bits(bitboard_all));
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

    assert(assert_that_piece_counter_is_correct());
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

    game_state.set_zobrist_key(Zobrist::hash(*this));

    game_state.set_num_pieces(number_of_1_bits(bitboard_all));
    update_piece_counter();

    game_state.clear_attacks_bb();
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
Board::Board() : game_state(), bitboard_all(0ULL) { clean(); }

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

    const Square origin_sq(normal_move.square_from());
    const Square end_sq(normal_move.square_to());
    const Piece origin_piece = get_piece(origin_sq);
    const Piece end_piece = get_piece(end_sq);

    put_piece(origin_piece, end_sq);
    remove_piece(origin_sq);

    // update zobrist hash with the movement of the pieces
    game_state.xor_zobrist(Zobrist::get_seed(origin_sq, origin_piece) ^ Zobrist::get_seed(end_sq, origin_piece));

    if (end_piece != Piece::EMPTY) {
        game_state.xor_zobrist(Zobrist::get_seed(end_sq, end_piece));
    }

    // captured piece will be Empty if move was not a capture
    game_state.set_last_captured_piece(piece_to_pieceType(end_piece));

    // if move is double push pawn then update the square where enPassant is available
    const bool is_pawn_move = (piece_to_pieceType(origin_piece) == PieceType::PAWN);
    const bool is_move_double_push = is_pawn_move &&
        ((origin_sq.row() == ROW_7 && end_sq.row() == ROW_5) || (origin_sq.row() == ROW_2 && end_sq.row() == ROW_4));

    if (is_move_double_push) {
        const Square eps(origin_sq.row() == ROW_2 ? ROW_3 : ROW_6, end_sq.col());
        game_state.set_en_passant_square(eps);
        check_and_modify_en_passant_rule();
    }
    else {

        game_state.set_en_passant_square(Square::INVALID);
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
    const Piece promo_piece = create_piece(promo_piece_type, promo_piece_color);

    remove_piece(origin_square);
    put_piece(promo_piece, end_square);

    // update zobrist hash with the movement of the pieces
    game_state.xor_zobrist(Zobrist::get_seed(origin_square, moved_piece) ^ Zobrist::get_seed(end_square, promo_piece));

    if (end_piece != Piece::EMPTY) {
        game_state.xor_zobrist(Zobrist::get_seed(end_square, end_piece));
    }

    // captured piece will be Empty if move was not a capture
    game_state.set_last_captured_piece(piece_to_pieceType(end_piece));
    //en Passant will be invalid
    game_state.set_en_passant_square(Square::INVALID);
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
        assert(get_piece(Square::E1) == Piece::W_KING);
        assert(get_piece(Square::H1) == Piece::W_ROOK);
        assert(is_empty(Square::F1) && is_empty(Square::G1));

        // update zobrist hash with the movement of the pieces
        game_state.xor_zobrist(Zobrist::get_seed(Square::E1, Piece::W_KING) ^
                               Zobrist::get_seed(Square::G1, Piece::W_KING));

        game_state.xor_zobrist(Zobrist::get_seed(Square::H1, Piece::W_ROOK) ^
                               Zobrist::get_seed(Square::F1, Piece::W_ROOK));

        put_piece(Piece::W_KING, Square::G1);
        put_piece(Piece::W_ROOK, Square::F1);
        remove_piece(Square::E1);
        remove_piece(Square::H1);
    }
    else if (castling_move == Move::castle_black_king()) {

        assert(game_state.castle_king_black());
        assert(get_piece(Square::E8) == Piece::B_KING);
        assert(get_piece(Square::H8) == Piece::B_ROOK);
        assert(is_empty(Square::F8) && is_empty(Square::G8));

        // update zobrist hash with the movement of the pieces
        game_state.xor_zobrist(Zobrist::get_seed(Square::E8, Piece::B_KING) ^
                               Zobrist::get_seed(Square::G8, Piece::B_KING));
        game_state.xor_zobrist(Zobrist::get_seed(Square::H8, Piece::B_ROOK) ^
                               Zobrist::get_seed(Square::F8, Piece::B_ROOK));

        put_piece(Piece::B_KING, Square::G8);
        put_piece(Piece::B_ROOK, Square::F8);
        remove_piece(Square::E8);
        remove_piece(Square::H8);
    }
    else if (castling_move == Move::castle_white_queen()) {

        assert(game_state.castle_queen_white());
        assert(get_piece(Square::E1) == Piece::W_KING);
        assert(get_piece(Square::A1) == Piece::W_ROOK);
        assert(is_empty(Square::D1) && is_empty(Square::C1) && is_empty(Square::B1));

        // update zobrist hash with the movement of the pieces
        game_state.xor_zobrist(Zobrist::get_seed(Square::E1, Piece::W_KING) ^
                               Zobrist::get_seed(Square::C1, Piece::W_KING));
        game_state.xor_zobrist(Zobrist::get_seed(Square::A1, Piece::W_ROOK) ^
                               Zobrist::get_seed(Square::D1, Piece::W_ROOK));

        put_piece(Piece::W_KING, Square::C1);
        put_piece(Piece::W_ROOK, Square::D1);
        remove_piece(Square::E1);
        remove_piece(Square::A1);
    }
    else if (castling_move == Move::castle_black_queen()) {

        assert(game_state.castle_queen_black());
        assert(get_piece(Square::E8) == Piece::B_KING);
        assert(get_piece(Square::A8) == Piece::B_ROOK);
        assert(is_empty(Square::D8) && is_empty(Square::C8) && is_empty(Square::B8));

        // update zobrist hash with the movement of the pieces
        game_state.xor_zobrist(Zobrist::get_seed(Square::E8, Piece::B_KING) ^
                               Zobrist::get_seed(Square::C8, Piece::B_KING));
        game_state.xor_zobrist(Zobrist::get_seed(Square::A8, Piece::B_ROOK) ^
                               Zobrist::get_seed(Square::D8, Piece::B_ROOK));

        put_piece(Piece::B_KING, Square::C8);
        put_piece(Piece::B_ROOK, Square::D8);
        remove_piece(Square::E8);
        remove_piece(Square::A8);
    }

    game_state.set_last_captured_piece(PieceType::EMPTY);

    //en Passant will be invalid
    game_state.set_en_passant_square(Square::INVALID);
}

void Board::unmake_castling_move(Move castling_move)
{
    assert(castling_move.is_valid());
    assert(castling_move.type() == MoveType::CASTLING);

    if (castling_move == Move::castle_white_king()) {

        assert(get_piece(Square::G1) == Piece::W_KING);
        assert(get_piece(Square::F1) == Piece::W_ROOK);

        put_piece(Piece::W_KING, Square::E1);
        put_piece(Piece::W_ROOK, Square::H1);
        remove_piece(Square::G1);
        remove_piece(Square::F1);
    }
    else if (castling_move == Move::castle_black_king()) {

        assert(get_piece(Square::G8) == Piece::B_KING);
        assert(get_piece(Square::F8) == Piece::B_ROOK);

        put_piece(Piece::B_KING, Square::E8);
        put_piece(Piece::B_ROOK, Square::H8);
        remove_piece(Square::G8);
        remove_piece(Square::F8);
    }
    else if (castling_move == Move::castle_white_queen()) {

        assert(get_piece(Square::C1) == Piece::W_KING);
        assert(get_piece(Square::D1) == Piece::W_ROOK);

        put_piece(Piece::W_KING, Square::E1);
        put_piece(Piece::W_ROOK, Square::A1);
        remove_piece(Square::C1);
        remove_piece(Square::D1);
    }
    else if (castling_move == Move::castle_black_queen()) {

        assert(get_piece(Square::C8) == Piece::B_KING);
        assert(get_piece(Square::D8) == Piece::B_ROOK);

        put_piece(Piece::B_KING, Square::E8);
        put_piece(Piece::B_ROOK, Square::A8);
        remove_piece(Square::C8);
        remove_piece(Square::D8);
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
    const Piece captured_pawn_piece(get_piece(captured_pawn_square));

    assert(piece_to_pieceType(get_piece(origin_square)) == PieceType::PAWN);
    assert(piece_to_pieceType(get_piece(captured_pawn_square)) == PieceType::PAWN);

    // update zobrist hash with the movement of the pieces
    game_state.xor_zobrist(Zobrist::get_seed(origin_square, attacker_pawn_piece) ^
                           Zobrist::get_seed(end_square, attacker_pawn_piece));

    game_state.xor_zobrist(Zobrist::get_seed(captured_pawn_square, captured_pawn_piece));

    put_piece(attacker_pawn_piece, end_square);
    remove_piece(origin_square);
    remove_piece(captured_pawn_square);

    game_state.set_last_captured_piece(PieceType::PAWN);

    //en Passant will be invalid
    game_state.set_en_passant_square(Square::INVALID);
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
    const ChessColor captured_pawn_color = opposite_color(get_color(attacker_pawn_piece));

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

        if (get_piece({ROW_1, COL_E}) != Piece::W_KING || get_piece({ROW_1, COL_H}) != Piece::W_ROOK) {
            game_state.set_castle_king_white(false);
            game_state.xor_zobrist(Zobrist::get_king_white_castle_seed());
        }
    }
    // Check for white queen-side castling rights
    if (game_state.castle_queen_white()) {

        if (get_piece({ROW_1, COL_E}) != Piece::W_KING || get_piece({ROW_1, COL_A}) != Piece::W_ROOK) {
            game_state.set_castle_queen_white(false);
            game_state.xor_zobrist(Zobrist::get_queen_white_castle_seed());
        }
    }

    // Check for black king-side castling rights
    if (game_state.castle_king_black()) {

        if (get_piece({ROW_8, COL_E}) != Piece::B_KING || get_piece({ROW_8, COL_H}) != Piece::B_ROOK) {
            game_state.set_castle_king_black(false);
            game_state.xor_zobrist(Zobrist::get_king_black_castle_seed());
        }
    }

    // Check for black queen-side castling rights
    if (game_state.castle_queen_black()) {

        if (get_piece({ROW_8, COL_E}) != Piece::B_KING || get_piece({ROW_8, COL_A}) != Piece::B_ROOK) {
            game_state.set_castle_queen_black(false);
            game_state.xor_zobrist(Zobrist::get_queen_black_castle_seed());
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
        game_state.set_en_passant_square(Square::INVALID);
        return;
    }

    const Piece capturable_pawn = eps.row() == ROW_6 ? Piece::B_PAWN : Piece::W_PAWN;
    const Piece attacker_pawn = eps.row() == ROW_6 ? Piece::W_PAWN : Piece::B_PAWN;

    // b) There is an enemy pawn in front of enPassantSquare
    if (get_piece(pawn_pushed_sq) != capturable_pawn) {
        game_state.set_en_passant_square(Square::INVALID);
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

    game_state.set_en_passant_square(has_pawn_attacker ? eps : Square::INVALID);

    // update hash with new en passant col
    if (game_state.en_passant_square().is_valid()) {
        game_state.xor_zobrist(Zobrist::get_en_passant_seed(eps.col()));
    }
}

/**
 * @brief recalculates all the attack bitboards for each piece in the position
 */
void Board::update_attacks_bb()
{
    if (game_state.attacks_updated()) {
        return;   // if the attacks are updated dont recalculate
    }

    game_state.clear_attacks_bb();

    uint64_t pieces = get_bitboard_all();

    const uint64_t blockers = pieces;


    while (pieces) {

        const Square square(pop_lsb(pieces));
        const Piece piece = get_piece(square);
        const ChessColor color = get_color(piece);
        const uint64_t attacks = PrecomputedMoveData::pieceMoves(square, piece, blockers);

        set_attacks_bb(piece, get_attacks_bb(piece) | attacks);
        set_attacks_bb(color, get_attacks_bb(color) | attacks);
    }

    game_state.set_attacks_updated(true);
}

/**
 * @brief recalculates all the number of pieces
 */
void Board::update_piece_counter()
{
    for (Piece piece = Piece(0); is_valid_piece(piece); piece = piece + 1) {
        if (piece == Piece::EMPTY) break;
        game_state.set_piece_counter(piece, number_of_1_bits(get_bitboard_piece(piece)));
    }
}

/**
 * @brief check that piece counter is correct (for debug)
 */
bool Board::assert_that_piece_counter_is_correct()
{
    for (Piece piece = Piece(0); is_valid_piece(piece); piece = piece + 1) {
        if (piece == Piece::EMPTY) break;
        if (int(game_state.get_piece_counter(piece)) != number_of_1_bits(get_bitboard_piece(piece))) {
            return false;
        }
    }

    return game_state.num_pieces() == number_of_1_bits(bitboard_all);
}