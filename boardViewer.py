import pygame
import sys
import os
from boardViewerSrc.chessBoard import ChessBoard
from boardViewerSrc.ui.textbox import TextBox
# Initialize pygame
pygame.init()

# Constants
SCREEN_WIDTH, SCREEN_HEIGHT = 700, 800

FONT = pygame.font.Font(None, 28)

ENGINE_PATH = "build/AlphaDeepChess"
if sys.platform.startswith("win"):
    ENGINE_PATH += ".exe"

if not os.path.exists(ENGINE_PATH):
    print(f"Error: Engine file not found at '{ENGINE_PATH}'")
    sys.exit(1)  # Exit the program with an error code

# Initialize board

board = ChessBoard(posX=0, posY=0, size=SCREEN_WIDTH, engine_path=ENGINE_PATH)
fenTextBox = TextBox(posX=0,posY=SCREEN_WIDTH,width=SCREEN_WIDTH,height=50,font=FONT, text=board.get_fen())

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Chess Board")

selected_square = None  # Store selected square

def draw():
    """Redraws the board."""
    board.draw(screen)
    fenTextBox.draw(screen)
    pygame.display.flip()

def main():
    global board
    running = True

    draw()

    while running:
        
        event = pygame.event.wait()  # Wait for an event, reducing CPU usage

        fenTextBox.handle_event(event)
        
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_r:
                board.rotate_orientation()

            draw()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouseX, mouseY = event.pos
            if board.is_clicked(mouseX,mouseY):
                board.on_click(mouseX, mouseY)
            
            draw()
    
    pygame.quit()

    del board 

if __name__ == "__main__":
    main()