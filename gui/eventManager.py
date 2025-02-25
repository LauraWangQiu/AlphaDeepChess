# eventManager.py
from gui.chessBoard import ChessBoard
from gui.ui.fenText import FenText

class EventManager:
    def __init__(self, chessBoard: ChessBoard, fenText: FenText):
        self.chessBoard = chessBoard
        self.fenText = fenText
        chessBoard.set_eventManager(self)
        fenText.set_eventManager(self)

    def fenTextModified(self, newFen: str) -> None:
        """Called by FenText when the user modifies the FEN."""
        if self.chessBoard.set_fen(newFen):
            self.fenText.fen_is_valid()
        else:
            self.fenText.fen_is_invalid()

    def chessBoardMakeMove(self, newFen: str) -> None:
        """Called by ChessBoard when the position changes."""
        self.fenText.set_fen(newFen)