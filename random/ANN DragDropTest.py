import tkinter as tk

DEFAULTS_WINDOW_WIDTH =  800
DEFAULTS_WINDOW_HEIGHT = 600
TAG_NEURON = "neuron"
TAG_NEURON_MASTER = "neuron_master"
TAG_CONNECTION = "connection"

class Connection:
	def __init__(self, canvas):
		self.tag = TAG_CONNECTION
		self.canvas = canvas
		self.width = 10
		self.height = 0

		self.handle = self.canvas.create_line(0, 0, self.width, 0)
		self.canvas.tag_bind(self.handle, "<ButtonPress-1>", self.onPressed)
		self.canvas.tag_bind(self.handle, "<ButtonRelease-1>", self.onReleased)

	def onPressed(self, event):
		print("pressed!")

	def onReleased(self, event):
		print("released")
		print(self.canvas.find_closest(event.x, event.y))

class Neuron:
	def __init__(self, canvas, coord):
		self.tag = TAG_NEURON
		self.canvas = canvas
		self.dragLoc = [0, 0]
		self.color = "orange"
		self.width = 50
		self.height = 50

		(x, y) = coord
		self.handle = self.canvas.create_rectangle(x, y, x + self.width, y + self.height, outline = self.color, fill = self.color, tags = self.tag)
		self.canvas.tag_bind(self.handle, "<ButtonPress-1>", self.onPressed)
		self.canvas.tag_bind(self.handle, "<ButtonRelease-1>", self.onReleased)
		self.canvas.tag_bind(self.handle, "<B1-Motion>", self.onMoved)

		self.inputs = []
		self.createInput()
		self.createInput()

		self.outputs = []
		self.createOutput()

	def coords(self):
		tmp = self.canvas.coords(self.handle)
		return (tmp[0], tmp[1])

	def onPressed(self, event):
		coords = self.canvas.coords(self.handle)
		self.dragLoc = [coords[0], coords[1]]

	def onReleased(self, event):
		pass

	def onMoved(self, event):
		delta_x = event.x - self.dragLoc[0]
		delta_y = event.y - self.dragLoc[1]
		self.dragLoc = [event.x, event.y]

		self.canvas.move(self.handle, delta_x, delta_y)

		for input in self.inputs:
			self.canvas.move(input.handle, delta_x, delta_y)

		for output in self.outputs:
			self.canvas.move(output.handle, delta_x, delta_y)

	def spawn(self, event):
		newCoords = list(self.coords())
		newCoords[0] += 100
		newCoords[1] += 0
		tmp = Neuron(self.canvas, tuple(newCoords))

	def setAsMaster(self):
		self.canvas.tag_unbind(self.handle, "<ButtonPress-1>")
		self.canvas.tag_unbind(self.handle, "<ButtonRelease-1>")
		self.canvas.tag_unbind(self.handle, "<B1-Motion>")

		self.canvas.tag_bind(self.handle, "<ButtonPress-1>", self.spawn)

	def orientChild(self, child):
		(x, y) = self.coords()
		childCoords = self.canvas.coords(child.handle)
		childWidth = childCoords[2] - childCoords[0]
		childHeight = childCoords[3] - childCoords[1]
		self.canvas.coords(child.handle, (x, y, x + childWidth, y + childHeight))

	def createInput(self):
		coords = self.coords()
		input = Connection(self.canvas)
		self.inputs.append(input)
		self.orderInputs()

	# [TODO] Make it so this spaces dynamically from the center instead of with static spacing from the top
	def orderInputs(self):
		spacing = 20
		for i, input in enumerate(self.inputs):
			self.orientChild(input)
			self.canvas.move(input.handle, -input.width, spacing * i)

	def createOutput(self):
		coords = self.coords()
		output = Connection(self.canvas)
		self.outputs.append(output)
		self.orderOutputs()

	# [TODO] Make it so this spaces dynamically from the center instead of with static spacing from the top
	def orderOutputs(self):
		spacing = 20
		for i, output in enumerate(self.outputs):
			self.orientChild(output)
			self.canvas.move(output.handle, self.width, spacing * i)

class SampleApp(tk.Tk):
	def __init__(self, *args, **kwargs):
		tk.Tk.__init__(self, *args, **kwargs)

		self.canvas = tk.Canvas(width = DEFAULTS_WINDOW_WIDTH, height = DEFAULTS_WINDOW_HEIGHT)
		self.canvas.pack(fill = "both", expand = True)

		# [TODO] Make this resize dynamically
		self.canvas.create_rectangle(0, 0, 150, DEFAULTS_WINDOW_HEIGHT, fill = "green")

		a = Neuron(self.canvas, (50, 50))
		a.setAsMaster()

if __name__ == "__main__":
	app = SampleApp()
	app.mainloop()
