#pragma once

#ifndef _UnionFind_

#include <vector>
#include <iostream>

/**
* 快速联合查找
* WQUPC (Weighted Quick Union with Path Compression)
* Any sequence of M union-find ops(abbr. operations) on N objects makes <= c( M + lg* N ) array access
* P.S. lg* N is iterate logarithm base 2, it means the number of time to make the lg of N to 1.
* e.g. lg* (2^65535) = 5) What a AMAZING function!
* But it is impossible to make the Complexity(时间复杂度) to O(N) (i.e.linear线性) as it was be proved by theory.
* @author DCMMC
*/
class UnionFind {
private:
	// Only when the index of array has the same value,
	//they are connected(i.e. them are in A Connected Components.
	std::vector<int> id;
	//To use quickUnionWeighted method.
	//记录每个site下面的子site的个数
	std::vector<int> size;

	//number of components
	int count;

public:
	/**
	* Initializes an empty union–find data structure with {@code n} sites
	* {@code 0} through {@code n-1}. Each site is initially in its own
	* component.
	* O(N)
	*
	* @param  n the number of sites
	* @throws IllegalArgumentException if {@code n < 0}
	*/
	UnionFind(int n) {
		if (n < 0) {
			std::cout << "参数n(" << n << ")必须是大于0的整数!\n";
			return;
		}

		count = n;

		//初始化, 每个位的id都不一样, 代表都没有连接
		for (int i = 0; i < n; i++) {
			id.push_back(i);
			size.push_back(i);
		}
	}

	/**
	* Returns the number of components.
	*
	* @return the number of components (between {@code 1} and {@code n})
	*/
	int getCount() const {
		return count;
	}

	/**
	* A better rootId Method. Decrease the Depth of the tree when get the rootId.
	* That called Path Compression.(路径压缩)
	* Also O(log N) but better.
	* @param index
	*           要找到其rootId的位的index
	* @return index指向的根位的id
	*/
	int rootId(int index) {
		while (index != id[index]) {
			//This addition line to set the value of indexfrom its parent to its grandparent.
			//This will Decrease the Depth to Depth - 1
			id[index] = id[id[index]];
			index = id[index];
		}

		return index;
	}

	/**
	* 判断p, q是否相连
	* O(log N) 
	* @param p
	*           判断p位和q位是否相连(即他们的rootId是否一样)
	* @param q
	*           判断p位和q位是否相连(即他们的rootId是否一样)
	* @return 如果p, q相连就返回true
	*/
	bool isConnected(int p, int q) {
		return rootId(p) == rootId(q);
	}

	/**
	* To Avoid create a Tall tree. We ues Weighted Tree to make sure A Smaller Tree is always linked
	* to the bottom of the Taller Tree.
	* Connect the root of smaller tree to the root of the larger tree. Let Tree Shorted and wider.
	* Improve the efficiency of union method.
	* O(log N) But Much less than the O(log N) of quickUnion Method. Because the Depth is more less.
	* @param p
	*         p与q相连
	* @param q
	*         p与q相连
	*/
	void quickUnion(int p, int q) {
		int pRootId = rootId(p);
		int qRootId = rootId(q);

		if (pRootId == qRootId)
			return;
		if (size[pRootId] < size[qRootId]) {
			id[pRootId] = qRootId;
			size[qRootId] += size[pRootId];
		}
		else {
			id[qRootId] = pRootId;
			size[pRootId] += size[qRootId];
		}

		count--;
	}
};

#endif // !_UnionFind_

