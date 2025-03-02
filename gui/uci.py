#uci.py

import subprocess

class Uci:
    def __init__(self, engine_path: str):

        self.in_infinite_search = False
        self.ENGINE_PATH = engine_path
        self.process = subprocess.Popen(
            self.ENGINE_PATH,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True
        )
        self.last_eval = 0
        self.last_best_move = None
        self.searching = False
        self.uci_start()

    def uci_start(self):
        """Initializes UCI mode and waits for the engine to be ready."""
        self.send_command("uci")
        self.wait_for("uciok")  # Ensure we receive "uciok"
        print("UCI mode initialized.")


    def set_fen(self, fen: str) -> None:
        """Sets a board position from a FEN string."""
        assert isinstance(fen, str), "fen must be a string"
        if self.searching:
            self.stop_search()

        self.send_command(f"position fen {fen}")
        self.send_command("isready")  # Force engine to process it
        self.wait_for("readyok")  # Ensure it's done
        
           
    def get_fen(self) -> str:
        """Gets the FEN of the current position."""
        if self.searching:
            self.stop_search()
        
        self.send_command("d")  # The "d" command outputs position details
        output = self.wait_for("Fen:")
        for line in output:
            if line.startswith("Fen:"):
                return line.split("Fen: ")[1].strip()
        return None
    
    def make_move(self, move: str) -> str:
        """Applies a move in UCI format, returns the new fen"""

        if self.searching:
            self.stop_search()
        
        self.send_command(f"position actualpos moves {move}")

        return self.get_fen()

    def get_legal_moves(self) -> list:
        """Retrieves all legal moves."""

        if self.searching:
            self.stop_search()

        self.send_command("go perft 1")  # "go perft 1" lists all legal moves
        output = self.wait_for("Nodes searched:")

        legal_moves = []
        for line in output:
            if line.startswith("Legal moves") or line.startswith("Execution time") or line.startswith("Nodes searched"):
                continue  # Skip the "Legal moves" header line
            if ":" in line:
                legal_moves.append(line.split(":")[0].strip())  # Extract move

        return legal_moves

    def start_search(self) -> None:
        '''Starts the infinite search'''
        '''print("start_search")

        assert self.searching == False, "start_search called but uci is currently searching"

        self.send_command("go")
        self.searching = True
        '''
    def stop_search(self) -> None:
        '''Stop the infinite search'''
        '''print("stop search")
        self.send_command("stop")     
        self.send_command("isready")  # Force engine to process it
        self.wait_for("readyok")  # Ensure it's done
        self.searching = False
        '''
    def get_search_info(self, eval, best_move):
        """Gets the latest search info"""
        #print("get_search_info")

        eval = self.last_eval
        best_move = self.last_best_move 

        if self.searching == False:
            return

        line = self.process.stdout.readline().strip()
        if line:
            try:
                parts = line.split()
            
                eval = int(parts[4])  # Extract evaluation score
                best_move = parts[-1]  # Extract best move
                self.last_eval = eval
                self.last_best_move = best_move
            except ValueError:
                raise ValueError(f"Could not get search info")      

    def send_command(self, command):
        """Sends a command to the chess engine."""
        assert isinstance(command, str), "command must be a string"

        if self.process:
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()
    
    def get_evaluation(self) -> float:
        self.send_command("eval")
        output = self.wait_for("Evaluation:")
        for line in output:
            if line.startswith("Evaluation:"):
                value = line.split("Evaluation:")[1].strip()
                try:
                    return float(value)
                except ValueError:
                    raise ValueError(f"Could not convert evaluation '{value}' to float")
        raise ValueError("No evaluation found in output")
    
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
    


    def __del__(self):
        """Stops the chess engine cleanly."""
        if self.searching:
            self.stop_search()

        self.send_command("quit")
        self.process.terminate()
        self.process.wait()
        self.process = None
