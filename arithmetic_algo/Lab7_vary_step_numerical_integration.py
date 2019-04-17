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
import time

# target func
# 在 x=0 处用罗比达法则计算其值为 1
target = lambda x: math.sin(x) / x if x != 0 else 1 # noqa
start = 0
end = 6

fig, ax = plt.subplots()

ax.set_xlim([-1, 7])
ax.set_ylim([-0.3, 1.1])
fig.suptitle('Vary Step Numerical Integration, enter to start.',
             fontsize=20)

X = np.arange(start, end, 0.1)
Y = [target(i) for i in X]
plt.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
plt.axhline(0, color='gray', linewidth=1.5)
plt.axvline(0, color='gray', linewidth=1.5)
plt.legend(loc='upper right')


def onpress(e):
    if e.key == 'enter':
        global target
        global start
        global end
        epsilon = 0.001
        h = end - start
        T_0 = (h * target(start) + target(end)) / 2
        T_1 = T_0 / 2 + h / 2 * target(start + h / 2)
        ax.clear()
        plt.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
        plt.axhline(0, color='gray', linewidth=1.5)
        plt.axvline(0, color='gray', linewidth=1.5)
        plt.legend(loc='upper right')
        ax.add_patch(patches.Polygon([[start, 0],
                                      [end, 0],
                                      [end, target(end)],
                                      [start, target(start)]],
                                     color='green',
                                     alpha=0.6))
        fig.canvas.draw()
        n = 1
        while math.fabs(T_1 - T_0) >= epsilon:
            time.sleep(3)
            h /= 2
            n *= 2
            T_0 = T_1
            T_1 = 0.5 * T_0
            ax.clear()
            plt.plot(X, Y, c='r', label="$\\frac{\\sin (x)}{x}$", linewidth=3)
            plt.axhline(0, color='gray', linewidth=1.5)
            plt.axvline(0, color='gray', linewidth=1.5)
            plt.legend(loc='upper right')
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
                                          [last, target(last)],
                                          [end, 0],
                                          [end, target(end)]],
                                         color='green',
                                         alpha=0.6))
            fig.canvas.draw()
        fig.suptitle('The integration is {:5f} with accuracy: {:5f}'.format(
            T_1, epsilon),
                     fontsize=20)
        fig.canvas.draw()


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
