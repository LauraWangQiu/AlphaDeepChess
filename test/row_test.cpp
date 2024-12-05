#include <iostream>
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
    std::cout << "row_to_char test :\n\n";

    for (Row i = ROW_1; i <= ROW_8; ++i) {
        if (row_to_char(i) != '1' + i) {
            std::cout << "TEST FAILED : row_to_char(i) != '1' + i\n";
        }
    }
}

static void is_valid_row_test()
{
    std::cout << "is_valid_row test :\n\n";

    for (Row i = ROW_1; i <= ROW_8; ++i) {
        if (!is_valid_row(i)) {
            std::cout << "TEST FAILED : is_valid_row_test : !is_valid_row(i)\n";
        }
    }

    if (is_valid_row(ROW_INVALID)) {
        std::cout << "TEST FAILED : is_valid_row_test : ROW_INVALID considered valid\n";
    }
}

static void row_plus_test()
{
    std::cout << "row+ test :\n\n";

    if (ROW_1 + 1 != ROW_2) {
        std::cout << "TEST FAILED : row_plus_test : ROW_1 + 1 != ROW_2\n";
    }

    if (ROW_8 + 1 != ROW_INVALID) {
        std::cout << "TEST FAILED : row_plus_test : ROW_8 + 1 != ROW_INVALID\n";
    }
}

static void row_minus_test()
{
    std::cout << "row- test :\n\n";

    if (ROW_2 - 1 != ROW_1) {
        std::cout << "TEST FAILED : row_minus_test : ROW_2 - 1 != ROW_1\n";
    }

    if (ROW_1 - 1 != ROW_INVALID) {
        std::cout << "TEST FAILED : row_minus_test : ROW_1 - 1 != ROW_INVALID\n";
    }
}

static void row_pre_increment_test()
{
    std::cout << "pre-increment test :\n\n";

    Row row = ROW_1;
    if (++row != ROW_2) {
        std::cout << "TEST FAILED : row_pre_increment_test : ++ROW_1 != ROW_2\n";
    }

    row = ROW_8;
    if (++row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_pre_increment_test : ++ROW_8 != ROW_INVALID\n";
    }
}

static void row_post_increment_test()
{
    std::cout << "post-increment test :\n\n";

    Row row = ROW_1;
    if (row++ != ROW_1 || row != ROW_2) {
        std::cout << "TEST FAILED : row_post_increment_test : ROW_1++ did not behave correctly\n";
    }

    row = ROW_8;
    if (row++ != ROW_8 || row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_post_increment_test : ROW_8++ did not behave correctly\n";
    }
}

static void row_pre_decrement_test()
{
    std::cout << "pre-decrement test :\n\n";

    Row row = ROW_2;
    if (--row != ROW_1) {
        std::cout << "TEST FAILED : row_pre_decrement_test : --ROW_2 != ROW_1\n";
    }

    row = ROW_1;
    if (--row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_pre_decrement_test : --ROW_1 != ROW_INVALID\n";
    }
}

static void row_post_decrement_test()
{
    std::cout << "post-decrement test :\n\n";

    Row row = ROW_2;
    if (row-- != ROW_2 || row != ROW_1) {
        std::cout << "TEST FAILED : row_post_decrement_test : ROW_2-- did not behave correctly\n";
    }

    row = ROW_1;
    if (row-- != ROW_1 || row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_post_decrement_test : ROW_1-- did not behave correctly\n";
    }
}

static void row_plus_equals_test()
{
    std::cout << "row+= test :\n\n";

    Row row = ROW_1;
    row += 1;
    if (row != ROW_2) {
        std::cout << "TEST FAILED : row_plus_equals_test : ROW_1 += 1 != ROW_2\n";
    }

    row += 7;
    if (row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_plus_equals_test : ROW_2 += 7 != ROW_INVALID\n";
    }
}

static void row_minus_equals_test()
{
    std::cout << "row-= test :\n\n";

    Row row = ROW_2;
    row -= 1;
    if (row != ROW_1) {
        std::cout << "TEST FAILED : row_minus_equals_test : ROW_2 -= 1 != ROW_1\n";
    }

    row -= 1;
    if (row != ROW_INVALID) {
        std::cout << "TEST FAILED : row_minus_equals_test : ROW_1 -= 1 != ROW_INVALID\n";
    }
}
