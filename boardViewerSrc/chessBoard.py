#chessBoard.py
import pygame
import chess
from boardViewerSrc.uci import Uci
from enum import Enum, auto

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

    WHITE_SQUARES_COLOR = (238, 238, 210)
    BLACK_SQUARES_COLOR = (118, 150, 86)
    SELECTED_SQUARE_COLOR = (0, 200, 0)
    LAST_MOVE_SQUARE_COLOR = (200, 200, 0)

    def __init__(self, posX, posY, size, engine_path, fen=chess.STARTING_FEN):

        assert isinstance(posX, (int, float)) and isinstance(posY, (int, float)), "posX and posY should be numbers"
        assert isinstance(size, (int, float)) , "size should be number"

        assert isinstance(engine_path, str), "engine_path must be a string"
        assert isinstance(fen, str), "fen must be a string"

        self.posX = posX
        self.posY = posY
        self.SIZE = size
        self.SQUARE_SIZE = size / self.NUM_COLS
        self.selected_square = None
        self.state = self.State.IDLE

        # Initialize engine
        self.UCI = Uci(engine_path)
        self.UCI.set_fen(fen)
        self.board = chess.Board(self.UCI.get_fen())

        self.load_images()
        self.update_legal_moves()
        self.orientation = self.Orientation.WHITE
        self.last_move = None

    def draw(self, screen):
        """Draws the chessboard and pieces with highlights."""
        
        assert screen 
        
        SQUARE_SIZE = self.SQUARE_SIZE

        for row in range(self.NUM_ROWS):
            for col in range(self.NUM_COLS):
                square = chess.square(col, (7 - row) if self.orientation == self.Orientation.WHITE else row) 
                color = self.WHITE_SQUARES_COLOR if (row + col) % 2 == 0 else self.BLACK_SQUARES_COLOR

                if self.selected_square == square:
                    color = self.SELECTED_SQUARE_COLOR
                elif self.selected_square and square in self.get_squares_to_move(self.selected_square):
                    color = self.SELECTED_SQUARE_COLOR
                elif self.last_move and (square == self.last_move.to_square or square == self.last_move.from_square):
                    color = self.LAST_MOVE_SQUARE_COLOR

                pygame.draw.rect(screen, color, (col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

                piece = self.board.piece_at(square)
                if piece:
                    screen.blit(self.PIECE_IMAGES[piece.symbol()], (col * SQUARE_SIZE, row * SQUARE_SIZE))

    def is_clicked(self, mouseX, mouseY):
        """Check if the board was clicked."""
        assert isinstance(mouseX, (int, float)) and isinstance(mouseY, (int, float)), "mouseX and mouseY should be numbers"

        return self.posX <= mouseX <= self.posX + self.SIZE and self.posY <= mouseY <= self.posY + self.SIZE

    def on_click(self, mouseX, mouseY):
        """Handles mouse clicks."""
        assert isinstance(mouseX, (int, float)) and isinstance(mouseY, (int, float)), "mouseX and mouseY should be numbers"

        col = int((mouseX - self.posX) // self.SQUARE_SIZE)
        row = int((mouseY - self.posY) // self.SQUARE_SIZE)

        clicked_square = chess.square(col, (7 - row) if self.orientation == self.Orientation.WHITE else row)

        if self.state == self.State.IDLE:
            if self.board.piece_at(clicked_square):  # Check if there is a piece to select
                self.selected_square = clicked_square
                self.state = self.State.PIECE_SELECTED
        elif self.state == self.State.PIECE_SELECTED:
            self.make_move(self.selected_square, clicked_square)
            self.selected_square = None
            self.state = self.State.IDLE

    def make_move(self, origin_sq, end_sq, promotion_piece=None):
        
        """Make a move on the board."""
        assert isinstance(origin_sq, int) and 0 <= origin_sq <= 63, f"Invalid origin square: {origin_sq}"
        assert isinstance(end_sq, int) and 0 <= end_sq <= 63, f"Invalid end square: {end_sq}"
    
        move = chess.Move(origin_sq, end_sq, promotion_piece) if promotion_piece else chess.Move(origin_sq, end_sq)

        if move in self.legal_moves:
            self.UCI.make_move(move.uci())  # Update engine
            self.board.push(move)  # Apply move on board
            self.update_legal_moves()
            self.last_move = move

    def update_legal_moves(self):
        """Updates the list of legal moves."""
        legal_moves_uci = self.UCI.get_legal_moves()
        self.legal_moves = []
        for uci_move in legal_moves_uci:
            self.legal_moves.append(chess.Move.from_uci(uci_move))

    def get_squares_to_move(self, origin_square):
        """Returns all possible end squares from a given origin square."""

        assert isinstance(origin_square, int) and 0 <= origin_square <= 63, f"Invalid origin square: {origin_square}"

        return [move.to_square for move in self.legal_moves if move.from_square == origin_square]

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
                image = pygame.image.load(f'boardViewerSrc/assets/{filename}')
                self.PIECE_IMAGES[piece] = pygame.transform.scale(image, (SQUARE_SIZE, SQUARE_SIZE))
            except Exception as e:
                print(f"Error loading image {filename}: {e}")

    def rotate_orientation(self):
        self.orientation = self.Orientation.WHITE if self.orientation == self.Orientation.BLACK else self.Orientation.BLACK

    def __del__(self):
        del self.UCI
