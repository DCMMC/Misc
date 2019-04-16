#!/bin/env python3
import math

def golden_search(a:float=math.pi*(-0.5),
				  b:float=math.pi*0.5,
				  e:float=0.001):
	"""
	用黄金分割搜索算法求cos(x),x∈[-π/2,π/2]的最大值
	设计出具体的程序，使之能够动态演示搜索过程
	@parameter a
	"""
	# check parameters
	if a >= b or e >= (b - a):
		print('a, b, e are invalid.')
		return
	x_1 = a + (b - a) * 0.382
	x_2 = a + (b - a) * 0.618
	f_1 = math.cos(x_1)
	f_2 = math.cos(x_2)
	print('result is between {:.6f}π and {:.6f}π.'.format(
		a / math.pi, b / math.pi))
	while b - a > e:
		print('cos({:.6f}π)={:.6f}, cos({:.6f}π)={:.6f}'.format(
			x_1 / math.pi, f_1, x_2 / math.pi, f_2))
		if f_2 > f_1:
			a = x_1
			x_1 = x_2
			f_1 = f_2
			x_2 = a + 0.618 * (b - a)
			f_2 = math.cos(x_2)
		else:
			b = x_2
			x_2 = x_1
			f_2 = f_1
			x_1 = a + 0.382 * (b - a)
			f_1 = math.cos(x_1)
		print('result is between {:.6f}π and {:.6f}π.'.format(
			a / math.pi, b / math.pi))
	print('find final: cos({:.6f}π)={:.6f}'.format(
		(a + b / 2) / math.pi, math.cos((a + b) / 2)))


def binary_search(a:float=-4.,
				  b:float=4.,
				  e:float=0.001,
				  f=lambda x: x - 2 * math.sin(x)):
	"""
	Recurrsive Binary search for `x=2sinx`
	a and b is analogy range to the roots
	"""
	if b - a >= e:
		# print('search in range ({:.4f}, {:.4f})'.format(a, b))
		mid = (a + b) / 2
		if (f(mid) - 0) <= e:
			print('find one root: {:.5f}'.format(mid))
		binary_search(a, mid)
		binary_search(mid, b)

if __name__ == '__main__':
	print('\n\ngolden search')
	golden_search()
	print('\n\nbinary search')
	binary_search()