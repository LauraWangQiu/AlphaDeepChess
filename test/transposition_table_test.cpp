#include "transposition_table.hpp"
#include "test_utils.hpp"

static void transposition_table_constructor_test();
static void transposition_entry_test();
static void transposition_table_get_entry_test();

void transposition_table_test()
{

    std::cout << "---------transposition table test---------\n\n";

    transposition_table_constructor_test();
    transposition_entry_test();
    transposition_table_get_entry_test();
}

static void transposition_table_constructor_test()
{
    const std::string test_name = "transposition_table_constructor_test";

    TranspositionTable tt(TranspositionTable::SIZE::MB_1);

    for (uint64_t key = 0ULL; key < tt.get_num_entries(); key++) {
        if (tt.get_entry(key) != TranspositionTable::Entry()) {
            PRINT_TEST_FAILED(test_name, "tt.get_entry(key).is_valid()");
        }
        if (tt.get_entry(key) != TranspositionTable::Entry::failed_entry()) {
            PRINT_TEST_FAILED(test_name, "tt.get_entry(key) != TranspositionTable::Entry::failed_entry()");
        }

        if (tt.get_entry(key).is_valid() != false) {
            PRINT_TEST_FAILED(test_name, "tt.get_entry(key).is_valid() != false");
        }
    }
}

static void transposition_entry_test()
{
    const std::string test_name = "transposition_entry_test";

    TranspositionTable::Entry entry;

    if (entry != TranspositionTable::Entry::failed_entry()) {
        PRINT_TEST_FAILED(test_name, "entry != TranspositionTable::Entry::failed_entry()");
    }

    if (entry.is_valid() != false) {
        PRINT_TEST_FAILED(test_name, "entry.is_valid() != false");
    }

    entry.node_type = TranspositionTable::NodeType::EXACT;

    if (entry.is_valid() != true) {
        PRINT_TEST_FAILED(test_name, "entry.is_valid() != true");
    }
}

static void transposition_table_get_entry_test()
{
    const std::string test_name = "transposition_table_get_entry_test";

    TranspositionTable tt(TranspositionTable::SIZE::MB_1);

    for (uint64_t key = 0ULL; key < tt.get_num_entries(); key++) {
        TranspositionTable::Entry test_entry(key, (int)(key), Move(3ULL), TranspositionTable::NodeType::EXACT, 1);

        tt.store_entry(test_entry);
    }

    for (uint64_t key = 0ULL; key < tt.get_num_entries(); key++) {
        TranspositionTable::Entry test_entry(key, (int)(key), Move(3ULL), TranspositionTable::NodeType::EXACT, 1);

        TranspositionTable::Entry readed_entry = tt.get_entry(key);

        if (readed_entry != test_entry) {
            PRINT_TEST_FAILED(test_name, "readed_entry != test_entry");
        }
    }

    for (uint64_t key = tt.get_num_entries(); key < 2 * tt.get_num_entries(); key++) {

        TranspositionTable::Entry readed_entry = tt.get_entry(key);

        if (readed_entry != TranspositionTable::Entry::failed_entry()) {
            PRINT_TEST_FAILED(test_name, "readed_entry != TranspositionTable::Entry::failed_entry()");
        }
        TranspositionTable::Entry test_entry(key, (int)(key), Move(4ULL), TranspositionTable::NodeType::UPPER_BOUND, 2);
        tt.store_entry(test_entry);
    }

    for (uint64_t key = 0ULL; key < tt.get_num_entries(); key++) {

        TranspositionTable::Entry readed_entry = tt.get_entry(key);

        if (readed_entry != TranspositionTable::Entry::failed_entry()) {
            PRINT_TEST_FAILED(test_name, "readed_entry != TranspositionTable::Entry::failed_entry()");
        }
    }

    for (uint64_t key = tt.get_num_entries(); key < 2 * tt.get_num_entries(); key++) {
        TranspositionTable::Entry test_entry(key, (int)(key), Move(4ULL), TranspositionTable::NodeType::UPPER_BOUND, 2);

        TranspositionTable::Entry readed_entry = tt.get_entry(key);

        if (readed_entry != test_entry) {
            PRINT_TEST_FAILED(test_name, "readed_entry != test_entry");
        }
    }
}