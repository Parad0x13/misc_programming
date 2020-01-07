''' Bryan Tremblay 20Jun14'''
import pygame
from pygame.locals import *
import sys
from Board import *

pygame.init()

screen = pygame.display.set_mode((250, 250))


def getInput():
	while True:
		for event in pygame.event.get():
			if(event.type == KEYDOWN):
				if(event.key == K_ESCAPE):
					pygame.quit()
					sys.exit()
				if(event.key == K_UP):return "UP"
				if(event.key == K_DOWN):return "DOWN"
				if(event.key == K_LEFT):return "LEFT"
				if(event.key == K_RIGHT):return "RIGHT"

board = Board(250, 4)
board.tick()
while(board.hasLost == False):
	board.render(screen)
	pygame.display.update()

	'''value = getInput()
	if(value == "UP" or value == "DOWN" or value == "LEFT" or value == "RIGHT"):
		if(board.swipe(value) == True):
			board.tick()'''
	board.makeBestMove()
	'''getInput()'''

board.render(screen)
pygame.display.update()
print("lost")

highest = board.highestValue()
print("Highest Value: ", highest)
