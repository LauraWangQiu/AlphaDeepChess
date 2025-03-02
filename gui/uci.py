#uci.py

import subprocess
import re
import select

class Uci:
    def __init__(self, engine_path: str):

        self.in_infinite_search = False
        self.ENGINE_PATH = engine_path
        self.process = subprocess.Popen(
            self.ENGINE_PATH,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            universal_newlines=True,
            bufsize=1 
        )
        self.latest_info = (0, 0, None)
        self.searching = False
        self.info_pattern = re.compile(r"info depth (\d+) score (-?\d+) bestMove (\S+)")

        self.uci_start()

    def uci_start(self):
        """Initializes UCI mode and waits for the engine to be ready."""
        self.send_command("uci")
        self.wait_for("uciok")  # Ensure we receive "uciok"


    def set_fen(self, fen: str) -> None:
        """Sets a board position from a FEN string."""

        self.send_command(f"position fen {fen}")
        self.send_command("isready")  # Force engine to process it
        self.wait_for("readyok")  # Ensure it's done

    def start_search(self, fen: str) -> None:
        '''Starts the infinite search'''

        if self.searching:
            self.stop_search()

        self.latest_info = (0, 0, None)

        self.set_fen(fen)
    
        self.send_command("go")
        self.searching = True
    
        
    def stop_search(self) -> None:
        '''Stop the infinite search'''

        if self.searching == False:
            return
        
        self.send_command("stop")     
        self.send_command("isready")  
        self.wait_for("readyok")  
        self.searching = False
        

    def get_search_info(self) -> tuple[int, int, str, bool]:
        """Gets the latest search info, returns (depth, score, bestMove) and if is new info"""

        if not self.searching:
            return self.latest_info + (False,)

        lines = []
        while True:
            # Check if data is available with zero timeout (non-blocking)
            rlist, _, _ = select.select([self.process.stdout.fileno()], [], [], 0)
            if not rlist:
                break
            line = self.process.stdout.readline()
            if not line:
                break
            lines.append(line.strip())

        # Process all collected lines to find the latest info
        for line in lines:
            match = self.info_pattern.search(line)
            if match:
                depth = int(match.group(1))
                score = int(match.group(2))
                best_move = match.group(3)
                self.latest_info = (depth, score, best_move)

        return self.latest_info + (True,)
    

    def send_command(self, command):
        """Sends a command to the chess engine."""

        if self.process:
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()
    
    def wait_for(self, expected):
        """
        Reads lines from the engine until a line contains the expected string.
        """

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
