''' Bryan Tremblay 06Jul14 '''
''' Designed to help with Blind Cubing memorization '''
import time, random

''' Simply prints letter scheme for Speffz: http://www.speedsolving.com/wiki/index.php/Speffz '''
def s():print("  ", end='')
def t():print("+--+--+--+", end='')
def h():t();s();t();s();t();s();t();s();t();s();t();s();print()
def r(v):print("|%c |%c |%c |" % (v[0], v[1], v[2]), end='');s()
def l(v):r(v[:3]);r(v[3:6]);r(v[6:9]);r(v[9:12]);r(v[12:15]);r(v[15:18])
def w(v):h();l(v);print()
print("\nUp          Left        Front       Right       Back        Down")
w("AABEEFIIJMMNQQRUUV");w("D BH FL JP NT RX V");w("DCCHGGLKKPOOTSSXWW");h()

set = list(map(chr, range(65, 89)))
count = 1
print("\nYou are timed to locate all 24 cubie pairs in the Speffz layout\nPress 'Enter' to begin")
input()
start = time.time()

while len(set) > 0:
	current = random.choice(set)
	set.remove(current)
	print("%i: %s" % (count, current), end='')
	count += 1
	input()

end = time.time()
print("It took you %.2f seconds to identify all cubie pairs" % (end-start))
