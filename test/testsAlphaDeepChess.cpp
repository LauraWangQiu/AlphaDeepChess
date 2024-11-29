#include "move_generator_test.cpp"
#include "board_test.cpp"
#include "game_state_test.cpp"
#include "square_test.cpp"
#include "move_test.cpp"
#include "move_list_test.cpp"
#include "piece_test.cpp"

int main()
{
    piece_test();
    game_state_test();
    square_test();
    move_test();
    move_list_test();
    board_test();
    move_generator_test();
    return 0;
}