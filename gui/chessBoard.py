#chessBoard.py
from tkinter import Canvas
import customtkinter as ctk
from PIL import Image, ImageTk
import chess
from gui.uci import Uci
from enum import Enum, auto
from collections import defaultdict

class ChessBoard:
    class State(Enum):
        IDLE = auto()
        PIECE_SELECTED = auto()
        PROMOTION_SELECTOR = auto()

    class Orientation(Enum):
        WHITE = auto()
        BLACK = auto()

    NUM_ROWS = 8
    NUM_COLS = 8

    WHITE_SQUARES_COLOR = "#eeeeed"
    BLACK_SQUARES_COLOR = "#769656"
    SELECTED_SQUARE_COLOR = "#00c800"
    LAST_MOVE_SQUARE_COLOR = "#c8c800"

    def __init__(self,window, size : int, UCI : Uci):
        
        self.window = window
    
        self.boardContainer = ctk.CTkFrame(window)
        self.boardContainer.grid(row=1, column=0, sticky="nsew", padx=5, pady=5)

        self.canvas = Canvas(self.boardContainer, width=size, height=size, highlightthickness=0)
        self.canvas.pack()

        self.SIZE = size
        self.SQUARE_SIZE = size / self.NUM_COLS
        self.selected_square = None
        self.state = self.State.IDLE

        self.UCI = UCI
        self.load_images()
        self.orientation = self.Orientation.WHITE
        self.last_move = None
        self.legal_moves_dict = defaultdict(list)

        self.set_fen(chess.STARTING_FEN)

        self.draw()
        window.bind("<KeyPress-r>", self.rotate_orientation)
        self.canvas.bind("<Button-1>", self.on_click)  
        self.canvas.bind("<KeyPress-r>", self.rotate_orientation)
        self.eventManager = None


    def draw(self):
        for row in range(self.NUM_ROWS):
            for col in range(self.NUM_COLS):
                self.draw_square(row,col)

    def draw_square(self, row, col):
        square = chess.square(col, (7 - row) if self.orientation == self.Orientation.WHITE else row) 
        color = self.WHITE_SQUARES_COLOR if (row + col) % 2 == 0 else self.BLACK_SQUARES_COLOR
        SQUARE_SIZE = self.SQUARE_SIZE

        if self.selected_square == square:
            color = self.SELECTED_SQUARE_COLOR
        elif self.selected_square and square in self.get_squares_to_move(self.selected_square):
            color = self.SELECTED_SQUARE_COLOR
        elif self.last_move and (square == self.last_move.to_square or square == self.last_move.from_square):
            color = self.LAST_MOVE_SQUARE_COLOR

        self.canvas.create_rectangle(col * SQUARE_SIZE,
                                        row * SQUARE_SIZE,
                                        col * SQUARE_SIZE + SQUARE_SIZE,
                                        row * SQUARE_SIZE +SQUARE_SIZE,
                                        fill=color, outline="")

        piece = self.board.piece_at(square)
        if piece:
            self.canvas.create_image(
                (col + 0.5) * SQUARE_SIZE,
                (row + 0.5) * SQUARE_SIZE,
                image=self.PIECE_IMAGES[piece.symbol()]
            )

    def on_click(self, event):
        posX, posY = 0, 0
        col = int((event.x - posX) // self.SQUARE_SIZE)
        row = int((event.y - posY) // self.SQUARE_SIZE)

        clicked_square = chess.square(col, (7 - row) if self.orientation == self.Orientation.WHITE else row)

        if self.state == self.State.IDLE:
            if self.board.piece_at(clicked_square):  # Check if there is a piece to select
                self.selected_square = clicked_square
                self.state = self.State.PIECE_SELECTED
        elif self.state == self.State.PIECE_SELECTED:
            self.make_move(self.selected_square, clicked_square)
            self.selected_square = None
            self.state = self.State.IDLE
        
        self.draw()  # Refresh board after move

    def make_move(self, origin_sq, end_sq, promotion_piece=None):
        
        """Make a move on the board."""
        assert isinstance(origin_sq, int) and 0 <= origin_sq <= 63, f"Invalid origin square: {origin_sq}"
        assert isinstance(end_sq, int) and 0 <= end_sq <= 63, f"Invalid end square: {end_sq}"
    
        move = chess.Move(origin_sq, end_sq, promotion_piece) if promotion_piece else chess.Move(origin_sq, end_sq)

        if move in self.legal_moves:
            self.UCI.make_move(move.uci())  # Update engine
            self.board.push(move)  # Apply move on board
            self.update_legal_moves()
            self.fen = self.UCI.get_fen()
            self.last_move = move
            self.eventManager.chessBoardMakeMove(self.fen)
            

    def update_legal_moves(self):
        """Updates the list of legal moves."""
        legal_moves_uci = self.UCI.get_legal_moves()
        self.legal_moves = []
        self.legal_moves_dict.clear()

        for uci_move in legal_moves_uci:
            move = chess.Move.from_uci(uci_move)
            self.legal_moves.append(move)
            self.legal_moves_dict[move.from_square].append(move.to_square)

    def get_squares_to_move(self, origin_square):
        """Returns all possible end squares from a given origin square."""

        #assert isinstance(origin_square, int) and 0 <= origin_square <= 63, f"Invalid origin square: {origin_square}"
        return self.legal_moves_dict.get(origin_square, [])

       # return [move.to_square for move in self.legal_moves if move.from_square == origin_square]

    def load_images(self):
        """Loads piece images."""
        SQUARE_SIZE = int(self.SQUARE_SIZE)
        piece_files = {
            'p': 'blackPawn.png', 'r': 'blackRook.png', 'n': 'blackKnight.png', 'b': 'blackBishop.png',
            'q': 'blackQueen.png', 'k': 'blackKing.png', 'P': 'whitePawn.png', 'R': 'whiteRook.png',
            'N': 'whiteKnight.png', 'B': 'whiteBishop.png', 'Q': 'whiteQueen.png', 'K': 'whiteKing.png'
        }

        self.PIECE_IMAGES = {}
        for piece, filename in piece_files.items():
            try:
                img = Image.open(f'gui/assets/{filename}')
                img = img.resize((SQUARE_SIZE, SQUARE_SIZE), Image.LANCZOS)
                self.PIECE_IMAGES[piece] = ImageTk.PhotoImage(img)  # Convert for Tkinter
            except Exception as e:
                print(f"Error loading image {filename}: {e}")

    def rotate_orientation(self, event=None):
        self.orientation = self.Orientation.WHITE if self.orientation == self.Orientation.BLACK else self.Orientation.BLACK
        self.draw()

    def is_valid_fen(self,fen: str) -> bool:
        try:
            chess.Board(fen)
            return True
        except ValueError:
            return False
    
    def get_fen(self):
        return self.fen

    def set_fen(self, fen: str) -> bool:
        '''set fen if fen is valid return true if fen is valid'''

        if self.is_valid_fen(fen):
            self.UCI.set_fen(fen)
            self.fen = fen
            self.board = chess.Board(fen)
            self.update_legal_moves()
            self.draw()
            return True
        else:
            return False

    def set_eventManager(self, eventManager):
        self.eventManager = eventManager

    def __del__(self):
        del self.UCI
