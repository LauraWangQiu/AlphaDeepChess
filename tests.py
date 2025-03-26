import subprocess
import platform

python_cmd = "python3" if platform.system() == "Linux" else "python"

# Test 1-4: Elo Performance Tests

test_configs = [
    # Test 1: Run 500 games with Stockfish at 1500 Elo
    {
        "games": 500,
        "st": 5,
        "depth": 5,
        "options": {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "1500"
            }
        },
        "pgn": "test1.pgn",
        "epd": "test1.epd",
        "log": "test1.log"
    },
    # Test 2: Run 500 games with Stockfish at 1800 Elo
    {
        "games": 500,
        "st": 5,
        "depth": 5,
        "options": {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "1800"
            }
        },
        "pgn": "test2.pgn",
        "epd": "test2.epd",
        "log": "test2.log"
    },
    # Test 3: Run 500 games with Stockfish at 2000 Elo
    {
        "games": 500,
        "st": 5,
        "depth": 5,
        "options": {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "2000"
            }
        },
        "pgn": "test3.pgn",
        "epd": "test3.epd",
        "log": "test3.log"
    },
    # Test 4: Run 500 games with Stockfish at 2200 Elo
    {
        "games": 500,
        "st": 5,
        "depth": 5,
        "options": {
            "Stockfish": {
                "UCI_LimitStrength": "true",
                "UCI_Elo": "2200"
            }
        },
        "pgn": "test4.pgn",
        "epd": "test4.epd",
        "log": "test4.log"
    }
]

for i, config in enumerate(test_configs):
    print(f"Running test {i + 1} with config: {config}")
    
    options_list = []
    for engine, options in config["options"].items():
        for option_name, option_value in options.items():
            options_list.append(f"{engine}.{option_name}={option_value}")
    
    cmd = [
        python_cmd, "compareEngines.py",
        "-games", str(config["games"]),
        "-st", str(config["st"]),
        "-depth", str(config["depth"]),
        "-pgn", config["pgn"],
        "-epd", config["epd"],
        "-log", config["log"],
        "-options"
    ] + options_list

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate()
    
    print(f"STDOUT: {stdout}\n")
    print(f"STDERR: {stderr}\n")
    