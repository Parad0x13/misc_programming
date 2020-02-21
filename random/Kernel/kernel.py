from PIL import Image   # http://effbot.org/imagingbook/image.htm
import cProfile

"""
image = Image.open("example.jpg")
pixels = image.getdata()

test = Image.new("RGB", (10, 10))
data = test.load()
data[0, 0] = (255, 255, 255)
width, height = test.size
for y in range(height):
    for x in range(width):
        print(data[x, y])
test.save("fff.bmp", "bmp")
"""

def isWithinBounds(lower, upper, x, y):
    if x < lower[0] or y < lower[0]:
        return False
    if x > upper[0] - 1 or y > upper[1] - 1:
        return False
    return True

class Kernel:
    def __init__(self, G = [
        [1, 1, 1],
        [1, 1, 1],
        [1, 1, 1]], image = None):
        # [TODO] Figure out how to handle non-odd axis', or forbid them
        self.G = G
        self.G_width = len(self.G[0])
        self.G_height = len(self.G)

        # [TODO] Find a more elegant solution to this...
        self.G_offset = []
        pivot_x = int(self.G_width / 2)
        pivot_y = int(self.G_height / 2)
        for y in range(-pivot_y, pivot_y + 1):
            row = []
            for x in range(-pivot_x, pivot_x + 1):
                row.append((x, y))
            self.G_offset.append(row)

        if image != None:
            self.associate(image)

    def apply(self):
        # [TODO] Add error checking if image is not defined
        tmp = Image.new("RGB", (self.image.width, self.image.height))
        tmp_data = tmp.load()

        for y in range(self.image.height):
            for x in range(self.image.width):
                weight = self.appliedValue(x, y)
                tmp_data[x, y] = (weight[0], weight[1], weight[2])

        #tmp.save("result.bmp", "bmp")
        tmp.save(str(self.G[0][0]) + ".bmp", "bmp")

    def appliedValue(self, x, y):
        r_weight = 0
        g_weight = 0
        b_weight = 0
        weight_count = 0

        for kernel_y in range(self.G_height):
            for kernel_x in range(self.G_width):
                offset = self.G_offset[kernel_y][kernel_x]
                x_new = x + offset[0]
                y_new = y + offset[1]
                if isWithinBounds((0, 0), self.image.size, x_new, y_new):
                    r_weight += self.imageData[x_new, y_new][0] * self.G[kernel_y][kernel_x]
                    g_weight += self.imageData[x_new, y_new][1] * self.G[kernel_y][kernel_x]
                    b_weight += self.imageData[x_new, y_new][2] * self.G[kernel_y][kernel_x]
                    weight_count += 1

                color = self.imageData[x, y]

        r_weight = int(r_weight / weight_count)
        g_weight = int(g_weight / weight_count)
        b_weight = int(b_weight / weight_count)
        return (r_weight, g_weight, b_weight)

    def associate(self, image):
        # [TODO] Add error checking if image name doesn't exist
        self.image = Image.open(image)
        self.imageData = self.image.load()

    def setpixel(self, x, y, r, g, b):
        # [TODO] Add error checking if image is not defined
        self.imageData[x, y] = (r, g, b)

    def save(self, name = "image.bmp"):
        # [TODO] Add error checking if image is not defined
        self.image.save(name, "bmp")

    def render(self):
        for g in self.G:
            print(g)

    def renderImage(self):
        # [TODO] Add error checking if image is not defined
        self.image.show()

kernel = Kernel(image = "example2.bmp")
cProfile.run("kernel.apply()")
