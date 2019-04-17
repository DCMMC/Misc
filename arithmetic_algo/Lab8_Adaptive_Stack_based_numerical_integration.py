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


fig, ax = plt.subplots()
# target func
# 在 x=0 处用罗比达法则计算其值为 1
target = lambda x: math.sin(x) / x if x != 0 else 1 # noqa
start = 0
end = 6
ax.set_xlim([-1, 7])
ax.set_ylim([-0.3, 1.1])

# 这个函数能够更加好的反映, 自适应方法的区间能够在梯度大的地方表现的更加密集
# target = lambda x: math.sin(1 / x) # noqa
# start = 0.1
# end = 1.2
# ax.set_xlim([0, 1.2])
# ax.set_ylim([-1.1, 1.1])

fig.suptitle('Stack-based Adaptive Numerical Integration, enter to start.',
             fontsize=20)

X = np.arange(start, end, 0.0001)
Y = [target(i) for i in X]
plt.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
plt.axhline(0, color='gray', linewidth=1.5)
plt.axvline(0, color='gray', linewidth=1.5)
plt.legend(loc='upper right')


def adaptive_numerical_integration(a, b, e=0.001):
    # 需要继续迭代以求更高精度的区间放入此 Stack
    global target
    h = b - a
    T_0 = 0.5 * h * (target(b) + target(a))
    E = e / (b - a)
    stack = [[a, b, T_0]]
    points = [a, b]

    def draw():
        ax.clear()
        plt.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
        plt.axhline(0, color='gray', linewidth=1.5)
        plt.axvline(0, color='gray', linewidth=1.5)
        plt.legend(loc='upper right')
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
        # time.sleep(0.01)
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


def onpress(e):
    if e.key == 'enter':
        global start
        global end
        adaptive_numerical_integration(start, end)


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
