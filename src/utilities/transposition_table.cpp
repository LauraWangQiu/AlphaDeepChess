/**
 * @file transposition_table.cpp
 * @brief transposition table utilities implementation.
 * 
 * https://www.chessprogramming.org/Transposition_Table
 * 
 */

#include "transposition_table.hpp"

std::vector<TranspositionTable::Entry> TranspositionTable::entries = initialization();

constexpr TranspositionTable::SIZE TT_DEFAULT_SIZE = TranspositionTable::SIZE::MB_64;

/**
 * @brief resize(SIZE)
 * 
 * resize the transposition table
 * 
 * @note new_size_mb must be power of two
 * 
 * @param[in] new_size_mb new size of the transposition table in mb, must be power of two
 * 
 */
void TranspositionTable::resize(SIZE new_size_mb)
{
    assert(new_size_mb != SIZE::INVALID);

    if (new_size_mb == SIZE::INVALID) {
        return;
    }

    const uint64_t size_table_bytes = mb_to_bytes(static_cast<uint64_t>(new_size_mb));
    uint64_t num_entries = size_table_bytes >> lsb(next_power_of_two(sizeof(Entry)));

    assert(is_power_of_two(num_entries));
    assert(is_power_of_two(size_table_bytes));
    assert(num_entries * next_power_of_two(sizeof(Entry)) == size_table_bytes);

    entries.resize(static_cast<int>(num_entries));
}

/**
 * @brief initialization(std::vector<Entry>)
 * 
 * initliazes the entries vector
 * 
 * @return std::vector<Entry> entries
 * 
 */
std::vector<TranspositionTable::Entry> TranspositionTable::initialization()
{
    const SIZE size_mb = TT_DEFAULT_SIZE;
    const uint64_t size_table_bytes = mb_to_bytes(static_cast<uint64_t>(size_mb));
    const uint64_t num_entries = size_table_bytes >> lsb(next_power_of_two(sizeof(Entry)));
    return std::vector<Entry>(int(num_entries));
}
