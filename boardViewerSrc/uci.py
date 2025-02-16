#uci.py

import subprocess

class Uci:
    def __init__(self, engine_path):

        assert isinstance(engine_path, str), "engine_path must be a string"

        self.ENGINE_PATH = engine_path
        self.process = subprocess.Popen(
            self.ENGINE_PATH,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True
        )
        self.uci_start()

    def uci_start(self):
        """Initializes UCI mode and waits for the engine to be ready."""
        self.send_command("uci")
        self.wait_for("uciok")  # Ensure we receive "uciok"
        print("UCI mode initialized.")


    def set_fen(self, fen):
        """Sets a board position from a FEN string."""
        assert isinstance(fen, str), "fen must be a string"

        self.send_command(f"position fen {fen}")
        self.send_command("isready")  # Force engine to process it
        self.wait_for("readyok")  # Ensure it's done
        
           
    def get_fen(self):
        """Gets the FEN of the current position."""
        self.send_command("d")  # The "d" command outputs position details
        output = self.wait_for("Fen:")
        for line in output:
            if line.startswith("Fen:"):
                return line.split("Fen: ")[1].strip()
        return None
    
    def make_move(self, move_string):
        """Applies a move in UCI format."""

        assert isinstance(move_string, str), "move_string must be a string"
        assert len(move_string) in (4, 5), f"Invalid UCI move length: {move_string}"

        self.send_command(f"position actualpos moves {move_string}")
        self.send_command("isready")  # Confirm engine has processed move
        self.wait_for("readyok")

    def get_legal_moves(self):
        """Retrieves all legal moves."""
        self.send_command("perft 1")  # "perft 1" lists all legal moves
        output = self.wait_for("Nodes searched:")

        legal_moves = []
        for line in output:
            if line.startswith("Legal moves") or line.startswith("Execution time") or line.startswith("Nodes searched"):
                continue  # Skip the "Legal moves" header line
            if ":" in line:
                legal_moves.append(line.split(":")[0].strip())  # Extract move

        return legal_moves

         
    def send_command(self, command):
        """Sends a command to the chess engine."""
        assert isinstance(command, str), "command must be a string"

        if self.process:
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()
    
    def get_best_move(self):
        """Requests the best move from the engine."""
        self.send_command("go depth 10")
        output = self.wait_for("bestmove")
        for line in output:
            if line.startswith("bestmove"):
                return line.split()[1]
        return None
    
    def wait_for(self, expected):
        """
        Reads lines from the engine until a line contains the expected string.
        """
        assert isinstance(expected, str), "expected must be a string"

        output = []
        if self.process:
            while True:
                line = self.process.stdout.readline().strip()
                if line:
                    output.append(line)
                    if expected in line:
                        break
        return output
    
    def stop(self):
        """Stops the chess engine cleanly."""
        self.send_command("quit")
        self.process.terminate()
        self.process.wait()
        self.process = None

    def __del__(self):
        self.stop()