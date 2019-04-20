#!/bin/env python
import numpy as np
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D # noqa
import matplotlib.pyplot as plt
# import matplotlib.animation as animation
from autograd import value_and_grad
from matplotlib.widgets import TextBox
import matplotlib.gridspec as gridspec
import traceback
from matplotlib.widgets import Button
from math import * # noqa


# f = lambda x, y: 1 / (x**2 + y**2 + 2) # noqa
fig = plt.figure()
plt.subplots_adjust(bottom=0.3)
started = False
ax = fig.gca(projection='3d')

gs = gridspec.GridSpec(2, 4, wspace=1.0)
gs.update(left=0.1, right=0.9, bottom=0.15, top=0.25, hspace=0.2)

line, = ax.plot([], [], [], 'b', label='Climb Mountain', lw=4)
point, = ax.plot([], [], [],  'bo', markersize=15)


def gradient_descent(start, end, f, interval, step, acc, init):

    # data
    X = np.arange(start[0], end[0], 0.01)
    Y = np.arange(start[1], end[1], 0.01)
    X, Y = np.meshgrid(X, Y)
    Z = f(X, Y)
    # Plot the surface.
    surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, # noqa
                           linewidth=0, antialiased=False) # noqa
    maxima = np.array([0., 0.]).reshape(-1, 1)
    ax.plot(*maxima, f(*maxima), 'r*', markersize=20)
    print(*maxima, '\n', f(*maxima))
    # ax.set_zlim(-0.1, 0.6)
    ax.view_init(azim=60, elev=10.)
    # fig.colorbar(surf, shrink=0.5, aspect=5)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')
    ax.set_zlabel('$z$')
    ax.legend(loc='upper left')

    climb(func=f, init=init, epsilon=acc, lambda_0=step) # noqa
    # print(len(path[0]))

    # animation.FuncAnimation(plt.figure(), animate(path, f),
    #                         frames=path.shape[1], interval=interval,
    #                         repeat_delay=200, blit=True)


def submit(val):
    global started
    initial_text = [tb_func.text, tb_acc.text, tb_start.text,
                    tb_end.text, tb_speed.text, tb_init.text,
                    tb_step.text]
    if not started:
        fig.suptitle('Click Run button to start golden search maxima.',
                     fontsize=20)

        started = True
        ax.clear()
        # print(initial_text)
        try:
            gradient_descent(start=eval(initial_text[2]),
                             end=eval(initial_text[3]),
                             f=eval('lambda x, y: ' + initial_text[0]),
                             interval=1000 // eval(initial_text[4]),
                             step=eval(initial_text[-1]),
                             init=np.array(eval(initial_text[-2]),
                                           dtype=float),
                             acc=eval(initial_text[1]))
        except Exception:
            ax.clear()
            traceback.print_exc()
        started = False
    else:
        pass


axes = [fig.add_subplot(gs[i, j]) for i, j in [[0, 0], [0, 1],
                                               [0, 2], [0, 3],
                                               [1, 0], [1, 1],
                                               [1, 2]]]
initial_text = ['1 / (x**2 + y**2 + 2)', '0.5', '-5, -5', '5, 5', '10',
                '2, 2', '0.01']
tb_func = TextBox(axes[0], 'Function', initial=initial_text[0])
tb_step = TextBox(axes[1], 'Step', initial=initial_text[1])
tb_start = TextBox(axes[2], 'Start(x, y)', initial=initial_text[2])
tb_end = TextBox(axes[3], 'End(x, y)', initial=initial_text[3])
tb_speed = TextBox(axes[4], 'Speed(flames/s)', initial=initial_text[4])
tb_init = TextBox(axes[5], 'Init point(x, y)', initial=initial_text[5])
tb_acc = TextBox(axes[6], 'Accuracy', initial=initial_text[6])
ax_button = fig.add_subplot(gs[1, 3])
btn = Button(ax_button, 'Run')
btn.on_clicked(submit)


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


def climb(func, init, epsilon, lambda_0):
    xy = init
    ax.plot([xy[0], ], [xy[1], ], [func(*xy), ], 'b*', markersize=20)
    fig.canvas.draw()
    func_grad = value_and_grad(lambda args: func(*args))
    l2 = lambda g: np.sqrt(float(g[0])**2 + float(g[1])**2) # noqa
    grad = func_grad(xy)[1]
    while l2(grad) >= epsilon:
        xy += np.array([float(grad[0]), float(grad[1])]) * lambda_0
        ax.plot([xy[0], ], [xy[1], ], [func(*xy), ], 'bo', markersize=10)
        fig.canvas.draw()
        grad = func_grad(xy)[1]


plt.show()
