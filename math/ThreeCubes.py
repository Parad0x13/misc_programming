# https://www.youtube.com/watch?v=zyG8Vlw5aAw
# More at https://www.youtube.com/watch?v=wymmCdLdPvM&list=PLt5AfwLFPxWJcqG5YM89Qes5gZdAFM4Q1

# 3 = 1, 1, 1 || 4, 4, -5

def findIntegers(k, boundry):
    print("Calculating for {} with boundry of {}".format(k, boundry))
    for x in range(boundry + 1):
        for y in range(boundry + 1):
            for z in range(boundry + 1):
                xPrime = x**3
                yPrime = y**3
                zPrime = z**3
                # We can test for these possible solutions:
                a = xPrime + yPrime + zPrime
                b = xPrime + yPrime - zPrime
                c = xPrime - yPrime + zPrime
                d = xPrime - yPrime - zPrime
                e = -xPrime + yPrime + zPrime
                f = -xPrime + yPrime - zPrime
                g = -xPrime - yPrime + zPrime
                h = -xPrime - yPrime - zPrime
                solutions = [a, b, c, d, e, f, g, h]
                if k in solutions:
                    #if current in notAllowed: continue
                    print("Solution for {} found, x = {}, y = {}, z = {}".format(k, x, y, z))
                    return

for k in range(1, 1000):
    able = True
    # Reason for this is explained here: https://youtu.be/wymmCdLdPvM?list=PLt5AfwLFPxWJcqG5YM89Qes5gZdAFM4Q1&t=212
    for K in range(99, 1000):
        if 9 * K + 4 == k or 9 * K + 5 == k:
        #if K % 9 == 4 or K % 9 == 5:    # Why this no worky!!! https://youtu.be/ASoz_NuIvP0?list=PLt5AfwLFPxWJcqG5YM89Qes5gZdAFM4Q1&t=36
            print("Cannot process {} as it is impossible".format(k))
            able = False
    if able: findIntegers(k, 1000)