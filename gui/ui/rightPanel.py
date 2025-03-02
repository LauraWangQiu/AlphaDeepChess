#rightPanel.py
import customtkinter as ctk

class RightPanel:
    def __init__(self, window):
        self.right_panel = ctk.CTkFrame(window, width=250)
        self.right_panel.grid(row=1, column=1, sticky="ns", padx=5, pady=5)
        self.right_panel.grid_rowconfigure(0, weight=0)
        self.right_panel.grid_rowconfigure(1, weight=1)
        self.right_panel.grid_rowconfigure(2, weight=0)  # Added for engine display
        
        self.searching = False
        
        # Engine evaluation switch
        self.engine_switch = ctk.CTkSwitch(
            self.right_panel,
            text="Engine Evaluation",
            font=("Arial", 20),
            width=200,
            command=self.engine_switch_clicked
        )
        self.engine_switch.pack(pady=5, padx=5)
        
        # Engine move display rectangle
        self.engine_display = ctk.CTkFrame(
            self.right_panel,
            height=40,
            fg_color="#2B2B2B"
        )
        self.engine_display.pack(fill="x", padx=5, pady=5)
        
        self.engine_eval_label = ctk.CTkLabel(
            self.engine_display,
            text="",
            anchor="w",
            font=("Arial", 20)
        )
        self.engine_eval_label.pack(padx=5, pady=5, fill="x")

        self.engine_move_label = ctk.CTkLabel(
            self.engine_display,
            text="",
            anchor="w",
            font=("Arial", 20)
        )
        self.engine_move_label.pack(padx=5, pady=5, fill="x")

        self.engine_depth_label = ctk.CTkLabel(
            self.engine_display,
            text="",
            anchor="w",
            font=("Arial", 20)
        )
        self.engine_depth_label.pack(padx=5, pady=5, fill="x")

        self.delay_between_get_info_ms = 10
        self.MAX_DELAY_BETWEEN_GET_INFO_MS = 500

        self.get_search_info()

    
    def set_eventManager(self, eventManager):
        self.eventManager = eventManager

    def engine_switch_clicked(self):
        if self.engine_switch.get():
            self.start_search()
        else:
            self.stop_search()

    def start_search(self):
        self.eventManager.start_search()
        self.delay_between_get_info_ms = 50
        self.searching = True
        self.engine_move_label.configure(text="Engine: On")


    def stop_search(self):
        self.searching = False
        self.eventManager.stop_search()
        self.engine_move_label.configure(text="Engine: Off")
        self.engine_eval_label.configure(text="")
        self.engine_move_label.configure(text="")
        self.engine_depth_label.configure(text="")
        self.eventManager.set_engine_move(None)


    def position_changed(self):
        if self.engine_switch.get():
            self.start_search()


    def get_search_info(self):
        """Method to continuously update the evaluation when engine is on"""

        self.right_panel.after(self.delay_between_get_info_ms, self.get_search_info) # repeat the next call

        if self.searching == False:
            return

        depth, eval, best_move, new_info = self.eventManager.get_search_info()

        if new_info == False:
            return

        self.delay_between_get_info_ms += 10 
        if self.delay_between_get_info_ms > self.MAX_DELAY_BETWEEN_GET_INFO_MS:
            self.delay_between_get_info_ms = self.MAX_DELAY_BETWEEN_GET_INFO_MS

        try:
            # Format the evaluation (positive for white, negative for black)
            if eval is not None:
                eval_text = f"Eval: {eval/100:+.2f}"
            else:
                eval_text = "Eval: N/A"
        except Exception as e:
            eval_text = f"Error: {str(e)}"

        best_move_text = f"Best Move: {best_move}"
        depth_text = f"Depth: {depth}"

        self.engine_eval_label.configure(text=eval_text)
        self.engine_move_label.configure(text=best_move_text)
        self.engine_depth_label.configure(text=depth_text)

        self.eventManager.set_engine_move(best_move)




