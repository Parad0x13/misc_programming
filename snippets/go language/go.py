import random

f = open("language.txt", "r").read().split("\n")
print(random.choice(f))

w = open("words.txt", "r").read().split("\n")
print(random.choice(w))