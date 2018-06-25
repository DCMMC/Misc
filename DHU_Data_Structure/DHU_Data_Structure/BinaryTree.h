#pragma once

#include <vector>         // std::vector
#include <functional>     // std::greater
#include <queue>
#include <stack>
#ifndef _Binary_Tree_
#define _Binary_Tree
#include "Queue.h"
#include <algorithm>

using namespace std;

/**
* Binary Tree implemented by 三叉链表
* @author DCMMC
* Create on 2017/11/25
*/

///////////// TODO: 加入线索化相关的函数之后, 原来的部分函数会出现问题!!! 还没一个一个去测试 /////////////

/**
* 异常类
* 表示空树异常
*/
class NullTreeException : public std::exception
{
	std::string _msg;
public:
	NullTreeException(const std::string& msg)
		: _msg(msg)
	{

	}

	virtual const char* what() const noexcept override
	{
		return _msg.c_str();
	}
};

//记录个数
int tmpCnt = 0;
template <class T>
bool countNodeOp(T t) {
	tmpCnt++;
	return true;
}

//哈夫曼结点中的结构体: 元素和对应的权重和频率
template<typename E>
class HuffmanItem {
public:
	E value;
	int weight;

	HuffmanItem() {

	}

	HuffmanItem(E value, int weigh) {
		this->value = value;
		this->weight = weigh;
	}

	friend ostream& operator<<(ostream& out, const HuffmanItem& s) {
		out << s.weight;
		return out;
	}
};

template<class T>
class BinaryTree {
private:	
	/* Inner Class */
	template<class Item>
	class _Node {
	public:
		Item item;
		_Node *leftChild = nullptr, *rightChild = nullptr, *parent = nullptr;
		//LTag和RTag分别表示左右结点是否表示前驱或后继
		bool LTag = false, RTag = false;

		//constructors
		_Node() {

		}

		_Node(Item item, _Node<Item> *leftChild = nullptr, _Node<Item> *rightChild = nullptr) {
			this->item = item;
			this->leftChild = leftChild;
			this->rightChild = rightChild;
		}

		~_Node() {
			leftChild = nullptr;
			rightChild = nullptr;
			parent = nullptr;
		}

	};

	
	/* Fields */
	_Node<T> *treeRoot = nullptr;
	unsigned size = 0;
	//用于generateBinaryTreeByPreOrderTraverseResult的index, FIXME, 不安全的操作
	int index = 0;
	//记录下用来表示null的特殊值, 不稳妥的操作 **FIXME**
	const T *nullPointerNotationPtr = NULL;

	/**
	* 递归生成BinaryTree, 参数为当前要加入的结点的数据的数组和下标
	* 并且数组是一棵树的先序遍历结果(并且是遍历所有的指针域的, 包括空链域, 其中空链域用某个特殊符号表示, 也算是一颗完全二叉树)
	*/
	void generateBinaryTreeByPreOrderTraverseResult(_Node<T> *parent, _Node<T> **node, const T *items, 
			const T &nullPointerNotation) {
		if (nullPointerNotation == items[index]) {
			*node = nullptr;
			index++;
		}
		else {
			size++;
			*node = new _Node<T>(items[index++]);
			(*node)->parent = parent;
			//递归(先序遍历)
			generateBinaryTreeByPreOrderTraverseResult(*node, &((*node)->leftChild), items, nullPointerNotation);
			generateBinaryTreeByPreOrderTraverseResult(*node, &((*node)->rightChild), items, nullPointerNotation);
		}
	}

	/* 递归生成假想完全二叉树的先序遍历序列 */
	void generatePreOrderCompleteSequence(const _Node<T> *node, vector<T>& result, const T &notation) {
		if (node) {
			result.push_back(node->item);
			generatePreOrderCompleteSequence(node->leftChild, result, notation);
			generatePreOrderCompleteSequence(node->rightChild, result, notation);
			return;
		}

		result.push_back(notation);
		return;
	}

	/* 复制树 */
	///////////////////////// TODO /////////////////////

	/**
	* 递归先序遍历操作
	*/
	bool preOrderRecursiveTraverseOp(_Node<T> *node, bool (*op) (T elem)) {
		if (node) {
			if (op(node->item))
				if (preOrderRecursiveTraverseOp(node->leftChild, op))
					if (preOrderRecursiveTraverseOp(node->rightChild, op))
						return true;

			return false;
		}

		return true;
	}

	/**
	* 递归中序遍历操作
	*/
	bool inOrderRecursiveTraverseOp(_Node<T> *node, bool(*op) (T elem)) {
		if (node) {
			if (inOrderRecursiveTraverseOp(node->leftChild, op))
				if (op(node->item))
					if (inOrderRecursiveTraverseOp(node->rightChild, op))
						return true;
			return false;
		}

		return true;
	}

	/**
	* 递归后序遍历操作
	*/
	bool sufOrderRecursiveTraverseOp(_Node<T> *node, bool(*op) (T elem)) {
		if (node) {
			if (sufOrderRecursiveTraverseOp(node->leftChild, op))
				if (sufOrderRecursiveTraverseOp(node->rightChild, op))
					if (op(node->item))
						return true;

			return false;
		}

		return true;
	}

	/* 使用参数中的值来找到值相同的第一个被后序遍历到的结点的指针 */
	_Node<T> *findNode(const T elem) const {
		return findNode(elem, treeRoot);
	}

	_Node<T> *findNode(const T elem, _Node<T> *node) const {
		if (node) {
			if (node->item == elem) {
				return node;
			}

			_Node<T> *tmp;
			return (tmp = (node->LTag ? NULL : findNode(elem, node->leftChild))) 
				? tmp : (node->RTag ? NULL : findNode(elem, node->rightChild));
		}
		
		return nullptr;
	}

	/* 后序遍历析构 */
	bool destorySubTree(_Node<T> **treeNode) {
		if (*treeNode) {
			try {
				destorySubTree(&((*treeNode)->leftChild));
				destorySubTree(&((*treeNode)->rightChild));
				//(*treeNode)->~_Node();
				delete (*treeNode);
				*treeNode = nullptr;
				size--;
			}
			catch (...) {
				return false;
			}
			return true;
		}

		return true;
	}

	/* 检验一个结点是否在当前(子)树里面 */
	bool verify(const _Node<T> *node, const _Node<T> *subTreeRoot) const {
		return (node && subTreeRoot) ?
			(node == subTreeRoot || verify(node, subTreeRoot->leftChild) || verify(node, subTreeRoot->rightChild))
			: false;
	}

	/* 
	* 在node下面插入子树, 参数node必须是当前树中的一个结点, flag为0表示插入为左子树, 为1表示插入为右子树
	* 原有的oldSubTree会变成subTree的右子树(FIXME: 我猜是最后一层最右边的节点的右子树)
	*/
	bool insertSubTree(_Node<T> *node, const bool flag, BinaryTree<T> &subTree) {
		if (verify(node, treeRoot)) {
			_Node<T> *oldSubTreeRoot = (flag ? node->rightChild : node->leftChild);

			if (flag)
				node->rightChild = subTree.getRootNode();
			else
				node->leftChild = subTree.getRootNode();
			size += subTree.getSize();

			if (oldSubTreeRoot) {
				_Node<T> *tmp = getRightmorstNode(subTree.getRootNode());
				tmp->rightChild = oldSubTreeRoot;
				oldSubTreeRoot->parent = tmp;
			}
			
			return true;
		}

		return false;
	}

	/* 获取树的最底下一层的最右边的结点, 基于层次遍历 */
	_Node<T> *getLastLevelRightNode(_Node<T> *node) {
		Queue<_Node<T> *> queue;
		queue.enqueue(node);
		_Node<T> *tmp = nullptr;

		while (!queue.isEmpty()) {
			tmp = queue.dequeue();

			if (tmp->leftChild)
				queue.enqueue(tmp->leftChild);
			if (tmp->rightChild)
				queue.enqueue(tmp->rightChild);
		}

		return tmp;
	}

	/* 递归获取整棵树最右边的结点 */
	_Node<T> *getRightmorstNode(_Node<T> *node) {
		if (node->rightChild)
			return getRightmorstNode(node->rightChild);
		else
			return node;
	}

	/* 统计二叉(子)树的结点个数 */
	int nodeCount(_Node<T> *subTreeRoot) {
		tmpCnt = 0;
		preOrderRecursiveTraverseOp(subTreeRoot, countNodeOp<T>);
		return tmpCnt;
	}

	/**
	* 按照 Ex18 的参考代码, 不过我觉得toString中实现的版本更加好
	* 采用中序遍历, 递归部分
	* @param node 当前结点
	*/
	void printBinaryTree(const _Node<T> *node, int i) const {
		if (node) {
			if (node->rightChild)
				printBinaryTree(node->rightChild, i + 1);

			for (int j = 0; j < i; j++)
				cout << "  ";

			cout << node->item << " " << endl;

			if (node->leftChild)
				printBinaryTree(node->leftChild, i + 1);
		}
	}

	//按树状打印建立起的哈夫曼树
	template<typename E>
	void printHuffmanTree(const _Node< HuffmanItem<E> > *node, int i) const {
		if (node) {
			if (node->rightChild)
				printHuffmanTree(node->rightChild, i + 1);

			for (int j = 0; j < i; j++)
				cout << "   ";

			cout << node->item.weight << "  " << endl;

			if (!node->leftChild && !node->rightChild) {
				for (int j = 0; j < i; j++)
					cout << "   ";

				cout << "(" << node->item.value << ")" << endl;
			}

			if (node->leftChild)
				printHuffmanTree(node->leftChild, i + 1);
		}
	}

	//先序递归遍历返回结点下的叶子结点的个数
	int countLeapNode(const _Node<T> *node) const {
		return node 
			? ( (!(node->leftChild) && !(node->rightChild)) 
				? 1 
				: countLeapNode(node->leftChild) + countLeapNode(node->rightChild) )
			: 0;
	}

	//判断结点是否是叶结点
	bool isLeap(_Node<T> *node) {
		return !(node->leftChild || node->rightChild);
	}

	//头递归交换树的所有左右结点
	void revoluteLeftAndRight(_Node<T> *node) {
		//递归的边界条件就是叶结点或者空结点
		if (node && !(isLeap(node))) {
			revoluteLeftAndRight(node->leftChild);
			revoluteLeftAndRight(node->rightChild);

			_Node<T> *tmp = node->leftChild;
			node->leftChild = node->rightChild;
			node->rightChild = tmp;
		}
	}

	//建立中序线索二叉树
	void generateInfixThreadedTree(_Node<T> *node, _Node<T> **pre) {
		if (node) {
			generateInfixThreadedTree(node->leftChild, pre);
			if (*pre) {
				if (!((*pre)->rightChild)) {
					(*pre)->RTag = true;
					(*pre)->rightChild = node;
				}

				if (!node->leftChild) {
					node->LTag = true;
					node->leftChild = *pre;
				}
			}
			*pre = node;
			generateInfixThreadedTree(node->rightChild, pre);
		}
	}

	/***************************** public ************************************/
public:

	/* Constructors */
	/* generate an empty tree */
	BinaryTree()  { }


	/**
	* 从参数数组中创建一颗二叉树, 并且是把参数当作二叉树先序遍历的结果来创建. nullPointerNotation表示空链域
	*/
	BinaryTree(const T *preOrderTraverseElements, const T &nullPointerNotation) 
		: nullPointerNotationPtr(new T(nullPointerNotation)) {
		generateBinaryTreeByPreOrderTraverseResult(nullptr, &treeRoot, preOrderTraverseElements, nullPointerNotation);
	}

	/* 从另外一颗二叉树拷贝一棵新的二叉树 */
	// C++ 11 Delegation, 必须要调用其他构造器, 在函数里面调用是局部变量...
	BinaryTree(BinaryTree &tree) : BinaryTree(tree.preOrderCompleteSequence(*(tree.getNullPointerNotation())),
		*(tree.getNullPointerNotation())) {
		
	}

	~BinaryTree()
	{
		//debug
		//cout << "debug...\n";
		
		destorySubTree(&treeRoot);
	}

	/* Methods */
	/* 递归先序遍历, 自定义操作 */
	void preOrderRecursiveTraverseOp(bool (*operate) (T elem)) {
		preOrderRecursiveTraverseOp(treeRoot, operate);
	}

	/* 递归中序遍历, 自定义操作 */
	void inOrderRecursiveTraverseOp(bool(*op) (T elem)) {
		inOrderRecursiveTraverseOp(treeRoot, op);
	}

	/* 递归后序遍历, 自定义操作 */
	void sufOrderRecursiveTraverseOp(bool(*op)(T elem)) {
		sufOrderRecursiveTraverseOp(treeRoot, op);
	}

	/* 非递归先序遍历 */
	vector<T> preOrderTraverseOp(bool(*operate) (T elem)) {
		_Node<T> *p = treeRoot;
		Stack<_Node<T> *> s;
		vector<T> vec;

		while (p || !s.isEmpty()) {
			if (p) {
				operate(p->item);
				vec.push_back(p->item);
				s.push(p);
				p = p->leftChild;
			}
			else {
				p = s.pop()->rightChild;
			}
		}

		return vec;
	}

	/* 非递归中序遍历 */
	vector<T> inOrderTraverseOp(bool(*operate) (T elem)) {
		_Node<T> *p = treeRoot;
		Stack<_Node<T> *> s;
		vector<T> vec;

		while (p || !s.isEmpty()) {
			if (p) {
				s.push(p);
				p = p->leftChild;
			}
			else {
				p = s.pop();
				operate(p->item);
				vec.push_back(p->item);
				p = p->rightChild;
			}
		}

		return vec;
	}

	/**
	* 非递归后序遍历
	* 原理是: 用栈存储, 先遍历到最左结点的最右结点, 然后访问它, 如果它是父结点的左子结点, 就继续遍历右子树的最左
	* 结点的最右结点, 否则访问其父结点
	*/
	vector<T> sufOrderTraverseOp(bool(*operate) (T elem)) {
		_Node<T> *p = treeRoot;
		Stack<_Node<T> *> s;
		vector<T> vec;

		while (p || !s.isEmpty()) {
			//遍历到最左结点或者最右结点
			while (p->leftChild || p->rightChild) {
				s.push(p);
				if (p->leftChild)
					p = p->leftChild;
				else
					s.pop();
				while (!(p->leftChild) && p->rightChild) {
					s.push(p);
					p = p->rightChild;
				}
			}

			//op
			if (!operate(p->item))
				return vector<T>();
			vec.push_back(p->item);

			//栈为空就跳过下面
			if (s.isEmpty()) {
				p = NULL;
				continue;
			}

			if (p == s.peek()->leftChild) {
				//进入最近一个祖先结点的右子结点
				while (!s.isEmpty() && !(s.peek()->rightChild)) {
					vec.push_back(p->item);
					//p = s.peek() 不作为条件, 只是放在这里可以较少点代码行数
					if ( (p = s.peek()) && !operate(s.pop()->item))
						return vector<T>();
				}
			}
			else {
				//访问到最近的拐过弯的祖先结点的右子结点
				//是否没有拐过弯?
				bool flag = true;

				while (!s.isEmpty() && (flag || !(s.peek()->rightChild) || p == s.peek()->rightChild)) {
					if (flag) {
						_Node<T> *tmp = s.pop();
						if ( !s.isEmpty() && (s.peek()->leftChild == tmp) && (tmp->rightChild == p))
							flag = false;
						
						s.push(tmp);
					}
						
					vec.push_back(p->item);
					//p = s.peek() 不作为条件, 只是放在这里可以较少点代码行数
					if ((p = s.peek()) && !operate(s.pop()->item))
						return vector<T>();
				}

			}
			
			p = s.isEmpty() ? NULL : s.peek()->rightChild;
		}

		return vec;
	}

	/* 生成假想拓展为完全二叉树的前序遍历序列, notation为特殊的空链域标志 */
	vector<T> preOrderCompleteSequence(const T &notation) {
		vector<T>& result = * (new vector<T>());

		generatePreOrderCompleteSequence(treeRoot, result, notation);

		return result;
	}

	/* 获取用来表示null的特殊值 */
	const T * getNullPointerNotation() const {
		return nullPointerNotationPtr;
	}

	/* 获取根结点中的Item */
	T getRootNodeItem() const {
		if (treeRoot == nullptr)
			throw new NullTreeException("This Tree is Null!");

		return treeRoot->item;
	}

	/* 获取树的元素个数 */
	int getSize() const {
		return size;
	}

	/* 使用结点中存储的信息来找到parent */
	T * getParentItem(const T x) const {
		_Node<T> *node = findNode(x);

		return node ? &(node->parent->item) : nullptr;
	}

	/* 使用队列实现的层次遍历 */
	void layerTraverse(bool (* op) (T elem)) {
		Queue<_Node<T> *> queue;
		queue.enqueue(treeRoot);
		_Node<T> *tmp = nullptr;

		while (!queue.isEmpty()) {
			if (!op((tmp = queue.dequeue())->item))
				return;
			
			if (tmp->leftChild) 
				queue.enqueue(tmp->leftChild);
			if (tmp->rightChild)
				queue.enqueue(tmp->rightChild);
		}
	}

	/* 获得x所在的结点的高度 */
	int getBinaryTreeHeight(T x, int flag = -1) {
		_Node<T> *node = findNode(x);
		if (!node)
			return 0;

		if (flag > -1)
			node = (0 == flag ? node->rightChild : node->leftChild);

		return getBinaryTreeHeight_Cursive(node);
	}

	//递归获取高度
	int getBinaryTreeHeight_Cursive(_Node<T> *node) {
		if (node == NULL)
			return 0;

		if ( (node->leftChild == NULL || node->LTag) && (node->rightChild == NULL || node->RTag))
			return 1;

		return std::max<int>(getBinaryTreeHeight_Cursive(node->leftChild), 
			getBinaryTreeHeight_Cursive(node->rightChild)) + 1;
	}

	/**
	* 按照树的结构来打印
	*/
	/////////////////////////////TODO/////////////////////
	string toString() {
		return "";
	}

	/**
	* 获取根结点到值为t的节点的路径, (中序遍历)深度优先搜索
	*/
	vector<T> * getPath(_Node<T> *root, const T t) const {
		Stack<_Node<T> *> s;
		_Node<T> *node = root;

		while (node || !s.isEmpty()) {
			//先遍历到最左边的结点
			while (node) {
				if (node->item == t) {
					//stack to vector
					T *pathArray = new T[s.getSize()];
					const int len = s.getSize();
					int i = 1;
					while (!s.isEmpty()) {
						pathArray[len - (i++)] = s.pop()->item;
					}

					vector<char> *vec = new vector<T>();
					vec->assign(pathArray, pathArray + len);
					return vec;
				}

				//node不是目标值, 先存进Stack
				s.push(node);
				node = node->leftChild;
			}

			//node已经是空的了, 从父节点的右子结点开始
			if (!s.isEmpty()) {
				node = s.pop();
				if (node->rightChild) {
					s.push(node);
					node = node->rightChild;
				}
				else if (s.peek()->leftChild == node) {
					//如果node是父节点的左孩子, 就回溯到上面最近的有右孩子的结点的右子结点
					while (s.peek()->rightChild == NULL) {
						s.pop();
					}
					node = s.peek()->rightChild;
				}
				else {
					//如果node是父结点的右孩子, 就回溯到上面最近的一个是父结点的左孩子的结点
					while (s.peek()->leftChild != node) {
						node = s.pop();
					}
					node = NULL;
				}

				/*
				node = s.peek()->rightChild;
				while (!node) {
					//如果rightChild也是null的话, 就把它的父节点(也就是栈顶元素)pop掉
					s.pop();
					node = s.peek()->rightChild;
				}
				*/
			}
		}

		// not found
		return new vector<T>();
	}

	//判断是否是相同的两颗树
	bool operator==(BinaryTree<T>& rhs)
	{
		vector<T> arr0 = preOrderCompleteSequence(*(getNullPointerNotation())),
			arr1 = rhs.preOrderCompleteSequence(*(rhs.getNullPointerNotation()));

		if (arr0.size() != arr1.size())
			return false;

		for (int i = 0; i < arr0.size(); i++) {
			if (arr0[i] != arr1[i])
				return false;
		}

		return true;
	}

	template<typename E>
	struct NodeAndIndex {
		_Node<E> *node;
		int index;
	};

	/**
	* 判断是否是完全二叉树
	* 基本思路: 层次遍历二叉树, 并且对每个遍历到的元素依次从1开始编号, 
	* 并且判断结点的左子结点的编号是否是父结点的编号的两倍
	*/
	bool isCompleteTree() const {
		//类似于层次遍历
		Queue< NodeAndIndex<T> > q = Queue< NodeAndIndex<T> >();
		//init list in c++ 11
		q.enqueue(NodeAndIndex<T>{ treeRoot, 1 });

		_Node<T> *tmp = NULL;
		int indexParent = 0;
		int cnt = 1;

		while (!q.isEmpty()) {
			indexParent = q.peek().index;
			tmp = q.dequeue().node;

			if (tmp->leftChild) {
				if (++cnt != 2 * indexParent)
					return false;
				q.enqueue(NodeAndIndex<T>{ tmp->leftChild, cnt });
			}

			if (tmp->rightChild)
				q.enqueue(NodeAndIndex<T>{ tmp->rightChild, ++cnt });
		}

		return true;
	}

	//叶结点的个数
	int countLeapNode() const {
		return countLeapNode(treeRoot);
	}

	//交换树的所有左右结点
	void binaryTree_Revolute() {
		revoluteLeftAndRight(treeRoot);
	}

	//查找距离两个结点最近的共同祖先
	//对于二叉链表实现的二叉树, 可以通过两个结点的路径的最后一个共同元素来判断
	T * findNearAncient(T &x, T &y) const {
		vector<T> vecX = getPath(x),
			vecY = getPath(y);

		T *result = NULL;
		int minSize = (vecX.size() < vecY.size() ? vecX.size() : vecY.size());
		for (int i = 0; i < minSize; i++) {
			if (vecX[i] != vecY[i])
				break;
			result = &(vecX[i]);
		}

		//result里面的东西莫名其妙被销毁了, 因为是局部变量吧...
		if (result) {
			char * ch = new char(*result);
			result = ch;
		}

		return result;
	}



	
	////////// 比较困难 //////////
	//从中缀式四则算术表达式字符串生成表达式二叉树

	//预处理算术表达式字符串
	vector<string> parseArithmicExp(string exp) {
		vector<string> result;
		int begin = -1, index = -1;
		for (char c : exp) {
			++index;
			if (c < '0' || c > '9') {
				if (begin != -1)
				{
					result.push_back(exp.substr(begin, index - begin));
					begin = -1;
				}
				result.push_back(string(1, c));
			}
			else if (begin == -1)
				begin = index;
		}

		if (begin != -1)
			result.push_back(exp.substr(begin, ++index - begin));

		return result;
	}

	//表达式二叉树中序递归带括号
	void infix_recursive_parentheses() const {
		infix_recursive_parentheses(treeRoot);
	}

	void infix_recursive_parentheses(const _Node<T> *node) const {
		if (node) {
			if ((node->item == "*" || node->item == "/") && 
				(node->leftChild->item == "+" || node->leftChild->item == "-")) {
				cout << "(";
				infix_recursive_parentheses(node->leftChild);
				cout << ")";
				cout << node->item;
				infix_recursive_parentheses(node->rightChild);
			}
			else if ((node->item == "*" || node->item == "/") &&
				(node->rightChild->item == "+" || node->rightChild->item == "-")) {
				infix_recursive_parentheses(node->leftChild);
				cout << node->item;
				cout << "(";
				infix_recursive_parentheses(node->rightChild);
				cout << ")";
			}
			else
			{
				infix_recursive_parentheses(node->leftChild);
				cout << node->item;
				infix_recursive_parentheses(node->rightChild);
			}
		}
	}

	//辅助函数--生成Node
	inline void processNode(Stack<_Node<T> *> &expStack, Stack<string> &ops) {
		_Node<T> *newNode = new _Node<T>(ops.pop());
		newNode->rightChild = expStack.pop();
		newNode->rightChild->parent = newNode;
		if (!expStack.isEmpty())
		{
			newNode->leftChild = expStack.pop();
			newNode->leftChild->parent = newNode;
		}

		expStack.push(newNode);
	}

	void expBinaryTreeFromInffix() {
		//reset
		treeRoot = NULL;

		string exp;
		getline(cin, exp);
		vector<string> expVector = parseArithmicExp(exp);
		
		//双栈
		//用来存放 + - (
		Stack<string> ops;
		//用来存放(子)树的根节点
		Stack<_Node<T> *> expStack;

		for (string element : expVector) {
			if (element == "(" || element == "*" || element == "/")
			{
				ops.push(element);
			}
			else if (element == "+" || element == "-" ) 
			{
				//遇到 + - 就把前面的(优先级高的) * / 合并成一个node并放进expStack
				while (!ops.isEmpty() && (ops.peek() == "*" || ops.peek() == "/")) 
				{
					processNode(expStack, ops);
				}
				
				ops.push(element);
			}
			else if (element == ")") {
				//遇到右括号就把左边对应左括号里面的表达式合并成一个node再放进expStack
				while (ops.peek() != "(") {
					processNode(expStack, ops);
				}
				//去掉这个 (
				ops.pop();
			}
			else
			{
				expStack.push(new _Node<T>(element));
			}
		}

		//逆序一下栈
		Stack<string> opsReserve;
		Stack<_Node<T> *> expReserve;

		for (int i = ops.getSize(); i > 0; i--) 
		{
			opsReserve.push(ops.pop());
		}
		for (int i = expStack.getSize(); i > 0; i--)
		{
			expReserve.push(expStack.pop());
		}

		while (!opsReserve.isEmpty())
		{
			_Node<T> *newNode = new _Node<T>(opsReserve.pop());
			newNode->leftChild = expReserve.pop();
			newNode->leftChild->parent = newNode;
			if (!expReserve.isEmpty())
			{
				newNode->rightChild = expReserve.pop();
				newNode->rightChild->parent = newNode;
			}

			expReserve.push(newNode);
		}
		
		treeRoot = expReserve.peek();
	}

	//从后缀表达式生成表达式二叉树
	void expBinaryTreeFromPostfix() {
		//reset
		treeRoot = NULL;

		string exp;
		getline(cin, exp);
		//cin read from infix
		std::istringstream stream(exp);
		//backup original stream
		streambuf *cin_backup = std::cin.rdbuf(stream.rdbuf());

		//存放结点
		Stack<_Node<T> *> expS;

		string element;

		while (cin >> element) {
			if (element == "+" || element == "-" || element == "*" || element == "/") {
				_Node<T> *newNode = new _Node<T>(element);
				newNode->rightChild = expS.peek();
				expS.pop()->parent = newNode;
				newNode->leftChild = expS.peek();
				expS.pop()->parent = newNode;
				expS.push(newNode);
			}
			else
			{
				expS.push(new _Node<T>(element));
			}
		}

		treeRoot = expS.peek();
	}

	//建立中序线索二叉树
	void generateInfixThreadedTree() {
		_Node<T> *tmp = NULL;
		_Node<T> **pre = &tmp;
		*pre = NULL;
		generateInfixThreadedTree(treeRoot, pre);
		if (*pre) {
			//树的中序遍历的最后一个结点为特殊的结点--RTag为true但是rightChild为NULL
			//用于中序遍历线索二叉树
			(*pre)->RTag = true;
			(*pre)->rightChild = NULL;
		}
	}

	//中序遍历中序线索二叉树
	bool infixTraverseInfixThreadedTree(bool (* op)(T t)) {
		_Node<T> *node = treeRoot;

		while (node && (!node->RTag || node->rightChild)) {
			while (node && !node->LTag && node->leftChild) {
				//访问其左字数为空的结点
				node = node->leftChild;
			}
			if (!op(node->item))
				return false;
			while (node && node->RTag && node->rightChild) {
				//访问后继结点
				node = node->rightChild;
				if (!op(node->item))
					return false;
			}
			node = node->rightChild;
		}

		//最后一个node
		if (node)
			op(node->item);
	}

	//sort
	template<class K>
	struct greater_than_weight
	{
		bool operator() (const _Node< HuffmanItem<K> > *a,
			const _Node< HuffmanItem<K> > *b) const
		{
			return a->item.weight > b->item.weight;
		}
	};

	//构建Huffman Tree
	template<typename E>
	void generateHuffmanTree(const vector<E> &values, const vector<int> &weights) {
		if (values.size() < 1 || values.size() != weights.size()) {
			cerr << "元素个数和对应的频度长度不一致!" << endl;
			//////////////////// TODO: throw an exception ///////////////////
			return;
		}

		//reset
		treeRoot = NULL;

		priority_queue < _Node< HuffmanItem<E> > *, std::vector<_Node< HuffmanItem<E> > *>,
			BinaryTree<T>::greater_than_weight<E> > nodes;
		for (decltype(values.size()) i = 0; i < values.size(); i++) {
			_Node< HuffmanItem<E> > *tmp =
				new _Node< HuffmanItem<E> >(HuffmanItem<E>(values[i], weights[i]));
			nodes.push(tmp);
		}

		while (nodes.size() > 1) {
			_Node< HuffmanItem<E> > *tmp = new _Node< HuffmanItem<E> >();
			tmp->leftChild = nodes.top();
			nodes.top()->parent = tmp;
			nodes.pop();
			tmp->rightChild = nodes.top();
			nodes.top()->parent = tmp;
			nodes.pop();
			tmp->item.weight =
				tmp->leftChild->item.weight + tmp->rightChild->item.weight;
			nodes.push(tmp);
		}

		treeRoot = nodes.top();

		return;
	}

	/**
	* 查找 Huffman 树 结点
	*/
	template<typename E>
	_Node< HuffmanItem<E> > *findHuffmanNode(const E elem) const {
		return findHuffmanNode<E>(elem, treeRoot);
	}

	template<typename E>
	_Node< HuffmanItem<E> > *findHuffmanNode(const E elem, _Node< HuffmanItem<E> > *node) const {
		if (node) {
			if (node->item.value == elem) {
				return node;
			}

			_Node< HuffmanItem<E> > *tmp;
			return (tmp = (node->LTag ? NULL : findHuffmanNode<E>(elem, node->leftChild)))
				? tmp : (node->RTag ? NULL : findHuffmanNode<E>(elem, node->rightChild));
		}

		return nullptr;
	}

	//获得元素值对应的Huffman Code
	template<typename E>
	stack<bool> getHuffmanCode(const E &x) const {
		stack<bool> result;

		_Node< HuffmanItem<E> > *node = findHuffmanNode<E>(x);

		if (node) {
			while (node && node->parent) {
				if (node == node->parent->leftChild)
					result.push(0);
				else
					result.push(1);
				node = node->parent;
			}
		}

		/*
		while (node && node->rightChild && node->rightChild->rightChild) {
			if (node->leftChild->item.value == x) {
				result.push_back(0);
				return result;
			}

			result.push_back(1);
			node = node->rightChild;
		}

		if (x == node->leftChild->item.value)
			result.push_back(0);
		else if (x == node->rightChild->item.value)
			result.push_back(1);
		*/


		return result;
	}


	//按照树状结构打印哈夫曼树
	template<typename E>
	void printHuffmanTree() {
		printHuffmanTree<E>(treeRoot, 0);
	}

	/***************************** Exercises *********************************************/

	/* Ex 6 */
	T *findItem(const T elem) {
		_Node<T> *node = findNode(elem);

		return node ? &(node->item) : NULL;
	}

	//Ex 8
	//flag: 0 左孩子 1 右孩子
	T *getChildItem(T &x, int flag) const {
		_Node<T> *node = findNode(x);

		if (node)
		{
			if (flag == 0)
				return &(node->leftChild->item);
			else if (flag == 1)
				return &(node->rightChild->item);
		}

		return NULL;
	}

	//Ex 9
	//flag: 0 左子树 1 右子树
	bool childTreeDestroy(const T &x, const bool flag)  {
		_Node<T> *node = findNode(x);

		return node ? (flag ? (node->rightChild ? destorySubTree(&(node->rightChild)) : false) 
			: (node->leftChild ? destorySubTree(&(node->leftChild)) : false) ) : false;
	}

	//Ex 10
	bool ex10(BinaryTree<T> &subTree, const T &x, const bool flag) {
		_Node<T> *node = findNode(x);

		return node ? insertSubTree(node, flag, subTree) : false;
	}
	
	/* 不安全的操作 */
	_Node<T> *getRootNode() const {
		return treeRoot;
	}

	//Ex 11
	bool replaceItem(const T &x, const T &newValue) {
		_Node<T> *node = findNode(x);

		if (node)
			node->item = newValue;
		
		return node != NULL;
	}

	//Ex12, flag=1表示取右兄弟, 0表示取左兄弟, 如果没有就返回NULL
	T *get_Sibling(const T &x, const bool flag) const {
		_Node<T> *node = findNode(x);

		//其实本题是想要求学生用递归来找到x对应的结点的父结点

		return node && node != treeRoot ? ( flag ? (node == node->parent->rightChild ? NULL : &(node->parent->rightChild->item))
			: (node == node->parent->leftChild ? NULL : &(node->parent->leftChild->item)) ) : NULL;
	}

	//Ex 15
	int countDegreeTwo() const {
		return countDegreeTwo(treeRoot);
	}

	int countDegreeTwo(_Node<T> *node) const {
		if (node) {
			return node->leftChild && node->rightChild ? (1 + countDegreeTwo(node->leftChild)
				+ countDegreeTwo(node->rightChild)) : (countDegreeTwo(node->leftChild)
					+ countDegreeTwo(node->rightChild));
		}

		return 0;
	}

	//Ex 17
	vector<T> &getPath(T t) const {
		return *getPath(treeRoot, t);
	}

	/**
	* Ex 18 按树状打印二叉树
	*/
	void printBinaryTree() {
		printBinaryTree(treeRoot, 0);
	}

	//Ex 12
	void ex12(char c) const {
		_Node<T> *node = findNode(c);
		if (node) {
			cout << node->item << "\n" << node->leftChild->item << "\n" << node->rightChild->item
				<< endl;
		}
		else
		{
			cout << "NULL" << endl;
		}
	}
};
#endif // !_Binary_Tree_