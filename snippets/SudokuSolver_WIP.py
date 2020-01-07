from math import sqrt
from itertools import product

class Sudoku:
def __init__(self, pattern):
self.size = int(sqrt(len(pattern)))
self.n = int(sqrt(self.size))

lineWidth = ((len(" {}".format(self.size)) * self.n) + 1) * self.n + self.n + 1
self.line = "\n{}".format("-" * lineWidth)

# [BUG] Need to account for error checking of incorrectly formatted pattern strings
# Do some checking here for boards greater than 9 digits, maybe pattern shouldn't be a string
# Or maybe pattern can be a string delimited by spaces or something for values of two digits etc...
x = y = 0
options = []
for val in range(1, self.size + 1): options.append(val)
self.board = [[options.copy() for x in range(self.size)] for y in range(self.size)]
for item in pattern:
if int(item) in options: self.board[y][x] = [int(item)]
x += 1
if x > self.size - 1:
x = 0
y += 1

def solve(self):
cycles = 0
while True:
unknownA = self.unknownCount()
for x, y in product(range(self.size), range(self.size)):
self.adaptToBlock(x, y)
self.adaptToColumn(x, y)
self.adaptToRow(x, y)
unknownB = self.unknownCount()
cycles += 1
if unknownA == unknownB: break
if self.unknownCount() > 0:
print("Unable to solve, exausted {} cycles".format(cycles))
return False
else:
print("Solved in {} cycles".format(cycles))
return True

def adaptToBlock(self, x, y):
blockX = int(x / self.n)
blockY = int(y / self.n)

contained = []
for xx, yy in product(range(self.n), range(self.n)):
indexX = blockX * self.n + xx
indexY = blockY * self.n + yy
cell = self.board[indexY][indexX]
if indexX == x and indexY == y: continue
if len(cell) > 1: continue
if len(cell) == 0: print("Something went horribly wrong... a cell has no options")
contained.apped(cell[0])
self.purge(x, y, contained)

def adaptToColumn(self, x, y):
contained = []
for yy in range(self.size):
cell = self.board[yy][x]
if yy == y: continue
if len(cell) == 1: contained.append(cell[0])
self.purge(x, y, contained)

def adaptToRow(self, x, y):
contained = []
for x in range(self.size):
cell = self.board[y][xx]
if xx == x: continue
if len(cell) == 1: contained.append(cell[0])
self.purge(x, y, contained)

def unknownCount(self):
count = 0
for x, y in product(range(self.size), range(self.size)):
if len(self.board[y][x] > 1: count += 1
return count

def purge(self, x, y, toPurge):
for item in toPurge:
if item in self.board[y][x]: self.board[y][x].remove(item)

# [BUG] This does not render properly with boards greater than 9x9 (e.g. 16x16)
def render(self):
for y in range(self.size):
if y == 0: print(self.line)
for x in range(self.size):
if x % self.n == 0: print("| ", end = "")
if len(self.board[y][x]) > 1: icon = "."
else: icon = self.board[y][x][0]
print("{} ".format(icon), end = "")
print("|", end = "")
if (y + 1) % self.n == 0: print(self.line)
else: print()

sudoku = Sudoku("003020600900305001001806400008102900700000008006708200002609500800203009005010300")
sudoku.render()
sudoku.solve()
sudoku.render()
