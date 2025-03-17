import os
import platform
import subprocess
import json
import argparse

def main(build_type, games, tc, st, depth, concurrency):
    os_name = platform.system()
    is_windows = os_name == "Windows"
    is_linux = os_name == "Linux"
    is_macos = os_name == "Darwin"

    default_comparator_dir = "enginesComparator"
    if is_windows:
        cutechess_internal_dir = "cutechess-1.3.1-win64"
    elif is_linux:
        cutechess_internal_dir = "cutechess-cli"
    elif is_macos:
        cutechess_internal_dir = "build"
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
            # Change this path depending on the OS
            "command": "./stockfish/stockfish/stockfish-windows-x86-64",
            "protocol": "uci"
        }
    ]

    with open(engines_config_path, 'w', encoding='utf8') as f:
        json.dump(engines_config_content, f, indent=4)

    results_path = "results.pgn"
    edp_path = "results.epd"
    cutechess_cmd = [
        os.path.join(cutechess_dir, "cutechess-cli"),
        "-engine", "conf=AlphaDeepChess",
        "-engine", "conf=Stockfish",
        "-each", f"tc={tc}",
        "-games", str(games),
        "-repeat",
        "-pgnout", results_path,
        "-epdout", edp_path,
        "-debug"
    ]

    if depth is not None:
        cutechess_cmd.append(f"depth={depth}")

    if st is not None:
        cutechess_cmd.append(f"st={st}")

    if concurrency is not None:
        cutechess_cmd.append(f"concurrency={concurrency}")

    subprocess.run(cutechess_cmd)

    print(f"Results saved in {results_path}.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare chess engines.")
    parser.add_argument("-buildType", type=str, default="Release", help="Build type")
    parser.add_argument("-games", type=int, default=10, help="Number of games")
    parser.add_argument("-tc", type=str, default="40/60", help="Time control")
    parser.add_argument("-st", type=str, help="Search time")
    parser.add_argument("-depth", type=int, help="Search depth")
    parser.add_argument("-concurrency", type=int, help="Concurrency level")
    args = parser.parse_args()

    main(args.buildType, args.games, args.tc, args.st, args.depth, args.concurrency)