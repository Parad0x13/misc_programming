# 2017.07.24.1751.EST
# I saw an interesting sequence called the Kolakoski sequence
# on a numberphile video on Youtube: https://www.youtube.com/watch?v=co5sOgZ3XcM
# I wanted to replicated it and see if I could generate sequences
# for sets other than {1, 2} which was used in the video
# The Kolakoski sequence is self defining, for example:
# 1, 2, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 1, 2, 1, 1, 2
# When counting the 'runs' of each value we see it defines itself
# Run of 1, run of 2, run of 2, run of 1, run of 1, run of 2 etc...

# This algorithm I did not come up with myself
# I took it from the wiki: https://en.wikipedia.org/wiki/Kolakoski_sequence
# 2017.07.24.1820.EST Wasn't able to get it to work, got bored. Continue later.
def generate(numbers):
	used = [numbers[0]]
	sequence = [numbers[0]]
	nextIndex = 0

	for x in range(10):
		lastUsed = sequence[-1]
		nextIndex += 1
		if nextIndex > len(numbers) - 1:
			nextIndex = 0
		value = numbers[nextIndex]
		if value not in used:
			for d in range(value):
				sequence.append(value)
			used.append(value)
		else:
			for d in range(lastUsed):
				sequence.append(value)
	print(sequence)

generate([1, 2])
