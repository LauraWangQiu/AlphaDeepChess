#fenText.py
import customtkinter as ctk

class FenText:

    BORDER_COLOR_INVALID = "#ff0000"
    BORDER_COLOR_DEFAULT = "#808080"

    def __init__(self, window, initialFen):
        
        # Create the textbox and initialize it with the current FEN from chessBoard
        self.fenText = ctk.CTkTextbox(window, height=100, border_width=2, undo=True)
        self.fenText.insert("0.0", initialFen)
        self.fenText.grid(row=2, column=0, columnspan=2, sticky="nsew", padx=5, pady=5)

        # Bind the <<Modified>> event to check the FEN on text change
        self.fenText.bind("<<Modified>>", self.on_text_change)

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