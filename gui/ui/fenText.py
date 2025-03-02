#fenText.py
import customtkinter as ctk

class FenText:

    BORDER_COLOR_INVALID = "#ff0000"
    BORDER_COLOR_DEFAULT = "#808080"

    FONT_SIZE = 18
    MAX_FEN_CHARACTERS = 100
    HEIGHT = FONT_SIZE + FONT_SIZE//2
    WIDTH = (FONT_SIZE + 1) * MAX_FEN_CHARACTERS

    def __init__(self, window, initialFen):
        
        self.fenText = ctk.CTkTextbox(
            window,
            height=self.HEIGHT,
            width=self.WIDTH,
            border_width=2,
            font=("Arial", self.FONT_SIZE),
            undo=True)
        
        self.fenText.insert("0.0", initialFen)
        self.fenText.grid(row=2, column=0, columnspan=1, padx=30, pady=30)

        
        self.fenText.bind("<<Modified>>", self.on_text_change)
        self.fenText.bind("<KeyRelease>", self.limit_text_length)  # Enforce max length

        # Reset the modified flag after initialization
        self.fenText.edit_modified(False)
        self.eventManager = None

    def set_fen(self, newFen):
        self.fenText.unbind("<<Modified>>")
        self.fenText.delete("0.0", "end")  # Clear the current content
        self.fenText.insert("0.0", newFen)  # Insert the new FEN
        self.fen_is_valid()
        self.fenText.edit_modified(False)
        # in 500 ms bind enable again the modified trigger
        self.fenText.after(500, lambda: self.fenText.bind("<<Modified>>", self.on_text_change))

    def on_text_change(self, event):
        """Callback triggered when the textbox content changes."""
        fen = self.fenText.get("0.0", "end").strip()
        self.eventManager.fenTextModified(fen)
        # Reset the modified flag to detect future changes
        self.fenText.edit_modified(False)

    def fen_is_invalid(self):
        """Notify the fen that its fen is invalid."""
        self.fenText.configure(border_color=self.BORDER_COLOR_INVALID)

    def fen_is_valid(self):
        """Notify the fen that its fen is valid."""
        self.fenText.configure(border_color=self.BORDER_COLOR_DEFAULT)  

    def set_eventManager(self, eventManager):
        self.eventManager = eventManager

    def limit_text_length(self, event):
        """Ensures the text does not exceed 100 characters."""
        text = self.fenText.get("0.0", "end").strip()
        if len(text) > self.MAX_FEN_CHARACTERS:
            self.fenText.delete("0.0", "end")
            self.fenText.insert("0.0", text[:self.MAX_FEN_CHARACTERS])  # Truncate