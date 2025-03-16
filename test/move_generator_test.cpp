#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <thread>
#include <sstream>
#include <mutex>
#include "perft.hpp"
#include "transposition_table.hpp"

static constexpr auto RESET_COLOR = "\033[0m";
static constexpr auto RED_COLOR = "\033[31m";
static constexpr auto GREEN_COLOR = "\033[32m";
static constexpr auto YELLOW_COLOR = "\033[33m";
static constexpr auto BLUE_COLOR = "\033[34m";
static constexpr auto MAGENTA_COLOR = "\033[35m";
static constexpr auto CYAN_COLOR = "\033[36m";
static constexpr auto WHITE_COLOR = "\033[37m";

struct PerftEntry
{
    std::string fen;
    std::vector<uint64_t> perftResults;
};

static const std::unordered_map<std::string, PerftEntry> perftResults = {
    {"FEN_START_POS",
     {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", {1, 20, 400, 8902, 197281, 4865609, 119060324}}},
    {"FEN_KIWIPETE",
     {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
      {1, 48, 2039, 97862, 4085603, 193690690, 8031647685}}},
    {"FEN_TALKCHESS",
     {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
      {1, 44, 1486, 62379, 2103487, 89941194, 3048196529}}},
    {"FEN_EDWARDS2",
     {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
      {1, 46, 2079, 89890, 3894594, 164075551, 6923051137}}},
    {"FEN_TEST4",
     {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
      {1, 6, 264, 9467, 422333, 15833292, 706045033}}},
    {"FEN_TEST4_MIRROR",
     {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
      {1, 6, 264, 9467, 422333, 15833292, 706045033}}},
    {"FEN_STRANGEMOVES",
     {"r3k2r/p1pppp1p/N3Q3/qpP2N2/5Kp1/8/Pp2p1pP/R6R b kq - 1 2",
      {1, 48, 2100, 86876, 3423970, 140410357, 5160619771}}}};

void move_generator_test()
{
    std::cout << "---------move generator test---------\n\n";

    TranspositionTable::resize(TranspositionTable::SIZE::MB_256);

    const int MAX_DEPTH = 5;

    // Collect keys in a vector to assign to threads
    std::vector<std::string> keys;
    for (const auto& pair : perftResults) {
        keys.push_back(pair.first);
    }

    // Vector to store each thread's output
    std::mutex cout_mutex;
    int threads_finished = 0;

    // Launch threads
    std::vector<std::thread> threads;
    for (size_t i = 0; i < keys.size(); ++i) {
        threads.emplace_back([i, &keys, &cout_mutex, &threads_finished]() {
            const auto& key = keys[i];
            const auto& entry = perftResults.at(key);
            std::ostringstream oss;
            oss << "\n\n" << key;

            for (size_t depth = 1; depth <= MAX_DEPTH; depth++) {
                int64_t time = 0;
                uint64_t nodes = 0U;
                MoveNodesList moveNodesList;

                perft(entry.fen, depth, moveNodesList, time);

                oss << '\n';

                for (const auto& moveNode : moveNodesList) {
                    nodes += moveNode.second;
                }

                oss << "Depth " << depth << " : ";
                oss << "Nodes " << YELLOW_COLOR << nodes << RESET_COLOR;
                oss << " Execution time: " << time << " ms  ";

                if (nodes == entry.perftResults[depth]) {
                    oss << GREEN_COLOR << "correct" << RESET_COLOR;
                }
                else {
                    oss << RED_COLOR << "incorrect " << RESET_COLOR << "should be: " << YELLOW_COLOR
                        << entry.perftResults[depth] << RESET_COLOR << '\n';

                    for (const auto& moveNode : moveNodesList) {
                        oss << moveNode.first.to_string() << ": " << moveNode.second << '\n';
                    }
                }
            }
            const std::string output = oss.str();

            {
                std::lock_guard<std::mutex> lock(cout_mutex);   // Synchronize access to std::cout
                std::cout << output << "\n";
                std::cout << "\nthreads finished " << ++threads_finished << "/" << keys.size() << std::endl;
            }
        });
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }
}