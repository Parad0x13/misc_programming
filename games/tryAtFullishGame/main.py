import pygame
from pygame.locals import *
pygame.init()

import sys, math, random

#Custom classes
import RenderEngine

#Constants
#none

class Game:
    def __init__(self):
        self.renderEngine = RenderEngine.RenderEngine()
        self.gameActive = True
    def gameLoop(self):
        while self.gameActive:
            self.renderEngine.render()
