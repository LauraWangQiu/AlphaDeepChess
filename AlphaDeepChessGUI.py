#AlphaDeepChess.py

import customtkinter as ctk
import sys
import os
from gui.chessBoard import ChessBoard
from gui.ui.navigationBar import NavigationBar
from gui.ui.fenText import FenText
from gui.ui.rightPanel import RightPanel
from gui.eventManager import EventManager
from gui.uci import Uci

ENGINE_PATH = "build/release/AlphaDeepChess"
if sys.platform.startswith("win"):
    ENGINE_PATH += ".exe"

if not os.path.exists(ENGINE_PATH):
    print(f"Error: Engine file not found at '{ENGINE_PATH}'")
    sys.exit(1)

class MainWindow:
    SCREEN_WIDTH = 1000
    SCREEN_HEIGHT = 1000

    def __init__(self):
        self.window = ctk.CTk()
        self.window.title("Chess Game")
        self.window.geometry(f"{self.SCREEN_WIDTH}x{self.SCREEN_HEIGHT}")
        
        # Configure grid layout
        #self.window.grid_rowconfigure(1, weight=1)  # Main content area
        self.window.grid_columnconfigure(0, weight=1)  # Chessboard column
        self.window.grid_columnconfigure(1, weight=0)  # Right panel column

        
        self.UCI = Uci(ENGINE_PATH)

        self.navigationBar = NavigationBar(self.window)
        
        self.chessBoard = ChessBoard(window=self.window, size=700)
        
        self.fenText = FenText(window=self.window, initialFen=self.chessBoard.get_fen())

        self.rightPanel = RightPanel(window=self.window)

        self.eventManager = EventManager(self.UCI ,self.chessBoard, self.fenText, self.rightPanel)
        
        # Set minimum window size
        self.window.minsize(800, 600)
        # Enable CTk's native scaling
        ctk.set_window_scaling(1.3)  # Improves high-DPI handling

        self.window.mainloop()

if __name__ == "__main__":
    MainWindow()