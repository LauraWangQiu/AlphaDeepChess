/**
 * @file uci.cpp
 * @brief uci services.
 *
 * uci implementation. 
 * 
 *  Uci protocol specifications
 *  https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf#file-uci-protocol-specification-txt
 * 
 */

#include "uci.hpp"
#include "history.hpp"
#include "evaluation.hpp"
#include "move_generator.hpp"
#include "perft.hpp"
#include "transposition_table.hpp"
#include <cassert>
#include <iostream>

/**
 * @brief Start position in FEN format.
 *
 * This constant represents the starting position of a chess game in 
 * Forsyth-Edwards Notation (FEN). It is used to initialize the chessboard
 * to the standard starting position.
 */
constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

/**
 * @brief loop
 * 
 * Uci main loop.
 * 
 */
void Uci::loop()
{
    static TokenArray tokens;

    std::string line;

    bool exit = false;

    board.load_fen(StartFEN);

    do {
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        size_t num_tokens = 0;
        size_t pos = 0;
        while (pos < line.length() && num_tokens < TOKEN_ARRAY_SIZE) {
            size_t start = pos;
            while (pos < line.length() && line[pos] != ' ')
                ++pos;
            if (pos > start) {
                tokens[num_tokens++] = std::string_view(line.data() + start, pos - start);
            }
            while (pos < line.length() && line[pos] == ' ')
                ++pos;
        }
        if (num_tokens == 0) continue;

        if (tokens.empty()) {
            continue;
        }

        std::string_view command = tokens[0];

        if (command == "uci") {
            uci_command_action();
        }
        else if (command == "isready") {
            is_ready_command_action();
        }
        else if (command == "ucinewgame") {
            new_game_command_action();
        }
        else if (command == "g" || command == "go") {
            go_command_action(tokens, num_tokens);
        }
        else if (command == "s" || command == "stop") {
            stop_command_action();
        }
        else if (command == "ponderhit") {
            ponderhit_command_action();
        }
        else if (command == "e" || command == "eval") {
            eval_command_action();
        }
        else if (command == "perft") {
            try {
                uint64_t depth = stoull(std::string(tokens[1]));
                perft_command_action(depth);
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : perft depth\n";
            }
        }
        else if (command == "p" || command == "position") {
            if (!position_command_action(tokens, num_tokens)) {
                std::cout << "error in setting the position\n";
            }
        }
        else if (command == "setoption") {
            if (!setoption_command_action(tokens, num_tokens)) {
                std::cout << "error in setoption command\n";
            }
        }
        else if (command == "d" || command == "diagram") {
            diagram_command_action();
        }
        else if (command == "h" || command == "help") {
            help_command_action();
        }
        else if (command == "q" || command == "quit" || command == "exit") {
            quit_command_action();
            exit = true;
        }
        else {
            unknown_command_action();
        }

    } while (!exit);
}

/**
 * @brief uci_command_action
 * 
 * @note tell engine to use the uci (universal chess interface),
 *       this will be sent once as a first command after program boot.
 * 
 * respond with "uciok"
 * 
 */
void Uci::uci_command_action() const
{
    std::cout << "id name AlphaDeepChess" << "\n";
    std::cout << "id author Juan Giron and Laura Wang" << "\n";
    std::cout << "uciok" << std::endl;
}

/**
 * @brief is_ready_command_action
 * 
 * @note  this command can be used to wait for the engine to be ready again or
 *        to ping the engine to find out if it is still alive.
 * 
 * Responds with "readyok"
 * 
 */
void Uci::is_ready_command_action() const { std::cout << "readyok" << std::endl; }

/**
 * @brief new_game_command_action
 * 
 * Uci main loop.
 * 
 */
void Uci::new_game_command_action()
{
    board.load_fen(StartFEN);
    History::clear();
    History::push_position(board.state().get_zobrist_key());
}

/**
 * @brief go_command_action
 * 
 * Starts the search in the searchThread.
 * 
 */
void Uci::go_command_action(const TokenArray& tokens, uint32_t num_tokens)
{
    // stop and wait for previous search to end
    stop_command_action();

    uint32_t depth = INF_DEPTH;
    uint32_t movetime = 0, wtime = 0, btime = 0, winc = 0, binc = 0;

    // Parse the command line arguments
    for (uint32_t i = 1; i < num_tokens; ++i) {
        if (tokens[i].empty()) {
            continue;
        }
        else if (tokens[i] == "movetime") {
            try {
                movetime = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "wtime") {
            try {
                wtime = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "btime") {
            try {
                btime = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "winc") {
            try {
                winc = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "binc") {
            try {
                binc = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "depth") {
            try {
                depth = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
                return;
            }
        }
        else if (tokens[i] == "infinite") {
            depth = INF_DEPTH;
        }
        else if (tokens[i] == "ponder") {
            //pondering.store(true);
        }
        else if (tokens[i] == "perft") {
            try {
                depth = std::stoul(std::string(tokens[++i]));
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go " << tokens[2] << "\n";
            }
            perft_command_action(depth);
            return;
        }
        else if (tokens[i] == "movestogo" || tokens[i] == "nodes" || tokens[i] == "mate") {
            std::cout << "Ignored argument for go: " << tokens[i] << " " << tokens[i] << "\n";
            i += 1;
        }
        else {
            std::cout << "Invalid argument for command : go " << tokens[i] << "\n";
            return;
        }
    }

    const ChessColor side_to_move = board.state().side_to_move();

    // Launch a new thread to search for the best move
    searchThread = std::thread([this, depth]() { search(stop_signal, searchResults, board, depth); });

    readerThread = std::thread([this]() {
        uint32_t depthReaded = 0;

        while (!stop_signal || (depthReaded < searchResults.depthReached)) {

            while (depthReaded < searchResults.depthReached) {
                const SearchResult& result = searchResults.results[depthReaded++];

                std::cout << "info depth " << result.depth << " score cp " << result.evaluation << " bestMove "
                          << Move(result.bestMove_data).to_string() << std::endl;
            }

            {
                std::unique_lock<std::mutex> lock(searchResults.mtx_data_available_cv);

                if (!stop_signal && depthReaded >= searchResults.depthReached) {
                    // thread goes to sleep until more data is available or search stop
                    searchResults.data_available_cv.wait(lock);
                }
            }
        }

        if (pondering.load()) {
            std::unique_lock<std::mutex> lock(ponderhitMutex);

            ponderhitCv.wait(lock);
        }

        std::cout << "bestmove " << Move(searchResults.results[depthReaded - 1].bestMove_data).to_string() << std::endl;
        //std::cout << " ponder " << Move(searchResults.ponderMove_data).to_string() << std::endl;

        searchResults.depthReached = 0;   // reset depthReached when we consume all data
    });

    if (movetime != 0 || wtime != 0 || btime != 0) {
        timerThread = std::thread([this, side_to_move, movetime, wtime, btime, winc, binc]() {
            if (pondering.load()) {
                std::unique_lock<std::mutex> lock(ponderhitMutex);

                ponderhitCv.wait(lock);
            }
            {
                std::unique_lock<std::mutex> lock(timerMutex);

                const uint32_t time = think_time(side_to_move, movetime, wtime, btime, winc, binc);

                // Wait for n milliseconds or until search_stopped becomes true
                if (!timerCv.wait_for(lock, std::chrono::milliseconds(time),
                                      [this] { return this->stop_signal.load(); })) {
                    // Timeout occurred and search_stopped is still false
                    lock.unlock();   // Unlock before calling stop_search()
                    stop_signal.store(true);
                }
            }
        });
    }
}

/**
 * @brief stop_command_action
 * 
 * Stops the search in the searchThread.
 * 
 */
void Uci::stop_command_action()
{
    stop_signal.store(true);

    timerCv.notify_one();

    pondering.store(false);
    ponderhitCv.notify_all();

    if (searchThread.joinable()) {
        searchThread.join();
    }

    if (readerThread.joinable()) {
        readerThread.join();
    }

    if (timerThread.joinable()) {
        timerThread.join();
    }
    stop_signal.store(false);
}

/**
 * @brief eval_command_action
 * 
 * Evaluate chess position.
 * 
 */
void Uci::eval_command_action() { std::cout << "Evaluation: " << evaluate_position(board) << std::endl; }

/**
 * @brief position_command_action
 * 
 * Set the position in the board specified in fen notation.
 * 
 *  @note position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
 * 
 *	set up the position described in fenstring on the internal board and
 *	play the moves on the internal chess board.
 *	if the game was played  from the start position the string "startpos" will be sent
 *	Note: no "new" command is needed. However, if this position is from a different game than
 *	the last position sent to the engine, the GUI should have sent a "ucinewgame" inbetween.
 *
 * @param[in] tokens buffer array with the user input tokens.
 * @param[in] num_tokens number of tokens.
 * 
 *  @return 
 *      - TRUE if success.
 *      - FALSE if error detected, probably error in user input.
 */
bool Uci::position_command_action(const TokenArray& tokens, uint32_t num_tokens)
{
    if (num_tokens < 2) {
        return false;
    }
    stop_command_action();

    uint32_t token_i = 1;

    if (tokens[token_i] == "startpos") {
        board.load_fen(StartFEN);
        History::clear();
        History::push_position(board.state().get_zobrist_key());
        token_i++;
    }
    else if (tokens[token_i] == "actualpos") {
        token_i++;
    }
    else if (tokens[token_i] == "fen") {
        token_i++;
        if (num_tokens < 3) {
            return false;
        }
        // parse the fen
        std::string fen;
        fen.reserve(100);   // 100 characters for a fen is enough

        while (token_i < num_tokens && tokens[token_i] != "moves") {
            fen += std::string(tokens[token_i++]) + " ";
        }
        if (fen.empty()) {
            return false;
        }

        fen.pop_back();   // remove last " "

        board.load_fen(fen);
        History::clear();
        History::push_position(board.state().get_zobrist_key());
    }
    else {
        return false;
    }

    // parse moves
    if (token_i < num_tokens && tokens[token_i++] == "moves") {

        MoveList move_list;
        while (token_i < num_tokens) {
            Move move = create_move_from_string(tokens[token_i++], board);
            if (!move.is_valid()) {
                return false;
            }
            board.make_move(move);
            History::push_position(board.state().get_zobrist_key());
        }
    }


    return true;
}

/**
 * @brief diagram_command_action
 * 
 * Prints the board and fen of the position.
 * 
 */
void Uci::diagram_command_action() const { std::cout << board << std::endl; }

/**
 * @brief help_command_action
 * 
 * Shows help information about commands.
 * 
 */
void Uci::help_command_action() const
{
    std::cout << "Commands:\n"
                 "----------------------------------------\n"
                 "uci\n"
                 "\tTell engine to use the UCI (Universal Chess Interface).\n"
                 "\tThe engine must respond with 'uciok'.\n\n"

                 "isready\n"
                 "\tSynchronize the engine with the GUI. The engine must respond with 'readyok'.\n\n"

                 "ucinewgame\n"
                 "\tStart of a new game.\n\n"

                 "position [fen <fenstring> | startpos | actualpos] moves <move1> .... <movei>\n"
                 "\tSet up the position on the internal board.\n\n"
                 "\t\tMove format:\n\n"
                 "\t\tThe move format is in long algebraic notation.\n"
                 "\t\tA nullmove from the Engine to the GUI should be sent as 0000.\n"
                 "\t\tExamples:  e2e4, e7e5, e1g1 (white short castling), e7e8q (for promotion)\n\n"

                 "go [depth <depth> | infinite | perft <perft_depth>]\n"
                 "[wtime <ms>] [btime <ms>] [winc <ms>] [binc <ms>] [movetime <ms>]\n"
                 "[nodes <x>] [mate <x>] [movestogo <x>]\n"
                 "\tStart calculating the best move until the specified depth.\n"
                 "\tIn order to finish search use stop command, \n\n"

                 "setoption name <id> value <value>\n"
                 "\tChange internal parameters of the chess engine \n"
                 "\t\tsetoption name Hash value <hash_table_size_mb_power_of_two>\n\n"

                 "stop\n"
                 "\tStop calculating.\n\n"

                 "quit\n"
                 "\tQuit the program.\n\n"

                 "perft depth\n"
                 "\tExecutes perft test to the desired depth.\n\n"

                 "d\n"
                 "\tDisplay the current position on the board.\n\n"

              << std::endl;
}

/**
 * @brief quit_command_action
 * 
 * Exits program.
 * 
 */
void Uci::quit_command_action()
{
    stop_command_action();
    std::cout << "goodbye" << std::endl;
}

/**
 * @brief perft_command_action
 * 
 * Executes perft test of the actual position.
 * 
 * @param[in] depth desired depth of the test.
 * 
 */
void Uci::perft_command_action(uint64_t depth) const
{
    int64_t time = 0;
    uint64_t nodes = 0U;
    MoveNodesList moveNodesList;

    perft(board.fen(), depth, moveNodesList, time);

    std::cout << '\n';

    for (const auto& moveNode : moveNodesList) {
        std::cout << moveNode.first.to_string() << ": " << moveNode.second << std::endl;
        nodes += moveNode.second;
    }
    std::cout << "\nNodes searched: " << nodes << "\nExecution time: " << time << " ms" << std::endl;
}

/**
 * @brief setoption_command_action
 * 
 * Setoption command detected, use to change internal parameters of the engine.
 * 
 * @param[in] tokens buffer array with the user input tokens.
 * @param[in] num_tokens number of tokens.
 * 
 *  @return 
 *      - TRUE if success.
 *      - FALSE if error detected, probably error in user input.
 */
bool Uci::setoption_command_action(const TokenArray& tokens, uint32_t num_tokens)
{
    if (num_tokens < 3) {
        std::cout << "Invalid setoption argument: setoption name <id> value\n";
        return false;
    }
    uint32_t token_i = 1;

    if (tokens[token_i++] != "name")
    {
        std::cout << "Invalid setoption argument: setoption name <id> value\n";
        return false;
    }
    
    if (tokens[token_i++] == "Hash") {

        if (tokens[token_i++] != "value")
        {
            std::cout << "Invalid setoption Hash argument: setoption name Hash value <hash_table_size_mb_power_of_two>\n";
            return false;
        }

        try {
            uint32_t size_mb = stoul(std::string(tokens[token_i++]));
            TranspositionTable::SIZE size_tt = TranspositionTable::int_to_tt_size(size_mb);

            if (size_tt != TranspositionTable::SIZE::INVALID) {
                TranspositionTable::resize(size_tt);
            }
            else {
                std::cout << "Invalid setoption Hash argument: setoption name Hash value <hash_table_size_mb_power_of_two>\n";
            }

        } catch (const std::exception& e) {
            std::cout << "Invalid setoption Hash argument: setoption name Hash value <hash_table_size_mb_power_of_two>\n";
            return false;
        }
    }
    else {
        std::cout << "Invalid setoption argument: setoption name <id> value\n";
        return false;
    }

    return true;
}

/**
 * @brief ponderhit_command_action
 * 
 * ponderhit command detected.
 * 
 */
void Uci::ponderhit_command_action()
{
    pondering.store(false);
    ponderhitCv.notify_all();
}

/**
 * @brief unknown_command_action
 * 
 * Invalid command detected.
 * 
 */
void Uci::unknown_command_action() const
{
    std::cout << "Unknown command, type help for more information" << std::endl;
}

/**
 * @brief create_move_from_string
 * 
 * Create the move from string representation and board actual position.
 * Check in the board if the move will be a capture, en passant, promotion.
 * 
 *  @note Move format:
 *
 *     The move format is in long algebraic notation.
 *     A nullmove from the Engine to the GUI should be sent as 0000.
 *     Examples:  e2e4, e7e5, e1g1 (white short castling), e7e8q (for promotion)
 *
 * @param[in] move_string string move representation (e.g a7a8Q : pawn move and queen promotion).
 * @param[in] board chess position.
 * 
 *  @return 
 *      - Move valid if success.
 *      - Move::null() if error detected, bad string move representation.
 */
Move Uci::create_move_from_string(std::string_view move_string, const Board& board) const
{
    const uint32_t string_lenght = move_string.length();
    // move string should have 4 or 5 characters
    if (string_lenght < 4 || string_lenght > 5) {
        return Move::null();
    }

    const Square sq_origin(move_string[0], move_string[1]);
    const Square sq_end(move_string[2], move_string[3]);

    if (sq_origin == Square::INVALID || sq_end == Square::INVALID) {
        return Move::null();
    }

    // check for castling move
    if (piece_to_pieceType(board.get_piece(sq_origin)) == PieceType::KING) {
        if (sq_origin == Square::E1 && sq_end == Square::G1) {
            return Move::castle_white_king();
        }
        else if (sq_origin == Square::E8 && sq_end == Square::G8) {
            return Move::castle_black_king();
        }
        else if (sq_origin == Square::E1 && sq_end == Square::C1) {
            return Move::castle_white_queen();
        }
        else if (sq_origin == Square::E8 && sq_end == Square::C8) {
            return Move::castle_black_queen();
        }
    }

    PieceType promo_piece = PieceType::KNIGHT;
    MoveType move_type = MoveType::NORMAL;

    // check for promotion move
    if (string_lenght == 5) {
        promo_piece = char_to_pieceType(move_string[4]);
        if (promo_piece == PieceType::EMPTY) {
            return Move::null();
        }
        move_type = MoveType::PROMOTION;
    }

    // check for en Passant
    if (piece_to_pieceType(board.get_piece(sq_origin)) == PieceType::PAWN &&
        board.state().en_passant_square() == sq_end) {

        move_type = MoveType::EN_PASSANT;
    }
    // Create and return the Move object
    return Move(sq_origin, sq_end, move_type, promo_piece);
}

/**
 * @brief calculate the time the bot has until stopping the search
 * 
 * @note pass 0 to not provide an argument
 * 
 * @param[in] us side to move
 * @param[in] movetime explicit think time (if provided it will be the think time)
 * @param[in] wtime white total time
 * @param[in] btime black total time
 * @param[in] winc white time increment per move
 * @param[in] binc black time increment per move
 * 
 *  @return (uint32_t) time to think
 * 
 */
uint32_t Uci::think_time(ChessColor us, uint32_t movetime, uint32_t wtime, uint32_t btime, uint32_t winc, uint32_t binc)
{

    if (movetime != 0) {
        return movetime;   // think time is explicitly provided
    }

    const uint32_t remaining_time = (us == ChessColor::WHITE) ? wtime : btime;
    const uint32_t increment = (us == ChessColor::WHITE) ? winc : binc;

    if (remaining_time >= 60000)   // time >= 1min
    {
        return 10000;   // think 5 seconds
    }
    else if (remaining_time >= 10000)   // 1min > time >= 10 seconds
    {
        return increment && increment < remaining_time ? increment : 3000;   // think the increment time or 3 seconds
    }
    else if (remaining_time >= 1000)   // 10 seconds > time >= 1 second
    {
        return increment && increment < remaining_time ? increment : 1000;   // think the increment time or 1 seconds
    }
    else if (remaining_time >= 200)   // 1 seconds > time >= 0.2 second
    {
        return 100;   // think 0.1 seconds
    }
    else {
        return 10;   // think 0.01 seconds
    }

    return 0;
}