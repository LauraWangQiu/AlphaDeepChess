#pragma once

/**
 * @file transposition_table.hpp
 * @brief transposition table utilities declaration.
 *
 * https://www.chessprogramming.org/Transposition_Table
 * 
 */

#include <vector>
#include <bit_utilities.hpp>
#include "move.hpp"

/**
 * @brief TranspositionTable
 *
 * Transposition table used as cache to store evaluation of chess positions
 * 
 */
class TranspositionTable
{
public:

    enum class SIZE : int;
    enum class NodeType : uint8_t;
    class Entry;

    /**
     * @brief get_entry(uint64_t)
     * 
     * returns the entry in the table indexed by the zobrist hash key
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * 
     * @return valid entry or failed entry
     * 
     */
    static inline constexpr Entry get_entry(uint64_t zobrist_key)
    {
        const Entry& entry = entries[index_in_table(zobrist_key)];

        return entry.is_valid() && entry.key == zobrist_key ? entry : Entry::failed_entry();
    }

    /**
     * @brief store_entry(uint64_t)
     * 
     * stores an entry in the transposition table
     * 
     * @param[in] zobrist zobrist hash key of the position 
     * @param[in] eval evaluation of the position
     * @param[in] move best mode of the position
     * @param[in] node_type node type 
     * @param[in] depth depth 
     * 
     */
    static inline constexpr void store_entry(uint64_t zobrist, int eval, Move move, NodeType node_type, uint8_t depth)
    {
        entries[index_in_table(zobrist)] = Entry(zobrist, eval, move, node_type, depth);
        assert(entries[index_in_table(zobrist)].is_valid());
        assert(move.is_valid());
    }

    /**
     * @brief store_entry(const Entry&)
     * 
     * stores an entry in the transposition table
     * 
     * @param[in] entry entry to store 
     * 
     */
    static inline constexpr void store_entry(const Entry& entry)
    {
        entries[index_in_table(entry.key)] = entry;
        assert(entries[index_in_table(entry.key)].is_valid());
        assert(entry.move.is_valid());
    }

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
    static void resize(SIZE new_size_mb);

    /**
     * @brief int_to_tt_size(int)
     * 
     * returns the corresponding enum SIZE.
     * @note size must be power of two, if invalid or too large size, MB_INVALID will be returned
     * 
     * @param[in] size_mb_power_of_two int with size in mb, must be a power of two
     * 
     * @return SIZE
     * 
     */
    static constexpr inline SIZE int_to_tt_size(int size_mb_power_of_two)
    {
        return is_power_of_two(size_mb_power_of_two) && 1 <= size_mb_power_of_two && size_mb_power_of_two <= 2048
            ? static_cast<SIZE>(size_mb_power_of_two)
            : SIZE::INVALID;
    }

    /**
     * @brief get_num_entries()
     *
     * return num of entries in transposition table
     * 
     * @return entries.size
     */
    static inline constexpr uint32_t get_num_entries() { return entries.size(); }

    TranspositionTable() = delete;
    ~TranspositionTable() = delete;

private:
    /**
     * @brief index_in_table(uint64_t)
     * 
     * calculates the real index key in the transposition table of a position, 
     * it crops the zobrist key with % operator so key fits in table.
     * 
     * @param[in] zobrist_key zobrist hash key of the position 
     * 
     * @return zobrist_key % num_entries
     * 
     */
    static inline constexpr uint64_t index_in_table(uint64_t zobrist_key)
    {
        assert(is_power_of_two(entries.size()));

        return zobrist_key & (uint64_t(entries.size()) - 1ULL);
    }

    /**
     * @brief initialization(std::vector<Entry>)
     * 
     * initliazes the entries vector
     * 
     * @return std::vector<Entry> entries
     * 
     */
    static std::vector<Entry> initialization();

    /**
     * @brief entries
     * 
     * vector of entries
     * 
     */
    static std::vector<TranspositionTable::Entry> entries;

public:

    /**
     * @brief TranspositionTable::SIZE
     *
     *  Size in MB of the transposition table, only powers of two are permitted.
     *    
     */
    enum class SIZE : int
    {
        MB_1 = 1 << 0,
        MB_2 = 1 << 1,
        MB_4 = 1 << 2,
        MB_8 = 1 << 3,
        MB_16 = 1 << 4,
        MB_32 = 1 << 5,
        MB_64 = 1 << 6,
        MB_128 = 1 << 7,
        MB_256 = 1 << 8,
        MB_512 = 1 << 9,
        MB_1024 = 1 << 10,
        MB_2048 = 1 << 11,
        INVALID
    };

    /**
     * @brief TranspositionTable::NodeType
     *
     * We usually do not find the exact value of a position but we can know that the
     * value is either too low or too high for us to be concerned with searching any further. 
     * If we have the exact value, of course, we store that in the transposition table. 
     * But if the value of our position is either high enough to set the lower bound, 
     * or low enough to set the upper bound, it is good to store that information also.
     * 
     */
    enum class NodeType : uint8_t
    {
        FAILED,        // failed entry
        EXACT,         // PV-Node, Score is Exact
        UPPER_BOUND,   // All-Node, Score is Upper Bound
        LOWER_BOUND,   // Cut-Node, Score is Lower Bound
        PERFT,         // Used only for perft command (ignored in the search)

    };

    /**
     * @brief TranspositionTable::Entry
     *
     *  @note Entry in the transposition table
     *  
     *   - key.        
     *   - evaluation. 
     *   - move.       
     *   - node_type.  
     *   - depth.      
     */
    class Entry
    {
    public:
        uint64_t key;         // zobrist key
        int evaluation;       // score of the position
        Move move;            // best move found in position
        NodeType node_type;   // node type
        uint8_t depth;        // depth where the calculation has been done

        constexpr Entry() : key(0ULL), evaluation(0), move(), node_type(NodeType::FAILED), depth(0U) { }

        constexpr Entry(const Entry& entry)
            : key(entry.key), evaluation(entry.evaluation), move(entry.move), node_type(entry.node_type),
              depth(entry.depth)
        { }

        constexpr Entry(uint64_t key, int evaluation, Move move, NodeType node_type, uint8_t depth)
            : key(key), evaluation(evaluation), move(move), node_type(node_type), depth(depth)
        { }

        // check if entry is valid
        inline constexpr bool is_valid() const { return node_type != NodeType::FAILED; }

        // returns a failed entry
        static inline constexpr Entry failed_entry() { return Entry(); }

        constexpr bool operator==(const Entry& other) const
        {
            return key == other.key && evaluation == other.evaluation && move == other.move &&
                node_type == other.node_type && depth == other.depth;
        }

        constexpr bool operator!=(const Entry& other) const { return !(*this == other); }

        constexpr Entry& operator=(const Entry& other)
        {
            if (this != &other)   // not a self-assignment
            {
                this->key = other.key;
                this->evaluation = other.evaluation;
                this->move = other.move;
                this->node_type = other.node_type;
                this->depth = other.depth;
            }
            return *this;
        }
    };
};
