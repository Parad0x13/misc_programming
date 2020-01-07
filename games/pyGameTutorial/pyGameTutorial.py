import pygame, sys, math, random
from pygame.locals import *

#Constants
charWidthPix = 8
charHeightPix = 12
screenWidthChar = 80
screenHeightChar = 25
screenWidthPix = (screenWidthChar*charWidthPix)
screenHeightPix = (screenHeightChar*charHeightPix)

eType_WATER = 100
eType_PLAYER = 101

pygame.init()
screen = pygame.display.set_mode((screenWidthPix, screenHeightPix), 0, 32)
pressed_keys = pygame.key.get_pressed()
clock = pygame.time.Clock()

class Vect:
    def __init__(self, _x, _y):
        self.x = _x
        self.y = _y
    def __add__(self, other): return Vect(self.x+other.x, self.y+other.y)
    def __sub__(self, other): return Vect(self.x-other.x, self.y-other.y)

class Entity:
    def __init__(self):
        self.loc = Vect(0, 0)
        self.name = 'defaultEntity'
        self.icon = pygame.image.load('Default.jpg').convert()

class World:
    def __init__(self):
        self.entities = []
        self.entities.append(Entity())

class Game:
    def __init__(self):
        self.gameActive = True
        self.world = World()
        self.camera = Vect(0, 0)
        self.prevTick = 0
        self.newTick = 0
    def Render(self):
        screen.fill((0, 0, 0))
        for entity in self.world.entities:
            offset = Vect(screenWidthPix/2, screenHeightPix/2) - self.camera
            screen.blit(entity.icon, (offset.x, offset.y))
        pygame.display.update()
    def HandleInput(self):
        global pressed_keys
        for event in pygame.event.get():
            if event.type == QUIT:
                self.gameActive = False
            if event.type == KEYDOWN or event.type == KEYUP:
                pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_ESCAPE]: self.gameActive = False
        if pressed_keys[K_LEFT]: self.camera -= Vect(1, 0)
        if pressed_keys[K_RIGHT]: self.camera += Vect(1, 0)
        if pressed_keys[K_UP]: self.camera -= Vect(0, 1)
        if pressed_keys[K_DOWN]: self.camera += Vect(0, 1)
    def tick(self):
        self.newTick = pygame.time.get_ticks()
        self.timeDelta = self.prevTick-self.newTick
        self.prevTick = self.newTick
    def GameLoop(self):
        while self.gameActive:
            self.HandleInput()
            self.Render()
            self.tick()

game = Game()
game.GameLoop()

print 'Goodbye!'
pygame.quit()
sys.exit()
