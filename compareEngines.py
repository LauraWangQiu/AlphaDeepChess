import os
import platform
import subprocess
import json
import argparse
import re

def main(build_type, games, tc, st, timemargin, depth, concurrency):
    os_name = platform.system()
    is_windows = os_name == "Windows"
    is_linux = os_name == "Linux"
    is_macos = os_name == "Darwin"

    default_comparator_dir = "enginesComparator"
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

    engines_config_path = "engines.json"
    engines_config_content = [
        {
            "name": "AlphaDeepChess",
            "command": f"../build/{build_type}/AlphaDeepChess",
            "protocol": "uci"
        },
        {
            "name": "Stockfish",
            "command": f"./stockfish/stockfish/{stockfish_internal_dir}",
            "protocol": "uci"
        }
    ]

    with open(engines_config_path, 'w', encoding='utf8') as f:
        json.dump(engines_config_content, f, indent=4)

    results_path = "results.pgn"
    edp_path = "results.epd"
    log_path = "log.txt"
    cutechess_cmd = [
        os.path.join(cutechess_dir, "cutechess-cli"),
        "-engine", "conf=AlphaDeepChess",
        "-engine", "conf=Stockfish",
        "-each"
    ]

    if depth is not None:
        cutechess_cmd.append(f"depth={depth}")

    if tc is not None:
        cutechess_cmd.append(f"tc={tc}")

    if st is not None:
        cutechess_cmd.append(f"st={st}")

    if timemargin is not None:
        cutechess_cmd.append(f"timemargin={timemargin}")

    if concurrency is not None:
        cutechess_cmd.append(f"concurrency={concurrency}")

    cutechess_cmd.extend([
        "-games", str(games),
        "-pgnout", results_path,
        "-epdout", edp_path,
        "-debug"
    ])

    with open(log_path, 'w') as log_file:
        process = subprocess.Popen(cutechess_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        for line in process.stdout:
            print(line, end='')
            if not re.match(r'^\d', line):
                log_file.write(line)
            # if "illegal move" in line.lower():
            #     print("Illegal move detected. Stopping execution.")
            #     process.terminate()
            #     break

        process.wait()

    print(f"Results saved in {results_path}.")
    print(f"Log saved in {log_path}.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare chess engines.")
    parser.add_argument("-buildType", type=str, default="Release", help="Build type")
    parser.add_argument("-games", type=int, default=10, help="Number of games")
    parser.add_argument("-tc", type=str, help="Time control")
    parser.add_argument("-st", type=int, help="Search time")
    parser.add_argument("-timemargin", default=500, type=int, help="Time margin")
    parser.add_argument("-depth", type=int, help="Search depth")
    parser.add_argument("-concurrency", type=int, help="Concurrency level")
    args = parser.parse_args()

    main(args.buildType, args.games, args.tc, args.st, args.timemargin, args.depth, args.concurrency)