import random

corners_f = ["a", "b", "c", "d"]
corners_b = ["A", "B", "C", "D"]
direction = ["", "'"]

previous = ""

def generateMove():
	global previous
	face = corners_f if random.randint(0, 1) else corners_b
	c = random.choice(face)
	retVal = c
	if previous == retVal:
		return generateMove()
	previous = retVal
	retVal += random.choice(direction)
	return retVal

def generateAlgorithm():
	retVal = ""
	for x in range(15):
		retVal += generateMove()
		retVal += " "
	return retVal

print(generateAlgorithm())
