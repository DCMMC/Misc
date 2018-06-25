#pragma once

#ifndef _MinPQ_

#include <vector>
#include <utility>
#include <iostream>

/**
* Index Minimum PriorityQueue
* ��ʵ��ʹ����, ���п��ܿͻ��˳���Ա���ƽ�е��������洢��ͬԪ�صĸ������͵���Ϣ, ������id[N]���洢��N��Ԫ�ص�id, height[N]�洢��
* N��Ԫ�ص�height, ��ʱ��ֱ��������index, ��index������������Щƽ�е�����.
* @template Compare ������������ int operator () �����struct, ��Ȼ�ͱ��뱨����23333
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
	//qp[i] = -1��ʾi���index����queue��
	//qp�洢����client�����index��pq��λ��
	std::vector<int> qp;
	//hold the keys value with priorities
	std::vector<Key *> keys;
	// ������
	int capacity;

public:
	/**
	* ������
	* @param capacity ����
	* @param key Ĭ������ֵ, �������Ϊ capacity + 1
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
			//����û���κ�index��queue��
			for (int i = 0; i < capacity + 1; i++)
				qp[i] = -1;
		}
	}

	/**
	* ������
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

		//����������һ��Ԫ�غ���, Ȼ��swim��ȥ
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

		//����ĳ���һ�����Ӵ����, �Ͷ����λ�ý���sink
		if ( Compare().operator()(*(keys[k]), *key) < 0 ) {
			keys[k] = key;
			sink(qp[k]);
		}
		else {
			keys[k] = key;
			//�����������swim
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
		//��pq�����һ��Ԫ�ؽ������Ҷ����Ԫ�ؽ���sink
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
	*		the index of minimal item, -1��ʾqueueû��, û����С��Ԫ�ص�index
	*/
	int minIndex() {
		return isEmpty() ? -1 : pq[1];
	}

	/**
	* remove a minimal item and return its index
	* @return
	*		the index of minimal item, ���PQΪ�վͷ���-1
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
	* ����pq��i��jλ���ϴ洢����Ϣ, ͬʱ��Ӧ�Ķ�qp�е�Ԫ�ؽ���
	* @param i ��� 1 <= i <= sizes
	* @param j ��� 1 <= j <= size
	*/
	void exch(int i, int j) {
		//�Ȱ�qp�м�¼��Ԫ�ؽ���, Ҳ���ǰ�qp�м�¼��keys��������Ԫ�ص�index��pq�洢��λ�õ�ֵ�Ƚ���
		qp[pq[i]] = j;
		qp[pq[j]] = i;

		//��pq�м�����������index������
		int tmp = pq[i];
		pq[i] = pq[j];
		pq[j] = tmp;
	}

	/**
	* less
	* �Ƚ�pq��i��j��λ���ϴ洢��index��Ӧ��Key
	*/
	bool more(int i, int j) {
		return Compare().operator()( *(keys[pq[i]]), *(keys[pq[j]]) ) > 0;
	}

	/**
	* Bottom-up reheapifying (swim)
	* O(logN)
	* @param k Ҫ������node��λ�� 1 <= k <= size
	*/
	void swim(int k) {
		//floor(k/2)����k�ĸ��ڵ��index, ��֤
		while (k > 1 && more(k / 2, k)) {
			exch(k / 2, k);
			k /= 2;
		}
	}

	/**
	* Top-down reheapofying (sink)
	* O(logN)
	* ͨ�����������С���ӽڵ���н���ֱ�������ӽڵ��key��Ҫ���ڵ��ڸ�node, �������Ѿ�����heap��bottom
	* k�ڵ�������ӽڵ��index�ֱ�Ϊ2k��2k + 1
	* @param k Ҫ������node��λ�� 1 <= k <= size
	*/
	void sink(int k) {
		//������Ǹ��ӽڵ�Ҫ����, ��Ȼ�Ļ����Ѿ�����heap��bottom��
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