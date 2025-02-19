#include "test_utils.hpp"

#include "piece.hpp"

static void piece_to_char_test();
static void char_to_piece_test();
static void char_to_pieceType_test();
static void pieceType_to_char_test();
static void get_color_test();
static void piece_to_pieceType_test();
static void create_piece_test();
static void raw_value_test();
static void is_slider_test();
static void piece_sum_test();
static void piece_sub_test();
static void pieceType_sum_test();
static void pieceType_sub_test();
static void is_valid_piece_test();
static void is_valid_pieceType_test();
static void is_valid_color_test();
static void opposite_color_test();

void piece_test()
{

    std::cout << "---------piece test---------\n\n";

    piece_to_char_test();
    char_to_piece_test();
    char_to_pieceType_test();
    pieceType_to_char_test();
    get_color_test();
    piece_to_pieceType_test();
    create_piece_test();
    raw_value_test();
    is_slider_test();
    piece_sum_test();
    piece_sub_test();
    pieceType_sum_test();
    pieceType_sub_test();
    is_valid_piece_test();
    is_valid_pieceType_test();
    is_valid_color_test();
    opposite_color_test();
}

static void piece_to_char_test()
{
    const std::string test_name = "piece_to_char_test";

    if (piece_to_char(Piece::W_PAWN) != 'P') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_PAWN) != 'P'");
    }
    if (piece_to_char(Piece::B_PAWN) != 'p') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_PAWN) != 'p'");
    }
    if (piece_to_char(Piece::W_KNIGHT) != 'N') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_KNIGHT) != 'N'");
    }
    if (piece_to_char(Piece::B_KNIGHT) != 'n') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_KNIGHT) != 'n'");
    }
    if (piece_to_char(Piece::W_BISHOP) != 'B') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_BISHOP) != 'B'");
    }
    if (piece_to_char(Piece::B_BISHOP) != 'b') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_BISHOP) != 'b'");
    }
    if (piece_to_char(Piece::W_QUEEN) != 'Q') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_QUEEN) != 'Q'");
    }
    if (piece_to_char(Piece::B_QUEEN) != 'q') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_QUEEN) != 'q'");
    }
    if (piece_to_char(Piece::W_ROOK) != 'R') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_ROOK) != 'R'");
    }
    if (piece_to_char(Piece::B_ROOK) != 'r') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_ROOK) != 'r'");
    }
    if (piece_to_char(Piece::W_KING) != 'K') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::W_KING) != 'K'");
    }
    if (piece_to_char(Piece::B_KING) != 'k') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::B_KING) != 'k'");
    }
    if (piece_to_char(Piece::EMPTY) != ' ') {
        PRINT_TEST_FAILED(test_name, "piece_to_char(Piece::EMPTY) != ' '");
    }
}

static void char_to_piece_test()
{
    const std::string test_name = "char_to_piece_test";

    if (char_to_piece('P') != Piece::W_PAWN) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('P') != Piece::W_PAWN");
    }
    if (char_to_piece('p') != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('p') != Piece::B_PAWN");
    }
    if (char_to_piece('N') != Piece::W_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('N') != Piece::W_KNIGHT");
    }
    if (char_to_piece('n') != Piece::B_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('n') != Piece::B_KNIGHT");
    }
    if (char_to_piece('B') != Piece::W_BISHOP) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('B') != Piece::W_BISHOP");
    }
    if (char_to_piece('b') != Piece::B_BISHOP) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('b') != Piece::B_BISHOP");
    }
    if (char_to_piece('R') != Piece::W_ROOK) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('R') != Piece::W_ROOK");
    }
    if (char_to_piece('r') != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('r') != Piece::B_ROOK");
    }
    if (char_to_piece('Q') != Piece::W_QUEEN) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('Q') != Piece::W_QUEEN");
    }
    if (char_to_piece('q') != Piece::B_QUEEN) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('q') != Piece::B_QUEEN");
    }
    if (char_to_piece('K') != Piece::W_KING) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('K') != Piece::W_KING");
    }
    if (char_to_piece('k') != Piece::B_KING) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('k') != Piece::B_KING");
    }
    if (char_to_piece(' ') != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "char_to_piece(' ') != Piece::EMPTY");
    }
    if (char_to_piece('x') != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "char_to_piece('x') != Piece::EMPTY");
    }
}

static void char_to_pieceType_test()
{
    const std::string test_name = "char_to_pieceType_test";

    if (char_to_pieceType('P') != PieceType::PAWN) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('P') != PieceType::PAWN");
    }
    if (char_to_pieceType('p') != PieceType::PAWN) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('p') != PieceType::PAWN");
    }
    if (char_to_pieceType('N') != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('N') != PieceType::KNIGHT");
    }
    if (char_to_pieceType('n') != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('n') != PieceType::KNIGHT");
    }
    if (char_to_pieceType('B') != PieceType::BISHOP) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('B') != PieceType::BISHOP");
    }
    if (char_to_pieceType('b') != PieceType::BISHOP) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('b') != PieceType::BISHOP");
    }
    if (char_to_pieceType('R') != PieceType::ROOK) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('R') != PieceType::ROOK");
    }
    if (char_to_pieceType('r') != PieceType::ROOK) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('r') != PieceType::ROOK");
    }
    if (char_to_pieceType('Q') != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('Q') != PieceType::QUEEN");
    }
    if (char_to_pieceType('q') != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('q') != PieceType::QUEEN");
    }
    if (char_to_pieceType('K') != PieceType::KING) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('K') != PieceType::KING");
    }
    if (char_to_pieceType('k') != PieceType::KING) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('k') != PieceType::KING");
    }
    if (char_to_pieceType(' ') != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType(' ') != PieceType::EMPTY");
    }
    if (char_to_pieceType('x') != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "char_to_pieceType('x') != PieceType::EMPTY");
    }
}

static void pieceType_to_char_test()
{
    const std::string test_name = "pieceType_to_char_test";

    if (pieceType_to_char(PieceType::KNIGHT) != 'n') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::KNIGHT) != 'n'");
    }
    if (pieceType_to_char(PieceType::PAWN) != 'p') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::PAWN) != 'p'");
    }
    if (pieceType_to_char(PieceType::BISHOP) != 'b') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::BISHOP) != 'b'");
    }
    if (pieceType_to_char(PieceType::ROOK) != 'r') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::ROOK) != 'r'");
    }
    if (pieceType_to_char(PieceType::QUEEN) != 'q') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::QUEEN) != 'q'");
    }
    if (pieceType_to_char(PieceType::KING) != 'k') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::KING) != 'k'");
    }
    if (pieceType_to_char(PieceType::EMPTY) != ' ') {
        PRINT_TEST_FAILED(test_name, "pieceType_to_char(PieceType::EMPTY) != ' '");
    }
}

static void get_color_test()
{
    const std::string test_name = "get_color_test";

    if (get_color(Piece::W_ROOK) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_ROOK) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_ROOK) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_ROOK) != ChessColor::BLACK");
    }
    if (get_color(Piece::W_PAWN) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_PAWN) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_PAWN) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_PAWN) != ChessColor::BLACK");
    }
    if (get_color(Piece::W_KNIGHT) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_KNIGHT) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_KNIGHT) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_KNIGHT) != ChessColor::BLACK");
    }
    if (get_color(Piece::W_BISHOP) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_BISHOP) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_BISHOP) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_BISHOP) != ChessColor::BLACK");
    }
    if (get_color(Piece::W_QUEEN) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_QUEEN) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_QUEEN) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_QUEEN) != ChessColor::BLACK");
    }
    if (get_color(Piece::W_KING) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::W_KING) != ChessColor::WHITE");
    }
    if (get_color(Piece::B_KING) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "get_color(Piece::B_KING) != ChessColor::BLACK");
    }
}

static void piece_to_pieceType_test()
{
    const std::string test_name = "piece_to_pieceType_test";

    if (piece_to_pieceType(Piece::W_KING) != PieceType::KING) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_KING) != PieceType::KING");
    }
    if (piece_to_pieceType(Piece::B_KING) != PieceType::KING) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_KING) != PieceType::KING");
    }
    if (piece_to_pieceType(Piece::W_QUEEN) != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_QUEEN) != PieceType::QUEEN");
    }
    if (piece_to_pieceType(Piece::B_QUEEN) != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_QUEEN) != PieceType::QUEEN");
    }
    if (piece_to_pieceType(Piece::W_PAWN) != PieceType::PAWN) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_PAWN) != PieceType::PAWN");
    }
    if (piece_to_pieceType(Piece::B_PAWN) != PieceType::PAWN) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_PAWN) != PieceType::PAWN");
    }
    if (piece_to_pieceType(Piece::W_KNIGHT) != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_KNIGHT) != PieceType::KNIGHT");
    }
    if (piece_to_pieceType(Piece::B_KNIGHT) != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_KNIGHT) != PieceType::KNIGHT");
    }
    if (piece_to_pieceType(Piece::W_BISHOP) != PieceType::BISHOP) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_BISHOP) != PieceType::BISHOP");
    }
    if (piece_to_pieceType(Piece::B_BISHOP) != PieceType::BISHOP) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_BISHOP) != PieceType::BISHOP");
    }
    if (piece_to_pieceType(Piece::W_ROOK) != PieceType::ROOK) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::W_ROOK) != PieceType::ROOK");
    }
    if (piece_to_pieceType(Piece::B_ROOK) != PieceType::ROOK) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::B_ROOK) != PieceType::ROOK");
    }
    if (piece_to_pieceType(Piece::EMPTY) != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "piece_to_pieceType(Piece::EMPTY) != PieceType::EMPTY");
    }
}

static void create_piece_test()
{
    const std::string test_name = "create_piece_test";

    if (create_piece(PieceType::KNIGHT, ChessColor::WHITE) != Piece::W_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "create_piece(KNIGHT, WHITE) != Piece::W_KNIGHT");
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::BLACK) != Piece::B_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "create_piece(KNIGHT, BLACK) != Piece::B_KNIGHT");
    }
    if (create_piece(PieceType::PAWN, ChessColor::WHITE) != Piece::W_PAWN) {
        PRINT_TEST_FAILED(test_name, "create_piece(PAWN, WHITE) != Piece::W_PAWN");
    }
    if (create_piece(PieceType::PAWN, ChessColor::BLACK) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "create_piece(PAWN, BLACK) != Piece::B_PAWN");
    }
    if (create_piece(PieceType::BISHOP, ChessColor::WHITE) != Piece::W_BISHOP) {
        PRINT_TEST_FAILED(test_name, "create_piece(BISHOP, WHITE) != Piece::W_BISHOP");
    }
    if (create_piece(PieceType::BISHOP, ChessColor::BLACK) != Piece::B_BISHOP) {
        PRINT_TEST_FAILED(test_name, "create_piece(BISHOP, BLACK) != Piece::B_BISHOP");
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::WHITE) != Piece::W_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "create_piece(KNIGHT, WHITE) != Piece::W_KNIGHT");
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::BLACK) != Piece::B_KNIGHT) {
        PRINT_TEST_FAILED(test_name, "create_piece(KNIGHT, BLACK) != Piece::B_KNIGHT");
    }
    if (create_piece(PieceType::ROOK, ChessColor::WHITE) != Piece::W_ROOK) {
        PRINT_TEST_FAILED(test_name, "create_piece(ROOK, WHITE) != Piece::W_ROOK");
    }
    if (create_piece(PieceType::ROOK, ChessColor::BLACK) != Piece::B_ROOK) {
        PRINT_TEST_FAILED(test_name, "create_piece(ROOK, BLACK) != Piece::B_ROOK");
    }
    if (create_piece(PieceType::QUEEN, ChessColor::WHITE) != Piece::W_QUEEN) {
        PRINT_TEST_FAILED(test_name, "create_piece(QUEEN, WHITE) != Piece::W_QUEEN");
    }
    if (create_piece(PieceType::QUEEN, ChessColor::BLACK) != Piece::B_QUEEN) {
        PRINT_TEST_FAILED(test_name, "create_piece(QUEEN, BLACK) != Piece::B_QUEEN");
    }
    if (create_piece(PieceType::KING, ChessColor::WHITE) != Piece::W_KING) {
        PRINT_TEST_FAILED(test_name, "create_piece(KING, WHITE) != Piece::W_KING");
    }
    if (create_piece(PieceType::KING, ChessColor::BLACK) != Piece::B_KING) {
        PRINT_TEST_FAILED(test_name, "create_piece(KING, BLACK) != Piece::B_KING");
    }
    if (create_piece(PieceType::EMPTY, ChessColor::WHITE) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "create_piece(EMPTY, WHITE) != Piece::EMPTY");
    }
    if (create_piece(PieceType::EMPTY, ChessColor::BLACK) != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "create_piece(EMPTY, BLACK) != Piece::EMPTY");
    }
}

static void raw_value_test()
{
    const std::string test_name = "raw_value_test";

    if (raw_value(PieceType::PAWN) != 100) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::PAWN) != 100");
    }
    if (raw_value(PieceType::KNIGHT) != 300) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::KNIGHT) != 300");
    }
    if (raw_value(PieceType::BISHOP) != 310) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::BISHOP) != 310");
    }
    if (raw_value(PieceType::ROOK) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::ROOK) != 500");
    }
    if (raw_value(PieceType::QUEEN) != 900) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::QUEEN) != 900");
    }
    if (raw_value(PieceType::KING) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::KING) != 500");
    }
    if (raw_value(PieceType::EMPTY) != 0) {
        PRINT_TEST_FAILED(test_name, "raw_value(PieceType::EMPTY) != 0");
    }

    if (raw_value(Piece::W_PAWN) != 100) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_PAWN) != 100");
    }
    if (raw_value(Piece::B_PAWN) != 100) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_PAWN) != 100");
    }
    if (raw_value(Piece::W_KNIGHT) != 300) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_KNIGHT) != 300");
    }
    if (raw_value(Piece::B_KNIGHT) != 300) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_KNIGHT) != 300");
    }
    if (raw_value(Piece::W_BISHOP) != 310) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_BISHOP) != 310");
    }
    if (raw_value(Piece::B_BISHOP) != 310) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_BISHOP) != 310");
    }
    if (raw_value(Piece::W_ROOK) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_ROOK) != 500");
    }
    if (raw_value(Piece::B_ROOK) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_ROOK) != 500");
    }
    if (raw_value(Piece::W_QUEEN) != 900) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_QUEEN) != 900");
    }
    if (raw_value(Piece::B_QUEEN) != 900) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_QUEEN) != 900");
    }
    if (raw_value(Piece::W_KING) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::W_KING) != 500");
    }
    if (raw_value(Piece::B_KING) != 500) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::B_KING) != 500");
    }
    if (raw_value(Piece::EMPTY) != 0) {
        PRINT_TEST_FAILED(test_name, "raw_value(Piece::EMPTY) != 0");
    }
}

static void is_slider_test()
{
    const std::string test_name = "is_slider_test";

    if (is_slider(PieceType::BISHOP) != true) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::BISHOP) != true");
    }
    if (is_slider(PieceType::QUEEN) != true) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::QUEEN) != true");
    }
    if (is_slider(PieceType::ROOK) != true) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::ROOK) != true");
    }
    if (is_slider(PieceType::PAWN) != false) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::PAWN) != false");
    }
    if (is_slider(PieceType::KNIGHT) != false) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::KNIGHT) != false");
    }
    if (is_slider(PieceType::KING) != false) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::KING) != false");
    }
    if (is_slider(PieceType::EMPTY) != false) {
        PRINT_TEST_FAILED(test_name, "is_slider(PieceType::EMPTY) != false");
    }
}

static void piece_sum_test()
{
    const std::string test_name = "piece_sum_test";

    if ((Piece)(Piece::W_PAWN + 6) != Piece::B_PAWN) {
        PRINT_TEST_FAILED(test_name, "(Piece)(Piece::W_PAWN + 6) != Piece::B_PAWN");
    }
    if (Piece::EMPTY + 6 != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "Piece::EMPTY + 6 != Piece::EMPTY");
    }
}

static void piece_sub_test()
{
    const std::string test_name = "piece_sub_test";

    if (Piece::B_PAWN - 6 != Piece::W_PAWN) {
        PRINT_TEST_FAILED(test_name, "Piece::B_PAWN - 6 != Piece::W_PAWN");
    }
    if (Piece::W_PAWN - 6 != Piece::EMPTY) {
        PRINT_TEST_FAILED(test_name, "Piece::W_PAWN - 6 != Piece::EMPTY");
    }
}

static void pieceType_sum_test()
{
    const std::string test_name = "pieceType_sum_test";

    if (PieceType::PAWN + 1 != PieceType::KNIGHT) {
        PRINT_TEST_FAILED(test_name, "PieceType::PAWN + 1 != PieceType::KNIGHT");
    }
    if (PieceType::EMPTY + 1 != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "PieceType::EMPTY + 1 != PieceType::EMPTY");
    }
}

static void pieceType_sub_test()
{
    const std::string test_name = "pieceType_sub_test";

    if (PieceType::KNIGHT - 1 != PieceType::PAWN) {
        PRINT_TEST_FAILED(test_name, "PieceType::KNIGHT - 1 != PieceType::PAWN");
    }
    if (PieceType::PAWN - 1 != PieceType::EMPTY) {

        PRINT_TEST_FAILED(test_name, "PieceType::PAWN - 1 != PieceType::EMPTY");
    }
}

static void is_valid_piece_test()
{
    const std::string test_name = "is_valid_piece_test";

    if (is_valid_piece(Piece::NUM_PIECES)) {
        PRINT_TEST_FAILED(test_name, "is_valid_piece(Piece::NUM_PIECES)");
    }
    if (is_valid_piece(Piece(-1))) {
        PRINT_TEST_FAILED(test_name, "is_valid_piece(Piece(-1))");
    }
    if (!is_valid_piece(Piece::W_PAWN)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_piece(Piece::W_PAWN)");
    }
    if (!is_valid_piece(Piece::EMPTY)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_piece(Piece::EMPTY)");
    }
}

static void is_valid_pieceType_test()
{
    const std::string test_name = "is_valid_pieceType_test";


    if (is_valid_pieceType(PieceType(7))) {
        PRINT_TEST_FAILED(test_name, "is_valid_pieceType(PieceType(7))");
    }
    if (is_valid_pieceType(PieceType(-1))) {
        PRINT_TEST_FAILED(test_name, "is_valid_pieceType(PieceType(-1))");
    }
    if (!is_valid_pieceType(PieceType::PAWN)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_pieceType(PieceType::PAWN)");
    }
    if (!is_valid_pieceType(PieceType::EMPTY)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_pieceType(PieceType::EMPTY)");
    }
}

static void is_valid_color_test()
{
    const std::string test_name = "is_valid_color_test";

    if (is_valid_color(ChessColor(-1))) {
        PRINT_TEST_FAILED(test_name, "is_valid_color(ChessColor(-1))");
    }
    if (is_valid_color(ChessColor(2))) {
        PRINT_TEST_FAILED(test_name, "is_valid_color(ChessColor(2))");
    }
    if (!is_valid_color(ChessColor::WHITE)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_color(ChessColor::WHITE)");
    }
    if (!is_valid_color(ChessColor::BLACK)) {
        PRINT_TEST_FAILED(test_name, "!is_valid_color(ChessColor::BLACK)");
    }
}

static void opposite_color_test()
{
    const std::string test_name = "opposite_color_test";

    if (opposite_color(ChessColor::WHITE) != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "opposite_color(ChessColor::WHITE) != ChessColor::BLACK");
    }

    if (opposite_color(ChessColor::BLACK) != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "opposite_color(ChessColor::BLACK) != ChessColor::WHITE");
    }
}