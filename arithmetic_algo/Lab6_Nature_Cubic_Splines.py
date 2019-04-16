#!/bin/env python3
# 三次自然样条
import numpy as np
import matplotlib.pyplot as plt

f = lambda X, Y: lambda x: np.sum([y_k * np.prod([ # noqa
            (x - x_i) / (X[k] - x_i) if i != k else 1 for
            i, x_i in enumerate(X)]) for k, y_k in enumerate(Y)])


def cubic_splines(X, Y):
    """
    三次自然样条插值
    Piecewise Polynomial Approximation
    """
    n = len(X)
    H = [(X[k+1] - X[k]) for k in range(n - 1)]
    A = {1: 2 * (H[0] + H[1])}
    for k in range(2, n - 1):
        A[k] = 2 * (H[k-1] + H[k]) - H[k-1]**2 / A[k-1]
    C = {}
    for k in range(1, n):
        C[k] = (Y[k] - Y[k-1]) / H[k-1]
    D = {}
    for k in range(1, n-1):
        D[k] = 6 * (C[k+1] - C[k])
    B = {1: D[1]}
    for k in range(2, n-1):
        B[k] = D[k] - B[k-1] * H[k-1] / A[k]
    S_two = {0: 0, (n-1): 0, (n-2): B[n-2] / A[n-2]}
    for k in range(n-3, 0, -1):
        S_two[k] = (B[k] - H[k] * S_two[k+1]) / A[k]

    def natural_cubic_spline(x):
        for k in range(0, n-1):
            if (k == 0 and x <= X[k]) or (X[k] <= x <= X[k+1]) or k == n-2:
                S_one = C[k+1] - S_two[k+1] * H[k] / 6 - S_two[k] * H[k] / 3
                return Y[k] + S_one * (x - X[k]) + \
                    S_two[k] * ((x - X[k]) ** 2) / 2 + \
                    (S_two[k+1] - S_two[k]) * ((x - X[k]) ** 3) / (6 * H[k])

    return natural_cubic_spline


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
        fun = cubic_splines(X, Y)
        xs = np.arange(0., 10., 0.005)
        ys = [fun(x) for x in xs]
        plt.plot(xs, ys, c='r', label="Cubic Splines", linewidth=3)
        plt.legend(loc='upper right')
        fig.canvas.draw()


ax.set_xlim([0, 10])
ax.set_ylim([0, 10])
fig.suptitle('Natural Cubic Spline interpolation, ' +
             'click some points and enter to start.',
             fontsize=20)
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
