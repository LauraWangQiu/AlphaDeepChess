# eventManager.py
from gui.chessBoard import ChessBoard
from gui.ui.fenText import FenText
from gui.ui.rightPanel import RightPanel
from gui.uci import Uci

class EventManager:
    def __init__(self, uci: Uci, chessBoard: ChessBoard, fenText: FenText, rightPanel: RightPanel):

        self.uci = uci
        self.chessBoard = chessBoard
        self.fenText = fenText
        self.rightPanel = rightPanel

        chessBoard.set_eventManager(self)
        fenText.set_eventManager(self)
        rightPanel.set_eventManager(self)

    def fenTextModified(self, newFen: str) -> None:
        """Called by FenText when the user modifies the FEN."""
        if self.chessBoard.set_fen(newFen):
            self.fenText.fen_is_valid()
            self.rightPanel.position_changed()
        else:
            self.fenText.fen_is_invalid()

    def chessBoardMakeMove(self, newFen: str) -> None:
        """Called by ChessBoard when the position changes."""
        self.fenText.set_fen(newFen)
        self.rightPanel.position_changed()

    def start_search(self) -> None:
        self.uci.start_search(self.chessBoard.get_fen())

    def stop_search(self) -> None:
        self.uci.stop_search()

    def get_search_info(self) -> tuple[int, int, str, bool]:
        '''return the depth, eval, the best move found and if new info found'''
        return self.uci.get_search_info()
    
    def set_engine_move(self, move: str) -> None:
        '''Set the engine move in UCI format'''
        self.chessBoard.set_engine_move(move)

    