import subprocess
import platform
import argparse

python_cmd = "python3" if platform.system() == "Linux" else "python"

def parse_options(options_list):
    options_dict = {}
    if options_list:
        for option in options_list:
            try:
                engine, key_value = option.split(".", 1)
                key, value = key_value.split("=", 1)
                if engine not in options_dict:
                    options_dict[engine] = {}
                options_dict[engine][key] = value
            except ValueError:
                print(f"Invalid option format: {option}. Expected format is 'engine_name.option_name=value'.")
    return options_dict

def get_engines_and_options(test_id):
    engines = []
    options = {}

    if test_id == 0:
        # Custom test 0: AlphaDeepChess vs Stockfish
        engines = ["AlphaDeepChess", "Stockfish"]
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
        options = {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "1500"
            }
        }
    elif test_id == 7:
        # Test 7: Elo Performance Test vs Stockfish (1800 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options = {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "1800"
            }
        }
    elif test_id == 8:
        # Test 8: Elo Performance Test vs Stockfish (2000 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options = {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "2000"
            }
        }
    elif test_id == 9:
        # Test 9: Elo Performance Test vs Stockfish (2200 Elo)
        engines = ["AlphaDeepChess", "Stockfish"]
        options = {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "2200"
            }
        }
    else:
        print(f"Invalid test_id: {test_id}")
        exit(1)

    return engines, options

def run_test(test_id, games, st, depth, pgn, epd, log):
    engines, options = get_engines_and_options(test_id)

    print(f"Running test ({test_id}) with the following configuration:")
    print(f"Games: {games}, Search Time: {st}, Depth: {depth}")
    print(f"PGN File: {pgn}, EPD File: {epd}, Log File: {log}")
    print(f"Engines: {engines}")
    print(f"Options: {options}")

    # Prepare options list for engines
    options_list = []
    if options:
        for engine, engine_options in options.items():
            for option_name, option_value in engine_options.items():
                options_list.append(f"{engine}.{option_name}={option_value}")

    cmd = [
        python_cmd, "compareEngines.py",
        "-games", str(games),
        "-st", str(st),
        "-depth", str(depth),
        "-pgn", pgn,
        "-epd", epd,
        "-log", log,
        "-engines"
    ] + engines + [
        "-options"
    ] + options_list

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate()

    print(f"STDOUT: {stdout}")
    print(f"STDERR: {stderr}")

def main():
    parser = argparse.ArgumentParser(description="Run specific tests.")
    parser.add_argument("--test-id", type=int, required=True, help="Specify the test ID to run (0-7)")
    parser.add_argument("--games", type=int, default=10, help="Number of games to play")
    parser.add_argument("--st", type=int, default=5, help="Search time per move (in seconds)")
    parser.add_argument("--depth", type=int, default=5, help="Search depth")
    parser.add_argument("--pgn", type=str, default="results.pgn", help="PGN output filename")
    parser.add_argument("--epd", type=str, default="results.epd", help="EPD output filename")
    parser.add_argument("--log", type=str, default="results.log", help="Log output filename")
    args = parser.parse_args()

    run_test(
        test_id=args.test_id,
        games=args.games,
        st=args.st,
        depth=args.depth,
        pgn=args.pgn,
        epd=args.epd,
        log=args.log
    )

if __name__ == "__main__":
    main()
