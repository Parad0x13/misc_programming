# https://www.youtube.com/watch?v=RFlTawWwLZc

import random

def rollSix():
    a = random.randint(1, 6)
    if a == 6: return True
    return False

def sixDie():
    count = 0
    for n in range(6):
        if rollSix(): count += 1
    return count

for n in range(1000):
    print(sixDie())
