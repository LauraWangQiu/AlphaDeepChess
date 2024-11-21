#include <iostream>

#include "move.hpp"

void move_test()
{
    std::cout << "---------move test---------\n\n";

    Move move;

    if (move.is_valid() != false) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != false\n";
    }

    move = Move(Square::SQ_B2, Square::SQ_B4);

    if (move.square_from() != Square::SQ_B2) {
        std::cout << "TEST FAILED : move_test : square_from() != Square::SQ_B2\n";
    }
    if (move.square_to() != Square::SQ_B4) {
        std::cout << "TEST FAILED : move_test : square_to() != Square::SQ_B4\n";
    }
    if (move.type() != MoveType::NORMAL) {
        std::cout << "TEST FAILED : move_test : move.type() != MoveType::NORMAL\n";
    }
    if (move.to_string() != "b2b4") {
        std::cout << "TEST FAILED : move_test : move.to_string() != b2b4\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move(Square::SQ_D2, Square::SQ_D1, MoveType::PROMOTION, PieceType::QUEEN);

    if (move.square_from() != Square::SQ_D2) {
        std::cout << "TEST FAILED : move_test : square_from() != Square::SQ_B2\n";
    }
    if (move.square_to() != Square::SQ_D1) {
        std::cout << "TEST FAILED : move_test : square_to() != Square::SQ_B4\n";
    }
    if (move.type() != MoveType::PROMOTION) {
        std::cout << "TEST FAILED : move_test : move.type() != MoveType::PROMOTION\n";
    }
    if (move.promotion_piece() != PieceType::QUEEN) {
        std::cout << "TEST FAILED : move_test : move.promotion_piece() != PieceType::QUEEN\n";
    }
    if (move.to_string() != "d2d1q") {
        std::cout << "TEST FAILED : move_test : move.to_string() != d2d1q\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move(Square::SQ_D5, Square::SQ_E6, MoveType::EN_PASSANT);

    if (move.square_from() != Square::SQ_D5) {
        std::cout << "TEST FAILED : move_test : square_from() != Square::SQ_D5\n";
    }
    if (move.square_to() != Square::SQ_E6) {
        std::cout << "TEST FAILED : move_test : square_to() != Square::SQ_E6\n";
    }
    if (move.type() != MoveType::EN_PASSANT) {
        std::cout << "TEST FAILED : move_test : move.type() != MoveType::EN_PASSANT\n";
    }
    if (move.to_string() != "d5e6") {
        std::cout << "TEST FAILED : move_test : move.to_string() != d5e6\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move::castle_black_king();

    if (move != Move(Square::SQ_E8, Square::SQ_G8, MoveType::CASTLING)) {
        std::cout << "TEST FAILED : move_test : move != Move(SQ_E8, SQ_G8, CASTLING\n";
    }
    if (move.to_string() != "e8g8") {
        std::cout << "TEST FAILED : move_test : move.to_string() != e8g8\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move::castle_black_queen();

    if (move != Move(Square::SQ_E8, Square::SQ_C8, MoveType::CASTLING)) {
        std::cout << "TEST FAILED : move_test : move != Move(SQ_E8, SQ_C8, CASTLING\n";
    }
    if (move.to_string() != "e8c8") {
        std::cout << "TEST FAILED : move_test : move.to_string() != e8c8\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move::castle_white_king();

    if (move != Move(Square::SQ_E1, Square::SQ_G1, MoveType::CASTLING)) {
        std::cout << "TEST FAILED : move_test : move != Move(SQ_E1, SQ_G1, CASTLING\n";
    }
    if (move.to_string() != "e1g1") {
        std::cout << "TEST FAILED : move_test : move.to_string() != e1g1\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move::castle_white_queen();

    if (move != Move(Square::SQ_E1, Square::SQ_C1, MoveType::CASTLING)) {
        std::cout << "TEST FAILED : move_test : move != Move(SQ_E1, SQ_C1, CASTLING\n";
    }
    if (move.to_string() != "e1c1") {
        std::cout << "TEST FAILED : move_test : move.to_string() != e1c1\n";
    }
    if (move.is_valid() != true) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != true\n";
    }

    move = Move::null();

    if (move.is_valid() != false) {
        std::cout << "TEST FAILED : move_test : move.is_valid() != false\n";
    }
}
