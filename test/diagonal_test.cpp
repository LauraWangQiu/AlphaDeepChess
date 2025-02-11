#include "test_utils.hpp"
#include "diagonal.hpp"
#include "square.hpp"

static void get_diagonal_mask_test();
static void get_antidiagonal_mask_test();


void diagonal_test()
{
    std::cout << "---------diagonal test---------\n\n";

    get_diagonal_mask_test();
    get_antidiagonal_mask_test();
}

static void get_diagonal_mask_test()
{
    const std::string test_name = "get_diagonal_mask_test";

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        const uint64_t diagonal_mask = get_diagonal_mask(sq.diagonal());
        if (diagonal_mask & sq.mask() == 0ULL) {
            PRINT_TEST_FAILED(test_name, "diagonal_mask & sq.mask() == 0ULL");
        }
        if (sq.north().is_valid()) {
            if ((diagonal_mask & sq.north().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(diagonal_mask & sq.north().mask()) != 0UL");
            }
        }
        if (sq.south().is_valid()) {
            if ((diagonal_mask & sq.south().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(diagonal_mask & sq.south().mask()) != 0UL");
            }
        }
        if (sq.east().is_valid()) {
            if ((diagonal_mask & sq.east().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(diagonal_mask & sq.east().mask()) != 0UL");
            }
        }
        if (sq.west().is_valid()) {
            if ((diagonal_mask & sq.west().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(diagonal_mask & sq.west().mask()) != 0UL");
            }
        }
    }
}

static void get_antidiagonal_mask_test()
{
    const std::string test_name = "get_antidiagonal_mask_test";

    for (Square sq = Square::SQ_A1; sq.is_valid(); sq++) {
        const uint64_t antidiagonal_mask = get_antidiagonal_mask(sq.antidiagonal());
        if (antidiagonal_mask & sq.mask() == 0ULL) {
            PRINT_TEST_FAILED(test_name, "antidiagonal_mask & sq.mask() == 0ULL");
        }

        if (sq.north().is_valid()) {
            if ((antidiagonal_mask & sq.north().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(antidiagonal_mask & sq.north().mask()) != 0UL");
            }
        }
        if (sq.south().is_valid()) {
            if ((antidiagonal_mask & sq.south().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(antidiagonal_mask & sq.south().mask()) != 0UL");
            }
        }
        if (sq.east().is_valid()) {
            if ((antidiagonal_mask & sq.east().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(antidiagonal_mask & sq.east().mask()) != 0UL");
            }
        }
        if (sq.west().is_valid()) {
            if ((antidiagonal_mask & sq.west().mask()) != 0ULL) {
                PRINT_TEST_FAILED(test_name, "(antidiagonal_mask & sq.west().mask()) != 0UL");
            }
        }
    }
}