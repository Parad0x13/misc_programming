'''		8 Memo by Bryan A. Tremblay_05Jul14
	8 Memo is designed to test your memory by having you sort between
	up to 8 unsorted blocks by swapping two at a time.

	This was made as a memory aid tool for blindfolded cube solving techniques

	Keys:
	[A][S][D][F]  [J][K][L][;] used to select spots 1-8 respectively
	U used to unselect and clear all current selections
	H used to give a hint incase you messed up for some reason in blindfolded mode
	ESC used to quit the program
'''
import random
import time
import pygame
from pygame.locals import *
import sys

pygame.init()

gameWidth = 500
gameHeight = 250
screen = pygame.display.set_mode((gameWidth, gameHeight))
pygame.display.set_caption("8Memo - Game to memorize and sort positions quickly")

def getInput():
	while True:
		for event in pygame.event.get():
			if(event.type == KEYDOWN):
				if(event.key == K_ESCAPE):
					pygame.quit()
					sys.exit()
				return event.key
				
class Game(object):
	def __init__(self, size, blindfold):
		self.isBlindfolded = blindfold
		if(self.isBlindfolded):
			self.shouldProcessSelections = False
		else:
			self.shouldProcessSelections = True
		self.gameSize = size
		self.boxSize = (gameWidth * 0.8) / self.gameSize
		self.xBuffer = self.boxSize * 0.1
		if(self.gameSize > 4):
			self.additionalXBuffer = self.xBuffer*8;
		else:
			self.additionalXBuffer = 0;
		self.positionSurface = pygame.Surface((self.boxSize*self.gameSize + self.xBuffer*self.gameSize - self.xBuffer + self.additionalXBuffer, self.boxSize))
		self.backgroundColor = (150, 150, 150)
		self.initNewGame()
		self.selectA = 0
		self.selectB = 0
		self.moveCount = 0

	def initNewGame(self):
		self.positions = [i+1 for i in range(self.gameSize)]
		self.winPositions = [i+1 for i in range(self.gameSize)]
		random.shuffle(self.positions)
		''' Incase I want to calculate best game mechanics later on '''
		self.startPositions = self.positions

	def render(self):
		if((self.isBlindfolded and self.shouldProcessSelections == False) or self.isBlindfolded == False):
			screen.fill(self.backgroundColor)
			self.positionSurface.fill(self.backgroundColor)
			self.renderPositions()
		else:
			screen.fill((50, 50, 50))
			myfont = pygame.font.SysFont("monospace", (int)(60))
			label = myfont.render("BLINDFOLDED", 1, (150, 150, 150))
			screen.blit(label, (screen.get_width()/2 - label.get_width()/2, screen.get_height()/2 - label.get_height()/2))
		pygame.display.update()

	def renderPositions(self):
		for x in range(0, self.gameSize):
			boxX = x*self.boxSize + x*self.xBuffer
			if(x > (4-1)):
				boxX = boxX + self.additionalXBuffer
			boxY = 0
			pygame.draw.rect(self.positionSurface, (75, 75, 75), (boxX, boxY, self.boxSize, self.boxSize), 0)
			myfont = pygame.font.SysFont("monospace", (int)(self.boxSize/2))
			label = myfont.render(str(self.positions[x]), 1, (255, 255, 0))
			self.positionSurface.blit(label, (boxX + self.boxSize/2 - label.get_width()/2, boxY + self.boxSize/2 - label.get_height()/2))
			''' '''
			if(x == self.selectA-1 or x == self.selectB-1):
				arrowSize = 10
				pointX = boxX + self.boxSize/2
				pointY = boxY + self.boxSize
				pygame.draw.polygon(self.positionSurface, (0, 255, 0), [[pointX-arrowSize, pointY], [pointX, pointY-arrowSize], [pointX+arrowSize, pointY]], 0)
		screen.blit(self.positionSurface, ((gameWidth - self.positionSurface.get_width())/2, (gameHeight - self.positionSurface.get_height())/2))

	def selectPosition(self, position):
		if(position <= self.gameSize and position <= self.gameSize):
			if(position > 0):
				if(self.selectA > 0):
					self.selectB = position
					''' '''
					tempA = self.positions[self.selectA-1]
					self.positions[self.selectA-1] = self.positions[self.selectB-1]
					self.positions[self.selectB-1] = tempA
					self.moveCount = self.moveCount + 1
				else:
					self.selectA = position

	def printHint(self):
		print(self.positions)

	def clearSelections(self):
		self.selectA = 0
		self.selectB = 0

	def hasWon(self):
		if(self.positions == self.winPositions):
			return True
		return False

	def startTimer(self):
		self.startTime = time.time()

	def endTimer(self):
		self.endTime = time.time()
		self.gameDuration = (self.endTime - self.startTime)*1000.0

	def playGame(size, blindfold):
		game = Game(size, blindfold)
		game.startTimer()
		while(game.hasWon() == False):
			game.render()
			value = getInput()
			if(value == K_q):game.printHint()
			if(value == K_u):game.clearSelections()
			if(game.shouldProcessSelections == False):
				if(value == K_b):game.shouldProcessSelections = True
			if(game.shouldProcessSelections == True):
				if(value == K_a):game.selectPosition(1)
				if(value == K_s):game.selectPosition(2)
				if(value == K_d):game.selectPosition(3)
				if(value == K_f):game.selectPosition(4)
				if(value == K_j):game.selectPosition(5)
				if(value == K_k):game.selectPosition(6)
				if(value == K_l):game.selectPosition(7)
				if(value == K_SEMICOLON):game.selectPosition(8)
			if(game.selectA > 0 and game.selectB > 0):
				game.clearSelections()
		game.endTimer()
		game.isBlindfolded = False
		game.render()
		print("Won the game in %.2f seconds with %i moves" % ((game.gameDuration / 1000.0), game.moveCount))

class CommandMenu(object):
	def __init__(self):
		self.selectedMode = 0

	def render(self):
		screen.fill((255, 175, 175))
		myfont = pygame.font.SysFont("monospace", (int)(39))
		''' '''
		regular_label = myfont.render("(R): Regular Game", 1, (255, 255, 0))
		screen.blit(regular_label, (0, 0))
		''' '''
		blindfolded_label = myfont.render("(B): Blindfolded Game", 1, (255, 255, 0))
		screen.blit(blindfolded_label, (0, regular_label.get_height()))
		''' '''
		myfont_extra = pygame.font.SysFont("monospace", (int)(20))
		blindfoldedExtra_label = myfont_extra.render("(When ready press 'b' to begin)", 1, (255, 255, 0))
		screen.blit(blindfoldedExtra_label, (115, blindfolded_label.get_offset()[1] + blindfolded_label.get_height()*2))
		''' '''
		blindfoldedExtra2_label = myfont_extra.render("(Press '?' for a hint)", 1, (255, 255, 0))
		screen.blit(blindfoldedExtra2_label, (115, blindfolded_label.get_offset()[1] + blindfolded_label.get_height()*2 + blindfoldedExtra_label.get_height()))

		pygame.display.update()

	def run(self):
		while(self.selectedMode == 0):
			self.render()
			value = getInput()
			if(value == K_r):self.selectedMode = 1
			if(value == K_b):self.selectedMode = 2
		if(self.selectedMode == 1):Game.playGame(8, False)
		if(self.selectedMode == 2):Game.playGame(8, True)

menu = CommandMenu()
menu.run()
