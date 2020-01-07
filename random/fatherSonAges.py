# 2019.09.17.1855.EST Started
# https://www.youtube.com/watch?v=10t24FC0CE8

# 2019.09.17.1858.EST Found (32, 23) and (41, 14)
# (32, 23) makes little sense as the father would have been 9 years old
# So the solution shoudl be (41, 14)
# Too lazy to solve this properly...
# Although the solution was kinda interesting

n = 55
for f in range(1, n + 1):
    for s in range(1, (n + 1) - f):
        if f > s and f + s == n and str(f)[::-1] == str(s):
            print("{}, {}".format(f, s))
