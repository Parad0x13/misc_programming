import pygame, sys, math, random
from pygame.locals import *
from collections import namedtuple as ntuple

PixelSize = ntuple('PixelSize', 'w h')
PixelLoc = ntuple('PixelLoc', 'x y')

#Constants
UP =        100
DOWN =      101
LEFT =      102
RIGHT =     103
RANDOM =    104

TOOFAR = 1000
BAD = 1001
GOOD = 1002
GREAT = 1003

screen_size = PixelSize(640, 480)
LeftLoc = PixelLoc(0, 50)
DownLoc = PixelLoc(100, 50)
UpLoc = PixelLoc(200, 50)
RightLoc = PixelLoc(300, 50)

pygame.init()

screen = pygame.display.set_mode(screen_size, 0, 32)
pressed_keys = pygame.key.get_pressed()
clock = pygame.time.Clock()

class Arrow:
    speed = 0.25
    def __init__(self, _direction):
        self.lastTick = clock.tick()
        self.direction = _direction
        if self.direction == RANDOM:
            value = random.randint(0, 4)
            if value == 0:self.direction = UP
            if value == 1:self.direction = DOWN
            if value == 2:self.direction = LEFT
            if value == 3:self.direction = RIGHT
        self.loc = PixelLoc(0, screen_size.h)
        if self.direction == UP:
            self.icon = pygame.image.load('Up.png').convert_alpha()
            self.loc = PixelLoc(UpLoc.x, self.loc.y)
        elif self.direction == DOWN:
            self.icon = pygame.image.load('Down.png').convert_alpha()
            self.loc = PixelLoc(DownLoc.x, self.loc.y)
        elif self.direction == LEFT:
            self.icon = pygame.image.load('Left.png').convert_alpha()
            self.loc = PixelLoc(LeftLoc.x, self.loc.y)
        elif self.direction == RIGHT:
            self.icon = pygame.image.load('Right.png').convert_alpha()
            self.loc = PixelLoc(RightLoc.x, self.loc.y)
    def logic(self):
        delta = clock.tick()-self.lastTick
        delta *= self.speed
        self.loc = PixelLoc(self.loc.x, self.loc.y-delta)
    def getConnectStatus(self, distance):
        if distance > 50:
            return TOOFAR
        if distance > 30:
            return BAD
        if distance > 10:
            return GOOD
        if distance > 0:
            return GREAT

class GameArrow:
    def __init__(self, _direction):
        self.direction = _direction
        self.determineLocation()
        self.upKeys = [K_UP, K_k]
    def logic(self):
        pass
    def Icon(self):
        global pressed_keys
        if self.direction == UP and pressed_keys[K_UP] == True:return pygame.image.load('ArrowUpH.png').convert_alpha()
        if self.direction == UP and pressed_keys[K_UP] == False:return pygame.image.load('ArrowUp.png').convert_alpha()
#        for key in self.upKeys:
#            if self.direction == UP and pressed_keys[key] == True:return pygame.image.load('ArrowUpH.png').convert_alpha()
#            if self.direction == UP and pressed_keys[key] == False:return pygame.image.load('ArrowUp.png').convert_alpha()
        if self.direction == DOWN and pressed_keys[K_DOWN] == True:return pygame.image.load('ArrowDownH.png').convert_alpha()
        if self.direction == DOWN and pressed_keys[K_DOWN] == False:return pygame.image.load('ArrowDown.png').convert_alpha()
        if self.direction == LEFT and pressed_keys[K_LEFT] == True:return pygame.image.load('ArrowLeftH.png').convert_alpha()
        if self.direction == LEFT and pressed_keys[K_LEFT] == False:return pygame.image.load('ArrowLeft.png').convert_alpha()
        if self.direction == RIGHT and pressed_keys[K_RIGHT] == True:return pygame.image.load('ArrowRightH.png').convert_alpha()
        if self.direction == RIGHT and pressed_keys[K_RIGHT] == False:return pygame.image.load('ArrowRight.png').convert_alpha()
    def determineLocation(self):
        if self.direction == UP:self.loc = UpLoc
        elif self.direction == DOWN:self.loc = DownLoc
        elif self.direction == LEFT:self.loc = LeftLoc
        elif self.direction == RIGHT:self.loc = RightLoc

class Game:
    def __init__(self):
        self.gameActive = True
        self.arrows = []
        #self.addArrow()
        pygame.time.set_timer(pygame.USEREVENT + 1, 1000)
        self.gameArrows = []
        self.LeftArrow = GameArrow(LEFT)
        self.RightArrow = GameArrow(RIGHT)
        self.UpArrow = GameArrow(UP)
        self.DownArrow = GameArrow(DOWN)
        self.gameArrows.append(self.LeftArrow)
        self.gameArrows.append(self.RightArrow)
        self.gameArrows.append(self.UpArrow)
        self.gameArrows.append(self.DownArrow)
        self.score = 0
    def gameLoop(self):
        while self.gameActive:
            self.handleInput()
            self.render()
            self.handleTimers()
            self.tick()
            pygame.time.wait(10)
    def handleTimers(self):
        for event in pygame.event.get():
            if event.type == pygame.USEREVENT + 1:
                self.addArrow()
    def handleInput(self):
        global pressed_keys
        for event in pygame.event.get():
            if event.type == QUIT:
                self.gameActive = False
            if event.type == KEYDOWN or event.type == KEYUP:
                pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_ESCAPE]: self.gameActive = False
        if pressed_keys[K_UP]:
            arrow = self.TopArrow()
            if arrow:
                distance = math.fabs(arrow.loc.y-self.UpArrow.loc.y)
                status = arrow.getConnectStatus(distance)
                if status == BAD:
                    self.arrows.remove(arrow)
                if status == GOOD:
                    self.score += 10
                    self.arrows.remove(arrow)
                if status == GREAT:
                    self.score += 30
                    self.arrows.remove(arrow)
    def render(self):
        screen.fill((0, 0, 0))
        for gameArrow in self.gameArrows:
            screen.blit(gameArrow.Icon(), gameArrow.loc)
        for arrow in self.arrows:
            screen.blit(arrow.icon, (arrow.loc.x, arrow.loc.y))
        font = pygame.font.Font(None, 32)
        text = font.render(str(self.score), True, (255, 255, 0), (0, 0, 0))
        textRect = text.get_rect()
        textRect.centerx = 15
        textRect.centery = screen.get_rect().height-15
        screen.blit(text, textRect)
        pygame.display.update()
    def tick(self):
        for arrow in self.arrows:
            arrow.logic()
            if arrow.loc.y < 0:
                self.arrows.remove(arrow)
    def TopArrow(self):
        if len(self.arrows) > 0:return self.arrows[0]
    def addArrow(self):
        #self.arrows.append(Arrow(RANDOM))
        self.arrows.append(Arrow(UP))

game = Game()
game.gameLoop()

print 'Goodbye!'
pygame.quit()
sys.exit()
