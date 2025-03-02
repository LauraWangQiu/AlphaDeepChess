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
        self.orientation = self.Orientation.WHITE
        self.last_move = None
        self.UCI = UCI
        self.legal_squares_dictionary = defaultdict(list)
        self.boardInitialize = False
        self.eventManager = None

        window.bind("<KeyPress-r>", self.rotate_orientation)
        self.canvas.bind("<Button-1>", self.on_click) 

        self.load_images()
        self.init_canvas_items()

        self.set_fen(chess.STARTING_FEN)

    def draw(self):

        """Update square colors and piece images based on current state."""
        for square in chess.SQUARES:
            # Update square color
            color = self.get_square_color(square)
            self.canvas.itemconfig(self.square_ids[square], fill=color)
            # Update piece image
            piece = self.board.piece_at(square)
            image_id = self.piece_image_ids[square]
            if piece:
                self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES[piece.symbol()], state="normal")
            else:
                self.canvas.itemconfig(image_id, state="hidden")

    def rotate_orientation(self, event=None):
        self.orientation = self.Orientation.WHITE if self.orientation == self.Orientation.BLACK else self.Orientation.BLACK
        self.init_canvas_items()
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
        

    def on_click(self, event):
        posX, posY = 0, 0
        col = int((event.x - posX) // self.SQUARE_SIZE)
        row = int((event.y - posY) // self.SQUARE_SIZE)

        clicked_square = chess.square(col, (7 - row) if self.orientation == self.Orientation.WHITE else row)

        if self.state == self.State.IDLE:
            if self.board.piece_at(clicked_square):  
                self.selected_square = clicked_square
                self.state = self.State.PIECE_SELECTED
        elif self.state == self.State.PIECE_SELECTED:
            self.make_move(self.selected_square, clicked_square)
            self.selected_square = None
            self.state = self.State.IDLE
        
        self.draw()  

    def make_move(self, origin_sq: int, end_sq: int, promotion_piece=None):
        
        """Make a move on the board."""
    
        move = chess.Move(origin_sq, end_sq, promotion_piece) if promotion_piece else chess.Move(origin_sq, end_sq)

        if move in self.board.legal_moves:
            self.fen = self.UCI.make_move(move.uci())  # Update engine
            self.board.push(move)  
            self.update_legal_moves()

            self.last_move = move
            self.eventManager.chessBoardMakeMove(self.fen)
            

    def update_legal_moves(self):
        """Updates the list of legal moves."""

        self.legal_squares_dictionary.clear()

        for move in self.board.legal_moves:
            self.legal_squares_dictionary[move.from_square].append(move.to_square)

    def get_squares_to_move(self, origin_square: int) -> list:
        """Returns all possible end squares from a given origin square."""
        return self.legal_squares_dictionary.get(origin_square, [])


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

    def init_canvas_items(self):
        """Initialize canvas items based on current orientation."""
        self.canvas.delete("all")
        self.square_ids = {} 
        self.piece_image_ids = {} 

        for row in range(self.NUM_ROWS):
            for col in range(self.NUM_COLS):

                if self.orientation == self.Orientation.WHITE:
                    square = chess.square(col, 7 - row)
                else:
                    square = chess.square(col, row)

                x1 = col * self.SQUARE_SIZE
                y1 = row * self.SQUARE_SIZE
                x2 = x1 + self.SQUARE_SIZE
                y2 = y1 + self.SQUARE_SIZE

                base_color = self.WHITE_SQUARES_COLOR if (row + col) % 2 == 0 else self.BLACK_SQUARES_COLOR
                rect_id = self.canvas.create_rectangle(x1, y1, x2, y2, fill=base_color, outline="")
                self.square_ids[square] = rect_id
                image_id = self.canvas.create_image(
                    (col + 0.5) * self.SQUARE_SIZE,
                    (row + 0.5) * self.SQUARE_SIZE,
                    image=None,
                    state="hidden"
                )
                self.piece_image_ids[square] = image_id

    def get_square_color(self, square):
        """Determine the color of a square based on its state."""
        row = 7 - chess.square_rank(square) if self.orientation == self.Orientation.WHITE else chess.square_rank(square)
        #col = chess.square_file(square) if self.orientation == self.Orientation.WHITE else 7 - chess.square_file(square)
        col = chess.square_file(square)
        base_color = self.WHITE_SQUARES_COLOR if (row + col) % 2 == 0 else self.BLACK_SQUARES_COLOR
        if self.selected_square == square or (self.selected_square and square in self.get_squares_to_move(self.selected_square)):
            return self.SELECTED_SQUARE_COLOR
        elif self.last_move and (square == self.last_move.from_square or square == self.last_move.to_square):
            return self.LAST_MOVE_SQUARE_COLOR
        return base_color

    def __del__(self):
        del self.UCI
