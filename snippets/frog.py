# 2019.09.15.1310.EST
# From video: https://www.youtube.com/watch?v=ZLTyX4zL2Fc
# I got 2.9290026 with N = 10e7, sounds like 2.92 repeating?
# Yea that sounds wrong honestly. Markov chains maybe?

import random

def play(n):
    hops = 0
    location = 0
    while True:
        hop = random.randint(1, n - location)
        location += hop
        hops += 1
        if location == n:
            return hops

N = 0
result = 0
while True:
    maximum = 10000
    temp = 0
    for n in range(maximum): temp += play(10)
    N += maximum
    temp /= maximum
    result += temp
    result /= 2
    print("{}: N = {}".format(result, N))
