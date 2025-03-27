#include "history.hpp"
#include "test_utils.hpp"

static void history_repetition_test();

void history_test()
{

    std::cout << "---------history test---------\n\n";

    history_repetition_test();
}

static void history_repetition_test()
{
    const std::string test_name = "history_repetition_test";

    const int history_max_size = History::MAX_SIZE();

    History::clear();

    History::push_position(1ULL);
    History::push_position(1ULL);

    if (History::threefold_repetition_detected(40U) == false) {
        PRINT_TEST_FAILED(test_name, "History::threefold_repetition_detected(1ULL) == false");
    }

    for (int i = 0; i <= history_max_size - 2; i++) {
        History::push_position(2ULL);
    }

    if (History::threefold_repetition_detected(40U) == true) {
        PRINT_TEST_FAILED(test_name, "History::threefold_repetition_detected(1ULL) == true");
    }

    History::push_position(2ULL);

    if (History::threefold_repetition_detected(40U) == false) {
        PRINT_TEST_FAILED(test_name, "History::threefold_repetition_detected(1ULL) == false");
    }

    History::clear();

    if (History::threefold_repetition_detected(40U) == true) {
        PRINT_TEST_FAILED(test_name, "History::threefold_repetition_detected(2ULL) == true");
    }
}
