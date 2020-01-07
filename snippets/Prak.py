#!/usr/bin/env python3

import sys
import time
import random
import calendar

def add(low, high, n):
    string = ""; total = 0
    for x in range(n):
        val = random.randint(low, high)
        total += val
        string = "{}{} ".format(string, val)
    string = string.strip()
    return [string, total]

class Game:
    def __init__(self, turns = 10):
        self.mode = "A"
        self.save = "Prak.sav"
        self.turns = turns
        if len(sys.argv) == 2:
            self.save = argv[1]

    def prompt(self):
        print("[a]dd\nA[n]alyze\n[E]xit")
        choice = input()
        if choice == "a": self.play()
        elif choice == "n": self.analyze()
        elif choice == "e": exit()
        else: prompt()

    def play(self):
        for turn in range(self.turns):
            question = add(0, 9, 6)
            print(question[0])
            beg = time.time()
            answer = input()
            end = time.time()
            delta = float("%.2f" % (end - beg))
            correct = False
            if int(answer) == question[1]: correct =True
            if correct:
                print("Correct!\n")
            else:
                print("Incorrect\n")
            self.log(question[0], correct, delta)

    def log(self, questionString, wasCorrect, delta):
        epoch = calendar.timegm(time.gmtime())
        correctIcon = "C"
        if wasCorrect == False:
            correctIcon = "I"
        string = "{},{},{},{},{}".format(self.mode, epoch, correctIcon, delta, questionString)
        with open(self.save, "a") as fh:
            fh.write(string + "\n")

game = Game(turns = 3)
game.play()