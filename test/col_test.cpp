#include "test_utils.hpp"
#include "col.hpp"

static void col_to_char_test();
static void col_is_valid_col_test();
static void col_plus_test();
static void col_minus_test();
static void col_pre_increment_test();
static void col_post_increment_test();
static void col_pre_decrement_test();
static void col_post_decrement_test();
static void col_plus_equals_test();
static void col_minus_equals_test();
static void get_col_mask_test();

void col_test()
{
    std::cout << "---------col test---------\n\n";

    col_to_char_test();
    col_is_valid_col_test();
    col_plus_test();
    col_minus_test();
    col_pre_increment_test();
    col_post_increment_test();
    col_pre_decrement_test();
    col_post_decrement_test();
    col_plus_equals_test();
    col_minus_equals_test();
    get_col_mask_test();
}

static void col_to_char_test()
{
    const std::string test_name = "col_to_char_test";

    for (Col i = COL_A; i <= COL_H; ++i) {
        if (col_to_char(i) != 'a' + i) {
            PRINT_TEST_FAILED(test_name, "col_to_char(i) != 'a' + i");
        }
    }
}

static void col_is_valid_col_test()
{
    const std::string test_name = "col_is_valid_col_test";

    for (Col i = COL_A; i <= COL_H; ++i) {

        if (!is_valid_col(i)) {
            PRINT_TEST_FAILED(test_name, "!is_valid_col(i)");
        }
    }

    if (is_valid_col(COL_INVALID)) {
        PRINT_TEST_FAILED(test_name, "is_valid_col(COL_INVALID)");
    }
}

static void col_plus_test()
{
    const std::string test_name = "col_plus_test";

    if (COL_A + 1 != COL_B) {
        PRINT_TEST_FAILED(test_name, "COL_A + 1 != COL_B");
    }

    if (COL_H + 1 != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "COL_H + 1 != COL_INVALID");
    }
}

static void col_minus_test()
{
    const std::string test_name = "col_minus_test";

    if (COL_B - 1 != COL_A) {
        PRINT_TEST_FAILED(test_name, "COL_B - 1 != COL_A");
    }

    if (COL_A - 1 != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "COL_A - 1 != COL_INVALID");
    }
}

static void col_pre_increment_test()
{
    const std::string test_name = "col_pre_increment_test";

    Col col = COL_A;
    if (++col != COL_B) {
        PRINT_TEST_FAILED(test_name, "++COL_A != COL_B");
    }

    col = COL_H;
    if (++col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "++COL_H != COL_INVALID");
    }
}

static void col_post_increment_test()
{
    const std::string test_name = "col_post_increment_test";

    Col col = COL_A;
    if (col++ != COL_A || col != COL_B) {
        PRINT_TEST_FAILED(test_name, "col++ != COL_A || col != COL_B");
    }

    col = COL_H;
    if (col++ != COL_H || col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "col++ != COL_H || col != COL_INVALID");
    }
}

static void col_pre_decrement_test()
{
    const std::string test_name = "col_pre_decrement_test";

    Col col = COL_B;
    if (--col != COL_A) {
        PRINT_TEST_FAILED(test_name, "--col != COL_A");
    }

    col = COL_A;
    if (--col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "--col != COL_INVALID");
    }
}

static void col_post_decrement_test()
{
    const std::string test_name = "col_post_decrement_test";

    Col col = COL_B;
    if (col-- != COL_B || col != COL_A) {
        PRINT_TEST_FAILED(test_name, "col-- != COL_B || col != COL_A");
    }

    col = COL_A;
    if (col-- != COL_A || col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "col-- != COL_A || col != COL_INVALID");
    }
}

static void col_plus_equals_test()
{
    const std::string test_name = "col_plus_equals_test";

    Col col = COL_A;
    col += 1;
    if (col != COL_B) {
        PRINT_TEST_FAILED(test_name, "col != COL_B");
    }

    col += 7;
    if (col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "col != COL_INVALID");
    }
}

static void col_minus_equals_test()
{
    const std::string test_name = "col_minus_equals_test";

    Col col = COL_B;
    col -= 1;
    if (col != COL_A) {
        PRINT_TEST_FAILED(test_name, "col != COL_A");
    }

    col -= 1;
    if (col != COL_INVALID) {
        PRINT_TEST_FAILED(test_name, "col != COL_INVALID");
    }
}

static void get_col_mask_test()
{
    const std::string test_name = "get_col_mask_test";

    constexpr uint64_t COL_A_MASK = 0x0101010101010101ULL;
    constexpr uint64_t COL_B_MASK = 0x0202020202020202ULL;
    constexpr uint64_t COL_C_MASK = 0x0404040404040404ULL;
    constexpr uint64_t COL_D_MASK = 0x0808080808080808ULL;
    constexpr uint64_t COL_E_MASK = 0x1010101010101010ULL;
    constexpr uint64_t COL_F_MASK = 0x2020202020202020ULL;
    constexpr uint64_t COL_G_MASK = 0x4040404040404040ULL;
    constexpr uint64_t COL_H_MASK = 0x8080808080808080ULL;

    static constexpr uint64_t COL_MASKS[8] = {COL_A_MASK, COL_B_MASK, COL_C_MASK, COL_D_MASK,
        COL_E_MASK, COL_F_MASK, COL_G_MASK, COL_H_MASK};

    for (Col col = COL_A; is_valid_col(col); col++) 
    {
        if (get_col_mask(col) != COL_MASKS[col]) {
            PRINT_TEST_FAILED(test_name, "get_col_mask(col) != COL_MASKS[col]");
        }
    }
}