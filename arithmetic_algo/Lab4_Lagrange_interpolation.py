#!/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

f = lambda X, Y: lambda x: np.sum([y_k * np.prod([ # noqa
            (x - x_i) / (X[k] - x_i) if i != k else 1 for
            i, x_i in enumerate(X)]) for k, y_k in enumerate(Y)])


fig, ax = plt.subplots()

X = []
Y = []


def onclick(event):
    global X, Y
    x, y = event.xdata, event.ydata
    # print('Click: ({}, {})'.format(x, y))
    X.append(x)
    Y.append(y)
    plt.plot(x, y, 'ro', markersize=10)
    plt.annotate('({:.2f}, {:.2f})'.format(x, y),
                 xy=(x, y), xytext=(-20, 20),
                 textcoords='offset points', ha='right', va='bottom',
                 bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.5),
                 arrowprops=dict(arrowstyle='->',
                                 connectionstyle='arc3,rad=0'))
    fig.canvas.draw()


def onpress(e):
    if e.key == 'enter':
        global X, Y
        fun = f(X, Y)
        xs = np.arange(0., 10., 0.005)
        ys = [fun(x) for x in xs]
        plt.plot(xs, ys, c='r', label="Lagrange", linewidth=3)
        fig.canvas.draw()


ax.set_xlim([0, 10])
ax.set_ylim([0, 10])
fig.suptitle('Click some points and enter to start.', fontsize=20)
fig.canvas.mpl_connect('button_press_event', onclick)
fig.canvas.mpl_connect('key_press_event', onpress)

# Maximize window size
manager = plt.get_current_fig_manager()
# https://stackoverflow.com/questions/32428193/saving-matplotlib-graphs-to-image-as-full-screen/32428266
try:
    # Option 1
    # QT backend
    manager.window.showMaximized()
except: # noqa
    try:
        # Option 2
        # TkAgg backend
        manager.resize(*manager.window.maxsize())
    except: # noqa
        try:
            # Option 3
            # WX backend
            manager.frame.Maximize(True)
        except: # noqa
            print('Cound not Maximize window szie, try to fullscreen')
            manager.full_screen_toggle()

plt.show()
