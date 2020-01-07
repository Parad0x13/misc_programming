'''
[TODO] Needs to calculate distance for health of current child
'''

import random

dir_U = "U"
dir_D = "D"
dir_L = "L"
dir_R = "R"

DNA_length = 10

'''
(E)mpty
(W)all
(S)nake
(G)oal
world[y][x]
'''
world = [
	["W", "W", "W", "W", "W"],
	["W", "W", "W", "W", "W"],
	["E", "E", "E", "E", "G"],
	["W", "W", "W", "W", "W"],
	["W", "W", "W", "W", "W"],
]
head = [0, 2]

def logWorld():
	for d in range(len(world)):
		print(world[d])
	print("")

def getLocation(loc):
	return world[loc[1]][loc[0]]

def setLocation(loc, state):
	world[loc[1]][loc[0]] = state

def tryMove(direction):
	global head
	newLocation = head[:]

	if direction == dir_U:
		newLocation[1] -= 1
	if direction == dir_D:
		newLocation[1] += 1
	if direction == dir_L:
		newLocation[0] -= 1
	if direction == dir_R:
		newLocation[0] += 1

	collide = getLocation(newLocation)

	if collide == "E" or collide == "G":
		setLocation(head, "S")
		head = newLocation
		return True
	return False

def generateDNA():
	retVal = ""
	directions = [dir_U, dir_D, dir_L, dir_R]
	for d in range(DNA_length):
		retVal += random.choice(directions)
	return retVal

'''
def iterate():
	continue
'''

DNA = generateDNA()
print(DNA)
instruction = 0

logWorld()

for instruction in range(len(DNA)):
	moved = 1
	if tryMove(DNA[instruction]):
		pass
	else:
		moved = 0

	if getLocation(head) == "G":
		print("You've won!")
		break

	if moved:
		pass
	else:
		print("Collided with something...")
		break

logWorld()
