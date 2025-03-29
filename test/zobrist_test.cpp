#include "zobrist.hpp"
#include "test_utils.hpp"
#include <stack>

static void zobrist_hash_test();

void zobrist_test()
{

    std::cout << "---------zobrist test---------\n\n";

    zobrist_hash_test();
}

static void zobrist_hash_test()
{
    const std::string test_name = "zobrist_hash_test_test";

    constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    const uint32_t num_moves = 35U;
    std::stack<GameState> game_states;

    Move moves[num_moves] = {
        Move(Square::D2, Square::D4),   // 1. d4
        Move(Square::F7, Square::F5),   // 1... f5
        Move(Square::E2, Square::E4),   // 2. e4
        Move(Square::F5, Square::E4),   // 2... fxe4
        Move(Square::B1, Square::C3),   // 3. Nc3
        Move(Square::G8, Square::F6),   // 3... Nf6
        Move(Square::C1, Square::G5),   // 4. Bg5
        Move(Square::E7, Square::E6),   // 4... e6
        Move(Square::C3, Square::E4),   // 5. Nxe4
        Move(Square::F8, Square::E7),   // 5... Be7
        Move(Square::G5, Square::F6),   // 6. Bxf6
        Move(Square::E7, Square::F6),   // 6... Bxf6
        Move(Square::G1, Square::F3),   // 7. Nf3
        Move::castle_black_king(),            // 7... O-O
        Move(Square::F1, Square::D3),   // 8. Bd3
        Move(Square::B7, Square::B6),   // 8... b6
        Move(Square::F3, Square::E5),   // 9. Ne5
        Move(Square::C8, Square::B7),   // 9... Bb7
        Move(Square::D1, Square::H5),   // 10. Qh5
        Move(Square::D8, Square::E7),   // 10... Qe7
        Move(Square::H5, Square::H7),   // 11. Qxh7+
        Move(Square::G8, Square::H7),   // 11... Kxh7
        Move(Square::E4, Square::F6),   // 12. Nxf6+
        Move(Square::H7, Square::H6),   // 12... Kh6
        Move(Square::E5, Square::G4),   // 13. Ng4+
        Move(Square::H6, Square::G5),   // 13... Kg5
        Move(Square::H2, Square::H4),   // 14. h4+
        Move(Square::G5, Square::F4),   // 14... Kf4
        Move(Square::G2, Square::G3),   // 15. g3+
        Move(Square::F4, Square::F3),   // 15... Kf3
        Move(Square::D3, Square::E2),   // 16. Be2+
        Move(Square::F3, Square::G2),   // 16... Kg2
        Move(Square::H1, Square::H2),   // 17. Rh2+
        Move(Square::G2, Square::G1),   // 17... Kg1
        Move::castle_white_queen()            // 18. O-O-O
    };

    Board board;
    board.load_fen(StartFEN);
    game_states.push(board.state());

    const uint64_t original_hash = board.state().get_zobrist_key();

    for (uint32_t i = 0; i < num_moves; i++) {

        board.make_move(moves[i]);
        if (i < num_moves - 1) {
            game_states.push(board.state());
        }

        if (board.state().get_zobrist_key() != Zobrist::hash(board)) {
            PRINT_TEST_FAILED(test_name, "board.state().get_zobrist_key() != Zobrist::hash(board)");
        }
    }

    for (int32_t i = num_moves - 1; !game_states.empty(); i--) {

        board.unmake_move(moves[i], game_states.top());
        game_states.pop();

        if (board.state().get_zobrist_key() != Zobrist::hash(board)) {
            PRINT_TEST_FAILED(test_name, "board.state().get_zobrist_key() != Zobrist::hash(board)");
        }
    }

    if (board.state().get_zobrist_key() != original_hash) {
        PRINT_TEST_FAILED(test_name, "board.state().get_zobrist_key() != original_hash");
    }
}