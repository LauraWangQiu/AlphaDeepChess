#include "test_utils.hpp"
#include "row.hpp"

static void row_to_char_test();
static void is_valid_row_test();
static void row_plus_test();
static void row_minus_test();
static void row_pre_increment_test();
static void row_post_increment_test();
static void row_pre_decrement_test();
static void row_post_decrement_test();
static void row_plus_equals_test();
static void row_minus_equals_test();
static void get_row_mask_test();

void row_test()
{
    std::cout << "---------row test---------\n\n";

    row_to_char_test();
    is_valid_row_test();
    row_plus_test();
    row_minus_test();
    row_pre_increment_test();
    row_post_increment_test();
    row_pre_decrement_test();
    row_post_decrement_test();
    row_plus_equals_test();
    row_minus_equals_test();
    get_row_mask_test();
}

static void row_to_char_test()
{
    const std::string test_name = "row_to_char_test";

    for (Row i = ROW_1; i <= ROW_8; ++i) {
        if (row_to_char(i) != '1' + i) {
            PRINT_TEST_FAILED(test_name, "row_to_char(i) != '1' + i");
        }
    }
}

static void is_valid_row_test()
{
    const std::string test_name = "is_valid_row_test";

    for (Row i = ROW_1; i <= ROW_8; ++i) {
        if (!is_valid_row(i)) {
            PRINT_TEST_FAILED(test_name, "is_valid_row_test : !is_valid_row(i)");
        }
    }

    if (is_valid_row(ROW_INVALID)) {
        PRINT_TEST_FAILED(test_name, "is_valid_row(ROW_INVALID)");
    }
}

static void row_plus_test()
{
    const std::string test_name = "row_plus_test";

    if (ROW_1 + 1 != ROW_2) {
        PRINT_TEST_FAILED(test_name, "ROW_1 + 1 != ROW_2");
    }

    if (ROW_8 + 1 != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "ROW_8 + 1 != ROW_INVALID");
    }
}

static void row_minus_test()
{
    const std::string test_name = "row_minus_test";

    if (ROW_2 - 1 != ROW_1) {
        PRINT_TEST_FAILED(test_name, "ROW_2 - 1 != ROW_1");
    }

    if (ROW_1 - 1 != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "ROW_1 - 1 != ROW_INVALID");
    }
}

static void row_pre_increment_test()
{
    const std::string test_name = "row_pre_increment_test";

    Row row = ROW_1;
    if (++row != ROW_2) {
        PRINT_TEST_FAILED(test_name, " ++ROW_1 != ROW_2");
    }

    row = ROW_8;
    if (++row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "++ROW_8 != ROW_INVALID");
    }
}

static void row_post_increment_test()
{
    const std::string test_name = "row_post_increment_test";

    Row row = ROW_1;
    if (row++ != ROW_1 || row != ROW_2) {
        PRINT_TEST_FAILED(test_name, "row++ != ROW_1 || row != ROW_2");
    }

    row = ROW_8;
    if (row++ != ROW_8 || row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "row++ != ROW_8 || row != ROW_INVALID");
    }
}

static void row_pre_decrement_test()
{
    const std::string test_name = "row_pre_decrement_test";

    Row row = ROW_2;
    if (--row != ROW_1) {
        PRINT_TEST_FAILED(test_name, "--ROW_2 != ROW_1");
    }

    row = ROW_1;
    if (--row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "--ROW_1 != ROW_INVALID");
    }
}

static void row_post_decrement_test()
{
    const std::string test_name = "row_post_decrement_test";

    Row row = ROW_2;
    if (row-- != ROW_2 || row != ROW_1) {
        PRINT_TEST_FAILED(test_name, "row-- != ROW_2 || row != ROW_1");
    }

    row = ROW_1;
    if (row-- != ROW_1 || row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "row-- != ROW_1 || row != ROW_INVALID");
    }
}

static void row_plus_equals_test()
{
    const std::string test_name = "row_plus_equals_test";

    Row row = ROW_1;
    row += 1;
    if (row != ROW_2) {
        PRINT_TEST_FAILED(test_name, "ROW_1 += 1 != ROW_2");
    }

    row += 7;
    if (row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "ROW_2 += 7 != ROW_INVALID");
    }
}

static void row_minus_equals_test()
{
    const std::string test_name = "row_minus_equals_test";

    Row row = ROW_2;
    row -= 1;
    if (row != ROW_1) {
        PRINT_TEST_FAILED(test_name, "ROW_2 -= 1 != ROW_1");
    }

    row -= 1;
    if (row != ROW_INVALID) {
        PRINT_TEST_FAILED(test_name, "ROW_1 -= 1 != ROW_INVALID");
    }
}

static void get_row_mask_test()
{
    const std::string test_name = "get_row_mask_test";

    constexpr uint64_t ROW_1_MASK = 0x00000000000000FFULL;
    constexpr uint64_t ROW_2_MASK = 0x000000000000FF00ULL;
    constexpr uint64_t ROW_3_MASK = 0x0000000000FF0000ULL;
    constexpr uint64_t ROW_4_MASK = 0x00000000FF000000ULL;
    constexpr uint64_t ROW_5_MASK = 0x000000FF00000000ULL;
    constexpr uint64_t ROW_6_MASK = 0x0000FF0000000000ULL;
    constexpr uint64_t ROW_7_MASK = 0x00FF000000000000ULL;
    constexpr uint64_t ROW_8_MASK = 0xFF00000000000000ULL;

    static constexpr uint64_t ROW_MASKS[8] = {ROW_1_MASK, ROW_2_MASK, ROW_3_MASK, ROW_4_MASK,
                                              ROW_5_MASK, ROW_6_MASK, ROW_7_MASK, ROW_8_MASK};


    for (Row row = ROW_1; is_valid_row(row); row++) {
        if (get_row_mask(row) != ROW_MASKS[row]) {
            PRINT_TEST_FAILED(test_name, "get_row_mask(row) != ROW_MASKS[row]");
        }
    }
}