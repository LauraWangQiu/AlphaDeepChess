#rightPanel.py
import customtkinter as ctk
from gui.uci import Uci

class RightPanel:
    def __init__(self, window, UCI: Uci):
        self.right_panel = ctk.CTkFrame(window, width=250)
        self.right_panel.grid(row=1, column=1, sticky="ns", padx=5, pady=5)
        self.right_panel.grid_rowconfigure(0, weight=0)
        self.right_panel.grid_rowconfigure(1, weight=1)
        self.right_panel.grid_rowconfigure(2, weight=0)  # Added for engine display
        
        self.UCI = UCI

        self.last_eval = 0
        self.last_move = None

        # Engine evaluation switch
        self.engine_switch = ctk.CTkSwitch(
            self.right_panel,
            text="Engine Evaluation",
            width=200,
            command=self.get_search_info  # Add command to update display
        )
        self.engine_switch.pack(pady=5, padx=5)
        
        # Engine move display rectangle
        self.engine_display = ctk.CTkFrame(
            self.right_panel,
            height=40,
            fg_color="#2B2B2B"  # Darker color to distinguish it
        )
        self.engine_display.pack(fill="x", padx=5, pady=5)
        
        # Label inside the engine display
        self.engine_move_label = ctk.CTkLabel(
            self.engine_display,
            text="Engine: Off",
            anchor="w"
        )
        self.engine_move_label.pack(padx=5, pady=5, fill="x")

        # Move list
        ctk.CTkLabel(self.right_panel, text="Move History").pack(pady=5)
        self.move_list = ctk.CTkTextbox(self.right_panel, wrap="none")
        self.move_list.pack(fill="both", expand=True, padx=5, pady=5)
    
    def set_eventManager(self, eventManager):
        self.eventManager = eventManager
        
    def position_changed(self):
        self.UCI.start_search()
        self.get_search_info()

    def get_search_info(self):
        """Method to continuously update the evaluation when engine is on"""

        print("get_search_info")
        if self.engine_switch.get():
            self.UCI.get_search_info(self.last_eval, self.last_move)
            self.right_panel.after(500, self.get_search_info)

            try:
                # Format the evaluation (positive for white, negative for black)
                if self.last_eval is not None:
                    if abs(self.last_eval) < 1000:  # Regular evaluation
                        display_text = f"Eval: {self.last_eval:+.2f}"
                    else:  # Mate score
                        display_text = f"Mate in {int(self.last_eval/1000)}"
                else:
                    display_text = "Eval: N/A"
            except Exception as e:
                display_text = f"Error: {str(e)}"
        else: 
            self.UCI.stop_search()
            display_text = "Engine: Off"
        
        self.engine_move_label.configure(text=display_text)

