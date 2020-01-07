# Bryan Tremblay 25Mar2015
# Attempt at making a Fermi-Pasta-Ulam lattice system

from graphics import *

win = GraphWin("My Animation", 800, 300, autoflush = False)

class Ball(Circle):
	def __init__(self, pos, size):
		Circle.__init__(self, pos, size)
		self.left = None
		self.right = None
		self.static = False
		self.delta = Point(0, 0)

	def calculate(self):
		if(self.static):
			return
		k = 0.01
		self.delta = Point(k * (self.left.getCenter().x + self.right.getCenter().x - 2*self.getCenter().x), k * (self.left.getCenter().y + self.right.getCenter().y - 2*self.getCenter().y))

	def apply(self):
		self.move(self.delta.x, self.delta.y)

	def setLeft(self, value):
		self.left = value
		value.right = self

	def setRight(self, value):
		self.right = value
		value.left = self

leftWall = Ball(Point(50, 100), 5)
rightWall = Ball(Point(750, 100), 5)

a = Ball(Point(125, 100), 10)
b = Ball(Point(150, 175), 10)
c = Ball(Point(700, 100), 10)

leftWall.draw(win)
a.draw(win)
b.draw(win)
c.draw(win)
rightWall.draw(win)

leftWall.setRight(a)
a.setRight(b)
b.setRight(c)
c.setRight(rightWall)

while True:
	a.calculate()
	b.calculate()
	c.calculate()
	a.apply()
	b.apply()
	c.apply()
	time.sleep(0.05)
	win.update()
