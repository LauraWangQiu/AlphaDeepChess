import os
import platform
import subprocess
import argparse
import re
import json

default_comparator_dir = "enginesComparator"
engines_config_path = "engines.json"

os_name = platform.system()
is_windows = os_name == "Windows"
is_linux = os_name == "Linux"
is_macos = os_name == "Darwin"

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

def fen_to_pgn_lines(fens):
    lines = []
    for i, fen in enumerate(fens):
        lines.append(f'[Event "FEN {i+1}"]')
        lines.append('[Site "?"]')
        lines.append('[Date "?"]')
        lines.append('[Round "?"]')
        lines.append('[White "?"]')
        lines.append('[Black "?"]')
        lines.append('[Result "*"]')
        lines.append('[SetUp "1"]')
        lines.append(f'[FEN "{fen.strip()}"]')
        lines.append("")
        lines.append("*\n")
    return lines

def generate_pgn_from_fens(input_file, output_file="positions.pgn"):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    comparator_dir = os.path.join(script_dir, "..", "..", default_comparator_dir)
    comparator_dir = os.path.abspath(comparator_dir)

    if not os.path.isabs(input_file):
        input_file = os.path.join(script_dir, input_file)

    if not os.path.exists(comparator_dir):
        raise FileNotFoundError(f"Directory does not exist: {comparator_dir}")
    if not os.path.exists(input_file):
        raise FileNotFoundError(f"Input file does not exist: {input_file}")
    
    with open(input_file, "r") as fin:
        fens = fin.readlines()

    pgn_lines = fen_to_pgn_lines(fens)

    output_path = os.path.join(comparator_dir, output_file)
    with open(output_path, "w") as fout:
        fout.write("\n".join(pgn_lines))

    print(f"PGN file generated: {output_path}")

def compare(engines, options, games, st, depth, tc, timemargin, concurrency, book, positions, pgn, epd, log, build_type):
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

    engines_config_content = []
    for engine in engines:
        engine_config = {
            "name": engine,
            "command": f"../build/{build_type}/{engine}" if engine != "Stockfish" else f"./stockfish/stockfish/{stockfish_internal_dir}",
            "protocol": "uci",
            "options": options_dict.get(engine, {}),
        }
        engines_config_content.append(engine_config)

    with open(engines_config_path, 'w', encoding='utf8') as f:
        json.dump(engines_config_content, f, indent=4)

    cutechess_cmd = [
        os.path.join(cutechess_dir, "cutechess-cli"),
    ]
    for engine in engines_config_content:
        cutechess_cmd.extend(["-engine", f"conf={engine['name']}"])

    for engine in engines_config_content:
        engine_options = []
        if "options" in engine:
            for option, value in engine["options"].items():
                engine_options.append(f"option.{option}={value}")
        if engine_options:
            cutechess_cmd.extend(engine_options)

    cutechess_cmd.append("-each")
    if st:
        cutechess_cmd.append(f"st={st}")
    if tc:
        cutechess_cmd.append(f"tc={tc}")
    if depth:
        cutechess_cmd.append(f"depth={depth}")
    if timemargin:
        cutechess_cmd.append(f"timemargin={timemargin}")
    if concurrency:
        cutechess_cmd.append(f"concurrency={concurrency}")

    cutechess_cmd.extend([
        "-games", str(games),
        "-pgnout", pgn,
        "-epdout", epd,
        "-debug"
    ])
    if book:
            cutechess_cmd.extend(["-openings", f"file={book}", "format=pgn", "order=random"])
    elif positions:
        book_file = "positions.pgn"
        generate_pgn_from_fens(positions, book_file)
        cutechess_cmd.extend(["-openings", f"file={book_file}", "format=pgn", "order=random"])

    with open(log, 'w') as log_file:
        process = subprocess.Popen(cutechess_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        for line in process.stdout:
            print(line, end='')
            log_file.write(line)

        process.wait()

    print(f"PGN saved in {pgn}.")
    print(f"EPD saved in {epd}.")
    print(f"Log saved in {log}.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare chess engines.")
    parser.add_argument("--games", type=int, help="Number of games")
    parser.add_argument("--st", type=int, help="Search time")
    parser.add_argument("--tc", type=str, help="Time control")
    parser.add_argument("--depth", type=int, help="Search depth")
    parser.add_argument("--timemargin", type=int, help="Time margin")
    parser.add_argument("--concurrency", type=int, help="Concurrency level")
    parser.add_argument("--book", type=str, help="Path to the opening book file (e.g., 'openings.pgn')")
    parser.add_argument("--positions", type=str, help="Path to the positions file (e.g., 'positions.fen')")
    parser.add_argument(
        "--engines",
        nargs="+",
        required=True,
        help="List of engine names to test"
    )
    parser.add_argument(
        "--options",
        nargs="*",
        help="List of UCI options for engines in the format 'engine_name.option_name=value' (e.g., 'Stockfish.UCI_LimitStrength=true AlphaDeepChess.UCI_Elo=1500')"
    )
    parser.add_argument("--pgn", type=str, help="PGN file path")
    parser.add_argument("--epd", type=str, help="EPD file path")
    parser.add_argument("--log", type=str, help="Log file")
    parser.add_argument("--buildType", default="release", type=str, help="Build type")
    args = parser.parse_args()

    compare(games=args.games, engines=args.engines, options=args.options, st=args.st, tc=args.tc, depth=args.depth, timemargin=args.timemargin, concurrency=args.concurrency, book=args.book, positions=args.positions, pgn=args.pgn, epd=args.epd, log=args.log, build_type=args.buildType)
