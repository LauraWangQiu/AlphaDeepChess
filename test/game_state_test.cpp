#include <iostream>

#include "game_state.hpp"

void game_state_test()
{
    std::cout << "---------game state test---------\n\n";

    GameState game_state;

    game_state.clean();
    game_state.set_en_passant_square(Square::SQ_B7);
    game_state.set_castle_king_black(true);
    game_state.set_castle_queen_black(true);
    game_state.set_castle_king_white(true);
    game_state.set_castle_queen_white(true);
    game_state.set_fifty_move_rule_counter(33ULL);
    game_state.set_last_captured_piece(PieceType::QUEEN);
    game_state.set_move_number(511);
    game_state.set_side_to_move(ChessColor::BLACK);
    game_state.set_triple_repetition_counter(3U);

    if (game_state.en_passant_square() != Square::SQ_B7) {
        std::cout << "TEST FAILED : game_state_test : en_passant_square() != Square::SQ_B7\n";
    }
    if (game_state.castle_king_black() != true) {
        std::cout << "TEST FAILED : game_state_test : castle_king_black() != true\n";
    }
    if (game_state.castle_queen_black() != true) {
        std::cout << "TEST FAILED : game_state_test : castle_queen_black() != true\n";
    }
    if (game_state.castle_king_white() != true) {
        std::cout << "TEST FAILED : game_state_test : ecastle_king_white() != true\n";
    }
    if (game_state.castle_queen_white() != true) {
        std::cout << "TEST FAILED : game_state_test : castle_queen_white() != true\n";
    }
    if (game_state.fifty_move_rule_counter() != 33ULL) {
        std::cout << "TEST FAILED : game_state_test : fifty_move_rule_counter() != 33ULL\n";
    }
    if (game_state.last_captured_piece() != PieceType::QUEEN) {
        std::cout << "TEST FAILED : game_state_test : last_captured_piece() != PieceType::QUEEN\n";
    }
    if (game_state.move_number() != 511) {
        std::cout << "TEST FAILED : game_state_test : move_number() != 511\n";
    }
    if (game_state.side_to_move() != ChessColor::BLACK) {
        std::cout << "TEST FAILED : game_state_test : side_to_move() != ChessColor::BLACK\n";
    }
    if (game_state.triple_repetition_counter() != 3) {
        std::cout << "TEST FAILED : game_state_test : triple_repetition_counter() != 3\n";
    }


    game_state.set_en_passant_square(Square::SQ_INVALID);
    game_state.set_castle_king_black(false);
    game_state.set_castle_queen_black(false);
    game_state.set_castle_king_white(false);
    game_state.set_castle_queen_white(false);
    game_state.set_fifty_move_rule_counter(1ULL);
    game_state.set_last_captured_piece(PieceType::EMPTY);
    game_state.set_move_number(1);
    game_state.set_side_to_move(ChessColor::WHITE);
    game_state.set_triple_repetition_counter(1U);

    if (game_state.en_passant_square() != Square::SQ_INVALID) {
        std::cout << "TEST FAILED : game_state_test : en_passant_square() != Square::SQ_INVALID\n";
    }
    if (game_state.castle_king_black() != false) {
        std::cout << "TEST FAILED : game_state_test : castle_king_black() != false\n";
    }
    if (game_state.castle_queen_black() != false) {
        std::cout << "TEST FAILED : game_state_test : castle_queen_black() != false\n";
    }
    if (game_state.castle_king_white() != false) {
        std::cout << "TEST FAILED : game_state_test : ecastle_king_white() != false\n";
    }
    if (game_state.castle_queen_white() != false) {
        std::cout << "TEST FAILED : game_state_test : castle_queen_white() != false\n";
    }
    if (game_state.fifty_move_rule_counter() != 1ULL) {
        std::cout << "TEST FAILED : game_state_test : fifty_move_rule_counter() != 1ULL\n";
    }
    if (game_state.last_captured_piece() != PieceType::EMPTY) {
        std::cout << "TEST FAILED : game_state_test : last_captured_piece() != PieceType::EMPTY\n";
    }
    if (game_state.move_number() != 1) {
        std::cout << "TEST FAILED : game_state_test : move_number() != 1\n";
    }
    if (game_state.side_to_move() != ChessColor::WHITE) {
        std::cout << "TEST FAILED : game_state_test : side_to_move() != ChessColor::WHITE\n";
    }
    if (game_state.triple_repetition_counter() != 1) {
        std::cout << "TEST FAILED : game_state_test : triple_repetition_counter() != 1\n";
    }
}
