import sys
import pygame
import string
import operator

pygame.init()

window = pygame.display.set_mode((640, 480), pygame.RESIZABLE)

array = []

class Glyph():
    def __init__(self, _name, _count):
        self.name = _name
        self.count = _count
    def log(self):
        print("Glyph: " + self.name + " Count: " + str(self.count))
    def isEqualTo(self, otherGlyph):
        if(otherGlyph.name == self.name):
            return 1
        return 0

def addGlyph(name):
    if(len(name) > 1):return
    if(name == " "):return
    if(name == "."):return
    if(name == "'"):return
    if(name == "!"):return

    global count
    global array
    alreadyContained = 0
    for glyph in array:
        if(glyph.name == name):
            glyph.count = glyph.count + 1
            alreadyContained = 1
    if(alreadyContained == 0):
        array.append(Glyph(name, 1))

def addGlyphsForString(string):
    toDo = string.lower()
    desired = "abcdefghijklmnopqrstuvwxyzÁÍÚÉÓÑáíúüéóñ"
    undesired = list(set(toDo) - set(desired))
    for d in range(0, len(undesired)):toDo = toDo.replace(undesired[d], "")
    # Now I'm going to cheat and replace accented letters with their natural counterparts
    # ÁÍÚÉÓÑáíúüéóñ
    toDo = toDo.replace('Á', 'a');toDo = toDo.replace('Í', 'i');toDo = toDo.replace('Ú', 'u');
    toDo = toDo.replace('É', 'e');toDo = toDo.replace('Ó', 'o');toDo = toDo.replace('Ñ', 'n');
    toDo = toDo.replace('á', 'a');toDo = toDo.replace('í', 'i');toDo = toDo.replace('ú', 'u');
    toDo = toDo.replace('ü', 'u');toDo = toDo.replace('é', 'e');toDo = toDo.replace('ó', 'o');
    toDo = toDo.replace('ñ', 'n');
    letters = list(toDo)
    for letter in letters:
        addGlyph(letter)

def render():
    global array
    largestCount = 0
    xBuffer = 2
    for glyph in array:
        if(glyph.count > largestCount):
            largestCount = glyph.count
    screenHeight = pygame.display.Info().current_h
    screenWidth = pygame.display.Info().current_w
    totalXBufferSize = len(array) * xBuffer
    defaultWidth = (screenWidth-totalXBufferSize) / len(array)
    defaultHeight = screenHeight
    if(largestCount > 0):
        defaultHeight = defaultHeight / largestCount

    window.fill((0, 0, 0))
    myFont = pygame.font.SysFont("arial", 10)
    for idx, glyph in enumerate(array):
        x = idx * defaultWidth + (idx+1)*xBuffer
        centerX = x + defaultWidth/2.0
        height = defaultHeight * glyph.count
        pygame.draw.rect(window, (255, 0, 0), (x, screenHeight, defaultWidth, -height), 0)
        blitHeight = screenHeight-(idx*((screenHeight-40)/len(array)))

        nameWidth = myFont.size(glyph.name)[0]
        nameX = centerX - nameWidth/2.0
        nameLabel = myFont.render(glyph.name, 1, (0, 255, 255))
        window.blit(nameLabel, (nameX, blitHeight-10))

        countWidth = myFont.size(str(glyph.count))[0]
        countLabel = myFont.render(str(glyph.count), 1, (255, 255, 0))
        window.blit(countLabel, (centerX-countWidth/2.0, blitHeight-20))
    pygame.display.flip()

def handleInput():
    for event in pygame.event.get():
            if(event.type == pygame.QUIT):
                sys.exit(0)
            elif(event.type == pygame.KEYDOWN):
                if(event.key == pygame.K_ESCAPE):
                    sys.exit(0)
                else:
                    addGlyph(pygame.key.name(event.key))
                    global array
            elif(event.type == pygame.VIDEORESIZE):
                global window
                window = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)
                #print(event.w)

file = open('input.txt', 'r')
string = file.read()
addGlyphsForString(string)
array.sort(key = operator.attrgetter('name'))

pygame.time.Clock().tick(60)
while True:
    render()
    handleInput()
