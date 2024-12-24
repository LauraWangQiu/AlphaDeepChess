#include <iostream>
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
}

static void col_to_char_test()
{
    std::cout << "col_to_char test :\n\n";

    for (Col i = COL_A; i <= COL_H; ++i) {
        if (col_to_char(i) != 'a' + i) {
            std::cout << "TEST FAILED : col_to_char_test : col_to_char(i) != 'a' + i\n";
        }
    }
}

static void col_is_valid_col_test()
{
    std::cout << "col_is_valid_col_test test :\n\n";

    for (Col i = COL_A; i <= COL_H; ++i) {
        if (!is_valid_col(i)) {
            std::cout << "TEST FAILED : is_valid_col_test : !is_valid_col(i)\n";
        }
    }

    if (is_valid_col(COL_INVALID)) {
        std::cout << "TEST FAILED : is_valid_col_test : COL_INVALID considered valid\n";
    }
}

static void col_plus_test()
{
    std::cout << "col+ test :\n\n";

    if (COL_A + 1 != COL_B) {
        std::cout << "TEST FAILED : col_plus_test : COL_A + 1 != COL_B\n";
    }

    if (COL_H + 1 != COL_INVALID) {
        std::cout << "TEST FAILED : col_plus_test : COL_H + 1 != COL_INVALID\n";
    }
}

static void col_minus_test()
{
    std::cout << "col- test :\n\n";

    if (COL_B - 1 != COL_A) {
        std::cout << "TEST FAILED : col_minus_test : COL_B - 1 != COL_A\n";
    }

    if (COL_A - 1 != COL_INVALID) {
        std::cout << "TEST FAILED : col_minus_test : COL_A - 1 != COL_INVALID\n";
    }
}

static void col_pre_increment_test()
{
    std::cout << "pre-increment test :\n\n";

    Col col = COL_A;
    if (++col != COL_B) {
        std::cout << "TEST FAILED : col_pre_increment_test : ++COL_A != COL_B\n";
    }

    col = COL_H;
    if (++col != COL_INVALID) {
        std::cout << "TEST FAILED : col_pre_increment_test : ++COL_H != COL_INVALID\n";
    }
}

static void col_post_increment_test()
{
    std::cout << "post-increment test :\n\n";

    Col col = COL_A;
    if (col++ != COL_A || col != COL_B) {
        std::cout << "TEST FAILED : col_post_increment_test : COL_A++ did not behave correctly\n";
    }

    col = COL_H;
    if (col++ != COL_H || col != COL_INVALID) {
        std::cout << "TEST FAILED : col_post_increment_test : COL_H++ did not behave correctly\n";
    }
}

static void col_pre_decrement_test()
{
    std::cout << "pre-decrement test :\n\n";

    Col col = COL_B;
    if (--col != COL_A) {
        std::cout << "TEST FAILED : col_pre_decrement_test : --COL_B != COL_A\n";
    }

    col = COL_A;
    if (--col != COL_INVALID) {
        std::cout << "TEST FAILED : col_pre_decrement_test : --COL_A != COL_INVALID\n";
    }
}

static void col_post_decrement_test()
{
    std::cout << "post-decrement test :\n\n";

    Col col = COL_B;
    if (col-- != COL_B || col != COL_A) {
        std::cout << "TEST FAILED : col_post_decrement_test : COL_B-- did not behave correctly\n";
    }

    col = COL_A;
    if (col-- != COL_A || col != COL_INVALID) {
        std::cout << "TEST FAILED : col_post_decrement_test : COL_A-- did not behave correctly\n";
    }
}

static void col_plus_equals_test()
{
    std::cout << "col+= test :\n\n";

    Col col = COL_A;
    col += 1;
    if (col != COL_B) {
        std::cout << "TEST FAILED : col_plus_equals_test : COL_A += 1 != COL_B\n";
    }

    col += 7;
    if (col != COL_INVALID) {
        std::cout << "TEST FAILED : col_plus_equals_test : COL_B += 7 != COL_INVALID\n";
    }
}

static void col_minus_equals_test()
{
    std::cout << "col-= test :\n\n";

    Col col = COL_B;
    col -= 1;
    if (col != COL_A) {
        std::cout << "TEST FAILED : col_minus_equals_test : COL_B -= 1 != COL_A\n";
    }

    col -= 1;
    if (col != COL_INVALID) {
        std::cout << "TEST FAILED : col_minus_equals_test : COL_A -= 1 != COL_INVALID\n";
    }
}
