#pragma once

#ifndef _MinPQ_

#include <vector>
#include <utility>
#include <iostream>

/**
* Index Minimum PriorityQueue
* 在实际使用中, 很有可能客户端程序员多个平行的数组来存储相同元素的各种类型的信息, 比如有id[N]来存储这N个元素的id, height[N]存储这
* N个元素的height, 这时候直接用排序index, 用index来访问所有这些平行的数组.
* @template Compare 比如是重载了 int operator () 的类或struct, 不然就编译报错了23333
* @author DCMMC
*/
template<class Key, class Compare>
class IndexMinPQ {
private:
	//number of elements on PQ
	int size = 0;
	//binary heap using 1-based indexing hold
	std::vector<int> pq;
	//inverse: qp[pq[i]] = pq[qp[i]] = i
	//qp[i] = -1表示i这个index不在queue中
	//qp存储的是client存入的index在pq的位置
	std::vector<int> qp;
	//hold the keys value with priorities
	std::vector<Key *> keys;
	// 堆容量
	int capacity;

public:
	/**
	* 构造器
	* @param capacity 容量
	* @param key 默认填充的值, 填入个数为 capacity + 1
	*/
	IndexMinPQ(int capacity = 10, Key *key = NULL) {
		this->capacity = capacity;
		pq = std::vector<int>(capacity + 1, 0);
		qp = std::vector<int>(capacity + 1, 0);

		size = 0;

		if (key != NULL) {
			for (int i = 0; i < capacity; i++) {
				qp[i] = ++size;
				pq[size] = i;
				keys.push_back(new int(*key));
			}
			delete key;
			qp[capacity] = -1;
		}
		else {
			//现在没有任何index在queue中
			for (int i = 0; i < capacity + 1; i++)
				qp[i] = -1;
		}
	}

	/**
	* 析构器
	* ####################  TODO  #########################
	*/
	~IndexMinPQ() {

	}

	/**
	* insert item
	* @param k
	*		associated index
	* @param key
	*		item to insert
	*/
	void insert(int k, Key *key) {
		if (size + 1 > capacity) {
			std::cerr << "This priority queue is FULL!" << std::endl;
			return;
		}

		//先添加在最后一个元素后面, 然后swim上去
		qp[k] = ++size;
		pq[size] = k;
		keys[k] = key;
		swim(size);
	}

	/**
	* change the item associated with k to {@code item}
	* @param k
	*		associated index
	* @param key
	*		new value in item associated with index k
	*/
	void change(int k, Key *key) {
		if (!contains(k)) {
			std::cerr << "index k not associated with any item!" << std::endl;
			return;
		}

		Key *tmp = keys[k];

		//如果改成了一个更加大的数, 就对这个位置进行sink
		if ( Compare().operator()(*(keys[k]), *key) < 0 ) {
			keys[k] = key;
			sink(qp[k]);
		}
		else {
			keys[k] = key;
			//否则就是向上swim
			swim(qp[k]);
		}

		delete tmp;
	}

	/**
	* is k associated with some item?
	* @param k
	*		index to be jduged
	* @return
	*		return true if k associated with some item, otherwise return false
	*/
	bool contains(int k) {
		return qp[k] != -1;
	}

	/**
	* remove k and its associated item
	* @param k
	*		the index of item to be removed
	*/
	void del(int k) {
		if (!contains(k)) {
			std::cerr << "index k not associated with any item!" << std::endl;
			return;
		}

		keys[k] = NULL;
		//和pq中最后一个元素交换并且对这个元素进行sink
		exch(qp[k], size--);
		sink(qp[k]);
		qp[k] = -1;
	}

	/**
	* return a minimal item
	* @return
	*		the minimal item
	*/
	Key *min() {
		return isEmpty() ? NULL : keys[pq[1]];
	}

	/**
	* return a minimal item's index
	* @return
	*		the index of minimal item, -1表示queue没空, 没有最小的元素的index
	*/
	int minIndex() {
		return isEmpty() ? -1 : pq[1];
	}

	/**
	* remove a minimal item and return its index
	* @return
	*		the index of minimal item, 如果PQ为空就返回-1
	*/
	int delMin() {
		if (isEmpty())
			return -1;

		int indexOfMin = pq[1];
		exch(1, size--);
		sink(1);
		keys[pq[size + 1]] = NULL;
		qp[pq[size + 1]] = -1;
		return indexOfMin;
	}

	/**
	* is the priority queue empty?
	* @return
	*		true if the priority queue is empty
	*/
	bool isEmpty() {
		return size == 0;
	}

	/**
	* number of items in the priority queue
	* @return
	*		items' number
	*/
	int getSize() {
		return this->size;
	}

private:

	/**
	* exch
	* 交换pq中i和j位置上存储的信息, 同时相应的对qp中的元素交换
	* @param i 序号 1 <= i <= sizes
	* @param j 序号 1 <= j <= size
	*/
	void exch(int i, int j) {
		//先把qp中记录的元素交换, 也就是把qp中记录着keys中这两个元素的index在pq存储的位置的值先交换
		qp[pq[i]] = j;
		qp[pq[j]] = i;

		//将pq中继续的这两个index给互换
		int tmp = pq[i];
		pq[i] = pq[j];
		pq[j] = tmp;
	}

	/**
	* less
	* 比较pq中i和j的位置上存储的index对应的Key
	*/
	bool more(int i, int j) {
		return Compare().operator()( *(keys[pq[i]]), *(keys[pq[j]]) ) > 0;
	}

	/**
	* Bottom-up reheapifying (swim)
	* O(logN)
	* @param k 要操作的node的位置 1 <= k <= size
	*/
	void swim(int k) {
		//floor(k/2)就是k的父节点的index, 易证
		while (k > 1 && more(k / 2, k)) {
			exch(k / 2, k);
			k /= 2;
		}
	}

	/**
	* Top-down reheapofying (sink)
	* O(logN)
	* 通过不断与其较小的子节点进行交换直到两个子节点的key都要大于等于该node, 或者是已经到了heap的bottom
	* k节点的两个子节点的index分别为2k和2k + 1
	* @param k 要操作的node的位置 1 <= k <= size
	*/
	void sink(int k) {
		//其左边那个子节点要存在, 不然的话就已经到了heap的bottom了
		while (k * 2 <= size) {
			int j = 2 * k;
			if (j < size && more(j, j + 1))
				++j;
			if (!more(k, j))
				break;
			exch(k, j);
			k = j;
		}
	}
	
};

#endif // !_MinPQ_