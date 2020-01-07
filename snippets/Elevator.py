# 2017.08.14.2035.EST
# It's somewhat counterintuitive but how elevator systems work can get quite complex.
# Different buildings might operate best with different elevator algorithms.
# I want to explore this and maybe generate a few cool stats along the way.

# [ASSUMPTION] Ground level is floor 0
# [ASSUMPTION] All floors are equally staffed
# [ASSUMPTION] No taboo floors
# [ASSUMPTION] No peak hours

import timeit

currentFloor = 0
floorCount = 10
elevatorFloorToFloorDuration = 10.0
elevatorWaitDuration = 10.0

# Elevator reports to called floor
# Elevator brings passenger(s) to desired floor
# Elevator waits
def standard():
	return 1.0

print(str(standard()))
