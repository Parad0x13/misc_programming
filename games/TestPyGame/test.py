import pygame, sys
from pygame.locals import *

class Vect:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.resolution = x*y
    def show(self):print "(%i, %i)" % (self.x, self.y)
    def __add__(self, other):
        if isinstance(other, (int, long)):return Vect(self.x+other, self.y+other)
        elif isinstance(other, Vect):return Vect(self.x+other.x, self.y+other.y)
    def __sub__(self, other):
        if isinstance(other, (int, long)):return Vect(self.x-other, self.y-other)
        elif isinstance(other, Vect):return Vect(self.x-other.x, self.y-other.y)
    def __mul__(self, other):
        if isinstance(other, (int, long)):return Vect(self.x*other, self.y*other)
        elif isinstance(other, Vect):return Vect(self.x*other.x, self.y*other.y)
    def __div__(self, other):
        if isinstance(other, (int, long)):return Vect(self.x/other, self.y/other)
        elif isinstance(other, Vect):return Vect(self.x/other.x, self.y/other.y)

#Constants
screen_size = Vect(400, 400)
screen = pygame.display.set_mode((screen_size.x, screen_size.y), 0, 32)
pressed_keys = pygame.key.get_pressed()
clock = pygame.time.Clock()

class Entity:
    loc = Vect(0, 0)
    icon = None
    size = None
    def __init__(self):
        self.assignIcon('Entity.png')
    def assignIcon(self, name):
        self.icon = pygame.image.load(name).convert_alpha()
        self.size = Vect(self.icon.get_width(), self.icon.get_height())
    def move(self, value):self.loc += value

class Player(Entity):
    def __init__(self):
        self.assignIcon('Player.png')

class Grass(Entity):
    def __init__(self):
        self.assignIcon('Grass.png')

class ClayHouse(Entity):
    def __init__(self):
        self.assignIcon('ClayHouse.png')

class World:
    entities = []
    tileSize = Vect(50, 50)
    def __init__(self):
        for y in range(5):
            for x in range(5):
                self.addToTile(Grass(), Vect(x-2, y-2))
        self.addToLocation(ClayHouse(), Vect(-100, -150))
    def addToTile(self, entity, tileLoc = Vect(0, 0), centered = True):
        loc = Vect(tileLoc.x*self.tileSize.x, tileLoc.y*self.tileSize.y)
        entity.loc = loc
        if centered:entity.loc -= entity.size/2
        if entity not in self.entities:
            self.entities.append(entity)
    def addToLocation(self, entity, loc = Vect(0, 0), centered = True):
        entity.loc = loc
        if centered:entity.loc -= entity.size/2
        if entity not in self.entities:
            self.entities.append(entity)

class Game:
    world = None
    gameActive = False
    player = None
    def __init__(self):
        self.world = World()
        self.player = Player()
        self.world.addToTile(self.player)
    def startGame(self):
        self.gameActive = True
        self.gameLoop()
    def gameLoop(self):
        while self.gameActive:
            self.handleInput()
            self.render()
            pygame.time.wait(5)
    def handleInput(self):
        global pressed_keys
        for event in pygame.event.get():
            if event.type == QUIT:self.gameActive = False
            if event.type == KEYUP or event.type == KEYDOWN:pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_ESCAPE]:self.gameActive = False
        if pressed_keys[K_w]:self.player.move(Vect(0, -1))
        if pressed_keys[K_s]:self.player.move(Vect(0, 1))
        if pressed_keys[K_a]:self.player.move(Vect(-1, 0))
        if pressed_keys[K_d]:self.player.move(Vect(1, 0))
    def render(self):
        screen.fill((0, 0, 0))
        for entity in self.world.entities:
            self.renderEntity(entity)
        self.renderEntity(self.player)
        pygame.display.update()
    def renderEntity(self, entity):
        camera = entity.loc+(screen_size/2)
        screen.blit(entity.icon, (camera.x, camera.y))

game = Game()
game.startGame()

pygame.quit()
sys.exit()
