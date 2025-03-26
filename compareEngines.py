import os
import platform
import subprocess
import argparse
import re
import json

default_comparator_dir = "enginesComparator"

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

def main(pgn_path, edp_path, log_path, build_type, games, tc, st, depth, timemargin, concurrency, options):
    os_name = platform.system()
    is_windows = os_name == "Windows"
    is_linux = os_name == "Linux"
    is_macos = os_name == "Darwin"

    if is_windows:
        cutechess_internal_dir = "cutechess-1.3.1-win64"
        stockfish_internal_dir = "stockfish-windows-x86-64"
    elif is_linux:
        cutechess_internal_dir = "cutechess-cli"
        stockfish_internal_dir = "stockfish-ubuntu-x86-64"
    elif is_macos:
        cutechess_internal_dir = "build"
        stockfish_internal_dir = "stockfish-macos-x86-64"
    cutechess_dir = os.path.join("cutechess", cutechess_internal_dir)

    os.chdir(default_comparator_dir)

    options_dict = parse_options(options)

    engines_config_path = "engines.json"
    engines_config_content = [
        {
            "name": "AlphaDeepChess",
            "command": f"../build/{build_type}/AlphaDeepChess",
            "protocol": "uci",
            "options": options_dict.get("AlphaDeepChess", {}),
        },
        {
            "name": "Stockfish",
            "command": f"./stockfish/stockfish/{stockfish_internal_dir}",
            "protocol": "uci",
            "options": options_dict.get("Stockfish", {}),
        }
    ]

    with open(engines_config_path, 'w', encoding='utf8') as f:
        json.dump(engines_config_content, f, indent=4)

    cutechess_cmd = [
        os.path.join(cutechess_dir, "cutechess-cli"),
        "-engine", "conf=AlphaDeepChess",
        "-engine", "conf=Stockfish",
    ]

    for engine in engines_config_content:
        engine_options = []
        if "options" in engine:
            for option, value in engine["options"].items():
                engine_options.append(f"option.{option}={value}")
        if engine_options:
            cutechess_cmd.extend(engine_options)

    cutechess_cmd.extend([
        "-each",
        f"tc={tc}" if tc else "",
        f"st={st}" if st else "",
        f"depth={depth}" if depth else "",
        f"timemargin={timemargin}" if timemargin else "",
        f"concurrency={concurrency}" if concurrency else "",
        "-games", str(games),
        "-pgnout", pgn_path,
        "-epdout", edp_path,
        "-debug"
    ])

    with open(log_path, 'w') as log_file:
        process = subprocess.Popen(cutechess_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        for line in process.stdout:
            print(line, end='')
            if not re.match(r'^\d', line):
                log_file.write(line)

        process.wait()

    print(f"PGN saved in {pgn_path}.")
    print(f"EPD saved in {edp_path}.")
    print(f"Log saved in {log_path}.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare chess engines.")
    parser.add_argument("-pgn", type=str, default="results.pgn", help="PGN file path")
    parser.add_argument("-epd", type=str, default="results.epd", help="EPD file path")
    parser.add_argument("-log", type=str, default="results.log", help="Log file")
    parser.add_argument("-buildType", type=str, default="Release", help="Build type")
    parser.add_argument("-games", type=int, default=10, help="Number of games")
    parser.add_argument("-tc", type=str, help="Time control")
    parser.add_argument("-st", type=int, help="Search time")
    parser.add_argument("-timemargin", default=500, type=int, help="Time margin")
    parser.add_argument("-depth", type=int, help="Search depth")
    parser.add_argument("-concurrency", type=int, help="Concurrency level")
    parser.add_argument(
        "-options",
        nargs="*",
        help="List of UCI options for engines in the format 'engine_name.option_name=value' (e.g., 'Stockfish.UCI_LimitStrength=true AlphaDeepChess.UCI_Elo=1500')"
    )
    args = parser.parse_args()

    main(args.pgn, args.epd, args.log, args.buildType, args.games, args.tc, args.st, args.depth, args.timemargin, args.concurrency, args.options)