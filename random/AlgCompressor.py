import itertools

movements = ["U", "D", "L", "R", "F", "B"]
orig = "R D2 L F2 R' D2 U2 B2 L' B' R2 F' L' B' D F' D2 B2"
alg = orig.split()

doubleSpins = []
primes = []

tmp = ""
for item in alg:
    if "2" in item:
        doubleSpins.append("1")
        item = item.replace("2", "")
    else:
        doubleSpins.append("0")

    if "'" in item:
        primes.append("1")
        item = item.replace("'", "")
    else:
        primes.append("0")

    tmp += item
alg = tmp

doubleSpins = "".join(doubleSpins)
primes = "".join(primes)

items = list(set(itertools.permutations(movements, r = 3)))
items.sort()
for movement in movements: items.append(movement)   # We want to ensure non-found patterns don't run to infinity
for d in range(len(items)): items[d] = "".join(items[d])
print("Generated {} distinct values".format(len(items)))    # Length of items must be below 256 to properly index in byte size

compressed = alg
while True:
#for d in range(2):
    shouldContinue = False
    for movement in movements:
        if movement in compressed:
            shouldContinue = True
            break

    if shouldContinue == False: break

    for item in items:
        if item in compressed:
            #print("Replacing {} with index {}".format(item, items.index(item)))
            index = items.index(item)
            compressed = compressed.replace(item, " {} ".format(index))
            break

compressed = compressed.split()
final = bytearray()
for value in compressed: final.append(int(value))
print("Result of length {}: {}".format(len(final), final))

#doubleSpins_bytes = bytearray()

totalBits = len(doubleSpins) + len(primes)
byteCount = totalBits // 8
if totalBits > byteCount * 8: byteCount += 1

#a = 1 << byteCount * 8 - 1
#print(a)

origSize = len(orig)
finalSize = len(final) + byteCount
ratio = origSize / finalSize
print("Reduced from length {} to length {}\t{}%".format(origSize, finalSize, ratio))
