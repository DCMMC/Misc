#!/bin/env python3
# 变步长数值积分算法
# 为了利用前面计算的结果:
# I_{2n} = \frac{1}{2} I_n + \frac{h}{2} \sum_{k=1}^n f(a + \frac{2k-1}{2}h)
# 并且根据以下 Lemma, 我们可以知道当前结果的精度
# Lemma: \left| I_{2n} - I_n \right| \le \epsilon \Rightarrow \left| I - I_{2n} \right| < \frac{1}{3} \epsilon # noqa
import math
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
import traceback
import time
from matplotlib.widgets import TextBox
import matplotlib.gridspec as gridspec
from matplotlib.widgets import Button
from math import * # noqa


# target func
# 在 x=0 处用罗比达法则计算其值为 1
# target = lambda x: math.sin(x) / x if x != 0 else 1 # noqa
# start = 0
# end = 6

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.3)
started = False

# ax.set_xlim([-1, 7])
# ax.set_ylim([-0.3, 1.1])
fig.suptitle('Vary Step Numerical Integration, click Run to start.',
             fontsize=20)

X = []
Y = []

gs = gridspec.GridSpec(2, 3, wspace=1.0)
gs.update(left=0.1, right=0.9, bottom=0.15, top=0.25, hspace=0.2)


def submit(val):
    global started
    global X
    global Y
    initial_text = [tb_func.text, tb_acc.text, tb_start.text,
                    tb_end.text, tb_sleep.text]
    if not started:
        fig.suptitle('Click Run button to start golden search maxima.',
                     fontsize=20)
        started = True
        try:
            target = eval('lambda x: ' + initial_text[0])
            start = eval(initial_text[2])
            end = eval(initial_text[3])
        except Exception: # noqa
            traceback.print_exc()
            ax.clear()
            started = False
            return
        if start >= end:
            ax.clear()
            started = False
            return
        X = np.arange(start, end, 0.1)
        X = np.append(X, [end, ])
        Y = [target(i) for i in X]
        try:
            vary_step_integration(target,
                                  start,
                                  end,
                                  eval(initial_text[4]),
                                  eval(initial_text[1]))
        except Exception: # noqa
            ax.clear()
            traceback.print_exc()
        started = False
    else:
        pass


axes = [fig.add_subplot(gs[i, j]) for i, j in [[0, 0], [0, 1], [1, 0], [1, 1],
                                               [0, 2]]]
initial_text = ['sin(x) / x if x != 0 else 1',
                '0.001', '0', '6', '1']
tb_func = TextBox(axes[0], 'Function', initial=initial_text[0])
tb_acc = TextBox(axes[1], 'Accuracy', initial=initial_text[1])
tb_start = TextBox(axes[2], 'Start', initial=initial_text[2])
tb_end = TextBox(axes[3], 'End', initial=initial_text[3])
tb_sleep = TextBox(axes[4], 'Sleep time(s)', initial=initial_text[4])
ax_button = fig.add_subplot(gs[1, 2])
btn = Button(ax_button, 'Run')
btn.on_clicked(submit)


def plot_func():
    global X
    global initial_text
    global Y
    ax.plot(X, Y, c='r', label=initial_text[0], linewidth=3)
    ax.axhline(0, color='gray', linewidth=1.5)
    ax.axvline(0, color='gray', linewidth=1.5)
    ax.legend(loc='upper right')


def vary_step_integration(target, start, end, sleep_time=1, epsilon=0.001):
    h = end - start
    T_0 = (h * target(start) + target(end)) / 2
    T_1 = T_0 / 2 + h / 2 * target(start + h / 2)
    ax.clear()
    plot_func()
    ax.add_patch(patches.Polygon([[start, 0],
                                  [end, 0],
                                  [end, target(end)],
                                  [start, target(start)]],
                                 color='green',
                                 alpha=0.6))
    fig.canvas.draw()
    n = 1
    while math.fabs(T_1 - T_0) >= epsilon:
        time.sleep(sleep_time)
        h /= 2
        n *= 2
        T_0 = T_1
        T_1 = 0.5 * T_0
        ax.clear()
        plot_func()
        last = start
        for k in range(1, n+1):
            right = start + (k - 0.5) * h
            ax.add_patch(patches.Polygon([[last, 0],
                                          [right, 0],
                                          [right, target(right)],
                                          [last, target(last)]],
                                         color='green',
                                         alpha=0.6))
            T_1 += (h / 2) * target(right)
            last = right
        ax.add_patch(patches.Polygon([[last, 0],
                                      [end, 0],
                                      [end, target(end)],
                                      [last, target(last)],
                                      ],
                                     color='green',
                                     alpha=0.6))
        fig.canvas.draw()
    fig.suptitle('The integration is {:5f} with accuracy: {:5f}'.format(
        T_1, epsilon),
                 fontsize=20)
    fig.canvas.draw()


# fig.canvas.mpl_connect('key_press_event', onpress)

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
