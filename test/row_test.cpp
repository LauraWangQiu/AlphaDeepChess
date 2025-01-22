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
