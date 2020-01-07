#!/usr/bin/env python

import random

T = 0x1 << 0
B = 0x1 << 1
F = 0x1 << 2
R = 0x1 << 3

icons = ["  ", "T ", "B ", "!", "* ", "*T", "*B", "!", "+ ", "+T", "+B", "!"]

class Board:
    def __init__(self):
        self.squares = [0x0] * 14
        self.squares[3] |= F
        self.squares[4] |= R
        self.squares[7] |= F
        self.squares[13] |= F

        self.start_T = 7
        self.start_B = 7
        self.end_T = 0
        self.end_B = 0

    def render(self):
        print "+-------+-------+-------+-------+               +-------+-------+"
        print "|       |       |       |       |               |       |       |"
        print "|   {}  |   {}  |   {}  |   {}  |               |   {}  |   {}  |".format(
                icons[self.squares[3] & ~B], icons[self.squares[ 2] & ~B], icons[self.squares[ 1] & ~B],
                icons[self.squares[0] & ~B], icons[self.squares[13] & ~B], icons[self.squares[12] & ~B])
        print "|       |       |       |       |               |       |       |"
        print "+-------+-------+-------+-------+-------+-------+-------+-------+"
        print "|       |       |       |       |       |       |       |       |"
        print "|   {}  |   {}  |   {}  |   {}  |   {}  |   {}  |   {}  |   {}  |".format(
                icons[self.squares[4]], icons[self.squares[5]], icons[self.squares[6]], icons[self.squares[7]],
                icons[self.squares[8]], icons[self.squares[9]], icons[self.squares[10]], icons[self.squares[11]])
        print "|       |       |       |       |       |       |       |       |"
        print "+-------+-------+-------+-------+-------+-------+-------+-------+"
        print "|       |       |       |       |               |       |       |"
        print "|   {}  |   {}  |   {}  |   {}  |               |   {}  |   {}  |".format(
                icons[self.squares[3] & ~T], icons[self.squares[ 2] & ~T], icons[self.squares[ 1] & ~T],
                icons[self.squares[0] & ~T], icons[self.squares[13] & ~T], icons[self.squares[12] & ~T])
        print "|       |       |       |       |               |       |       |"
        print "+-------+-------+-------+-------+               +-------+-------+"

    def opponent(self, active):
        retVal = T
        if active == T: retVal = B
        return retVal

    def place(self, active, square):
        current = self.squares[square]
        inactive = self.opponent(active)

        if square <  4: current = current & ~inactive
        if square > 11: current = current & ~inactive

        # Check if active attacks
        # [TODO] Add cannot attack on star squares mechanic
        if current & inactive == True:
            self.squares[square] = self.squares[square] & ~inactive
            self.squares[square] |= active
        else:
            self.squares[square] |= active

        # [TODO] Add attacking your own pieces mechanic
        # [TODO] Add in reroll mechanic
        return True

    def getPiece(self, active):
        """ This will randomly grab some available piece of type active """
        retVal = None
        # First search for items on board
        for square in range(len(self.squares)):
            if self.squares[square] & active:
                return square

        # Then return one from start if exists
        return -1

    def play(self):
        active = T
        self.render()
        for d in range(2):
            roll = random.randint(0, 5)
            print "{} rolls a {}".format(icons[active].strip(), roll)
            if roll == 0: pass
            else: self.place(active, roll - 1)

            if   active == T: active = B
            elif active == B: active = T
            self.render()

board = Board()
board.play()
