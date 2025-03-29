import subprocess
import re
import queue
import threading
import sys

class Uci:
    def __init__(self, engine_path: str):
        self.in_infinite_search = False
        self.ENGINE_PATH = engine_path
        
        # Set creationflags for Windows to avoid showing a console window
        creationflags = subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0
        self.process = subprocess.Popen(
            self.ENGINE_PATH,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            universal_newlines=True,
            bufsize=1,
            creationflags=creationflags
        )
        
        # Initialize queue and thread for reading engine output
        self.output_queue = queue.Queue()
        self.read_thread = threading.Thread(target=self.read_output, daemon=True)
        self.read_thread.start()
        
        self.latest_info = (0, 0, None)
        self.searching = False
        self.info_pattern = re.compile(r"info depth (\d+) score cp (-?\d+) bestMove (\S+)")
        
        self.uci_start()

    def read_output(self):
        """Background thread function to read engine output and put it in the queue."""
        while True:
            line = self.process.stdout.readline()
            if not line:  # Pipe closed when process terminates
                break
            self.output_queue.put(line.strip())

    def uci_start(self):
        """Initializes UCI mode and waits for the engine to be ready."""
        self.send_command("uci")
        self.wait_for("uciok")

    def set_position(self, fen_and_moves: str) -> None:
        """Sets a board position from an string with this format : fen moves move1 move2 ..."""
        self.send_command(f"position fen {fen_and_moves}")
        self.send_command("isready")
        self.wait_for("readyok")

    def start_search(self, fen_and_moves: str) -> None:
        """Starts the infinite search."""
        if self.searching:
            self.stop_search()
        self.latest_info = (0, 0, None)
        self.set_position(fen_and_moves)
        self.send_command("go")
        self.searching = True

    def stop_search(self) -> None:
        """Stops the infinite search."""
        if not self.searching:
            return
        self.send_command("stop")
        self.send_command("isready")
        self.wait_for("readyok")
        self.searching = False

    def get_search_info(self) -> tuple[int, int, str, bool]:
        """Gets the latest search info, returns (depth, score, bestMove) and if it’s new info."""
        if not self.searching:
            return self.latest_info + (False,)

        new_info = False
        while True:
            try:
                line = self.output_queue.get_nowait()  # Non-blocking get
                match = self.info_pattern.search(line)
                if match:
                    depth = int(match.group(1))
                    score = int(match.group(2))
                    best_move = match.group(3)
                    self.latest_info = (depth, score, best_move)
                    new_info = True
            except queue.Empty:
                break  # No more lines available

        return self.latest_info + (new_info,)

    def send_command(self, command):
        """Sends a command to the chess engine."""
        if self.process:
            self.process.stdin.write(command + "\n")
            self.process.stdin.flush()

    def wait_for(self, expected):
        """Waits until the expected string is received in the output."""
        while True:
            line = self.output_queue.get()  # Blocks until a line is available
            if expected in line:
                return

    def close(self):
        self.send_command("quit")
        self.process.terminate()
        self.process.wait()
        self.process = None

    def __del__(self):
        """Stops the chess engine cleanly."""
        self.close()