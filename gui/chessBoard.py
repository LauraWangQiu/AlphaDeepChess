#chessBoard.py
from tkinter import Canvas
import customtkinter as ctk
from PIL import Image, ImageTk
import chess
from enum import Enum, auto
from collections import defaultdict

class ChessBoard:
    class State(Enum):
        IDLE = auto()
        PIECE_SELECTED = auto()
        PROMOTION_SELECTOR = auto()

    NUM_ROWS = 8
    NUM_COLS = 8

    WHITE_SQUARES_COLOR = "#eeeeed"
    BLACK_SQUARES_COLOR = "#769656"
    SELECTED_SQUARE_COLOR = "#00c800"
    LAST_MOVE_SQUARE_COLOR = "#c8c800"
    ENGINE_MOVE_SQUARE_COLOR = "#ADD8E6"
    PROMOTION_SELECTOR_SQUARE_COLOR = "#808080"


    def __init__(self, window, size : int):
        
        self.window = window
    
        self.boardContainer = ctk.CTkFrame(window)
        self.boardContainer.grid(row=1, column=0, sticky="nsew", padx=5, pady=5)

        self.canvas = Canvas(self.boardContainer, width=size, height=size, highlightthickness=0)
        self.canvas.pack()

        self.SIZE = size
        self.SQUARE_SIZE = size / self.NUM_COLS
        self.selected_square = None
        self.state = self.State.IDLE
        self.orientation = chess.WHITE
        self.last_move = None
        self.engine_move = None
        self.promotion_queen_selector_square = None
        self.promotion_knight_selector_square = None
        self.promotion_rook_selector_square = None
        self.promotion_bishop_selector_square = None

        self.legal_squares_dictionary = defaultdict(list)
        self.boardInitialize = False
        self.eventManager = None

        window.bind("<KeyPress-r>", self.rotate_orientation)
        self.canvas.bind("<Button-1>", self.on_click) 

        self.load_images()
        self.init_canvas_items()

        self.set_fen(chess.STARTING_FEN)

    def draw(self) -> None:

        """Update square colors and piece images based on current state."""
        for row in range(self.NUM_ROWS):
            for col in range(self.NUM_COLS):

                square = chess.square(col, row)

                # Update square color
                color = self.get_square_color(square)
                self.canvas.itemconfig(self.square_ids[square], fill=color)
                # Update piece image

                piece = self.board.piece_at(square)
                image_id = self.piece_image_ids[square]

                promo_color = chess.WHITE if row >= 4 else chess.BLACK

                if square == self.promotion_queen_selector_square:
                    self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES['Q' if promo_color == chess.WHITE else 'q'], state="normal")
                elif square == self.promotion_knight_selector_square:
                    self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES['N' if promo_color == chess.WHITE else 'n'], state="normal")
                elif square == self.promotion_rook_selector_square:
                    self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES['R' if promo_color == chess.WHITE else 'r'], state="normal")
                elif square == self.promotion_bishop_selector_square:
                    self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES['B' if promo_color == chess.WHITE else 'b'], state="normal")
                elif piece:
                    self.canvas.itemconfig(image_id, image=self.PIECE_IMAGES[piece.symbol()], state="normal")
                else:
                    self.canvas.itemconfig(image_id, state="hidden")

    def rotate_orientation(self, event = None) -> None:
        self.orientation = chess.WHITE if self.orientation == chess.BLACK else chess.BLACK
        self.init_canvas_items()
        self.draw()

    def is_valid_fen(self, fen: str) -> bool:
        try:
            chess.Board(fen)
            return True
        except ValueError:
            return False
    
    def get_fen(self) -> str:
        return self.board.fen()

    def set_fen(self, fen: str) -> bool:
        '''set fen if fen is valid return true if fen is valid'''

        if self.is_valid_fen(fen):
            self.last_move = None
            self.engine_move = None
            self.promotion_queen_selector_square = None
            self.promotion_knight_selector_square = None
            self.promotion_rook_selector_square = None
            self.promotion_bishop_selector_square = None
            self.board = chess.Board(fen)
            self.update_legal_moves()
            self.draw()
            return True
        else:
            return False

    def set_eventManager(self, eventManager) -> None:
        self.eventManager = eventManager
        

    def on_click(self, event) -> None:
        posX, posY = 0, 0
        col = int((event.x - posX) // self.SQUARE_SIZE)
        row = int((event.y - posY) // self.SQUARE_SIZE)
        
        if self.orientation == chess.WHITE:
            row = 7 - row
        else:
            col = 7 - col

        clicked_square = chess.square(col, row)

        if self.state == self.State.IDLE:
            if self.board.piece_at(clicked_square):  
                self.selected_square = clicked_square
                self.state = self.State.PIECE_SELECTED
        elif self.state == self.State.PIECE_SELECTED:
            if (
                self.board.piece_type_at(self.selected_square) == chess.PAWN
                and (row == 7 or row == 0)
                and clicked_square in self.get_squares_to_move(self.selected_square)
            ):
                d = +1 if row == 0 else -1

                self.promotion_queen_selector_square = clicked_square
                self.promotion_knight_selector_square = chess.square(col, row + d)
                self.promotion_rook_selector_square = chess.square(col, row + 2*d)
                self.promotion_bishop_selector_square = chess.square(col, row + 3*d)

                self.state = self.State.PROMOTION_SELECTOR
            else:
                self.make_move(self.selected_square, clicked_square)
                self.selected_square = None
                self.state = self.State.IDLE
        elif self.state == self.State.PROMOTION_SELECTOR:
                
                promo_square = self.promotion_queen_selector_square

                if clicked_square == self.promotion_queen_selector_square:
                    self.make_move(self.selected_square, promo_square, chess.QUEEN)
                elif clicked_square == self.promotion_knight_selector_square:
                    self.make_move(self.selected_square, promo_square, chess.KNIGHT)
                elif clicked_square == self.promotion_rook_selector_square:
                    self.make_move(self.selected_square, promo_square, chess.ROOK)
                elif clicked_square == self.promotion_bishop_selector_square:
                    self.make_move(self.selected_square, promo_square, chess.BISHOP)

                self.selected_square = None
                self.promotion_queen_selector_square = None
                self.promotion_knight_selector_square = None
                self.promotion_rook_selector_square = None
                self.promotion_bishop_selector_square = None
                self.state = self.State.IDLE

        self.draw()  

    def make_move(self, origin_sq: chess.Square, end_sq: chess.Square, promotion_piece : chess.PieceType = None) -> None:
        
        """Make a move on the board."""

        move = chess.Move(origin_sq, end_sq, promotion=promotion_piece)

        if move in self.board.legal_moves:
            self.board.push(move)  
            self.update_legal_moves()

            self.last_move = move
            self.engine_move = None
            self.eventManager.chessBoardMakeMove(self.get_fen())
            

    def update_legal_moves(self) -> None:
        """Updates the legal squares dictionary."""

        self.legal_squares_dictionary.clear()

        for move in self.board.legal_moves:
            self.legal_squares_dictionary[move.from_square].append(move.to_square)

    def get_squares_to_move(self, origin_square: int) -> list:
        """Returns all possible end squares from a given origin square."""
        return self.legal_squares_dictionary.get(origin_square, [])


    def load_images(self) -> None:
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

    def init_canvas_items(self) -> None:
        """Initialize canvas items based on current orientation."""
        self.canvas.delete("all")
        self.square_ids = {} 
        self.piece_image_ids = {} 

        for row in range(self.NUM_ROWS):
            for col in range(self.NUM_COLS):
                
                if self.orientation == chess.WHITE:
                    square = chess.square(col, 7 - row)
                else:
                    square = chess.square(7 - col, row)

                x1 = col * self.SQUARE_SIZE
                y1 = row * self.SQUARE_SIZE
                x2 = x1 + self.SQUARE_SIZE
                y2 = y1 + self.SQUARE_SIZE

                rect_id = self.canvas.create_rectangle(x1, y1, x2, y2, fill=self.BLACK_SQUARES_COLOR, outline="")
                self.square_ids[square] = rect_id
                image_id = self.canvas.create_image(
                    (col + 0.5) * self.SQUARE_SIZE,
                    (row + 0.5) * self.SQUARE_SIZE,
                    image=None,
                    state="hidden"
                )
                self.piece_image_ids[square] = image_id


    def get_square_color(self, square : chess.Square) -> str:
        """Determine the color of a square based on its state."""
        row = chess.square_rank(square)
        col = chess.square_file(square)                             

        if (
            self.promotion_queen_selector_square
            and (
                square == self.promotion_queen_selector_square
                or square == self.promotion_knight_selector_square
                or square == self.promotion_rook_selector_square
                or square == self.promotion_bishop_selector_square
            )
        ):
            return self.PROMOTION_SELECTOR_SQUARE_COLOR
        elif self.selected_square == square or (self.selected_square and square in self.get_squares_to_move(self.selected_square)):
            return self.SELECTED_SQUARE_COLOR
        elif self.engine_move and (square == self.engine_move.from_square or square == self.engine_move.to_square):
            return self.ENGINE_MOVE_SQUARE_COLOR
        elif self.last_move and (square == self.last_move.from_square or square == self.last_move.to_square):
            return self.LAST_MOVE_SQUARE_COLOR
        elif self.orientation == chess.WHITE:
            return self.WHITE_SQUARES_COLOR if ((7 - row) + col) % 2 == 0 else self.BLACK_SQUARES_COLOR
        else:
            return self.WHITE_SQUARES_COLOR if (row + col) % 2 == 0 else self.BLACK_SQUARES_COLOR

    def set_engine_move(self, move: str) -> None:
        '''Set the engine move in UCI format'''
        if move is not None:
            self.engine_move = chess.Move.from_uci(move)
            self.draw()

