import subprocess
import platform
import argparse

python_cmd = "python3" if platform.system() == "Linux" else "python"

def parse_options(engine_name, options_str):
    options = {}
    if options_str:
        for option in options_str.split(","):
            try:
                key, value = option.split("=")
                options[key.strip()] = value.strip()
            except ValueError:
                print(f"Invalid option format: {option}. Expected format is 'key=value'.")
    return {engine_name: options}

def get_engines_and_options(test_id, engine2, stockfish_options2):
    engines = []
    options = {}

    if test_id == 0:
        # Custom test 0: engine1 vs engine2
        engines = [
            "AlphaDeepChess_1",
            "AlphaDeepChess_2" if engine2 == "AlphaDeepChess" else "Stockfish"
        ]
        if engine2 == "Stockfish":
            options.update(parse_options("Stockfish", stockfish_options2))
    elif test_id == 1:
        # Test 1: AlphaDeepChess_search_basic vs AlphaDeepChess_search_multithread
        engines = ["AlphaDeepChess_search_basic", "AlphaDeepChess_search_multithread"]
    elif test_id == 2:
        # Test 2: AlphaDeepChess_search_basic vs AlphaDeepChess_search_transposition_table
        engines = ["AlphaDeepChess_search_basic", "AlphaDeepChess_search_transposition_table"]
    elif test_id == 3:
        # Test 3: search_transposition_table vs search_tt_reductions
        engines = ["AlphaDeepChess_search_transposition_table", "AlphaDeepChess_search_tt_reductions"]
    elif test_id == 4:
        # Test 4: move_generator_basic vs move_generator_magic_bitboards
        engines = ["AlphaDeepChess_move_generator_basic", "AlphaDeepChess_move_generator_magic_bitboards"]
    elif test_id == 5:
        # Test 5: evaluation_dynamic vs evaluation_safety_mobility
        engines = ["AlphaDeepChess_evaluation_dynamic", "AlphaDeepChess_evaluation_safety_mobility"]
    elif test_id == 6:
        # Test 6: Elo Performance Test vs Stockfish (1500 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options.update(parse_options("Stockfish", "UCI_LimitStrength=true,UCI_Elo=1500"))
    elif test_id == 7:
        # Test 7: Elo Performance Test vs Stockfish (1800 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options.update(parse_options("Stockfish", "UCI_LimitStrength=true,UCI_Elo=1800"))
    elif test_id == 8:
        # Test 8: Elo Performance Test vs Stockfish (2000 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options.update(parse_options("Stockfish", "UCI_LimitStrength=true,UCI_Elo=2000"))
    elif test_id == 9:
        # Test 9: Elo Performance Test vs Stockfish (2200 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options.update(parse_options("Stockfish", "UCI_LimitStrength=true,UCI_Elo=2200"))
    else:
        print(f"Invalid test_id: {test_id}")
        exit(1)

    return engines, options
    
def test(test_id, engine2, stockfish_options2, games=None, st=None, tc=None, depth=None, timemargin=None, concurrency=None, book=None, positions=None, pgn=None, epd=None, log=None):
    engines, options = get_engines_and_options(test_id, engine2, stockfish_options2)

    print(f"Running test ({test_id})")
    print(f"Engines: {engines}")
    print(f"Options: {options}")
    print(f"Games: {games}, Search Time: {st}, Depth: {depth}, Time Control: {tc}, Time margin: {timemargin}, Concurrency: {concurrency}")
    print(f"Book: {book}")
    print(f"Positions: {positions}")
    print(f"PGN File: {pgn}, EPD File: {epd}, Log File: {log}")

    # Prepare options list for engines
    options_list = []
    if options:
        for engine, engine_options in options.items():
            for option_name, option_value in engine_options.items():
                options_list.append(f"{engine}.{option_name}={option_value}")

    cmd = [
        python_cmd, "./test/manual_tests/compareEngines.py",
    ]
    if games:
        cmd += ["--games", str(games)]
    if st:
        cmd += ["--st", str(st)]
    if tc and tc != "inf":
        cmd += ["--tc", str(tc)]
    if depth:
        cmd += ["--depth", str(depth)]
    if timemargin:
        cmd += ["--timemargin", str(timemargin)]
    if concurrency:
        cmd += ["--concurrency", int(concurrency)]
    if book:
            cmd += ["--book", book]
    if positions:
            cmd += ["--positions", positions]
    if pgn:
        cmd += ["--pgn", pgn]
    if epd:
        cmd += ["--epd", epd]
    if log:
        cmd += ["--log", log]

    cmd += ["--engines"] + engines + ["--options"] + options_list

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate()

    print(f"STDOUT: {stdout}")
    print(f"STDERR: {stderr}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run specific tests.")
    parser.add_argument("--test-id", type=int, required=True, help="Specify the test ID to run (0-7)")
    parser.add_argument("--games", type=int, default=10, help="Number of games to play")
    parser.add_argument("--st", type=int, default=5, help="Search time per move (in seconds)")
    parser.add_argument("--tc", type=str, help="Time control (e.g., 40/60 or inf for unlimited")
    parser.add_argument("--depth", type=int, help="Search depth")
    parser.add_argument("--timemargin", type=int, default=500, help="Time margin in milliseconds for each move (Cutechess -timemargin)")
    parser.add_argument("--concurrency", type=int, help="Concurrency (Number of threads)")
    parser.add_argument("--engine2", type=str, required=True, choices=["AlphaDeepChess", "Stockfish"], help="Engine 2 selection")
    parser.add_argument("--stockfish-options2", type=str, default="", help="Options for Stockfish engine 2 (e.g., 'UCI_LimitStrength=true,UCI_Elo=2000')")
    parser.add_argument("--book", type=str, help="PGN file with book openings")
    parser.add_argument("--positions", type=str, help="FEN file with positions")
    parser.add_argument("--pgn", type=str, default="results.pgn", help="PGN output filename")
    parser.add_argument("--epd", type=str, default="results.epd", help="EPD output filename")
    parser.add_argument("--log", type=str, default="results.log", help="Log output filename")
    args = parser.parse_args()

    test(
        test_id=args.test_id,
        engine2=args.engine2,
        stockfish_options2=args.stockfish_options2,
        games=args.games,
        st=args.st,
        tc=args.tc,
        depth=args.depth,
        timemargin=args.timemargin,
        concurrency=args.concurrency,
        book=args.book,
        positions=args.positions,
        pgn=args.pgn,
        epd=args.epd,
        log=args.log
    )
