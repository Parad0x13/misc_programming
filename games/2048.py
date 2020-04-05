class Board():
    def __init__(self):
        self.data = []
        for n in range(3): self.data.append([0, 0, 0])
        self.data[2][0] = 2
        self.data[2][2] = 2

        self.data[1][0] = 2
        self.data[1][2] = 2

    def render(self):
        for row in self.data:
            print(row)

    def play(self):
        direction = input("direction?")
        if direction == "w":
            """
            val = 0
            val += self.data[0][0]
            val += self.data[1][0]
            val += self.data[2][0]
            self.data[0][0] = val
            self.data[1][0] = 0
            self.data[2][0] = 0

            val = 0
            val += self.data[0][1]
            val += self.data[1][1]
            val += self.data[2][1]
            self.data[0][1] = val
            self.data[1][1] = 0
            self.data[2][1] = 0

            val = 0
            val += self.data[0][2]
            val += self.data[1][2]
            val += self.data[2][2]
            self.data[0][2] = val
            self.data[1][2] = 0
            self.data[2][2] = 0
            """
            w = len(self.data[0])
            h = len(self.data)
            # Where I left off here!!!!

        else: self.play()

board = Board()
board.render()
board.play()
board.render()
