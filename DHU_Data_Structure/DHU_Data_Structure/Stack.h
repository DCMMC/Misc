#pragma once

#include <exception>
#include <string>

//fuck C++98 DHU OJ
#ifndef SSTR_FLAG
#define SSTR_FLAG
#include <sstream>
//std::dec sets the basefield format flag for the str stream to decimal(十进制)
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif // !SSTR_FLAG
/**
* 异常类
* 表示空栈异常
*/
class NullStackException : public std::exception
{
	std::string _msg;
public:
	NullStackException(const std::string& msg)
		: _msg(msg)
	{

	}

	virtual const char* what() const noexcept override
	{
		return _msg.c_str();
	}
};

/**
* class comment: Template Stack using SingleLinkedList (LIFO)
* 顺便弱弱的问一下正在审阅代码的TA或老师(如果看到的话=,=): 这些数据结构的类可以不可以不按照给的推荐原型?
* 用我自己写的原型行吗, 就像这个Stack的实现一样:)
* P.S. 我可能在源码里面的注释中的有些术语描述得有点不像C++的风格, 因为我大一学的是Java
* @author DCMMC
* created by DCMMC on 2017/10/26
* finished on 2017/10/27
*/
template<class T>
class Stack {
private:
	/* Inner Class */
	template<class Item>
	class _Node {
	public:
		Item item;
		//一定要记得初始化为null 
		_Node *next = nullptr;

		//constructors
		_Node() {

		}

		_Node(Item item) {
			this->item = item;
		}

		~_Node() {
			// Don't try to second-guess the user and do something they might not expect.
			//delete this->item;
		}
	};

	/**
	* iterator 迭代器
	*/
	template<class Item>
	//struct class都可以用于嵌套类
	struct _Stack_iterator {
	public:
		//标准声明
		typedef _Stack_iterator<Item> iterator_type;
		typedef _Stack_iterator<const Item> const_iterator_type;
		//声明迭代器种类
		typedef std::forward_iterator_tag iterator_category;
		//声明类型, 如果有pointer或者reference都要像value_type这样声明
		typedef Item value_type;

		_Node<Item> *node;

		/* constructors */
		_Stack_iterator() {

		}

		/**
		* @param node 栈顶的node
		*/
		_Stack_iterator(_Node<Item> *node) {
			this->node = node;
		}

		~_Stack_iterator()
		{
			node = nullptr;
		}

		/* overloaded operations*/

		bool operator==(const _Stack_iterator &rsi) {
			return node == rsi.node;
		}

		bool operator!=(const _Stack_iterator &rsi) {
			return node != rsi.node;
		}

		Item operator*() {
			return node->item;
		}

		Item* operator->() {
			return &(operator*());
		}

		_Stack_iterator operator++() {
			node = node->next;

			return *this;
		}
	};

	/**
	* 判断ostream中是否有符合Arg的<<重载操作符
	*/
	template<typename Arg>
	struct has_operaotor
	{
	private:
		template<typename U, U> struct SFINAE {};
		template<typename A>
		static char check(SFINAE<std::ostream& (std::ostream::*)(A), std::ostream::operator<<>*);
		template<typename A> static int check(...);
	public:
		static
			constexpr bool value = sizeof(check<Arg>(0)) == sizeof(char);
	};

	/* Fields */

	//记录当前Stack中的元素个数
	int size = 0;

	//头结点, 也就是栈顶, 初始化为null
	_Node<T> *head;

	//尾巴结点, 也就是栈底
	_Node<T> *tail;

public:
	//using类似于typedef
	using iterator = _Stack_iterator<T>;
	using const_iterator = _Stack_iterator<const T>;

	/* Constructors */
	//default
	Stack() {

	}

	//析构器
	~Stack()
	{
		/*
		typename Stack<T>::_Node<T> *tmp;

		//FIXME
		// 可能是程序逻辑的问题, 反正就是 head会有一大堆的0xCCCCCCCC这种被VS初始化的东西, 所以迫不得已必须加一个额外的检查
		while (head && head != nullptr && (unsigned) head != 0xCCCCCCCC) {
		tmp = head;
		head = head->next;
		delete tmp;
		}

		size = 0;
		head = nullptr;
		tail = nullptr;
		*/
	}

	/* public functions */

	/**
	* 入栈
	* @param item 新元素
	*/
	void push(T item) {
		if (size == 0) {
			//如果LinkedList还是空的
			head = new _Node<T>(item);
			tail = head;
		}
		else {
			_Node<T> *tmpHead = new _Node<T>(item);
			tmpHead->next = head;
			head = tmpHead;
		}

		size++;
	}

	/**
	* 出栈
	* @return 栈顶元素
	* @throw 如果是栈空的时候还要强行pop, 就抛出该异常
	*/
	T pop() {
		if (isEmpty())
			throw NullStackException("栈为空!");

		T tmp = head->item;

		_Node<T> *oldHead = head;

		head = head->next;

		delete oldHead;

		size--;

		return tmp;
	}

	/**
	* 从Stack中取出最后一个添加到Stack的元素, 并且不会把这个元素从Stack中删除, 这里会把元素强制向下转型
	* @return 最后一个添加到Stack的元素
	*/
	T peek() const {
		if (isEmpty())
			throw new NullStackException("栈为空!");

		return head->item;
	}

	/**
	* 获得当前Stack存储了多少个元素
	* @return 当前Stack存储的多少个元素
	*/
	int getSize() const {
		return size;
	}

	/**
	* 判断Stack是否已满, SLL的实现方案直接都返回true
	* @return 判断Stack是否已满
	*/
	bool isFull() const {
		return false;
	}

	/**
	* 判断栈是否为空
	* @return true, 如果栈为空, 反之返回false
	*/
	bool isEmpty() const {
		return size == 0 || head == nullptr || tail == nullptr;
	}

	/**
	* 获得泛型T的类型名, 然而这个是个implemention-depend特性, Microsoft Visual C++ uses human-readable strings; GCC does not.
	*/
	const char * getTypeName() {
		return typeid(T).name();
	}

	/* iterator */

	iterator begin() {
		return iterator(head);
	}

	iterator end() {
		//到了空就是结束了
		return nullptr;
	}
};