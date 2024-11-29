#include <iostream>

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
}

static void piece_to_char_test()
{
    std::cout << "piece_to_char test :\n\n";


    if (piece_to_char(Piece::W_PAWN) != 'P') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_PAWN) != 'P'\n";
    }
    if (piece_to_char(Piece::B_PAWN) != 'p') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::B_PAWN) != 'p'\n";
    }
    if (piece_to_char(Piece::W_KNIGHT) != 'N') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_KNIGHT) != 'N'\n";
    }
    if (piece_to_char(Piece::B_KNIGHT) != 'n') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::B_KNIGHT) != 'n'\n";
    }
    if (piece_to_char(Piece::W_BISHOP) != 'B') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_BISHOP) != 'B'\n";
    }
    if (piece_to_char(Piece::B_BISHOP) != 'b') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::B_BISHOP) != 'b'\n";
    }
    if (piece_to_char(Piece::W_QUEEN) != 'Q') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_QUEEN) != 'Q'\n";
    }
    if (piece_to_char(Piece::B_QUEEN) != 'q') {
        std::cout << "TEST FAILED : piece_test : ppiece_to_char(Piece::B_QUEEN) != 'q'\n";
    }
    if (piece_to_char(Piece::W_ROOK) != 'R') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_ROOK) != 'R'\n";
    }
    if (piece_to_char(Piece::B_ROOK) != 'r') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::B_ROOK) != 'r'\n";
    }
    if (piece_to_char(Piece::W_KING) != 'K') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::W_KING) != 'K'\n";
    }
    if (piece_to_char(Piece::B_KING) != 'k') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::B_KING) != 'k'\n";
    }
    if (piece_to_char(Piece::EMPTY) != ' ') {
        std::cout << "TEST FAILED : piece_test : piece_to_char(Piece::EMPTY) != ' '\n";
    }
}

static void char_to_piece_test()
{
    std::cout << "char_to_piece test :\n\n";

    if (char_to_piece('P') != Piece::W_PAWN) {
        std::cout << "TEST FAILED : char_to_piece('P') != Piece::W_PAWN\n";
    }
    if (char_to_piece('p') != Piece::B_PAWN) {
        std::cout << "TEST FAILED : char_to_piece('n') != Piece::B_PAWN\n";
    }
    if (char_to_piece('N') != Piece::W_KNIGHT) {
        std::cout << "TEST FAILED : char_to_piece('N') != Piece::W_KNIGHT\n";
    }
    if (char_to_piece('n') != Piece::B_KNIGHT) {
        std::cout << "TEST FAILED : char_to_piece('n') != Piece::B_KNIGHT\n";
    }
    if (char_to_piece('B') != Piece::W_BISHOP) {
        std::cout << "TEST FAILED : char_to_piece('B') != Piece::W_BISHOP\n";
    }
    if (char_to_piece('b') != Piece::B_BISHOP) {
        std::cout << "TEST FAILED : char_to_piece('b') != Piece::B_BISHOP\n";
    }
    if (char_to_piece('R') != Piece::W_ROOK) {
        std::cout << "TEST FAILED : char_to_piece('R') != Piece::W_ROOK\n";
    }
    if (char_to_piece('r') != Piece::B_ROOK) {
        std::cout << "TEST FAILED : char_to_piece('r') != Piece::B_ROOK\n";
    }
    if (char_to_piece('Q') != Piece::W_QUEEN) {
        std::cout << "TEST FAILED : char_to_piece('Q') != Piece::W_QUEEN\n";
    }
    if (char_to_piece('q') != Piece::B_QUEEN) {
        std::cout << "TEST FAILED : char_to_piece('q') != Piece::B_QUEEN\n";
    }
    if (char_to_piece('K') != Piece::W_KING) {
        std::cout << "TEST FAILED : char_to_piece('K') != Piece::W_KING\n";
    }
    if (char_to_piece('k') != Piece::B_KING) {
        std::cout << "TEST FAILED : char_to_piece('k') != Piece::B_KING\n";
    }
    if (char_to_piece(' ') != Piece::EMPTY) {
        std::cout << "TEST FAILED : char_to_piece(' ') != Piece::EMPTY\n";
    }
    if (char_to_piece('x') != Piece::EMPTY) {
        std::cout << "TEST FAILED : char_to_piece('x') != Piece::EMPTY\n";
    }
}

static void char_to_pieceType_test()
{
    std::cout << "char_to_pieceType test :\n\n";

    if (char_to_pieceType('P') != PieceType::PAWN) {
        std::cout << "TEST FAILED : char_to_pieceType('P') != PieceType::PAWN\n";
    }
    if (char_to_pieceType('p') != PieceType::PAWN) {
        std::cout << "TEST FAILED : char_to_pieceType('n') != PieceType::PAWN\n";
    }
    if (char_to_pieceType('N') != PieceType::KNIGHT) {
        std::cout << "TEST FAILED : char_to_pieceType('N') != PieceType::KNIGHT\n";
    }
    if (char_to_pieceType('n') != PieceType::KNIGHT) {
        std::cout << "TEST FAILED : char_to_pieceType('n') != PieceType::KNIGHT\n";
    }
    if (char_to_pieceType('B') != PieceType::BISHOP) {
        std::cout << "TEST FAILED : char_to_pieceType('B') != PieceType::BISHOP\n";
    }
    if (char_to_pieceType('b') != PieceType::BISHOP) {
        std::cout << "TEST FAILED : char_to_pieceType('b') != PieceType::BISHOP\n";
    }
    if (char_to_pieceType('R') != PieceType::ROOK) {
        std::cout << "TEST FAILED : char_to_pieceType('R') != PieceType::ROOK\n";
    }
    if (char_to_pieceType('r') != PieceType::ROOK) {
        std::cout << "TEST FAILED : char_to_pieceType('r') != PieceType::ROOK\n";
    }
    if (char_to_pieceType('Q') != PieceType::QUEEN) {
        std::cout << "TEST FAILED : char_to_pieceType('Q') != PieceType::QUEEN\n";
    }
    if (char_to_pieceType('q') != PieceType::QUEEN) {
        std::cout << "TEST FAILED : char_to_pieceType('q') != PieceType::QUEEN\n";
    }
    if (char_to_pieceType('K') != PieceType::KING) {
        std::cout << "TEST FAILED : char_to_pieceType('K') != PieceType::KING\n";
    }
    if (char_to_pieceType('k') != PieceType::KING) {
        std::cout << "TEST FAILED : char_to_pieceType('k') != PieceType::KING\n";
    }
    if (char_to_pieceType(' ') != PieceType::EMPTY) {
        std::cout << "TEST FAILED : char_to_pieceType(' ') != PieceType::EMPTY\n";
    }
    if (char_to_pieceType('x') != PieceType::EMPTY) {
        std::cout << "TEST FAILED : char_to_pieceType('x') != PieceType::EMPTY\n";
    }
}

static void pieceType_to_char_test()
{
    std::cout << "pieceType_to_char test :\n\n";

    if (pieceType_to_char(PieceType::KNIGHT) != 'n') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::KNIGHT) != 'n'\n";
    }
    if (pieceType_to_char(PieceType::PAWN) != 'p') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::PAWN) != 'p'\n";
    }
    if (pieceType_to_char(PieceType::BISHOP) != 'b') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::BISHOP) != 'b'\n";
    }
    if (pieceType_to_char(PieceType::ROOK) != 'r') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::ROOK) != 'r'\n";
    }
    if (pieceType_to_char(PieceType::QUEEN) != 'q') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::QUEEN) != 'q'\n";
    }
    if (pieceType_to_char(PieceType::KING) != 'k') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::KING) != 'k'\n";
    }
    if (pieceType_to_char(PieceType::EMPTY) != ' ') {
        std::cout << "TEST FAILED : pieceType_to_char(PieceType::EMPTY) != ' '\n";
    }
}

static void get_color_test()
{
    std::cout << "get_color test :\n\n";

    if (get_color(Piece::W_ROOK) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_ROOK) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_ROOK) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_ROOK) != ChessColor::BLACK\n";
    }
    if (get_color(Piece::W_PAWN) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_PAWN) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_PAWN) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_PAWN) != ChessColor::BLACK\n";
    }
    if (get_color(Piece::W_KNIGHT) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_KNIGHT) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_KNIGHT) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_KNIGHT) != ChessColor::BLACK\n";
    }
    if (get_color(Piece::W_BISHOP) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_BISHOP) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_BISHOP) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_BISHOP) != ChessColor::BLACK\n";
    }
    if (get_color(Piece::W_QUEEN) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_QUEEN) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_QUEEN) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_QUEEN) != ChessColor::BLACK\n";
    }
    if (get_color(Piece::W_KING) != ChessColor::WHITE) {
        std::cout << "TEST FAILED : get_color(Piece::W_KING) != ChessColor::WHITE\n";
    }
    if (get_color(Piece::B_KING) != ChessColor::BLACK) {
        std::cout << "TEST FAILED : get_color(Piece::B_KING) != ChessColor::BLACK\n";
    }
}

static void piece_to_pieceType_test()
{
    std::cout << "piece_to_pieceType test :\n\n";

    if (piece_to_pieceType(Piece::W_KING) != PieceType::KING) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_KING) != PieceType::KING\n";
    }
    if (piece_to_pieceType(Piece::B_KING) != PieceType::KING) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_KING) != PieceType::KING\n";
    }
    if (piece_to_pieceType(Piece::W_QUEEN) != PieceType::QUEEN) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_QUEEN) != PieceType::QUEEN\n";
    }
    if (piece_to_pieceType(Piece::B_QUEEN) != PieceType::QUEEN) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_QUEEN) != PieceType::QUEEN\n";
    }
    if (piece_to_pieceType(Piece::W_PAWN) != PieceType::PAWN) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_PAWN) != PieceType::PAWN\n";
    }
    if (piece_to_pieceType(Piece::B_PAWN) != PieceType::PAWN) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_PAWN) != PieceType::PAWN\n";
    }
    if (piece_to_pieceType(Piece::W_KNIGHT) != PieceType::KNIGHT) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_KNIGHT) != PieceType::KNIGHT\n";
    }
    if (piece_to_pieceType(Piece::B_KNIGHT) != PieceType::KNIGHT) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_KNIGHT) != PieceType::KNIGHT\n";
    }
    if (piece_to_pieceType(Piece::W_BISHOP) != PieceType::BISHOP) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_BISHOP) != PieceType::BISHOP\n";
    }
    if (piece_to_pieceType(Piece::B_BISHOP) != PieceType::BISHOP) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_BISHOP) != PieceType::BISHOP\n";
    }
    if (piece_to_pieceType(Piece::W_ROOK) != PieceType::ROOK) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::W_ROOK) != PieceType::ROOK\n";
    }
    if (piece_to_pieceType(Piece::B_ROOK) != PieceType::ROOK) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::B_ROOK) != PieceType::ROOK\n";
    }
    if (piece_to_pieceType(Piece::EMPTY) != PieceType::EMPTY) {
        std::cout << "TEST FAILED : piece_to_pieceType(Piece::EMPTY) != PieceType::EMPTY\n";
    }
}

static void create_piece_test()
{
    std::cout << "create_piece test :\n\n";

    if (create_piece(PieceType::KNIGHT, ChessColor::WHITE) != Piece::W_KNIGHT) {
        std::cout << "TEST FAILED : create_piece(KNIGHT, WHITE) != Piece::W_KNIGHT\n";
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::BLACK) != Piece::B_KNIGHT) {
        std::cout << "TEST FAILED : create_piece(KNIGHT, BLACK) != Piece::B_KNIGHT\n";
    }
    if (create_piece(PieceType::PAWN, ChessColor::WHITE) != Piece::W_PAWN) {
        std::cout << "TEST FAILED : create_piece(PAWN, WHITE) != Piece::W_PAWN\n";
    }
    if (create_piece(PieceType::PAWN, ChessColor::BLACK) != Piece::B_PAWN) {
        std::cout << "TEST FAILED : create_piece(PAWN, BLACK) != Piece::B_PAWN\n";
    }
    if (create_piece(PieceType::BISHOP, ChessColor::WHITE) != Piece::W_BISHOP) {
        std::cout << "TEST FAILED : create_piece(PAWN, WHITE) != Piece::W_BISHOP\n";
    }
    if (create_piece(PieceType::BISHOP, ChessColor::BLACK) != Piece::B_BISHOP) {
        std::cout << "TEST FAILED : create_piece(BISHOP, BLACK) != Piece::B_BISHOP\n";
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::WHITE) != Piece::W_KNIGHT) {
        std::cout << "TEST FAILED : create_piece(KNIGHT, WHITE) != Piece::W_KNIGHT\n";
    }
    if (create_piece(PieceType::KNIGHT, ChessColor::BLACK) != Piece::B_KNIGHT) {
        std::cout << "TEST FAILED : create_piece(KNIGHT, BLACK) != Piece::B_KNIGHTn";
    }
    if (create_piece(PieceType::ROOK, ChessColor::WHITE) != Piece::W_ROOK) {
        std::cout << "TEST FAILED : create_piece(ROOK, WHITE) != Piece::W_ROOK\n";
    }
    if (create_piece(PieceType::ROOK, ChessColor::BLACK) != Piece::B_ROOK) {
        std::cout << "TEST FAILED : create_piece(ROOK, BLACK) != Piece::B_ROOK\n";
    }
    if (create_piece(PieceType::QUEEN, ChessColor::WHITE) != Piece::W_QUEEN) {
        std::cout << "TEST FAILED : create_piece(QUEEN, WHITE) != Piece::W_QUEEN\n";
    }
    if (create_piece(PieceType::QUEEN, ChessColor::BLACK) != Piece::B_QUEEN) {
        std::cout << "TEST FAILED : create_piece(QUEEN, BLACK) != Piece::B_QUEEN\n";
    }
    if (create_piece(PieceType::KING, ChessColor::WHITE) != Piece::W_KING) {
        std::cout << "TEST FAILED : create_piece(KING, WHITE) != Piece::W_KING\n";
    }
    if (create_piece(PieceType::KING, ChessColor::BLACK) != Piece::B_KING) {
        std::cout << "TEST FAILED : create_piece(KING, BLACK) != Piece::B_KING\n";
    }
    if (create_piece(PieceType::EMPTY, ChessColor::WHITE) != Piece::EMPTY) {
        std::cout << "TEST FAILED : create_piece(EMPTY, WHITE) != Piece::EMPTY\n";
    }
    if (create_piece(PieceType::EMPTY, ChessColor::BLACK) != Piece::EMPTY) {
        std::cout << "TEST FAILED : create_piece(EMPTY, BLACK) != Piece::EMPTY\n";
    }
}

static void raw_value_test()
{
    std::cout << "raw_value test :\n\n";

    if (raw_value(PieceType::PAWN) != 100) {
        std::cout << "TEST FAILED : raw_value(PieceType::PAWN) != 100\n";
    }
    if (raw_value(PieceType::KNIGHT) != 300) {
        std::cout << "TEST FAILED : raw_value(PieceType::KNIGHT) != 300\n";
    }
    if (raw_value(PieceType::BISHOP) != 310) {
        std::cout << "TEST FAILED : raw_value(PieceType::BISHOP) != 310\n";
    }
    if (raw_value(PieceType::ROOK) != 500) {
        std::cout << "TEST FAILED : raw_value(PieceType::ROOK) != 500\n";
    }
    if (raw_value(PieceType::QUEEN) != 900) {
        std::cout << "TEST FAILED : raw_value(PieceType::QUEEN) != 900\n";
    }
    if (raw_value(PieceType::KING) != 500) {
        std::cout << "TEST FAILED : raw_value(PieceType::KING) != 500\n";
    }
    if (raw_value(PieceType::EMPTY) != 0) {
        std::cout << "TEST FAILED : raw_value(PieceType::EMPTY) != 0\n";
    }

    if (raw_value(Piece::W_PAWN) != 100) {
        std::cout << "TEST FAILED : raw_value(Piece::W_PAWN) != 100\n";
    }
    if (raw_value(Piece::B_PAWN) != 100) {
        std::cout << "TEST FAILED : raw_value(Piece::B_PAWN) != 100\n";
    }
    if (raw_value(Piece::W_KNIGHT) != 300) {
        std::cout << "TEST FAILED : raw_value(Piece::W_KNIGHT) != 300\n";
    }
    if (raw_value(Piece::B_KNIGHT) != 300) {
        std::cout << "TEST FAILED : raw_value(Piece::B_KNIGHT) != 300\n";
    }
    if (raw_value(Piece::W_BISHOP) != 310) {
        std::cout << "TEST FAILED : raw_value(Piece::W_BISHOP) != 310\n";
    }
    if (raw_value(Piece::B_BISHOP) != 310) {
        std::cout << "TEST FAILED : raw_value(Piece::B_BISHOP) != 310\n";
    }
    if (raw_value(Piece::W_ROOK) != 500) {
        std::cout << "TEST FAILED : raw_value(Piece::W_ROOK) != 500\n";
    }
    if (raw_value(Piece::B_ROOK) != 500) {
        std::cout << "TEST FAILED : raw_value(Piece::B_ROOK) != 500\n";
    }
    if (raw_value(Piece::W_QUEEN) != 900) {
        std::cout << "TEST FAILED : raw_value(Piece::W_QUEEN) != 900\n";
    }
    if (raw_value(Piece::B_QUEEN) != 900) {
        std::cout << "TEST FAILED : raw_value(Piece::B_QUEEN) != 900\n";
    }
    if (raw_value(Piece::W_KING) != 500) {
        std::cout << "TEST FAILED : raw_value(Piece::W_KING) != 500\n";
    }
    if (raw_value(Piece::B_KING) != 500) {
        std::cout << "TEST FAILED : raw_value(Piece::B_KING) != 500\n";
    }
    if (raw_value(Piece::EMPTY) != 0) {
        std::cout << "TEST FAILED : raw_value(Piece::EMPTY) != 0\n";
    }
}

static void is_slider_test()
{
    std::cout << "is_slider test :\n\n";

    if (is_slider(PieceType::BISHOP) != true) {
        std::cout << "TEST FAILED : is_slider(PieceType::BISHOP) != true\n";
    }
    if (is_slider(PieceType::QUEEN) != true) {
        std::cout << "TEST FAILED : is_slider(PieceType::QUEEN) != true\n";
    }
    if (is_slider(PieceType::ROOK) != true) {
        std::cout << "TEST FAILED : is_slider(PieceType::ROOK) != true\n";
    }
    if (is_slider(PieceType::PAWN) != false) {
        std::cout << "TEST FAILED : is_slider(PieceType::PAWN) != false\n";
    }
    if (is_slider(PieceType::KNIGHT) != false) {
        std::cout << "TEST FAILED : is_slider(PieceType::KNIGHT) != false\n";
    }
    if (is_slider(PieceType::KING) != false) {
        std::cout << "TEST FAILED : is_slider(PieceType::KING) != false\n";
    }
    if (is_slider(PieceType::EMPTY) != false) {
        std::cout << "TEST FAILED : is_slider(PieceType::EMPTY) != false\n";
    }
}

static void piece_sum_test()
{
    std::cout << "piece_sum test :\n\n";

    Piece p = (Piece)(Piece::W_PAWN + 6);
    if ((Piece)(Piece::W_PAWN + 6)!= Piece::B_PAWN) {
        std::cout << "TEST FAILED : Piece::W_PAWN + 6 != Piece::B_PAWN\n";
    }
    if (Piece::EMPTY + 6 != Piece::EMPTY) {
        std::cout << "TEST FAILED : Piece::EMPTY + 6 != Piece::EMPTY\n";
    }
}

static void piece_sub_test()
{
    std::cout << "piece_sub test :\n\n";

    if (Piece::B_PAWN - 6 != Piece::W_PAWN) {
        std::cout << "TEST FAILED : Piece::B_PAWN - 6 != Piece::W_PAWN\n";
    }
    if (Piece::W_PAWN - 6 != Piece::EMPTY) {
        std::cout << "TEST FAILED : Piece::W_PAWN - 6 != Piece::EMPTY\n";
    }
}

static void pieceType_sum_test()
{
    std::cout << "pieceType_sum test :\n\n";

    if (PieceType::PAWN + 1 != PieceType::KNIGHT) {
        std::cout << "TEST FAILED : PieceType::PAWN + 1 != PieceType::KNIGHT\n";
    }
    if (PieceType::EMPTY + 1 != PieceType::EMPTY) {
        std::cout << "TEST FAILED : PieceType::EMPTY + 1 != PieceType::EMPTY\n";
    }
}

static void pieceType_sub_test()
{
    std::cout << "pieceType_sub test :\n\n";

    if (PieceType::KNIGHT - 1 != PieceType::PAWN) {
        std::cout << "TEST FAILED : PieceType::KNIGHT - 1 != PieceType::PAWN\n";
    }
    if (PieceType::PAWN - 1 != PieceType::EMPTY) {
        std::cout << "TEST FAILED : PieceType::PAWN - 1 != PieceType::EMPTY\n";
    }
}
