import customtkinter as ctk
import sys
import os
from gui.chessBoard import ChessBoard


ENGINE_PATH = "build/AlphaDeepChess"
if sys.platform.startswith("win"):
    ENGINE_PATH += ".exe"

if not os.path.exists(ENGINE_PATH):
    print(f"Error: Engine file not found at '{ENGINE_PATH}'")
    sys.exit(1)  # Exit the program with an error code


class MainWindow:

    SCREEN_WIDTH = 1000
    SCREEN_HEIGHT = 1000

    def __init__(self):
        self.window = ctk.CTk()
        self.window.title("Chess Game")
        self.window.geometry(f"{self.SCREEN_WIDTH}x{self.SCREEN_HEIGHT}")
        self.board = ChessBoard(window=self.window, posX=0, posY=0, size=600, engine_path=ENGINE_PATH)
        self.window.mainloop()
        
if __name__ == "__main__":
    MainWindow()