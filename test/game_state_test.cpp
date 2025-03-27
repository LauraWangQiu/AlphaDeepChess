#include "test_utils.hpp"
#include "game_state.hpp"

void game_state_test()
{
    std::cout << "---------game state test---------\n\n";

    const std::string test_name = "game_state_test";

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

    if (game_state.en_passant_square() != Square::SQ_B7) {
        PRINT_TEST_FAILED(test_name, "game_state.en_passant_square() != Square::SQ_B7");
    }
    if (game_state.castle_king_black() != true) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_king_black() != true");
    }
    if (game_state.castle_queen_black() != true) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_queen_black() != true");
    }
    if (game_state.castle_king_white() != true) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_king_white() != true");
    }
    if (game_state.castle_queen_white() != true) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_queen_white() != true");
    }
    if (game_state.fifty_move_rule_counter() != 33ULL) {
        PRINT_TEST_FAILED(test_name, "game_state.fifty_move_rule_counter() != 33ULL");
    }
    if (game_state.last_captured_piece() != PieceType::QUEEN) {
        PRINT_TEST_FAILED(test_name, "game_state.last_captured_piece() != PieceType::QUEEN");
    }
    if (game_state.move_number() != 511) {
        PRINT_TEST_FAILED(test_name, "game_state.move_number() != 511");
    }
    if (game_state.side_to_move() != ChessColor::BLACK) {
        PRINT_TEST_FAILED(test_name, "game_state.side_to_move() != ChessColor::BLACK");
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

    if (game_state.en_passant_square() != Square::SQ_INVALID) {
        PRINT_TEST_FAILED(test_name, "game_state.en_passant_square() != Square::SQ_INVALID");
    }
    if (game_state.castle_king_black() != false) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_king_black() != false");
    }
    if (game_state.castle_queen_black() != false) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_queen_black() != false");
    }
    if (game_state.castle_king_white() != false) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_king_white() != false");
    }
    if (game_state.castle_queen_white() != false) {
        PRINT_TEST_FAILED(test_name, "game_state.castle_queen_white() != false");
    }
    if (game_state.fifty_move_rule_counter() != 1ULL) {
        PRINT_TEST_FAILED(test_name, "game_state.fifty_move_rule_counter() != 1ULL");
    }
    if (game_state.last_captured_piece() != PieceType::EMPTY) {
        PRINT_TEST_FAILED(test_name, "game_state.last_captured_piece() != PieceType::EMPTY");
    }
    if (game_state.move_number() != 1) {
        PRINT_TEST_FAILED(test_name, "game_state.move_number() != 1");
    }
    if (game_state.side_to_move() != ChessColor::WHITE) {
        PRINT_TEST_FAILED(test_name, "game_state.side_to_move() != ChessColor::WHITE");
    }
}
