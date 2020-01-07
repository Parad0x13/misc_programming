from __future__ import print_function

def generateOccurrences(bytes):
	retVal = {}
	for byte in data:
		if byte in retVal:
			retVal[byte] += 1
		else:
			retVal[byte] = 1
	return retVal

def generateSorted(dictionary):
	return sorted(occurrences.items(), key = lambda x: x[1])

# [TODO] Ensure this works on all input, 0 bytes to arbitrary size
def generateTree(occurrences):
	retVal = occurrences

	if len(retVal) > 1:
		arranged = generateSorted(retVal)

		a = arranged[0]
		b = arranged[1]
		sum = a[1] + b[1]
		retVal.pop(a[0])
		retVal.pop(b[0])
		node = (a, b)

		retVal[node] = sum

		return generateTree(retVal)

	return retVal

f = open("test.txt", "rb")
data = f.read()		# [TODO] Don't do this in one read cycle
occurrences = generateOccurrences(data)
tree = generateTree(occurrences)
print(tree)
