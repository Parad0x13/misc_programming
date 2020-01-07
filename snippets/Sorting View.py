# Thought it might be a cool idea to draw a sorting algorithm live

from __future__ import print_function

from tkinter import *
from random import shuffle

n = 200

master = Tk()

w = Canvas(master, width = 500, height = 500, bd = 0, highlightthickness = 0)
w.configure(bg = "black")
w.pack()

weights = [i for i in range(n)]
shuffle(weights)

for i in range(n):
	w.create_line(i, 0, i, weights[i], fill = "red")

master.mainloop()
