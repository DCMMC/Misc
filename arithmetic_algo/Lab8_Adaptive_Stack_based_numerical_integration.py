#!/bin/env python3
# 自适应数值积分算法
# 前面说到的变步长是一种基于上一轮划分结果基础上再减半微分大小的方法
# 对于 runger 现象比较严重的函数, e.g. 在函数某些地方特别平缓而在某些地方
# 有特别陡峭, 如果要提高其精度, 我们只能将整个积分区域都划分的特别小,
# 这对于平缓部分无疑是一种浪费, 所以我们采用自适应方法(使用 Stack 的思想)
# 将前后变化比较大的区间放入栈顶, 这样我们就可以获得更加高效的计算过程
import math
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
import time
from matplotlib.widgets import TextBox
import matplotlib.gridspec as gridspec
from matplotlib.widgets import Button
from math import * # noqa
import traceback


fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.3)
# target func
# 在 x=0 处用罗比达法则计算其值为 1
# target = lambda x: math.sin(x) / x if x != 0 else 1 # noqa
# start = 0
# end = 6

# 这个函数能够更加好的反映, 自适应方法的区间能够在梯度大的地方表现的更加密集
# target = lambda x: math.sin(1 / x) # noqa
# start = 0.1
# end = 1.2
# ax.set_xlim([0, 1.2])
# ax.set_ylim([-1.1, 1.1])

fig.suptitle('Stack-based Adaptive Numerical Integration, click Run to start.',
             fontsize=20)
X = []
Y = []
started = False

gs = gridspec.GridSpec(2, 3, wspace=1.0)
gs.update(left=0.1, right=0.9, bottom=0.15, top=0.25, hspace=0.2)


def submit(val):
    global started
    global X
    global Y
    global initial_text
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
            started = False
            ax.clear()
            return
        X = np.arange(start, end, 0.1)
        X = np.append(X, [end, ])
        Y = [target(i) for i in X]
        try:
            adaptive_numerical_integration(target,
                                           start,
                                           end,
                                           eval(initial_text[4]),
                                           eval(initial_text[1]))
        except Exception: # noqa
            traceback.print_exc()
            ax.clear()
        started = False
    else:
        pass


axes = [fig.add_subplot(gs[i, j]) for i, j in [[0, 0], [0, 1], [1, 0], [1, 1],
                                               [0, 2]]]
initial_text = ['sin(x) / x if x != 0 else 1',
                '0.001', '0', '6', '0.01']
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
    global Y
    global initial_text
    ax.plot(X, Y, c='r', label=initial_text[0], linewidth=3)
    ax.axhline(0, color='gray', linewidth=1.5)
    ax.axvline(0, color='gray', linewidth=1.5)
    ax.legend(loc='upper right')


def adaptive_numerical_integration(target, a, b, sleep_time=0.01, e=0.001):
    # 需要继续迭代以求更高精度的区间放入此 Stack
    h = b - a
    T_0 = 0.5 * h * (target(b) + target(a))
    E = e / (b - a)
    stack = [[a, b, T_0]]
    points = [a, b]

    def draw():
        ax.clear()
        ax.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
        ax.axhline(0, color='gray', linewidth=1.5)
        ax.axvline(0, color='gray', linewidth=1.5)
        ax.legend(loc='upper right')
        for i in range(len(points) - 1):
            ax.add_patch(patches.Polygon([[points[i], 0],
                                          [points[i+1], 0],
                                          [points[i+1], target(points[i+1])],
                                          [points[i], target(points[i])]],
                                         color='green',
                                         alpha=0.6))
        fig.canvas.draw()

    draw()
    integration = 0.
    while len(stack) > 0:
        time.sleep(sleep_time)
        interval = stack[-1]
        del stack[-1]
        # 这个公式就是从 Lab7 那个公式一般化为 n=1 的情况(i.e. 一个梯形细分为两个)
        T_1 = 0.5 * interval[-1] + 0.5 * (interval[1] - interval[0]) \
            * target((interval[0] + interval[1]) / 2)
        if math.fabs(T_1 - interval[-1]) >= E * (interval[1] - interval[0]):
            mid = (interval[0] + interval[1]) / 2
            stack.append([
                interval[0], mid,
                0.5 * (mid - interval[0]) * (target(interval[0]) + target(mid))
            ])
            stack.append([
                mid, interval[1],
                0.5 * (mid - interval[0]) * (target(interval[1]) + target(mid))
            ])
            idx = points.index(interval[0])
            # 插入新的坐标, python 写起来实在是太爽了 :)
            points[idx:(idx+1)] = [interval[0], mid, interval[1]]
            draw()
        else:
            integration += T_1
    fig.suptitle('The integration is {:5f} with accuracy: {:5f}'.format(
        integration, e),
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
