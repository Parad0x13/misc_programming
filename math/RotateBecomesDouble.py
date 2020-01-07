# 2017.07.23.2106.EST
# Smallest positive number that doubles when it's last digit becomes it's first
# I saw this video from MindYourDecisions on YouTube and thought I'd try it
# I wrote this small script to see if I could brute force it out
# 2017.07.23.2113.EST Couldn't find it
# Decided instead to let this run while I go watch Game of Thrones...
# 2017.07.24.1729.EST It's far into the next day and still no solution
# I've decided to stop the script and check the video for a solution
# https://www.youtube.com/watch?v=1lHDCAIsyb8
# Okay no wonder I couldn't find it, the number is:
#	105,263,157,894,736,842 (18 digits)
# The way he got this solution was interesting
# and I don't fully understand it yet
# I might, in the future, write this script to find additional solutions

for x in range(0, 1000000000000000000):
	orig = x
	string = str(x)
	string = string[-1] + string
	string = string[:-1]
	result = int(string)
	if(orig * 2 == result):
		print("Got:", result)
