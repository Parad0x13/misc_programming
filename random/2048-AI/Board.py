''' Bryan Tremblay 20Jun14'''
import pygame
import copy
from random import randint

class Board(object):

	def __init__(self, size, segments):
		self.segments = segments
		self.blocks = [[0]*self.segments for x in range(self.segments)]
		self.hasMerged = [[0]*self.segments for x in range(self.segments)]
		self.surface = pygame.Surface((size, size))
		self.blockSize = size/self.segments
		self.hasLost = 0

	def render(self, screen):
		self.surface.fill((50, 50, 50))
		for y in range(0, self.segments):
			for x in range(0, self.segments):
				self.renderBlock(x, y)
		screen.blit(self.surface, (0, 0))

	def renderText(self):
		for y in range(0, self.segments):
			for x in range(0, self.segments):
				print(self.blocks[x][y], end="")
			print("")
		print("")

	def renderBlock(self, x, y):
		bufferSize = self.blockSize * 0.1
		pygame.draw.rect(self.surface, (75, 75, 75), (self.blockSize*x + bufferSize/2, self.blockSize*y + bufferSize/2, self.blockSize - bufferSize, self.blockSize - bufferSize), 0)
		if(self.blocks[x][y] > 0):
			myfont = pygame.font.SysFont("monospace", (int)(self.blockSize/2))
			label = myfont.render(str(self.blocks[x][y]), 1, (255, 255, 0))
			self.surface.blit(label, (self.blockSize*x + self.blockSize/2 - label.get_width()/2, self.blockSize*y + self.blockSize/2 - label.get_height()/2))

	def emptyCount(self):
		retVal = 0
		for y in range(0, self.segments):
			for x in range(0, self.segments):
				if(self.blocks[x][y] == 0):
					retVal = retVal + 1
		return retVal

	def makeBestMove(self):
		up = self.copySwipe("UP")
		down = self.copySwipe("DOWN")
		left = self.copySwipe("LEFT")
		right = self.copySwipe("RIGHT")

		selected = None

		if(up.blocks != self.blocks):
			selected = up
		if(down.blocks != self.blocks):
			if(selected == None):
				selected = down
			elif(down.score() > selected.score()):
				selected = down
		if(left.blocks != self.blocks):
			if(selected == None):
				selected = left
			elif(left.score() > selected.score()):
				selected = left
		if(right.blocks != self.blocks):
			if(selected == None):
				selected = right
			elif(right.score() > selected.score()):
				selected = right

		if(selected == up):self.swipe("UP")
		if(selected == down):self.swipe("DOWN")
		if(selected == left):self.swipe("LEFT")
		if(selected == right):self.swipe("RIGHT")
		self.tick()

	def makeRightDownUpLeftMove(self):
		''' Prioritizes Right over Down over Up of Left '''
		if(self.trySwipe("RIGHT") == True):
			self.swipe("RIGHT")
		elif(self.trySwipe("DOWN") == True):
			self.swipe("DOWN")
		elif(self.trySwipe("UP") == True):
			self.swipe("UP")
		else:
			self.swipe("LEFT")
		self.tick()

	def makeRandomMove(self):
		value = ["UP", "DOWN", "LEFT", "RIGHT"][randint(0, 3)]
		if(self.swipe(value) == True):
			self.tick()

	'''	Things I want to prioritize:
		Prioritize higher values?
		The largest value is in a corner?
		The right column is at least filled?
	'''
	def score(self):
		retVal = 0
		highestValue = self.highestValue()
		retVal = retVal + highestValue
		cornerIndex = self.segments-1
		if(self.blocks[cornerIndex][0] > 0):retVal = retVal + 1
		if(self.blocks[cornerIndex][1] > 0):retVal = retVal + 2
		if(self.blocks[cornerIndex][2] > 0):retVal = retVal + 3
		if(self.blocks[cornerIndex][3] > 0):retVal = retVal + 4
		if(self.blocks[cornerIndex][cornerIndex] == highestValue):retVal = retVal + 50
		return retVal

	def tick(self):
		emptyCount = self.emptyCount()
		if(emptyCount == 0):
			self.hasLost = True
			return
		selected = randint(1, emptyCount) - 1
		fillValueType = randint(0, 1)
		if(fillValueType == 0):
			fillValue = 2
		elif(fillValueType == 1):
			fillValue = 4
		passedEmpties = 0
		for y in range(0, self.segments):
			for x in range(0, self.segments):
				if(self.blocks[x][y] == 0):
					if(passedEmpties == selected):
						self.blocks[x][y] = fillValue
					passedEmpties = passedEmpties + 1
		'''Check if game now lost since block has been placed'''
		if(self.emptyCount() == 0):
			if(self.trySwipe("UP") == False and
			self.trySwipe("DOWN") == False and
			self.trySwipe("LEFT") == False and
			self.trySwipe("RIGHT") == False):
				self.hasLost = True

	def trySwipe(self, direction):
		testBoard = copy.deepcopy(self)
		return(testBoard.swipe(direction))

	def copySwipe(self, direction):
		retVal = copy.deepcopy(self)
		retVal.swipe(direction)
		return retVal

	def swipe(self, direction):
		originalBlocks = [row[:] for row in self.blocks]
		self.hasMerged = [[0]*self.segments for x in range(self.segments)]
		if(direction == "UP"):
			for x in range(0, self.segments):
				for tries in range(0, self.segments-1):
					for y in range(0, self.segments):
						if(y-1 < 0):
							continue
						self.push(x, y, x, y-1)
		if(direction == "DOWN"):
			for x in range(0, self.segments):
				for tries in range(0, self.segments-1):
					for y in reversed(range(0, self.segments)):
						if(y+1 > self.segments-1):
							continue
						self.push(x, y, x, y+1)
		if(direction == "LEFT"):
			for y in range(0, self.segments):
				for tries in range(0, self.segments-1):
					for x in range(0, self.segments):
						if(x-1 < 0):
							continue
						self.push(x, y, x-1, y)
		if(direction == "RIGHT"):
			for y in range(0, self.segments):
				for tries in range(0, self.segments-1):
					for x in reversed(range(0, self.segments)):
						if(x+1 > self.segments-1):
							continue
						self.push(x, y, x+1, y)
		''' Check if player tried to swipe in a blocked direction'''
		if(self.blocks == originalBlocks):
			return False
		return True

	def push(self, fromX, fromY, toX, toY):
		if(self.blocks[fromX][fromY] > 0):
			if(self.blocks[fromX][fromY] == self.blocks[toX][toY] and (self.hasMerged[toX][toY] == 0 and self.hasMerged[fromX][fromY] == 0)):
				self.hasMerged[toX][toY] = 1
				self.blocks[toX][toY] = self.blocks[toX][toY] + self.blocks[fromX][fromY]
				self.blocks[fromX][fromY] = 0
			if(self.blocks[toX][toY] == 0):
				self.blocks[toX][toY] = self.blocks[fromX][fromY]
				self.blocks[fromX][fromY] = 0

	def highestValue(self):
		retVal = 0
		for y in range(0, self.segments):
			for x in range(0, self.segments):
				if(self.blocks[x][y] > retVal):
					retVal = self.blocks[x][y]
		return retVal
