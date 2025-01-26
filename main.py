import subprocess
import threading
import queue
import time

import pygame
import chess

# Path to the engine and pieces images
PATH_TO_EXE = "./build/AlphaDeepChess.exe"
PATH_TO_PIECES_IMAGES = "./assets/"

# Pieces images filenames
BLACK_PAWN_IMAGE = "blackPawn.png"
BLACK_KNIGHT_IMAGE = "blackKnight.png"
BLACK_BISHOP_IMAGE = "blackBishop.png"
BLACK_ROOK_IMAGE = "blackRook.png"
BLACK_QUEEN_IMAGE = "blackQueen.png"
BLACK_KING_IMAGE = "blackKing.png"
WHITE_PAWN_IMAGE = "whitePawn.png"
WHITE_KNIGHT_IMAGE = "whiteKnight.png"
WHITE_BISHOP_IMAGE = "whiteBishop.png"
WHITE_ROOK_IMAGE = "whiteRook.png"
WHITE_QUEEN_IMAGE = "whiteQueen.png"
WHITE_KING_IMAGE = "whiteKing.png"

# Board colors
WHITE_COLOR = (240, 217, 181)
BLACK_COLOR = (181, 136, 99)

# Board size and square size
BOARD_SIZE = 480  # In pixels
SQUARE_SIZE = BOARD_SIZE // 8

class ChessBoard:
    def __init__(self, pathToEngine):
        self.engine = subprocess.Popen(
            pathToEngine,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        self.running = True
        self.programCommandQueue = queue.Queue()
        self.userCommandQueue = queue.Queue()
        self.lastCommandSent = None
        self.currentFen = None
        self.chessPiecesImages = self._loadChessPieces()
        self.thread = threading.Thread(target=self._enqueue, daemon=True)
        self.thread.start()

    """
        Reads the output from the engine and puts it in the queue    
    """
    def _enqueue(self):
        for line in iter(self.engine.stdout.readline, ''):
            line = line.strip()
            if line:
                self.programCommandQueue.put(line)
    
    """
        Sends a command to the engine

        Args:
            command: command to send to the engine
    """
    def sendCommand(self, command):
        self.engine.stdin.write(command + "\n")
        self.engine.stdin.flush()
        self.lastCommandSent = command

    """
        Reads the output from the queue while the timeout is not reached
        
        Args:
            timeout: max time to wait for a response from the engine
        
        Returns:
            List of read lines from the engine
    """
    def read(self, timeout=2):
        readInfo = []
        startTime = time.time()

        while True:
            try:
                line = self.programCommandQueue.get(timeout=0.1)
                if line:
                    readInfo.append(line)
            except queue.Empty:
                if time.time() - startTime > timeout:
                    break

        return readInfo
    
    """
        Sends "d" command and returns the FEN from the engine. If not possible, returns None

        Returns:
            FEN from the engine or None    
    """
    def getFen(self):
        self.sendCommand("d")
        readInfo = self.read()
        for line in readInfo:
            if line.startswith("Fen:"):
                return line.split("Fen:")[1].strip()
        return None

    """
        Loads the chess pieces images

        Returns:
            Dictionary with the chess pieces images
    """
    def _loadChessPieces(self):
        pieces = {}
        piecesDictionary = {
            'p': BLACK_PAWN_IMAGE,
            'n': BLACK_KNIGHT_IMAGE,
            'b': BLACK_BISHOP_IMAGE,
            'r': BLACK_ROOK_IMAGE,
            'q': BLACK_QUEEN_IMAGE,
            'k': BLACK_KING_IMAGE,
            'P': WHITE_PAWN_IMAGE,
            'N': WHITE_KNIGHT_IMAGE,
            'B': WHITE_BISHOP_IMAGE,
            'R': WHITE_ROOK_IMAGE,
            'Q': WHITE_QUEEN_IMAGE,
            'K': WHITE_KING_IMAGE,
        }
        for piece, file in piecesDictionary.items():
            filePath = PATH_TO_PIECES_IMAGES + file
            pieces[piece] = pygame.image.load(filePath)
            pieces[piece] = pygame.transform.scale(pieces[piece], (SQUARE_SIZE, SQUARE_SIZE))
        return pieces

    """
        Draws the chessboard in the window

        Args:
            window: window to draw the chessboard
    """
    def _drawBoard(self, window):
        for row in range(8):
            for column in range(8):
                color = WHITE_COLOR if (row + column) % 2 == 0 else BLACK_COLOR
                pygame.draw.rect(
                    window,
                    color,
                    pygame.Rect(column * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE)
                )

    """
        Draws the chess pieces in the board based in the FEN

        Args:
            window: window to draw the chess pieces
            fen: FEN from the engine
    """
    def _drawPieces(self, window):
        if self.currentFen is None:
            return
        
        board = chess.Board(self.currentFen)
        for row in range(8):
            for column in range(8):
                index = chess.square(column, 7 - row)
                piece = board.piece_at(index)
                if piece:
                    pieceFen = piece.symbol()
                    window.blit(
                        self.chessPiecesImages[pieceFen],
                        (column * SQUARE_SIZE, row * SQUARE_SIZE)
                    )

    """
        Draws a chessboard based in read FEN from engine
    """
    def drawChessboard(self):
        pygame.init()

        # Initializes window
        window = pygame.display.set_mode((BOARD_SIZE, BOARD_SIZE))
        pygame.display.set_caption("Chessboard")

        while self.running:
            # Event handling
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False

            # Check if there is a command to send from user
            if not self.userCommandQueue.empty():
                command = self.userCommandQueue.get()
                self.sendCommand(command)
                print(self.read())

            # Get FEN from the engine only when 
            # the last command sent was not "d"
            # or any other command that changes the FEN
            if self.lastCommandSent != "d":
                self.currentFen = self.getFen()
            self._drawBoard(window)
            self._drawPieces(window)

            pygame.display.flip()

        pygame.quit()
    
    """
        Terminates the engine and the thread
    """
    def finish(self):
        self.engine.terminate()
        self.thread.join()

"""
    Thread for reading user commands and sending them to the engine
"""
def processCommands(chessBoard):
    while chessBoard.running:
        command = input()
        if command.lower() == "quit":
            chessBoard.running = False
            break
        # Put the command in the queue to be processed by the engine
        chessBoard.userCommandQueue.put(command)

if __name__ == "__main__":
    engine = ChessBoard(PATH_TO_EXE)

    try:
        engine.sendCommand("uci")
        print(engine.read())
        
        engine.sendCommand("isready")
        print(engine.read())

        # Start a thread for processing user commands
        commandThread = threading.Thread(target=processCommands, args=(engine,))
        commandThread.start()

        # Includes loop to draw the chessboard
        # calling getFen() method to get the 
        # FEN from the engine
        engine.drawChessboard()

        commandThread.join()

    finally:
        engine.finish()

# Chess pieces images got from: https://commons.wikimedia.org/wiki/Category:PNG_chess_pieces/Standard_transparent