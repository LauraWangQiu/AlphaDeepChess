#pragma once

/**
 * @file transposition_table.hpp
 * @brief transposition table utilities declaration.
 *
 * https://www.chessprogramming.org/Transposition_Table
 * 
 */

#include <vector>

/**
 * @brief TranspositionTable
 *
 * Transposition table used as cache to store evaluation of chess positions
 * 
 */
class TranspositionTable
{
public:
    /**
     * @brief NodeType
     *
     * We usually do not find the exact value of a position but we can know that the
     * value is either too low or too high for us to be concerned with searching any further. 
     * If we have the exact value, of course, we store that in the transposition table. 
     * But if the value of our position is either high enough to set the lower bound, 
     * or low enough to set the upper bound, it is good to store that information also.
     * 
     */
    typedef enum class NodeType
    {
        FAILED,        // failed entry
        EXACT,         // PV-Node, Score is Exact
        UPPER_BOUND,   // All-Node, Score is Upper Bound
        LOWER_BOUND    // Cut-Node, Score is Lower Bound
    };

    class Entry
    {
    public:
        uint64_t key;         // zobrist key
        int evaluation;       // score of the position
        Move best_move;       // best move found in position
        NodeType node_type;   // node type
        uint8_t depth;        // depth where the calculation has been done

        Entry() : key(0ULL), evaluation(0), best_move(), node_type(NodeType::FAILED), depth(0U) { }

        Entry(uint64_t key, int evaluation, Move best_move, NodeType node_type, uint8_t depth)
            : key(key), evaluation(evaluation), best_move(best_move), node_type(node_type), depth(depth)
        { }

        // check if entry is valid
        bool is_valid() const { return node_type != NodeType::FAILED; }

        // returns a failed entry
        static Entry failed_entry() { return Entry(); }
    };

    /**
     * @brief get_entry(uint64_t)
     * 
     * returns the entry in the table indexed by the zobrist hash key
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * @param[in] depth actual depth of the search 
     * @param[in] ply ply from root in the search 
     * @param[in] alpha alpha value in the search 
     * @param[in] beta beta value in the search 
     * 
     * @return valid entry or failed entry
     * 
     */
    Entry get_entry(uint64_t zobrist_key) const
    {
        const Entry& entry = entries[index_in_table(zobrist_key)];

        return entry.is_valid() && entry.key == zobrist_key ? entry : Entry::failed_entry();
    }

    /**
     * @brief store_entry(uint64_t)
     * 
     * returns the entry in the table indexed by the zobrist hash key
     * 
     * @param[in] zobrist zobrist hash key of the position 
     * @param[in] eval evaluation of the position
     * @param[in] move best mode of the position
     * @param[in] node_type node type 
     * @param[in] depth depth 
     * 
     * @return entries[index_in_table(zobrist_key)]
     * 
     */
    void store_entry(uint64_t zobrist, int eval, Move move, NodeType node_type, uint8_t depth)
    {
        entries[index_in_table(zobrist)] = {zobrist, eval, move, node_type, depth};
    }

    /**
     * @brief TranspositionTable()
     * 
     * constructor of the TranspositionTable.
     * 
     */
    TranspositionTable(uint32_t size_MB = 64U) : num_entries(0ULL)
    {
        const uint32_t size_table_bytes = size_MB * 1024 * 1024;
        num_entries = size_table_bytes / sizeof(Entry);

        entries.resize(num_entries);
    }

    /**
     * @brief ~TranspositionTable()
     * 
     * destructor of the TranspositionTable.
     * 
     */
    ~TranspositionTable() { }

private:
    /**
     * @brief index_in_table(uint64_t)
     * 
     * calculates the real index key in the transposition table of a position, 
     * it crops the zobrist key with % operator so key fits in table.
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * 
     * @return index key of the entry in the table
     * 
     */
    uint64_t index_in_table(uint64_t zobrist_key) const { return zobrist_key % num_entries; }

    /**
     * @brief entries
     * 
     * vector of entries
     * 
     */
    std::vector<Entry> entries;

    /**
     * @brief num_entries
     * 
     * size of the entries vector
     * 
     */
    uint32_t num_entries;
};