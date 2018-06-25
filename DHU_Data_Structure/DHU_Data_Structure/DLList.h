#pragma once

#include <iostream>
#include <stdlib.h>
#include <exception>

#ifndef nullptr
#define nullptr 0
#endif

using namespace std;

/**
* 双向链表ADT
* Created on 2017/10/13
* @author DCMMC
*/
template<class Item>
class LinkedList
{
public:
	/* Inner Class -- Node */
	class Node
	{
	public:
		//节点中保存的元素
		Item item;

		//下一个节点, 初始化为null
		Node * next = nullptr;

		//上一个节点
		Node * previous = nullptr;

		Node()
		{

		}

		Node(Item item)
		{
			this->item = item;
		}

		~Node()
		{

		}

		Item setItem(Item& item)
		{
			this->item = item;
			return item;
		}

		Node * setNext(Node * next)
		{
			this->next = next;
			return next;
		}

		Node * setPrevious(Node * previous)
		{
			this->previous = previous;
			return previous;
		}
	};

private:
	//当前List中的元素个数
	int size;

	//头节点也就是最早加入的节点, 初始为null
	Node * first = nullptr;

	//尾节点也就是最晚加入的节点, 初始化为null
	Node * last = nullptr;

	//为ReverseArrayIterator迭代器记录对List的操作次数, 防止在迭代的过程中List被更改
	int opsCnt;

	/**
	* 检查offset是否合法
	* @param offset
	*           要检查的offset
	*/
	void outOfBoundsCheck(int offset) {
		string s1, s2, s3, s4, s5;
		s1 = "序号";
		s2 = offset;
		s3 = "在本List中不存在, 请输入0 ~ ";
		s4 = getSize() - 1;
		s5 = "的数";
		if (offset < 0 || offset >= getSize())
			throw length_error(s1 + s2 + s3 + s4 + s5);
	}

public:
	/**************************************
	* Constructors                       *
	**************************************/
	/**
	* 默认构造器
	*/
	LinkedList()
	{
		//inti non static data member
		size = 0;
	}

	LinkedList(const Item * array)
	{

	}

	/*
	* 析构函数
	*/
	~LinkedList()
	{
		// FIXME 还是有问题 例如Ex9
		/*
		Node * current;
		for (int i = 0; i < getSize(); i++)
		{
			current = first;
			first = first->next;
			delete current;
		}
		*/

	}

	/**************************************
	* Methods                            *
	**************************************/
	/**
	* 获得当前Stack存储了多少个元素
	* @return 当前Stack存储的多少个元素
	*/
	int getSize() {
		return size;
	}

	/**
	* 判断Stack是否是空的
	* @return 判断Stack是否是空的
	*/
	bool isEmpty() {
		return size == 0;
	}

	/**
	* 判断Stack是否已满, resizing-capacity array的实现方案直接都返回true
	* @return 判断Stack是否已满
	*/
	bool isFull() {
		//这是原来fixed-capacity array实现的stack的方案
		//return getSize() >= MAXSIZE;

		//新的resizing-capacity array实现的stack, 直接返回false.
		return false;
	}

	/**
	* 从DoubleLinkedList中的前端添加新的元素(模拟LIFO)
	* @param item 新元素
	*/
	void addFirst(Item item) {
		//如果LinkedList里面还没有任何元素
		if (first == nullptr) {
			last = first = new Node(item);
			size++;
			opsCnt++;
		}
		else {
			Node * tmpFirst = new Node(item);
			tmpFirst->next = this->first;
			this->first->previous = tmpFirst;
			first = tmpFirst;
			size++;
			opsCnt++;
		}
	}

	/**
	* 从DoubleLinkedList中的后端添加新的元素(模拟FIFO)
	* @param item 新元素
	*/
	void addLast(Item item) {
		//如果LinkedList里面还没有任何元素
		if (last == nullptr) {
			last = first = new Node(item);
			size++;
			opsCnt++;
		}
		else {
			Node * tmpLast = new Node(item);
			last->next = tmpLast;
			tmpLast->previous = this->last;
			last = tmpLast;
			size++;
			opsCnt++;
		}
	}

	/**
	* 从给定的offset后面插入指定的值
	* @param offset
	*           在offset后插入新的节点
	* @param item
	*           新的这个节点中Item的值
	* @throws IndexOutOfBoundsException 如果offset不存在就抛出异常
	*/
	void add(int offset, Item item) {
		outOfBoundsCheck(offset);

		int index = 0;
		Node * current = first;
		while (current->next != nullptr) {
			if (index++ == offset) {
				//找到该offset所在的Node
				Node * newNode = new Node(item);
				newNode->previous = current;
				newNode->next = current->next;
				current->next = newNode;
				size++;
				opsCnt++;
			}

			//继续向后遍历
			current = current->next;
		}

		//那就可能是last那个Node
		if (index == offset) {
			//找到该offset所在的Node
			Node * newNode = new Node(item);
			newNode->previous = current;
			newNode->next = current->next;
			current->next = newNode;
			last = newNode;
			size++;
			opsCnt++;
		}

	}

	/**
	* 从任意的offset中获取item, 并把这个item所在的Node从List中删除
	* @param offset
	*           要获取的元素的offset, 0 <= offset <= getSize() - 1
	* @return 要获取的元素
	* @throws IndexOutOfBoundsException 如果offset不存在就抛出异常
	* @throws NoSuchElementException 如果List为空
	*/
	Item pop(int offset)  
	{
		outOfBoundsCheck(offset);
		if (getSize() == 0)
			throw std::logic_error("This DoubleLinkedList is empty!");

		int index = 0;
		Node * current = first;
		while (current->next != nullptr) {
			if (index++ == offset) {
				//如果获取到该offset所在的Node
				//如果是first
				if (current->previous == nullptr) {
					//如果List只有一个元素
					if (first->next == nullptr)
						first = last = nullptr;
					else {
						first = first->next;
						first->previous = nullptr;
					}
				}
				else {
					current->previous->next = current->next;
				}
				size--;
				opsCnt++;
				Item tmpItem = current->item;
				delete current;
				return tmpItem;
			}

			//继续向后遍历
			current = current.next;
		}

		//如果是last(first)
		//记得减回去
		if (index == offset) {
			//如果只有一个元素
			if (getSize() == 1) {
				first = last = nullptr;
			}
			else {
				last = last->previous;
				last->next = nullptr;
			}
			size--;
			opsCnt++;
			Item tmpItem = current->item;
			delete current;
			return tmpItem;
		}

		return nullptr;
	}

	/**
	* 返回List前端的元素, 并把该元素从List中删除.(模拟LIFO)
	* @throws NoSuchElementException
	* if the client attempts to remove an item from an empty list
	* @return List前端第一个元素
	*/
	Item popFirst()  
	{
		if (getSize() == 0)
			throw std::logic_error("This DoubleLinkedList is empty!");

		return pop(0);
	}

	/**
	* 返回List后端的元素, 并把该元素从List中删除.(模拟FIFO)
	* @throws NoSuchElementException
	* if the client attempts to remove an item from an empty list
	* @return List后端最后一个元素
	*/
	Item popLast() 
	{
		if (getSize() == 0)
			throw std::logic_error("This DoubleLinkedList is empty!");

		Item lastItem = last->item;

		//如果只有一个元素
		if (getSize() == 1) {
			first = last = nullptr;
		}
		else {
			last = last->previous;
			delete last->next;
			last->next = nullptr;
		}
		size--;
		opsCnt++;
		return lastItem;
	}

	/**
	* 用List中删除指定offset的元素
	* @offset   要删除的元素的序号
	* @throws IndexOutOfBoundsException 如果offset不存在就抛出异常
	*/
	void remove(int offset)   {
		outOfBoundsCheck(offset);

		pop(offset);
	}


	/**
	* 返回List后端的元素, 并且不会删除这个元素
	* @return List前端第一个元素
	*/
	Item * getFirst() {
		//如果List为空就返回null
		if (first == nullptr)
			return nullptr;

		return &(first->item);
	}

	/**
	* 返回List后端的元素, 并且不会删除这个元素
	* @return List后端最后一个元素
	*/
	Item getLast() {
		//如果List为空就返回null
		if (last == nullptr)
			return nullptr;

		return last->item;
	}

	/**
	* 从任意的offset中获取item, 并且不会删除这个元素
	* @param offset
	*           要获取的元素的offset, 0 <= offset <= getSize() - 1
	* @return 要获取的元素
	* @throws IndexOutOfBoundsException 如果offset不存在就抛出异常
	*/
	Item * get(int offset) {
		outOfBoundsCheck(offset);

		int index = 0;
		Node * current = first;
		while (current->next != nullptr) {
			if (index++ == offset)
				return &(current->item);

			//继续向后遍历
			current = current->next;
		}

		//如果是last
		if (index == offset)
			return &(current->item);

		return nullptr;
	}

	/**
	* 设置头结点(仅用于练习题)
	*/
	Node * setFirstNode(Node * first) {
		this->first = first;
		return this->first;
	}

	/**
	* 设置尾结点(仅用于练习题)
	*/
	Node * setLastNode(Node * last) {
		this->last = last;
		return this->last;
	}

	/**
	* 返回offset指定位置的Node的指针(仅用于练习题)
	* @param Offset 0 <= offset < getSize()
	*/
	Node * getNode(int offset) {
		outOfBoundsCheck(offset);

		int index = 0;
		Node * current = first;
		while (current->next) {
			if (index++ == offset)
				return current;

			//继续向后遍历
			current = current->next;
		}

		//如果是last
		if (index == offset)
			return current;

		//Undefined Case
		return nullptr;
	}

	/*****************一个不安全的操作********************/
	//仅仅用于Exercise 8
	void setSize(const int sz) {
		size = sz;
	}

	/*
	* TODO: 迭代器, 更多的构造函数, toString函数, toArray函数
	*/

	/**
	* 遍历输出
	*/
	void listTraverse() const {
		if (first == nullptr) {
			cout << endl;
			return;
		}

		Node * current = first;
		while (current->next) {
			cout << current->item << " ";
			current = current->next;
		}
		//垃圾OJ， 还要多打一个回车
		cout << current->item << " " << endl;
	}
};
