'''	First topcoder problem that I worked on after a friend showed me the site '''

toTest = [999, 999, 999, 999]
toTest = [1, 1, 3, 1, 5]
toTest = [9, 7, 5, 3, 1]
toTest = [7, 47, 5, 113, 73, 179, 53]
toTest = [749, 999, 125, 1]
toTest = [1, 509, 253, 503, 125, 497, 247, 491, 61, 485, 241, 479, 119, 473, 235, 467, 29, 461, 229, 455, 113, 449, 223, 443, 55, 437, 217, 431, 107, 425, 211, 419, 13, 413, 205, 407, 101, 401, 199, 395, 49, 389, 193, 383, 95, 377]

answer = []

def tryForcingIntoDelta(input, delta):
	retVal = input[:]
	for x in range(1, len(retVal)):
		_delta = retVal[x] - retVal[x-1]
		if _delta > delta:
			return []
		if _delta != delta:
			retVal[x] = retVal[x]<<1
			return tryForcingIntoDelta(retVal, delta)
	return retVal

'''	There will always be 2 values in the list that are correct, we can call those 'anchors'
	that means if the set is to progress algorithmically those two anchors will give a true
	delta of the difference between their values divided by the difference of the index.
	Because we don't know which two values are anchors we have to assume they all are	'''
def listOfPossibleDeltas(input):
	retVal = []
	for a in range(0, len(input)-1):
		for b in range(a+1, len(input)):
			retVal.append(int((input[a] - input[b]) / abs(b-a)))
			retVal.append(int((input[b] - input[a]) / abs(b-a)))
	return sorted(list(set(retVal)))

def testWithMultiplierAndDeltas(input, multiplier, deltas):
	_input = input[:]
	_input[0] = _input[0]<<(multiplier-1)
	for delta in deltas:
		possibleList = tryForcingIntoDelta(_input, delta)
		if len(possibleList) > 0:
			return possibleList
	return []

'''	Multiplier cannot be larger than 1000
	I should be able to optimize the multiplier '''
for multiplier in range(1, 1000):
	possibleAnswer = testWithMultiplierAndDeltas(toTest, multiplier, listOfPossibleDeltas(toTest))
	if len(possibleAnswer) > 0:
		answer = possibleAnswer
		break

print("Found answer of ", answer)
