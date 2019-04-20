#!/bin/env python3
from math import * # noqa
import math
import traceback
# import matplotlib
import matplotlib.pyplot as plt # noqa
import numpy as np # noqa
import time # noqa
from matplotlib.widgets import TextBox # noqa
import matplotlib.gridspec as gridspec # noqa
from matplotlib.widgets import Button # noqa


# y_range = [0., 1.]
# x_range = [-0.5 * math.pi, 0.5 * math.pi]
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
    X = np.arange(left, right, zero_unit(right - left) / 100)
    Y = [f(i) for i in X]
    ax.plot(X, Y, c='r', label=initial_text[0], linewidth=3)
    ax.axhline(0, color='gray', linewidth=1.5)
    ax.axvline(0, color='gray', linewidth=1.5)
    ax.legend(loc='upper right')
    ax.set_xlim(left - (right - left) / 10, right + (right - left) / 10)
    # ax.set_ylim([y_range[0] - 0.1, y_range[1] + 0.1])


fig.suptitle('Click Run button to start binary search for roots.',
             fontsize=20)

# fig.canvas.mpl_connect('key_press_event', lambda e: golden_search() if
#                         e.key == 'enter' else None)
gs = gridspec.GridSpec(2, 3, wspace=1.0)
gs.update(left=0.1, right=0.9, bottom=0.15, top=0.25, hspace=0.2)


def submit(val):
    global started
    initial_text = [tb_func.text, tb_acc.text, tb_start.text, tb_end.text,
                    tb_sleep.text]
    if not started:
        started = True
        # print(initial_text)
        try:
            a, b = eval(initial_text[2]), eval(initial_text[3])
            e = eval(initial_text[1])
            if a >= b:
                print('参数 start 和 end 必须满足 start < end!')
                return
            if (b - a) < e:
                print('已经满足精度条件 e!')
                return
            f = eval('lambda x: ' + initial_text[0])
            plot_func(a, b, f)
            fig.canvas.draw()
            binary_search(a=a, b=b,
                          f=f,
                          sleep_time=eval(initial_text[4]),
                          e=e)
        except Exception: # noqa
            traceback.print_exc()
            ax.clear()
        started = False
    else:
        pass


axes = [fig.add_subplot(gs[i, j]) for i, j in [[0, 0], [0, 1], [1, 0], [1, 1],
                                               [0, 2]]]
initial_text = ['x - 2*sin(x)', '0.05', '-4', '4', '0']
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
    return ax.annotate(text,
                       xy=(x, y), xytext=(-20, 20),
                       textcoords='offset points', ha='right', va='bottom',
                       bbox=dict(boxstyle='round,pad=0.5', fc='yellow',
                                 alpha=0.5),
                       arrowprops=dict(arrowstyle='->',
                                       connectionstyle='arc3,rad=0'))


def draw_working_range(left, right):
    ax.fill_between(np.arange(left, right, 0.001),
                    [1.1 for _ in np.arange(left, right, 0.001)],
                    [-0.1 for _ in np.arange(left, right, 0.001)],
                    color="crimson", alpha=0.4)
    ax.axvline(left, color='r', linewidth=3, alpha=0.5, linestyle='--')
    ax.axvline(right, color='r', linewidth=3, alpha=0.5, linestyle='--')
    ax.axvline((left + right) / 2, color='r', linewidth=7, alpha=0.8,
               linestyle='--')


roots = []
annotate_cnt = 0


def binary_search(f, a: float = -4.,
                  b: float = 4.,
                  e: float = 0.01,
                  last_anno=None,
                  sleep_time=1):
    """
    Recurrsive Binary search for `x=2sinx`
    a and b is analogy range to the roots
    """
    global roots
    global annotate_cnt
    if last_anno is not None:
        last_anno.remove()
    if b - a >= e:
        # print('search in range ({:.4f}, {:.4f})'.format(a, b))
        mid = (a + b) / 2
        # print(f(mid))
        anno = annotate(mid, f(mid), 'f({:.2f})={:.4f}'.format(mid, f(mid)))
        fig.canvas.draw()
        time.sleep(sleep_time)
        # avoid repeat roots
        for i in roots:
            if math.fabs(mid - i) <= 4 * e:
                anno.remove()
                return
        if (math.fabs(f(mid) - 0)) < e:
            roots.append(mid)
            for i, root in enumerate(roots):
                annotate(root, f(root), 'Root {}'.format(i + 1))
            fig.canvas.draw()
            # anno.remove()
            # 不停止, 为了多根
            # return
        binary_search(a=a, b=mid, last_anno=anno, f=f, e=e,
                      sleep_time=sleep_time)
        binary_search(a=mid, b=b, f=f, e=e, sleep_time=sleep_time)


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
