import time
import random

import curses
scr = curses.initscr()
curses.curs_set(0)
curses.noecho()

curses.start_color()
curses.init_pair(1, 10, curses.COLOR_BLACK)    # Light Green
curses.init_pair(2, 2, curses.COLOR_BLACK)    # Green

class Node:
    def __init__(self, tail = 15):
        self.tail = tail

        h, w = scr.getmaxyx()
        self.x = random.randint(0, w - 1)
        self.y = 0

        self.text = ""
        for n in range(h): self.text += chr(random.randint(33, 126))

        self.should_die = False

    def tick(self):
        h, w = scr.getmaxyx()

        self.y += 1
        if self.y - self.tail == h:
            self.should_die = True

    def render(self):
        try: scr.addstr(self.y, self.x, self.text[self.y], curses.color_pair(1))
        except: pass

        for y in range(self.tail):
            try: scr.addstr(self.y - y - 1, self.x, self.text[self.y - y - 1], curses.color_pair(2))
            except: pass

# [TODO] Render this dynamically via 'blitting' instead of scr.addstr for every single character
# The way I'm doing it now is EXTREMELY slow and is noticibly bad at keeping up with higher FPS'
class Matrix:
    def __init__(self, node_count = 50, FPS = 30, tail = 15):
        self.node_count = node_count
        self.FPS = FPS
        self.tail = tail

    def run(self):
        nodes = []
        while True:
            h, w = scr.getmaxyx()

            if len(nodes) < self.node_count:
                nodes.append(Node(tail = self.tail))

            # I'm sure there's a significantly more elegant way of doing this with set booleans and stuff but w/e...
            toRemove = []
            for node in nodes:
                if node.should_die:
                    toRemove.append(node)
            for node in toRemove:
                nodes.remove(node)
                del node

            scr.clear()

            for node in nodes:
                node.render()
                node.tick()

            scr.refresh()

            time.sleep(1.0 / self.FPS)

matrix = Matrix()
matrix.run()
