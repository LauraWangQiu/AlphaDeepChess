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

        # Engine evaluation switch
        self.engine_switch = ctk.CTkSwitch(
            self.right_panel,
            text="Engine Evaluation",
            width=200,
            command=self.update_engine_display  # Add command to update display
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
    
    def update_engine_display(self):
        """Update the engine display based on switch state"""
        if self.engine_switch.get():  # If switch is ON
            try:
                eval_score = self.UCI.get_evaluation()
                print(eval_score)
                # Format the evaluation (positive for white, negative for black)
                if eval_score is not None:
                    if abs(eval_score) < 1000:  # Regular evaluation
                        display_text = f"Eval: {eval_score:+.2f}"
                    else:  # Mate score
                        display_text = f"Mate in {int(eval_score/1000)}"
                else:
                    display_text = "Eval: N/A"
            except Exception as e:
                display_text = f"Error: {str(e)}"
        else:  # If switch is OFF
            display_text = "Engine: Off"
        
        self.engine_move_label.configure(text=display_text)

    def update(self):
        """Method to continuously update the evaluation when engine is on"""
        if self.engine_switch.get():
            self.update_engine_display()
            self.right_panel.after(1000, self.update)  # Update every second