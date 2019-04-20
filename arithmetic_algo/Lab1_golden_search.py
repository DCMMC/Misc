#!/bin/env python3
# 黄金分割搜索
import math
import traceback
import matplotlib.pyplot as plt
import numpy as np
import time
from matplotlib.widgets import TextBox
import matplotlib.gridspec as gridspec
from matplotlib.widgets import Button
from math import * # noqa


# y_range = [0., 1.]
fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.3)
started = False
# f = lambda x: math.cos(x) # noqa


def zero_unit(n):
    u = 1
    while n < 1:
        n *= 10
        u /= 10
    return u


def plot_func(left, right, f):
    global initial_text
    if left >= right:
        print('参数必须 left < right!')
        return
    X = np.arange(left, right, zero_unit(right - left) / 100)
    Y = [f(i) for i in X]
    ax.plot(X, Y, c='r', label=initial_text[0], linewidth=3)
    ax.axhline(0, color='gray', linewidth=1.5)
    ax.axvline(0, color='gray', linewidth=1.5)
    ax.legend(loc='upper right')
    ax.set_xlim(left - (right - left) / 10, right + (right - left) / 10)
    # ax.set_ylim([y_range[0] - 0.1, y_range[1] + 0.1])


fig.suptitle('Click Run button to start golden search maxima.',
             fontsize=20)

# fig.canvas.mpl_connect('key_press_event', lambda e: golden_search() if
#                         e.key == 'enter' else None)
gs = gridspec.GridSpec(2, 3, wspace=1.0)
gs.update(left=0.1, right=0.9, bottom=0.15, top=0.25, hspace=0.2)


def submit(val):
    global started
    initial_text = [tb_func.text, tb_acc.text, tb_start.text,
                    tb_end.text, tb_sleep.text]
    if not started:
        fig.suptitle('Click Run button to start golden search maxima.',
                     fontsize=20)

        started = True
        # print(initial_text)
        try:
            golden_search(a=eval(initial_text[2]), b=eval(initial_text[3]),
                          f=eval('lambda x: ' + initial_text[0]),
                          sleep=eval(initial_text[4]),
                          e=eval(initial_text[1]))
        except Exception:
            ax.clear()
            traceback.print_exc()
        started = False
    else:
        pass


axes = [fig.add_subplot(gs[i, j]) for i, j in [[0, 0], [0, 1], [1, 0], [1, 1],
                                               [0, 2]]]
initial_text = ['cos(x)', '0.01', '-0.5*pi', '0.5*pi', '1']
tb_func = TextBox(axes[0], 'Function', initial=initial_text[0])
tb_acc = TextBox(axes[1], 'Accuracy', initial=initial_text[1])
tb_start = TextBox(axes[2], 'Start', initial=initial_text[2])
tb_end = TextBox(axes[3], 'End', initial=initial_text[3])
tb_sleep = TextBox(axes[4], 'Sleep time(s)', initial=initial_text[4])
ax_button = fig.add_subplot(gs[1, 2])
btn = Button(ax_button, 'Run')
btn.on_clicked(submit)


#
# for tb in [tb_func, tb_acc, tb_start, tb_end]:
#     tb.on_submit(submit)


def annotate(x, y, text):
    ax.annotate(text,
                xy=(x, y), xytext=(-20, 20),
                textcoords='offset points', ha='right', va='bottom',
                bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.5),
                arrowprops=dict(arrowstyle='->',
                                connectionstyle='arc3,rad=0'))


def draw_working_range(left, right, golden_l, golden_r):
    rg = np.arange(left, right, 0.001)
    rg = np.append(rg, [right, ])
    ax.fill_between(rg,
                    [1.1 for _ in rg],
                    [-0.1 for _ in rg],
                    color="crimson", alpha=0.4)
    ax.axvline(left, color='r', linewidth=3, alpha=0.5, linestyle='--')
    ax.axvline(right, color='r', linewidth=3, alpha=0.5, linestyle='--')
    ax.axvline(golden_l, color='r', linewidth=5, alpha=0.8, linestyle='--')
    ax.axvline(golden_r, color='r', linewidth=5, alpha=0.8, linestyle='--')


def golden_search(a: float = math.pi*(-0.5),
                  b: float = math.pi*0.5,
                  f=lambda x: math.cos(x),
                  sleep=1,
                  e: float = 0.001):
    """
    用黄金分割搜索算法求cos(x),x∈[-π/2,π/2]的最大值
    设计出具体的程序，使之能够动态演示搜索过程
    @parameter a
    """
    # check parameters
    if a >= b or e >= (b - a):
        print('a, b, e are invalid.')
        return
    ax.clear()
    plot_func(a, b, f=f)
    fig.canvas.draw()
    x_1 = a + (b - a) * 0.382
    x_2 = a + (b - a) * 0.618
    f_1 = f(x_1)
    f_2 = f(x_2)
    draw_working_range(a, b, x_1, x_2)
    fig.canvas.draw()
    # print('result is between {:.6f}π and {:.6f}π.'.format(
    #     a / math.pi, b / math.pi))
    while b - a > e:
        time.sleep(sleep)
        ax.clear()
        plot_func(a, b, f=f)
        # print('cos({:.6f}π)={:.6f}, cos({:.6f}π)={:.6f}'.format(
        #     x_1 / math.pi, f_1, x_2 / math.pi, f_2))
        if f_2 > f_1:
            annotate(x_2, f_2, 'f({:2f})={:2f}'.format(x_2, f_2))
            a = x_1
            x_1 = x_2
            f_1 = f_2
            x_2 = a + 0.618 * (b - a)
            f_2 = f(x_2)
            draw_working_range(a, b, x_1, x_2)
        else:
            annotate(x_1, f_1, 'f({:2f})={:2f}'.format(x_1, f_1))
            b = x_2
            x_2 = x_1
            f_2 = f_1
            x_1 = a + 0.382 * (b - a)
            f_1 = f(x_1)
            draw_working_range(a, b, x_1, x_2)
            # print('result is between {:.6f}π and {:.6f}π.'.format(
            #     a / math.pi, b / math.pi))
        fig.canvas.draw()
    # print('find final: cos({:.6f}π)={:.6f}'.format(
    #     (a + b / 2) / math.pi, f((a + b) / 2)))
    fig.suptitle('Finally find result: {:.6f} at {:.2f}'.format(f((a+b)/2),
                 (a+b)/2))


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
