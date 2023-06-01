# 2023.06.01.0350.EST
"""
An interesting idea about how much scrap you could make using what I think is called the Marygold gambling strategy?
At least in rust
From what I see here it is NOT worth the effort for the time involved
"""

import random

selection = 1

# Rust's default wheel configuration
wheel = [20, 1, 3, 1, 5, 1, 3, 1, 10, 1, 3, 1, 5, 1, 5, 3, 1, 10, 1, 3, 1, 5, 1, 3, 1]

# Average time between roulette wheel games in seconds
timePerGame = 57

class Game:
    def __init__(self, selection = 1, funds = 1000, bet = 10, iterations = 1):
        self.selection = selection
        self.funds = funds
        self.initialFunds = self.funds
        self.bet = bet
        self.iterations = iterations

    def play(self):
        pool = self.bet
        for n in range(self.iterations):
            print("Playing iteration {} with {} funds and a pool of {}".format(n, self.funds, pool))

            result = random.choice(wheel)
            if result == self.selection:
                self.funds += pool
                pool = self.bet
                print("You win with a pool of {} resulting in {} funds".format(pool, self.funds))
            else:
                self.funds -= pool
                pool *= 2
                print("You lose and the pool has doubled to {} resulting in {} funds".format(pool, self.funds))

            if self.funds <= 0:
                print("You have lost all your funds and are now poor")
                return

            print()
        print("It would have taken about {} minutes to win/lose {} scrap".format((timePerGame*self.iterations)/60, self.funds - self.initialFunds))

game = Game(iterations = 500, funds = 5000, bet = 5)
game.play()
