import pygame
from pygame.locals import *

#Constants
screenWidth = 320
screenHeight = 200

class RenderEngine:
    def __init__(self):
        self.screen = pygame.display.set_mode((screenWidth, screenHeight), 0, 32)
    def render(self):
        pygame.display.update()
