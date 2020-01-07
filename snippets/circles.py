import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons

fig, ax = plt.subplots()
plt.subplots_adjust(left = 0.25, bottom = 0.25)
t1_0 = 0
t2_0 = 0
xs = []
ys = []
theta1_delta = 0.1
theta2_delta = 0.2
for i in range(1000):
    t1 = theta1_delta * i
    t2 = theta2_delta * i
    xs.append(math.cos(t1))
    ys.append(math.sin(t2))
l, = plt.plot(xs, ys, lw = 1, color = "red")

axcolor = "lightgoldenrodyellow"
axfreq = plt.axes([0.25, 0.1, 0.65, 0.03], facecolor = axcolor)
axamp = plt.axes([0.25, 0.15, 0.65, 0.03], facecolor = axcolor)

samp = Slider(axamp, "Theta1", 0.1, 10.0, valinit = t1_0)
sfreq = Slider(axfreq, "Theta2", 0.1, 30.0, valinit = t2_0)

def update(val):
    amp = samp.val
    freq = sfreq.val
    #l.set_ydata(amp * np.sin(2 * np.pi * freq * t))
    xs = []
    ys = []
    theta1_delta = 0.1 + amp
    theta2_delta = 0.2 + freq
    for i in range(1000):
        t1 = theta1_delta * i
        t2 = theta2_delta * i
        xs.append(math.cos(t1))
        ys.append(math.sin(t2))
    l.set_xdata(xs)
    l.set_ydata(ys)
    fig.canvas.draw_idle()
sfreq.on_changed(update)
samp.on_changed(update)

resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', color = axcolor, hovercolor = "0.975")

def reset(event):
    sfreq.reset()
    samp.reset()
button.on_clicked(reset)

rax = plt.axes([0.025, 0.5, 0.15, 0.15], facecolor = axcolor)
radio = RadioButtons(rax, ("red", "blue", "green"), active = 0)

def colorfunc(label):
    l.set_color(label)
    fig.canvas.draw_idle()
radio.on_clicked(colorfunc)

plt.axis("equal")
plt.show()
