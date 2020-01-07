"""
2017.11.14.1632.EST

----==== Hexapawn Example ====----
--== Game Generation ==--
GameState representation (3x3)
	All states, such as empty spaces and players, are indicated by a single character (ID)
	The A.I. is assigned it's own ID
	[A][A][A]
	[ ][ ][ ]
	[B][B][B]
	When serialized becomes a string:
	"AAA   BBB"
	Potential moves are generated as a list of integers
	E.g. for initial state: [0, 0, 0, 2, 2, 2, 0, 0, 0]
	Win conditions are simplified to [-1]
	Loss conditions are simplified to [-2]
	Draw conditions are simplified to [-3]
	Key value pair of state : coins is added to the master dictionary
	[WARN] States are limited to length of a single character, which there are only a limited number of

--== Gameplay Setup ==--
Empty gameplay list is created
activeState is set to initialState

--== Gameplay ==--
[NOTE] Assumes A.I. always goes 1st
[NOTE] To account for A.I. going 2nd you can just swap the user IDs
A.I. picks a coin
If no coins are available the A.I. conceeds and the game is considered a loss
Win condition is assessed
List of activeState and selected coin is pushed to gameplay list
New state is calculated as a new string and activeState is set to that state
Player picks a coin
Win condition is assessed
Repeat

--== Gameplay End ==--
Upon A.I. loss iterate through list of gameStates and apply penalty
Upon A.I. win iterate through list of gameStates and apply reward
"""

import sys
import random

winVal = -1
lossVal = -2
drawVal = -3

gameStates = {}
gameplay = []
activeState = None

# Going to use a trivial example of XXX where first X is win condition
initialState = "   "
gameStates["   "] = [2, 2, 2]
gameStates["A  "] = [winVal]
gameStates[" A "] = [2, 0, 2]
gameStates["  A"] = [2, 2, 0]
gameStates["BA "] = [lossVal]
gameStates[" AB"] = [2, 0, 0]
gameStates["AAB"] = [winVal]

# I'm sure there's a significantly better way of doing this...
def choseCoin(coins):
	if coins[0] < 0:
		return coins[0]

	# First we identify all coins that are possible and pool them
	pool = []
	for coinIndex in range(0, len(coins)):
		coin = coins[coinIndex]
		# [TODO] I can probably do away with this if coin > 0 thing can't I?
		if coin > 0:
			for count in range(0, coin):
				pool.append(coinIndex)

	if len(pool) == 0:
		print("SOMETHING HAS GONE SERIOUSLY WRONG!!!")

	return random.choice(pool)

def playGame():
	AI_ID = 'A'
	currentState = initialState
	while True:
		coins = gameStates.get(currentState)
		if coins == None:
			print("[ERROR] Game has generated a non-existing gameState")
			return drawVal
			#sys.exit()

		# Make a decision
		newState = currentState
		coin = choseCoin(coins)
		gameplay.append([newState, coin])

		# Update gameplay
		if coin >= 0:
			newState = newState[:coin] + AI_ID + newState[coin + 1:]
		currentState = newState

		if coin == winVal:
			return coin
		if coins == lossVal:
			return coin
		if coins == drawVal:
			return coin

result = playGame()
for play in gameplay:
	state = play[0]
	coin = play[1]
	coins = gameStates[state]
	if coins[0] >= 0:
		coins[coin] += 1
		gameStates[state] = coins

print(gameStates)
