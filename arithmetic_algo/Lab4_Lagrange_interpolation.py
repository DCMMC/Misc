#!/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

f = lambda x, X, Y: np.sum([y * np.prod([ # noqa
            (x - x_i) / (X[idx] - x_i) if idx != x_idx else 1 for
            x_idx, x_i in enumerate(X)]) for idx, y in enumerate(Y)])

fig = plt.figure()


def onclick(event):
    x, y = event.xdata, event.ydata
    print('Click: ({}, {})'.format(x, y))
    fig.canvas.draw()


fig.canvas.mpl_connect('button_press_event', onclick)
plt.show()
