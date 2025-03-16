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

#include "evaluation.hpp"
#include "move_generator.hpp"
#include "perft.hpp"
#include "transposition_table.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

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

    std::string command;
    std::string line;

    bool exit = false;

    board.load_fen(StartFEN);

    do {
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);

        std::string token;

        int num_tokens = 0;

        while ((num_tokens < TOKEN_ARRAY_SIZE) && (iss >> token)) {
            tokens[num_tokens++] = token;
        }

        if (tokens.empty()) {
            continue;
        }

        command = tokens[0];

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
            go_command_action(tokens);
        }
        else if (command == "s" || command == "stop") {
            stop_command_action();
        }
        else if (command == "e" || command == "eval") {
            eval_command_action();
        }
        else if (command == "perft") {
            try {
                uint64_t depth = stoull(tokens[1]);
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
        // Clear the tokens array
        tokens.fill("");
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
void Uci::uci_command_action() const { std::cout << "uciok" << std::endl; }

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
void Uci::new_game_command_action() { board.load_fen(StartFEN); }

/**
 * @brief go_command_action
 * 
 * Starts the search in the searchThread.
 * 
 */
void Uci::go_command_action(const TokenArray& tokens)
{
    uint32_t depth = INFINITE_DEPTH;

    // Parse the command line arguments
    for (uint32_t i = 1; i < tokens.size(); ++i) {
        if (tokens[i].empty()) {
            continue;
        }
        else if (tokens[i] == "depth") {
            try {
                depth = std::stoul(tokens[++i]);
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go depth\n";
                return;
            }
        }
        else if (tokens[i] == "infinite") {
            depth = INFINITE_DEPTH;
        }
        else if (tokens[i] == "perft") {
            try {
                depth = std::stoul(tokens[++i]);
            } catch (const std::exception& e) {
                std::cout << "Invalid argument for command : go perft\n";
            }
            perft_command_action(depth);
            return;
        }
        else {
            std::cout << "Invalid argument for command : go ( " << tokens[i] << " )\n";
            return;
        }
    }

    // stop and wait for previous search to end
    search_stop();

    if (searchThread.joinable()) {
        searchThread.join();
    }
    if (readerThread.joinable()) {
        readerThread.join();
    }

    // Launch a new thread to search for the best move
    searchThread = std::thread([this, depth]() { search_best_move(searchResults, board, depth); });

    readerThread = std::thread([this]() {
        int depthReaded = 0;

        std::cout << "Search started" << std::endl;

        while (is_search_running() || (depthReaded < searchResults.depthReached)) {

            while (depthReaded < searchResults.depthReached) {
                const SearchResult& result = searchResults.results[depthReaded++];

                std::cout << "info depth " << result.depth << " score " << result.evaluation << " bestMove "
                          << Move(result.bestMove_data).to_string() << std::endl;
            }

            {
                std::unique_lock<std::mutex> lock(searchResults.mtx_data_avaliable_cv);

                if (is_search_running() && depthReaded >= searchResults.depthReached) {
                    // thread goes to sleep until more data is avaliable or search stop
                    searchResults.data_avaliable_cv.wait(lock);
                }
            }
        }

        std::cout << "Search finished" << std::endl;
    });
}

/**
 * @brief stop_command_action
 * 
 * Stops the search in the searchThread.
 * 
 */
void Uci::stop_command_action()
{
    search_stop();

    if (searchThread.joinable()) {
        searchThread.join();
    }

    if (readerThread.joinable()) {
        readerThread.join();
    }
}

/**
 * @brief eval_command_action
 * 
 * Evaluate chess position.
 * 
 */
void Uci::eval_command_action() const { std::cout << "Evaluation: " << evaluate_position(board) << std::endl; }

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
    uint32_t token_i = 1;

    if (tokens[token_i] == "startpos") {
        board.load_fen(StartFEN);
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
        std::string fen = "";

        while (token_i < num_tokens && tokens[token_i] != "moves") {
            fen += tokens[token_i++] + " ";
        }
        if (fen.empty()) {
            return false;
        }

        fen.pop_back();   // remove last " "

        board.load_fen(fen);
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
                 "\tStart calculating the best move until the specified depth.\n"
                 "\tIn order to finish search use stop command, \n\n"

                 "setoption [hash <size_mb_power_of_two>]\n"
                 "\tChange internal parameters of the chess engine \n\n"

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
void Uci::quit_command_action() const { std::cout << "goodbye" << std::endl; }

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
    if (num_tokens < 2) {
        return false;
    }
    uint32_t token_i = 1;

    if (tokens[token_i++] == "hash") {
        try {
            uint32_t size_mb = stoul(tokens[token_i++]);
            TranspositionTable::SIZE size_tt = TranspositionTable::int_to_tt_size(size_mb);

            if (size_tt != TranspositionTable::SIZE::INVALID) {
                TranspositionTable::resize(size_tt);
            }
            else {
                std::cout << "Invalid size for command : setoption hash <mb size power of two>\n";
            }

        } catch (const std::exception& e) {
            std::cout << "Invalid argument for command : setoption hash\n";
            return false;
        }
    }
    else {
        return false;
    }

    return true;
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
Move Uci::create_move_from_string(const std::string& move_string, const Board& board) const
{
    uint32_t string_lenght = move_string.length();
    // move string should have 4 or 5 characters
    if (string_lenght < 4 || string_lenght > 5) {
        return Move::null();
    }

    Square sq_origin(move_string.substr(0, 2));
    Square sq_end(move_string.substr(2, 2));

    if (sq_origin == Square::SQ_INVALID || sq_end == Square::SQ_INVALID) {
        return Move::null();
    }

    // check for castling move
    if (piece_to_pieceType(board.get_piece(sq_origin)) == PieceType::KING) {
        if (sq_end == Square::SQ_G1) {
            return Move::castle_white_king();
        }
        else if (sq_end == Square::SQ_G8) {
            return Move::castle_black_king();
        }
        else if (sq_end == Square::SQ_C1) {
            return Move::castle_white_queen();
        }
        else if (sq_end == Square::SQ_C8) {
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