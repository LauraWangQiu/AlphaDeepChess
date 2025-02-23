import customtkinter as ctk
import sys
import os
from gui.chessBoard import ChessBoard

ENGINE_PATH = "build/AlphaDeepChess"
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
        self.window.grid_rowconfigure(1, weight=1)  # Main content area
        self.window.grid_columnconfigure(0, weight=1)  # Chessboard column
        self.window.grid_columnconfigure(1, weight=0)  # Right panel column
        
        # Create and place navbar
        self.navbar = ctk.CTkFrame(self.window, height=40)
        self.navbar.grid(row=0, column=0, columnspan=2, sticky="nsew")
        self.add_navbar_content()
        
        # Create and place right panel
        self.right_panel = ctk.CTkFrame(self.window, width=250)
        self.right_panel.grid(row=1, column=1, sticky="ns", padx=5, pady=5)
        self.add_right_panel_content()
        
        # Create chessboard container
        self.chess_container = ctk.CTkFrame(self.window)
        self.chess_container.grid(row=1, column=0, sticky="nsew", padx=5, pady=5)
        
        # Initialize chessboard
        self.board = ChessBoard(
            window=self.chess_container, 
            posX=0, 
            posY=0, 
            size=600, 
            engine_path=ENGINE_PATH
        )
        
        # Create and place text box
        self.text_box = ctk.CTkTextbox(self.window, height=100)
        self.text_box.insert("0.0", self.board.get_fen())
        self.text_box.grid(row=2, column=0, columnspan=2, sticky="nsew", padx=5, pady=5)
        self.window.mainloop()
        

    def add_navbar_content(self):
        # Add navbar components
        self.navbar.grid_columnconfigure(0, weight=1)
        
        # Left-aligned items
        left_frame = ctk.CTkFrame(self.navbar, fg_color="transparent")
        left_frame.grid(row=0, column=0, sticky="w")
        ctk.CTkButton(left_frame, text="New Game").grid(row=0, column=0, padx=5)
        ctk.CTkButton(left_frame, text="Settings").grid(row=0, column=1, padx=5)
        
        # Right-aligned items
        right_frame = ctk.CTkFrame(self.navbar, fg_color="transparent")
        right_frame.grid(row=0, column=1, sticky="e")
        ctk.CTkLabel(right_frame, text="Status:").grid(row=0, column=0, padx=5)
        self.status_label = ctk.CTkLabel(right_frame, text="Ready")
        self.status_label.grid(row=0, column=1, padx=5)

    def add_right_panel_content(self):
        # Add right panel components
        self.right_panel.grid_rowconfigure(0, weight=0)
        self.right_panel.grid_rowconfigure(1, weight=1)
        
        # Move list
        ctk.CTkLabel(self.right_panel, text="Move History").pack(pady=5)
        self.move_list = ctk.CTkTextbox(self.right_panel, wrap="none")
        self.move_list.pack(fill="both", expand=True, padx=5, pady=5)
        
        # Game info
        ctk.CTkLabel(self.right_panel, text="Game Information").pack(pady=5)
        self.game_info = ctk.CTkTextbox(self.right_panel, height=100)
        self.game_info.pack(fill="x", padx=5, pady=5)

if __name__ == "__main__":
    MainWindow()