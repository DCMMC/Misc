#!/bin/env python
import numpy as np
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D # noqa
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from autograd import value_and_grad

f = lambda x, y: 1 / (x**2 + y**2 + 2) # noqa

# data
X = np.arange(-5, 5, 0.01)
Y = np.arange(-5, 5, 0.01)
X, Y = np.meshgrid(X, Y)
Z = f(X, Y)

fig = plt.figure()
ax = fig.gca(projection='3d')

# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
maxima = np.array([0., 0.]).reshape(-1, 1)
ax.plot(*maxima, f(*maxima), 'r*', markersize=20)
ax.set_zlim(-0.1, 0.6)
ax.view_init(azim=60, elev=10.)
fig.colorbar(surf, shrink=0.5, aspect=5)

line, = ax.plot([], [], [], 'b', label='Climb Mountain', lw=4)
point, = ax.plot([], [], [],  'bo', markersize=15)
ax.set_xlabel('$x$')
ax.set_ylabel('$y$')
ax.set_zlabel('$z$')
ax.legend(loc='upper left')

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


def climb(func=f, init=[3., 3.], epsilon=0.01, lambda_0=0.5):
    path = []
    path.append([init[0], ])
    path.append([init[1], ])
    xy = init
    func_grad = value_and_grad(lambda args: func(*args))
    l2 = lambda g: np.sqrt(float(g[0])**2 + float(g[1])**2) # noqa
    grad = func_grad(xy)[1]
    while l2(grad) >= epsilon:
        xy += np.array([float(grad[0]), float(grad[1])]) * lambda_0
        path[0].append(xy[0])
        path[1].append(xy[1])
        grad = func_grad(xy)[1]
    return np.array(path)


def animate(path):
    def foo(i):
        line.set_data(*path[:, :i+1])
        line.set_3d_properties(f(*path[:, :i+1]))
        point.set_data(*path[:, i:i+1])
        point.set_3d_properties(f(*path[:, i:i+1]))
        return line, point
    return foo


path = climb()
print(len(path[0]))

anim = animation.FuncAnimation(fig, animate(path),
                               frames=path.shape[1], interval=60,
                               repeat_delay=5, blit=True)

plt.show()
