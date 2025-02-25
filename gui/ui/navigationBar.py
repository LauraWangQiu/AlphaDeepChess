#navigationBar.py

import customtkinter as ctk


class NavigationBar:

    def __init__(self, window):
        self.navbar = ctk.CTkFrame(window, height=40)
        self.navbar.grid(row=0, column=0, columnspan=2, sticky="nsew")
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