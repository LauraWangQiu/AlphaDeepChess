import pygame

class TextBox:
    def __init__(self, posX, posY, width, height, font, text=""):
        self.rect = pygame.Rect(posX, posY, width, height)
        self.text = text
        self.is_selected = False
        self.font = font
        self.bg_color = (0, 0, 0)  # Black background
        self.border_color = (0, 255, 0)  # Green border when selected
        self.text_color = (200, 200, 200)  # Gray text

    def draw(self, surface):
        """Draw the input box and text centered."""
        # Draw background
        pygame.draw.rect(surface, self.bg_color, self.rect)

        # Draw border
        border_color = self.border_color if self.is_selected else (100, 100, 100)
        pygame.draw.rect(surface, border_color, self.rect, 2)

        # Render text
        text_surface = self.font.render(self.text, True, self.text_color)
        
        # center and left-align text
        x_offset = self.rect.x + 10  # Small padding from left
        y_centered = self.rect.y + (self.rect.height - text_surface.get_height()) // 2
        surface.blit(text_surface, (x_offset, y_centered))

    def handle_event(self, event):
        """Handle user input for the text box."""
        if event.type == pygame.MOUSEBUTTONDOWN:
            self.is_selected = self.rect.collidepoint(event.pos)

        elif event.type == pygame.KEYDOWN and self.is_selected:
            if event.key == pygame.K_RETURN:
                self.is_selected = False
            elif event.key == pygame.K_BACKSPACE:
                self.text = self.text[:-1]  # Remove last character
            else:
                self.text += event.unicode  # Append typed character

    def isSelected(self):
        return self.is_selected
