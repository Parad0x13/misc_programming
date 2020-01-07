import pygame, sys, math, random
from pygame.locals import *

#Constants
BLOCK_WATER = 100
CharacterWidth = 8
CharacterHeight = 12
ScreenCharacterWidth = 80
ScreenCharacterHeight = 25
LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3

pygame.init()
screen = pygame.display.set_mode((CharacterWidth*ScreenCharacterWidth, CharacterHeight*ScreenCharacterHeight), 0, 32)

#Opt out of doing it in-code
playerIcon=pygame.image.load("Player.jpg").convert()
waterIcon=pygame.image.load("Water_1.jpg").convert()

world = []
entities = []

pressed_keys = pygame.key.get_pressed()

gameThrottle = 0.1
friction = 0.002

class Vect():
    def __init__(self, _x = 0, _y = 0):
        self.x = _x
        self.y = _y

class Player:
    def __init__(self):
        self.internalLoc = Vect(20, 10)
        self.vel = Vect(0, 0)
    def logic(self):
        self.move()
    def move(self):
        self.internalLoc.x += self.vel.x
        self.internalLoc.y += self.vel.y
        self.vel.x -= self.vel.x*friction
        self.vel.y -= self.vel.y*friction
    def moveGrainy(self, direction):
        self.vel = Vect(0, 0)
        if direction == LEFT: self.internalLoc.x-=1
        elif direction == RIGHT: self.internalLoc.x+=1
        elif direction == UP: self.internalLoc.y-=1
        elif direction == DOWN: self.internalLoc.y+=1
    def loc(self):
        return Vect(math.floor(self.internalLoc.x), math.floor(self.internalLoc.y))
player = Player()

class Entity:
    def __init__(self):
        self.icon = pygame.image.load("Goal.jpg").convert()
        self.loc = Vect(random.randint(0, ScreenCharacterWidth), random.randint(0, ScreenCharacterHeight))

def initWorld():
    for y in xrange(ScreenCharacterHeight):
        for x in xrange(ScreenCharacterWidth):
            world.append(BLOCK_WATER)

def render():
    for y in xrange(ScreenCharacterHeight):
        for x in xrange(ScreenCharacterWidth):
            toBlit = None
            if world[y*ScreenCharacterWidth+x] == BLOCK_WATER:
                toBlit = waterIcon
            screen.blit(toBlit, (x*CharacterWidth, y*CharacterHeight))
    for entity in entities:
        screen.blit(entity.icon, (entity.loc.x*CharacterWidth, entity.loc.y*CharacterHeight))
    screen.blit(playerIcon, (player.loc().x*CharacterWidth, player.loc().y*CharacterHeight))
    pygame.display.update()

def handleInput():
    global pressed_keys
    #Grab Input
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == KEYDOWN or event.type == KEYUP:
            pressed_keys = pygame.key.get_pressed()
            if pressed_keys[K_a]: player.moveGrainy(LEFT)
            if pressed_keys[K_d]: player.moveGrainy(RIGHT)
            if pressed_keys[K_w]: player.moveGrainy(UP)
            if pressed_keys[K_s]: player.moveGrainy(DOWN)
            #Process Key Input
    if pressed_keys[K_ESCAPE]:
        pygame.quit()
        sys.exit()
    if pressed_keys[K_LEFT]: player.vel.x-=0.001*gameThrottle
    if pressed_keys[K_RIGHT]: player.vel.x+=0.001*gameThrottle
    if pressed_keys[K_UP]: player.vel.y-=0.001*gameThrottle
    if pressed_keys[K_DOWN]: player.vel.y+=0.001*gameThrottle

def handleCollisions():
    global entities
    entitiesToRemove = []
    for entity in entities:
        if entity.loc.x == player.loc().x and entity.loc.y == player.loc().y:
            entitiesToRemove.append(entity)
    for x in xrange(len(entitiesToRemove)): entities.append(Entity())
    entities = list(set(entities) - set(entitiesToRemove))

initWorld()
entities.append(Entity())
while True:
    render()
    handleInput()
    player.logic()
    handleCollisions()
