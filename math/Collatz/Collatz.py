# Bryan Tremblay 25Mar2015
# Graphs the escape point of numbers for the Collatz equation

from graphics import *

width = 1800
height = 300
win = GraphWin("My Animation", width, height, autoflush = False)

cache = {}

def nextInSequence(value):
	if(value % 2 == 0):
		return value/2
	else:
		return 3*value+1

def escapeOf(initialValue, currentValue, currentIteration):
	if(currentValue in cache):
		correctIterations = currentIteration + cache[currentValue] - 1
		cache[initialValue] = correctIterations
		return correctIterations

	next = nextInSequence(currentValue)
	if(next == 1):
		cache[initialValue] = currentIteration
		return currentIteration
	else:
		return escapeOf(initialValue, next, currentIteration+1)

total = width
lineDelta = width / total
for i in range(1, total):
	val = escapeOf(i, i, 1)
	l = Line(Point(lineDelta * i, height), Point(lineDelta * i, height-val))
	l.draw(win)

win.getMouse()
win.close()
