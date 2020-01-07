import pygame, sys, random
from pygame.locals import *

#Constants
ALIVE = True
DEAD = False

class Vect:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.resolution = x*y

class World:
    def __init__(self, size):
        self.size = size
        self.array1 = []
        self.array2 = []
        self.primaryArray = self.array1
        self.secondaryArray = self.array2
        for d in range(size.resolution):
            tmp = random.randint(0, 1)
            if tmp == 0:
                self.array1.append(DEAD)
                self.array2.append(DEAD)
            else:
                self.array1.append(ALIVE)
                self.array2.append(ALIVE)
    def locWithinLimits(self, loc):
        retVal = loc
#        retVal.x = loc.x%self.size.x
#        retVal.y = loc.y%self.size.y
        if retVal.x == -1:retVal.x = self.size.x-1
        elif retVal.x == screenSize.x:retVal.x = 0
        if retVal.y == -1:retVal.y = screenSize.y-1
        elif retVal.y == screenSize.y:retVal.y = 0
        return retVal
    def valueAtLoc(self, loc):
        fixedLoc = self.locWithinLimits(loc)
        return self.primaryArray[self.indexForLoc(fixedLoc)]
    def indexForLoc(self, loc):
        return loc.y*self.size.x+loc.x
    def locForIndex(self, index):
        x = index-((index/screenSize.x)*screenSize.x)
        y = index/screenSize.x
        return Vect(x, y)
    def swapArrays(self):
        if self.primaryArray == self.array1:
            self.primaryArray = self.array2
            self.secondaryArray = self.array1
        else:
            self.primaryArray = self.array1
            self.secondaryArray = self.array2
    def iterate(self):
        for y in range(self.size.y):
            for x in range(self.size.x):
                index = self.indexForLoc(Vect(x, y))
                self.secondaryArray[index] = self.primaryArray[index]
                friends = 0
                if self.primaryArray[self.valueAtLoc(Vect(x-1, y-1))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x, y-1))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x+1, y-1))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x-1, y))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x+1, y))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x-1, y+1))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x, y+1))] == ALIVE: friends+=1
                if self.primaryArray[self.valueAtLoc(Vect(x+1, y+1))] == ALIVE: friends+=1
                ###
                if self.primaryArray[index] == ALIVE:
                    if friends < 2 or friends > 3:self.secondaryArray[index] = DEAD
                else:
                    if friends == 3:self.secondaryArray[index] = ALIVE
        for d in range(self.size.resolution):
            self.primaryArray[d] = self.secondaryArray[d]
        #self.primaryArray = self.secondaryArray
        #self.swapArrays()
#        t = self.primaryArray
#        self.primaryArray = self.secondaryArray

screenSize = Vect(320, 200)
pygame.init()
screen = pygame.display.set_mode((screenSize.x, screenSize.y), 0, 32)
pressed_keys = pygame.key.get_pressed()
clock = pygame.time.Clock()
gameActive = True
world = World(screenSize)

def render():
    screen.fill((0, 0, 255))
    for index in range(screenSize.resolution):
        loc = world.locForIndex(index)
        status = world.primaryArray[index]
        if status == ALIVE:screen.set_at((loc.x, loc.y), (255, 255, 255))
        elif status == DEAD:screen.set_at((loc.x, loc.y), (0, 0, 0))
    pygame.display.flip()

def handleInput():
    global pressed_keys, gameActive
    for event in pygame.event.get():
        if event.type == QUIT:
            gameActive = False
        if event.type == KEYDOWN or event.type == KEYUP:
            pressed_keys = pygame.key.get_pressed()
        if pressed_keys[K_ESCAPE]:gameActive = False

def logic():
    world.iterate()

iteration = 0
while gameActive:
    handleInput()
    render()
    logic()
    print iteration
    iteration += 1
    #pygame.time.wait(100)

print 'Goodbye!'
pygame.quit()
sys.exit()
