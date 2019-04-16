#!/bin/env python3
# Refs:
# [1]. http://math.umd.edu/~petersd/460/interp460.pdf
import numpy as np
import matplotlib.pyplot as plt


def newton_interpolation(X, Y):
    """
    本质上就是 Lagrange 插值法, 不过在时间复杂度上更加优秀
    """
    # X = [x_0, /cdots, x_{n-1}]
    # Calculate the divided difference by constructing divided difference table
    A = Y
    n = len(X)
    # python 的 range(a, b) 是 [a, b)
    # 计算复杂度: 对于 n 个点(x_0, /cdots, x_{n-1}) 有 (n(n+1)/2)D + n(n+1)A
    for k in range(1, n):
        for j in range(n-1, k-1, -1):
            A[j] = (A[j] - A[j-1]) / (X[j] - X[j-k])

    def newton(x_input):
        # use Horner's Rule to calculate P_n(x)
        p = A[-1]
        for i in range(-2, -n-1, -1):
            p = p * (x_input - X[i]) + A[i]
        return p

    return newton


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
        fun = newton_interpolation(X, Y)
        xs = np.arange(0., 10., 0.005)
        ys = [fun(x) for x in xs]
        plt.plot(xs, ys, c='r', label="Newton", linewidth=3)
        plt.legend(loc='upper right')
        fig.canvas.draw()


ax.set_xlim([0, 10])
ax.set_ylim([0, 10])
fig.suptitle('Newton interpolation, click some points and enter to start.',
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
