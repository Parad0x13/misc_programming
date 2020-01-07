"""
2018.01.29.1647.EST
Saw an interesting video on superpermutations and I wanted to try
and write my own solver for it. Who knows? Maybe I'll discover something!
https://www.youtube.com/watch?v=wJGE4aEWc28

Doesn't work great yet but maybe in the future I'll improve upon it!
"""

# [TODO] Include a minimal deBruijn sequence generator too

from itertools import permutations
from itertools import product
import numpy as np

def generateSuperset(input):
    assert isinstance(input, list)

    superset = ""
    perms = permutations(range(1, n + 1))
    for perm in perms:
        formattedPerm = "".join(str(x) for x in perm)
        # [TODO] Add checks to see if we can partially append instead
        if formattedPerm not in superset:
            superset += formattedPerm
    return superset

n = 3

allPerms = permutations(range(1, n + 1))
for perm in allPerms:
    perm = list(perm)
    superset = generateSuperset(perm)
    print(superset + " len(" + str(len(superset)) + ")")
