// DHUDataStructure.cpp: 定义控制台应用程序的入口点。
//

//Windows API 头文件
// #include "stdafx.h"

/**
* 驱动程序
*/

#include <vector>
#include <string>
//Exercise
#include "Exercise-SqList.h"
#include "Exercise-LinkedList.h"
#include "Exercise-SqStack.h"
#include "Exercise-Queue.h"
#include "Exercise-Tree.h"
#include "Exercise-Graph.h"

#include <vector>         // std::vector
#include <functional>     // std::greater
#include <queue>


using namespace std;


/**
* 按照用户输入生成LinkedList对象
*/
//注意这里的&表示传递的是引用而不是值
//返回对象本身, 而不是像非引用返回值那样返回返回值的一个副本
inline SqList<int>& readAndGenerateSqList() {
	int listLen = 0;
	cin >> listLen;

	/*
	bbb = listLen;

	if (bbb != 20) {
	cerr << "\n" << bbb << "\n";
	cerr << "\n\\\\\\\\\\\\\\\\begin\\\\\\\\\\\\\\\\\n";
	string str;
	while (getline(cin, str)) {
	cerr << str << "\n";
	}
	cerr << "\n\\\\\\\\\\\\\\\\end\\\\\\\\\\\\\\\\\n";
	}
	*/


	//跳过回车
	//cin.ignore();

	//调用一般构造器, 也可以是SqList<int> sqList(listLen);
	//注意: 将list声明为静态变量以免因为是局部变量在函数退出的时候被销毁!
	//这里申请的是栈内存, 会自动释放, 用class SqList<int> * pSqList = new SqList(20);申请的是堆内存, 需要手动delete pSqList;来释放 
	//static SqList<int> list = SqList<int>(listLen);
	//注意可能会内存泄漏! 因为我没打算delete23333
	//ListSize设置为默认20
	SqList<int> *list = new SqList<int>(listLen);

	if (listLen == 0) {
		cin.get();
		return *list;
	}

	for (int i = 0; i < listLen; i++) {
		int in = 0;
		cin >> in;
		(*list).SetElem(i + 1, in);
	}

	(*list).SetListLength(listLen);

	(*list).ListTraverse();

	return *list;
}

/**
* 从输入生成Poly_LinkedList
*/
inline LinkedList<CoefficientAndExponent>& readAndGeneratePloyLinkedList() {
	int listLen = 0;
	cin >> listLen;
	//跳过回车
	//cin.ignore();

	LinkedList<CoefficientAndExponent> *list = new LinkedList<CoefficientAndExponent>();

	if (listLen == 0) {
		//get '/n'
		cin.get();
		return *list;
	}

	int tmpNum = 0;
	//读取一行中的数据(空格隔开的), 系数
	for (int i = 0; i < listLen; i++) {
		cin >> tmpNum;

		(*list).addLast(CoefficientAndExponent(tmpNum));
	}

	//读取一行中的数据(空格隔开的), 指数
	LinkedList<CoefficientAndExponent>::Node *node = list->getNode(0);

	for (int i = 0; i < listLen; i++) {
		cin >> tmpNum;

		(node->item).exponent = tmpNum;
		node = node->next;
	}

	return *list;
}

/**
* 读取并生成存放任意大整数的LinkedList
*/
inline LinkedList<int>& readAndGenerateBigInteger(int &bitLength) {
	LinkedList<int> *bigInteger = new LinkedList<int>();

	string value;

	int length = 0;

	//getline(cin, value);
	char ch;

	while ((ch = getchar()) != EOF && ch != (char)26 && ch != '\n') {
		value += (ch == '-' ? "-" : SSTR(ch - '0'));
	}

	if (value[0] == '-') {
		for (int i = value.size() - 1; i >= 1;) {
			//从最后面开始往前遍历
			if (i >= 4) {
				bigInteger->addFirst((value[i] - '0') + (value[i - 1] - '0') * 10 
					+ (value[i - 2] - '0') * 100 + (value[i - 3] - '0') * 1000);
				i -= 4;
				length += 4;
			}
			else {
				//index 从1开始, 因为0是'-'
				int tmp = 0, index = 1;
				while (index <= i) {
					length++;
					tmp = tmp * 10 + (value[index++] - '0');
				}
				bigInteger->addFirst(tmp);
				i = 0;
			}
		}
		bitLength = length;
		bigInteger->addFirst(-1);
	}
	else {
		for (int i = value.size() - 1; i >= 0;) {
			//从最后面开始往前遍历
			if (i >= 4) {
				bigInteger->addFirst((value[i] - '0') + (value[i - 1] - '0') * 10
					+ (value[i - 2] - '0') * 100 + (value[i - 3] - '0') * 1000);
				i -= 4;
				length += 4;
			}
			else {
				int tmp = 0, index = 0;
				while (index <= i) {
					length++;
					tmp = tmp * 10 + (value[index++] - '0');
				}
				bigInteger->addFirst(tmp);
				i = -1;
			}
		}
		bitLength = length;
		bigInteger->addFirst(1);
	}	

	return *bigInteger;
}

/* 生成二叉树 */
BinaryTree<char> &generateBinaryTree(const char nullPointerNotation = '#') {
	string str;
	getline(cin, str, '\n');
	//split by spaces
	istringstream iss(str);
	vector<char> result;
	for (string s; iss >> s; ) {
		result.push_back(s[0]);
	}

	return *(new BinaryTree<char>(&result[0], nullPointerNotation));
}

//输出图的信息
template<typename T, typename E>
void printGraph(AdjListGraph<T, E> &g, const bool printVerNum = false,
		const bool printArcNum = false) {
	vector<T> v = g.getVertexs();
	cout << v[0];
	for (typename vector<T>::iterator itr = v.begin() + 1; itr != v.end(); itr++)
		cout << " " << *itr;
	cout << "\n";
	if (printVerNum)
		cout << g.getVerNum() << "\n";
	if (printArcNum)
		cout << g.getArcNum() << "\n";
	vector< vector< pair<int, E> > > adjLists = g.getAdjLists();
	typename vector<T>::iterator iVer = v.begin();
	for (typename vector< vector< pair<int, E> > >::iterator itr = adjLists.begin(); itr != adjLists.end(); itr++) {
		cout << *iVer;
		iVer++;
		if (g.getGraphTypeName() == "UDN" || g.getGraphTypeName() == "DN") {
			for (typename vector< pair<int, E> >::iterator i = (*itr).begin(); i != (*itr).end(); i++)
				cout << "->" << (*i).first << "(" << (*i).second << ")";
		}
		else
		{
			for (typename vector< pair<int, E> >::iterator i = (*itr).begin(); i != (*itr).end(); i++)
				cout << "->" << (*i).first;
		}
		
		cout << "\n";
	}
}

/**
* 读取并生成邻接表版本的图
*/
template<typename T, typename E = bool>
AdjListGraph<T, E> generateAdjListGraph(const bool isNetwork) {
	string kindStr;
	getline(cin, kindStr);
	AdjListGraph<T, E> g;
	typename AdjListGraph<T, E>::graphTypes kind;
	if (kindStr == "UDG")
		kind = g.UDG;
	else if (kindStr == "DG")
		kind = g.DG;
	else if (kindStr == "UDN")
		kind = g.UDN;
	else if (kindStr == "DN")
		kind = g.DN;
	else
		return g;
	int verSize = 0;
	cin >> verSize;
	vector<T> vers;
	for (int i = 0; i < verSize; i++) {
		T c;
		cin >> c;
		vers.push_back(c);
	}
	int edgeSize = 0;
	cin >> edgeSize;
	vector< pair<int, int> > edges;
	int tail = 0, head = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> tail >> head;
		edges.push_back(pair<int, int>(tail, head));
	}

	if (isNetwork) {
		vector<E> weights;
		E w = 0;
		for (int i = 0; i < edgeSize; i++) {
			cin >> w;
			weights.push_back(w);
		}
		g = AdjListGraph<T, E>(kind, vers, edges, weights);
	}
	else
		g = AdjListGraph<T, E>(kind, vers, edges);

	return g;
}

//输出图的信息
template<typename T, typename E>
void printMatrixGraph(AdjMatrixGraph<T, E> &g, const bool printVerNum = false,
	const bool printArcNum = false) {
	vector<T> v = g.getVers();
	cout << v[0];
	for (typename vector<T>::iterator itr = v.begin() + 1; itr != v.end(); itr++)
		cout << " " << *itr;
	cout << "\n";
	if (printVerNum)
		cout << g.GetVerNum() << "\n";
	if (printArcNum)
		cout << g.GetEdgeNum() << "\n";
	
	vector< vector<E> > edges = g.getAdjMatrix();
	cout << "\n";
	for (typename vector< vector<E> >::iterator i = edges.begin(); i != edges.end(); i++) {
		cout << *((*i).begin());
		for (typename vector<E>::iterator j = (*i).begin() + 1; j != (*i).end(); j++) {
			cout << "\t" << *j;
		}
		cout << "\n";
	}
}

/**
* 读取并生成邻接矩阵版本的图
*/
template<typename T, typename E>
AdjMatrixGraph<T, E> generateAdjMatrixGraph() {
	string kindStr;
	getline(cin, kindStr);
	AdjMatrixGraph<T, E> g;
	int verSize = 0;
	cin >> verSize;
	vector<T> vers;
	for (int i = 0; i < verSize; i++) {
		T c;
		cin >> c;
		vers.push_back(c);
	}

	E noEdgeFlag;
	cin >> noEdgeFlag;

	int edgeSize = 0;
	cin >> edgeSize;
	vector< pair<int, int> > edges;
	int tail = 0, head = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> tail >> head;
		edges.push_back(pair<int, int>(tail, head));
	}

	vector<E> weights;
	E w = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> w;
		weights.push_back(w);
	}
	g = AdjMatrixGraph<T, E>(kindStr, vers.size(), edges.size(), noEdgeFlag, vers, edges, weights);

	return g;
}

int main(void) {
	//只要打印wchar_t字符，均加此语句，至少不会出错，此语句最好在程序初始化处  
	setlocale(LC_ALL, "");
	
	/*
	cout << "顺序表 Driver Client" << endl;

	//调用无参构造器（默认构造器）来创建对象
	SqList<wchar_t> sqList;

	int index = 0;
	while (!sqList.ListFull()) {
		//宽字节前面加前缀Ｌ
		sqList.SetElem(index, L'烫' + index);
		index++;
		sqList.SetListLength(index);
	}

	printf("这个最大容量为%d的顺序表中的元素个数: %d\n", sqList.GetListSize(), sqList.ListLength());

	//打印顺序表的内容
	//ListTraverse函数用的cout有点辣鸡...
	//sqList.ListTraverse();
	wchar_t tmp = sqList.GetElem(1), next = NULL;
	wcout << tmp << L" ";
	while (sqList.NextElem(tmp, next)) {
		wcout << tmp << L" ";
		tmp = next;
		next = NULL;
	}
	cout << endl;

	//Exercise 1
	cout << "Exercise 1" << endl;
	
	SqList<int> list = readAndGenerateSqList();

	//读取逆置位置
	int m = 1;
	cin >> m;

	//输出待处理顺序表的遍历结果
	list.ListTraverse();

	Exchange(list, m);
	//输出逆置结果
	list.ListTraverse();

	// Exercise 2
	cout << "Exercise 2\n";
	list = readAndGenerateSqList();
	SqList<int> list1 = readAndGenerateSqList();
	list.ListTraverse();
	list1.ListTraverse();
	cout << ListCompare(list, list1) << '\n';

	//Exercise 3
	cout << "Exercise 3" << endl;
	list = readAndGenerateSqList();
	list.ListTraverse();
	Purge_Sq(list);
	list.ListTraverse();

	//Exercise 4
	cout << "Exercise 4" << endl;
	//list = readAndGenerateSqList();
	list = readAndGenerateSqList();
	//list1 = readAndGenerateSqList();
	list1 = readAndGenerateSqList();
	list.ListTraverse();
	list1.ListTraverse();
	Union_Sq(list, list1);
	list.ListTraverse();

	//Exercise 5
	cout << "Exercise 5" << endl;
	//list = readAndGenerateSqList();
	list = readAndGenerateSqList();

	list.ListTraverse();
	Purge_Sq_OL(list);
	list.ListTraverse();

	//Exercise 6
	cout << "Exercise 6" << endl;
	//list = readAndGenerateSqList();
	list = readAndGenerateSqList();
	//list1 = readAndGenerateSqList();
	list1 = readAndGenerateSqList();
	SqList<int> list2 = SqList<int>(list.ListLength() < list1.ListLength() ? list.ListLength() : list1.ListLength());
	list.ListTraverse();
	list1.ListTraverse();
	Intersect_Sq_OL_C(list, list1, list2);
	list2.ListTraverse();

	//Ex 7
	cout << "Exercise 7" << endl;
	LinkedList<int> &llist = readAndGenerateLinkedList();
	//读取逆置位置
	m = 1;
	cin >> m;

	llist.listTraverse();
	Exchange_L(llist, m);
	llist.listTraverse();

	//Ex 8
	cout << "Exercise 8" << endl;
	llist = readAndGenerateLinkedList();
	LinkedList<int> &llist2 = readAndGenerateLinkedList();
	//list = readAndGenerateLinkedList();
	llist.listTraverse();
	llist2.listTraverse();
	Linklist_Contact(llist, llist2);
	llist.listTraverse();

	//Ex 9
	cout << "Exercise 9" << endl;
	llist = readAndGenerateLinkedList();
	llist2 = readAndGenerateLinkedList();
	llist.listTraverse();
	llist2.listTraverse();
	Merge_L(llist, llist2);
	llist.listTraverse();
	
	//Ex 10
	cout << "Exercise 10" << endl;
	llist = readAndGenerateLinkedList();
	llist2 = readAndGenerateLinkedList();
	llist.listTraverse();
	llist2.listTraverse();
	Merge_L_Order(llist, llist2);
	llist.listTraverse();

	//Ex 11
	cout << "Exercise 11" << endl;
	llist = readAndGenerateLinkedList();
	llist.listTraverse();
	Purge_Lk_OL(llist);
	llist.listTraverse();

	//Ex 12
	cout << "Exercise 12" << endl;
	LinkedList<int> llist = readAndGenerateLinkedList(); 
	LinkedList<int> llist2 = readAndGenerateLinkedList();
	llist.listTraverse();
	llist2.listTraverse();
	Merge_L_Pure_Order(llist, llist2);
	llist.listTraverse();
	
	//Ex 13
	cout << "Exercise 13" << endl;
	llist = readAndGenerateLinkedList();
	llist2 = readAndGenerateLinkedList();
	llist.listTraverse();
	llist2.listTraverse();
	Merge_Cur_Linklist(llist, llist2);
	traversePseudoCircleLinkedList(llist);

	//Ex 14
	cout << "Exercise 14" << endl;
	//read operate
	int operate = 0;
	cin >> operate;
	//ignore enter
	//cin.get();

	LinkedList<CoefficientAndExponent> &dul_llist = readAndGeneratePloyLinkedList(),
		&dul_llist2 = readAndGeneratePloyLinkedList();
	Add_Poly(dul_llist, dul_llist2, operate);

	//Ex 15
	cout << "Exercise 15" << endl;
	int length0, length1;
	LinkedList<int> operand0 = readAndGenerateBigInteger(length0), operand1 = readAndGenerateBigInteger(length1);
	string result;

	char tmpCharArr[5] = { 0 };

	//print
	if (operand0.getFirst() == -1) {
		cout << "-";	
	}
	
	LinkedList<int>::Node *node1 = operand0.getNode(1);

	cout << node1->item << " ";
	node1 = node1->next;

	if (node1) {
		while (node1->next) {
			sprintf_s(tmpCharArr, "%04d", node1->item);
			cout << tmpCharArr << " ";
			node1 = node1->next;
		}
		sprintf_s(tmpCharArr, "%04d", node1->item);
		cout << tmpCharArr << " " << endl;
	}
	else
		cout << endl;

	if (operand1.getFirst() == -1) {
		cout << "-";
	}
	
	node1 = operand1.getNode(1);

	cout << node1->item << " ";
	node1 = node1->next;

	if (node1) {
		while (node1->next) {
			sprintf_s(tmpCharArr, "%04d", node1->item);
			cout << tmpCharArr << " ";
			node1 = node1->next;
		}

		sprintf_s(tmpCharArr, "%04d", node1->item);
		cout << tmpCharArr << " " << endl;
	}
	else
		cout << endl;

	Long_Int_Add(operand0, operand1, result, length0, length1);
	cout << result << endl;
	
	//Ex 16
	cout << "Exercise 16" << endl;
	LinkedList<typename Person> llist;

	int listLen = 0;
	cin >> listLen;

	int tmpNum = 0;
	//读取一行中的数据(空格隔开的)
	for (int i = 1; i <= listLen; i++) {
		cin >> tmpNum;
		llist.addLast(::Person(i, tmpNum));
	}

	int m = 0;
	cin >> m;

	//print
	LinkedList<typename Person>::Node *node = llist.getNode(0);

	while (node) {
		cout << node->item.index << " " << node->item.key << "\n";
		node = node->next;
	}

	cout << endl;

	joseph(llist, m);
	*/

	/* Stack习题 */
	
	/*
	//Stack unit test driver
	//foreach遍历测试
	cout << "foreach遍历测试" << "\n";
	try {
		Stack<char> stack;
		stack.push('t');
		stack.push('e');
		stack.push('s');
		stack.push('t');
		stack.pop();

		//遍历
		//for (char i : stack) {
		//	cout << i << endl;
		//}
		for (Stack<char>::iterator itr = stack.begin(); itr != stack.end(); ++itr) {
			std::cout << *itr << std::endl;
		}

		//throw exception
		stack.pop();
		stack.pop();
		stack.pop();
		stack.pop();
	}
	catch (const NullStackException& e) {
		cout << std::string("Exception: ") + e.what() << endl;
	}
	*/
	
	
	//Stack<char> stack;

	//Ex 1
	//Invert_Input(stack);

	//Ex 2
	/*
	int data = 0;
	std::cin >> data;
	int format = 2;
	std::cin >> format;
	Conversion(stack, data, format);
	*/

	//Ex 3
	//std::cout << (Matching(stack) ? "Correct" : "Wrong") << std::endl;

	//Ex 4
	/*
	//input
	Stack<std::string> strStack;
	std::string infix;
	std::getline(std::cin, infix);
	std::string postfix = infixToSuffix(strStack, infix);
	//output
	std::cout << postfix << std::endl;
	*/

	//Ex 5
	/*
	//input
	std::string infix;
	std::getline(std::cin, infix);
	cout << (int)calc_suffix(infix) << endl;
	*/

	//Ex 6
	/*
	std::string str;
	std::getline(std::cin, str);
	std::cout << trainScheduling(str) << std::endl;
	*/

	//Ex 7
	/*
	//input
	int row = 0, col = 0, inRow = 0, inCol = 0, outRow = 0, outCol = 0;
	std::cin >> row >> col >> inRow >> inCol >> outRow >> outCol;
	int **maze = (int **) malloc(sizeof(int *) * row);
	for (int i = 0; i < row; i++) {
		maze[i] = (int *)malloc(sizeof(int) * col);
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			std::cin >> maze[i][j];
		}
	}
	std::cout << maze_path(maze, inRow, inCol, outRow, outCol, row, col) << std::endl;
	//free
	for (int i = 0; i < row; i++) {
		free(maze[i]);
	}
	free(maze);
	*/

	//Ex 8
	/*
	//input
	std::string str, str1, str2;
	std::getline(std::cin, str, '@');
	int length = str.length() + 1;

	if (str.at(length / 2 - 1) == '&') {
		str1 = str.substr(0, length / 2 - 1);
		str2 = str.substr(length / 2, length - 1);
		std::cout << (parlindrome_judge(str1, str2) ? "true" : "false") << std::endl;
	}
	else
	{
		std::cout << "false" << std::endl;
	}
	*/
	
	//Ex 9
	/*
	int n = 1;
	std::cin >> n;
	int *arr = new int[n];
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}
	std::cout << MaxRecusive(arr, n);
	*/

	//Ex 10
	/*
	LinkedList<int> list = readAndGenerateLinkedList();
	list.listTraverse();
	List_Order_Recursive(list, 0, list.getSize() - 1);
	*/

	/* 背包问题 */
	//knap(8, 6);

	/* Queue Exercises */
	/*
	//Stack unit test driver
	//foreach遍历测试
	cout << "foreach遍历测试" << "\n";
	try {
		Queue<char> queue;
		queue.enqueue('t');
		queue.enqueue('e');
		queue.enqueue('s');
		queue.enqueue('t');
		queue.dequeue();

		//遍历
		//for (char i : queue) {
		//	cout << i << endl;
		//}
		for (Queue<char>::iterator itr = queue.begin(); itr != queue.end(); ++itr) {
			std::cout << *itr << std::endl;
		}

		//throw exception
		queue.dequeue();
		queue.dequeue();
		queue.dequeue();
		queue.dequeue();
	}
	catch (const NullQueueException& e) {
		cout << std::string("Exception: ") + e.what() << endl;
	}
	*/

	//Ex 1
	/*
	int n = 1;
	cin >> n;
	Queue<int> queue;
	YangHuiTriangle(queue, n);
	*/

	//Ex 2
	//farmerProblem();
	

	//Ex 3
	/*
	string str = "";
	getline(cin, str);
	//remove blanks
	for (string::size_type i = str.find(" "); i != string::npos; i = str.find(" ")) {
		str.erase(i, 1);
	}
	decode(str);
	*/

	//Ex 4
	/*
	int n = 0;
	cin >> n;
	int *arr = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	for (int i = 0; i < n; i++)
		queuetraining(arr[i]);
	*/
	
	//Ex 5
	/*
	int n = 0, m = 0;
	cin >> n >> m;
	monkey_king(n, m);
	*/

	// Binary Tree ADT Exercises
	//Ex 1
	/*
	char notation = 0;
	cin >> notation;
	cin.get();
	string str;
	getline(cin, str, '\n');
	//split by spaces
	istringstream iss(str);
	vector<char> result;
	for (string s; iss >> s; ) {
		result.push_back(s[0]);
	}
	*/

	//ADT_Tree_Ex1(notation, &result[0]);

	//Ex 3
	//ADT_Tree_Ex3(notation, &result[0]);

	//Ex 5
	//ADT_Tree_Ex5(notation, &result[0]);
	
	//Ex 6
	//ADT_Tree_Ex6(notation, &result[0]);

	//Ex 7
	//ADT_Tree_Ex7(notation, &result[0]);

	//Ex 9
	//ADT_Tree_Ex9(notation, &result[0]);

	//Ex 10
	/*
	char notation = 0;
	cin >> notation;
	cin.get();
	string str;
	getline(cin, str, '\n');
	//split by spaces
	istringstream iss(str);
	vector<char> result;
	for (string s; iss >> s; ) {
		result.push_back(s[0]);
	}

	BinaryTree<char> &tree0 = BinaryTree<char>(&result[0], notation);
	
	result = vector<char>();
	str = "";
	getline(cin, str, '\n');
	//split by spaces
	iss = istringstream(str);
	for (string s; iss >> s; ) {
		result.push_back(s[0]);
	}
	BinaryTree<char> &tree1 = BinaryTree<char>(&result[0], notation);
	*/

	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];

	BinaryTree<char> &tree0 = generateBinaryTree(notation), &tree1 = generateBinaryTree(notation);

	ADT_Tree_Ex10(tree0, tree1);
	*/

	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];

	BinaryTree<char> &tree0 = generateBinaryTree(notation), &tree1 = generateBinaryTree(notation);

	BinaryTree<char> *tree2 = new BinaryTree<char>(tree0);
	tree2->preOrderRecursiveTraverseOp(operate);
	*/

	//Ex 11
	//ADT_Tree_Ex11(notation, &result[0]);

	//Ex 12
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	char x = 0;
	int flag = 0;
	cin >> x;
	cin >> flag;
	char *bro = tree.get_Sibling(x, flag);
	if (bro == NULL)
		cout << "NULL" << endl;
	else
		cout << *bro << endl;
		*/

	//Ex 13
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	BinaryTree<char> &tree2 = generateBinaryTree(notation);
	cout << ((tree == tree2) ? "true" : "false") << endl;
	*/

	//Ex 14
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	cout << (tree.isCompleteTree() ? "true" : "false") << endl;
	*/

	//Ex 15
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	cout << tree.countDegreeTwo() << endl;
	*/	

	//Ex 17
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	char x = 0;
	cin >> x;

	vector<char> path = tree.getPath(x);
	for (vector<char>::iterator itr = path.begin(); itr != path.end(); itr++) {
		cout << *itr << ' ';
	}
	*/


	//Ex 18
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	tree.printBinaryTree();
	*/

	//Ex 19
	//ADT_Tree_Ex19(notation, &result[0]);

	///////////算法练习_二叉树///////////////
	//Ex 1
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	tree.preOrderRecursiveTraverseOp(operate);
	cout << "\n";
	tree.inOrderRecursiveTraverseOp(operate);
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operate);
	cout << "\n\n";
	tree.binaryTree_Revolute();
	tree.preOrderRecursiveTraverseOp(operate);
	cout << "\n";
	tree.inOrderRecursiveTraverseOp(operate);
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operate);
	cout << endl;
	*/

	//Ex 5
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	tree.sufOrderTraverseOp(operate);
	*/

	//Ex 6
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	char x = 0, y = 0;
	cin >> x >> y;
	char *ch = tree.findNearAncient(x, y);
	if (ch) {
		cout << *ch << endl;
	}
	else
	{
		cout << "NULL" << endl;
	}
	*/

	//Ex 7
	/*
	BinaryTree<string> tree = BinaryTree<string>();
	tree.expBinaryTreeFromInffix();	
	tree.preOrderRecursiveTraverseOp(operateString);
	cout << "\n";
	tree.inOrderRecursiveTraverseOp(operateString);
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operateString);
	cout << endl;
	*/

	//Ex 8
	/*
	BinaryTree<string> tree = BinaryTree<string>();
	tree.expBinaryTreeFromInffix();
	tree.preOrderRecursiveTraverseOp(operateString);
	cout << "\n";
	tree.infix_recursive_parentheses();
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operateString);
	cout << endl;
	*/
	
	//Ex 9
	/*
	BinaryTree<string> tree = BinaryTree<string>();
	tree.expBinaryTreeFromPostfix();
	tree.preOrderRecursiveTraverseOp(operateString);
	cout << "\n";
	tree.infix_recursive_parentheses();
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operateString);
	cout << endl;
	*/


	//Ex 10
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	tree.generateInfixThreadedTree();
	tree.infixTraverseInfixThreadedTree(operate);
	*/

	//Ex 12
	/*
	char notation = 0;
	string str;
	getline(cin, str);
	notation = str[0];
	BinaryTree<char> &tree = generateBinaryTree(notation);
	tree.generateInfixThreadedTree();
	char c = 0;
	cin >> c;
	tree.infixTraverseInfixThreadedTree(operate);
	cout << endl;
	tree.ex12(c);
	*/

	//Ex 13
	/*
	BinaryTree<HuffmanItem<char>> tree;
	int size = 0;
	cin >> size;
	vector<char> values;
	vector<int> weights;
	for (int i = 0; i < size; i++) {
		char c = 0;
		cin >> c;
		values.push_back(c);
	}
	for (int i = 0; i < size; i++) {
		int w = 0;
		cin >> w;
		weights.push_back(w);
	}
	tree.generateHuffmanTree<char>(values, weights);
	tree.layerTraverse(operateHuffmanWeight);
	cout << "\n";
	tree.preOrderRecursiveTraverseOp(operateHuffmanWeight);
	cout << "\n";
	tree.inOrderRecursiveTraverseOp(operateHuffmanWeight);
	cout << "\n";
	tree.sufOrderRecursiveTraverseOp(operateHuffmanWeight);
	cout << "\n\n";
	for (char c : values) {
		cout << c << ":";
		for (bool b : tree.getHuffmanCode<char>(c)) {
			cout << (b ? "1 " : "0 ");
		}
		cout << "\n";
	}
	*/

	//Ex 14
	
	// BinaryTree<HuffmanItem<string>> tree;
	// int size = 0;
	// cin >> size;
	// vector<string> values;
	// vector<int> weights;
	// for (int i = 0; i < size; i++) {
	// 	string c;
	// 	cin >> c;
	// 	values.push_back(c);
	// }
	// for (int i = 0; i < size; i++) {
	// 	int w = 0;
	// 	cin >> w;
	// 	weights.push_back(w);
	// }
	// tree.generateHuffmanTree<string>(values, weights);
	// tree.printHuffmanTree<string>();
	// tree.getHuffmanCode(values[0]);
	// tree.getHuffmanCode(values[2]);
	

	////////////////// Graph /////////////////////////
	// Ex 1
	/*
	string kindStr;
	getline(cin, kindStr);
	AdjListGraph<char> g;
	AdjListGraph<char>::graphTypes kind;
	if (kindStr == "UDG")
		kind = g.UDG;
	else if (kindStr == "DG")
		kind = g.DG;
	else if (kindStr == "UDN")
		kind = g.UDN;
	else if (kindStr == "DN")
		kind = g.DN;
	else
		return -1;
	int verSize = 0;
	cin >> verSize;
	vector<char> vers;
	for (int i = 0; i < verSize; i++) {
		char c = 0;
		cin >> c;
		vers.push_back(c);
	}
	g = AdjListGraph<char>(kind, vers);
	cout << g.getGraphTypeName() << "\n";
	vector<char> v = g.getVertexs();
	for (vector<char>::iterator itr = v.begin(); itr != v.end(); itr++)
		cout << *itr << " ";
	cout << "\n";
	vector< vector<int> > adjLists = g.getAdjLists();
	vector<char>::iterator iVer = v.begin();
	for (vector< vector<int> >::iterator itr = adjLists.begin(); itr != adjLists.end(); itr++) {
		cout << *iVer;
		iVer++;
		for (vector<int>::iterator i = (*itr).begin(); i != (*itr).end(); i++)
			cout << " " << *i;
		cout << "\n";
	}
	*/

	//Ex 2
	/*
	string kindStr;
	getline(cin, kindStr);
	AdjListGraph<char> g;
	AdjListGraph<char>::graphTypes kind;
	if (kindStr == "UDG")
	kind = g.UDG;
	else if (kindStr == "DG")
	kind = g.DG;
	else if (kindStr == "UDN")
	kind = g.UDN;
	else if (kindStr == "DN")
	kind = g.DN;
	else
	return -1;
	int verSize = 0;
	cin >> verSize;
	vector<char> vers;
	for (int i = 0; i < verSize; i++) {
		char c = 0;
		cin >> c;
		vers.push_back(c);
	}
	int edgeSize = 0;
	cin >> edgeSize;
	vector< pair<int, int> > edges;
	int tail = 0, head = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> tail >> head;
		edges.push_back(pair<int, int>(tail, head));
	}
	g = AdjListGraph<char>(kind, vers, edges);
	cout << g.getGraphTypeName() << "\n";
	vector<char> v = g.getVertexs();
	cout << v[0];
	for (vector<char>::iterator itr = v.begin() + 1; itr != v.end(); itr++)
		cout << " " << *itr;
	cout << "\n";
	vector< vector<int> > adjLists = g.getAdjLists();
	vector<char>::iterator iVer = v.begin();
	for (vector< vector<int> >::iterator itr = adjLists.begin(); itr != adjLists.end(); itr++) {
		cout << *iVer;
		iVer++;
		for (vector<int>::iterator i = (*itr).begin(); i != (*itr).end(); i++)
			cout << "->" << *i;
		cout << "\n";
	}
	*/

	//Ex 3
	/*
	string kindStr;
	getline(cin, kindStr);
	AdjListGraph<char, int> g;
	AdjListGraph<char, int>::graphTypes kind;
	if (kindStr == "UDG")
	kind = g.UDG;
	else if (kindStr == "DG")
	kind = g.DG;
	else if (kindStr == "UDN")
	kind = g.UDN;
	else if (kindStr == "DN")
	kind = g.DN;
	else
	return -1;
	int verSize = 0;
	cin >> verSize;
	vector<char> vers;
	for (int i = 0; i < verSize; i++) {
		char c = 0;
		cin >> c;
		vers.push_back(c);
	}
	int edgeSize = 0;
	cin >> edgeSize;
	vector< pair<int, int> > edges;
	int tail = 0, head = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> tail >> head;
		edges.push_back(pair<int, int>(tail, head));
	}
	vector<int> weights;
	int w = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> w;
		weights.push_back(w);
	}
	g = AdjListGraph<char, int>(kind, vers, edges, weights);
	printGraph<char, int>(g);
	*/
	
	//Ex 4
	/*
	string kindStr;
	getline(cin, kindStr);
	AdjListGraph<char> g;
	AdjListGraph<char>::graphTypes kind;
	if (kindStr == "UDG")
	kind = g.UDG;
	else if (kindStr == "DG")
	kind = g.DG;
	else if (kindStr == "UDN")
	kind = g.UDN;
	else if (kindStr == "DN")
	kind = g.DN;
	else
	return -1;
	int verSize = 0;
	cin >> verSize;
	vector<char> vers;
	for (int i = 0; i < verSize; i++) {
		char c = 0;
		cin >> c;
		vers.push_back(c);
	}
	int edgeSize = 0;
	cin >> edgeSize;
	vector< pair<int, int> > edges;
	int tail = 0, head = 0;
	for (int i = 0; i < edgeSize; i++) {
		cin >> tail >> head;
		edges.push_back(pair<int, int>(tail, head));
	}
	g = AdjListGraph<char>(kind, vers, edges);
	char c = 0;
	cin >> c;
	cout << g.getGraphTypeName() << "\n";
	printGraph<char, bool>(g);
	g.addVertex(c);
	cout << "\n";
	printGraph<char, bool>(g);
	*/

	//Ex 5
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	char c = 0;
	cin >> c;
	cout << g.getGraphTypeName() << "\n";
	printGraph(g);
	cout << "\n" << g.getVertexIndex(c);
	*/

	//Ex 6
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	int index = -1;
	cin >> index;	
	cout << g.getGraphTypeName() << "\n";
	printGraph(g, 1, 1);
	cout << "\n";
	g.removeVertexByIndex(index);
	printGraph(g, 1, 1);
	*/

	//Ex 7
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	int index = -1;
	cin >> index;
	printGraph(g);
	cout << "\n";
	cout << g.getNextAdjVerIndex(index);
	*/

	//Ex 8
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	int index = -1, base = -1;
	cin >> index >> base;
	printGraph(g);
	cout << "\n";
	cout << g.getNextAdjVerIndex(index, base);
	*/

	//Ex 9
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	int tail = -1, head = -1;
	cin >> tail >> head;
	printGraph(g, 0, 1);
	cout << "\n";
	cout << (g.addArc(tail, head) ? "true" : "false") << "\n";
	printGraph(g, 0, 1);
	*/

	//Ex 10
	/*
	AdjListGraph<char, int> g = generateAdjListGraph<char, int>(true);
	int tail = -1, head = -1, w;
	cin >> tail >> head >> w;
	printGraph(g, 0, 1);
	cout << "\n";
	cout << (g.addArc(tail, head, w) ? "true" : "false") << "\n";
	printGraph(g, 0, 1);
	*/

	//Ex 11
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(false);
	int tail = -1, head = -1;
	cin >> tail >> head;
	printGraph(g, 0, 1);
	cout << "\n";
	cout << (g.removeArc(tail, head) ? "true" : "false") << "\n";
	printGraph(g, 0, 1);
	*/

	//Ex 12
	/*
	AdjListGraph<string> g = generateAdjListGraph<string, bool>(false);
	int index = 0;
	cin >> index;
	printGraph(g, 0, 0);
	cout << "\n";
	vector<string> dfs = g.DFSTraverse(index);
	cout << dfs[0];
	for (vector<string>::iterator i = dfs.begin() + 1; i != dfs.end(); i++)
		cout << "->" << *i;
	*/

	//Ex 13
	/*
	AdjListGraph<string> g = generateAdjListGraph<string, bool>(false);
	int index = 0;
	cin >> index;
	printGraph(g, 0, 0);
	cout << "\n";
	vector<string> dfs = g.BFSTraverse(index);
	cout << dfs[0];
	for (vector<string>::iterator i = dfs.begin() + 1; i != dfs.end(); i++)
	cout << "->" << *i;
	*/

	//Ex 14
	/*
	AdjListGraph<char, int> g = generateAdjListGraph<char, int>(1);
	int tail = -1, head = -1;
	cin >> tail >> head;
	int *c = g.getWeight(tail, head);
	printGraph(g, 0, 1);
	cout << "\n";
	if (c) 
	{
		cout << "true\n";
		cout << *c;
	}
	else
	{
		cout << "false";
	}
	*/
	
	//Ex 15
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	int tail = -1, head = -1;
	cin >> tail >> head;
	printGraph(g, 0, 0);
	cout << "\n";
	cout << (g.isAdjVers(tail, head) ? "true" : "false");
	*/

	//Ex 16
	// AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	// int index = 0;
	// cin >> index;
	// printGraph(g, 0, 0);
	// cout << "\n" << g.getOutDegree(index);

	//Ex 17
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	int index = 0;
	cin >> index;
	printGraph(g, 0, 0);
	cout << "\n" << g.getInDegree(index);
	*/

	//Ex 18
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	int tail = -1, head = -1;
	cin >> tail >> head;
	printGraph(g, 0, 0);
	cout << "\n" << (g.hasPath(tail, head) ? "true" : "false");
	*/

	//Ex 19
	/*/
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	printGraph(g, 0, 0);
	cout << "\n";
	vector<char> sorted;
	bool b = g.topologicalSort(sorted);
	if (sorted.size() > 0) {
		cout << sorted[0];
		for (vector<char>::iterator i = sorted.begin() + 1; i != sorted.end(); i++)
			cout << "->" << *i;
		cout << "\n";
	}
	//else
	//	cout << "\n";
	cout << (b ? "true" : "false");
	*/

	//Ex 20
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	printGraph(g, 0, 0);
	cout << "\n";
	cout << (g.hasCircle() ? "true" : "false");
	*/

	//Ex 21
	/*
	AdjListGraph<char> g = generateAdjListGraph<char, bool>(0);
	printGraph(g, 0, 0);
	cout << "\n";
	vector<char> sorted;
	cout << (g.topologicalSort(sorted) ? "true" : "false");
	*/
	
	/*
	SqList<int> l = readAndGenerateSqList();

	l.ListTraverse();
	MergeSort::mergeSort(l);
	*/
	
	/*
	SqList<int> l = readAndGenerateSqList();

	int deleted = 0;
	cin >> deleted;

	l.ListTraverse();
	cout << endl;

	l.deleteItem(deleted);

	if (l.ListLength() > 0) {
		cout << endl;
		l.ListTraverse();
	}
	else
	cout << "\nempty\n";
	*/


	// 小组作业 5
	/*
	AdjMatrixGraph<char, int> g = generateAdjMatrixGraph<char, int>();
	vector< pair<char, char> > verPairs;
	vector<int> lowcosts;
	int u = 0;
	cin >> u;

	g.miniSpanTreePrim(u, verPairs, lowcosts);
	cout << g.GetGraphKind() << "\n";
	printMatrixGraph<char, int>(g, 0, 0);

	if (verPairs.size() < 1) {
		return 0;
	}

	cout << "(" << verPairs[0].first << "," << verPairs[0].second << "," << lowcosts[0] << ")";
	for (int i = 1; i < lowcosts.size(); i++) {
		cout << ",(" << verPairs[i].first << "," << verPairs[i].second << "," << lowcosts[i] << ")";
	}
	*/

	// 小组作业 6
	/*
	// P.S. 测试用例真的是, 都没有说清楚到底是 int 还是 char 还是 string, 统一按照 string 处理 
	AdjListGraph<string, int> g = generateAdjListGraph<string, int>(1);

	vector< pair<pair<string, string>, int> > mstTree;
	cout << g.getGrapgTypeString() << endl;

	g.miniSpanTreeKruskal(mstTree);

	int cost = 0;

	for (int i = 0; i < mstTree.size(); i++) {
		cout << "(" << mstTree[i].first.first << "," << mstTree[i].first.second << ")," << mstTree[i].second << "\n";
		cost += mstTree[i].second;
	}

	cout << endl;

	// OJ 没有考虑清楚 MST 的权值问题, 对于非连通图, 没有 MST, 也就是不存在 MST 的权值
	// 所以 0 不能代表 MST 不存在, 例如 一个完全连通图三个顶点的值为 6, -2, -4, 那么 其 MST 就是 0 
	//if (mstTree.size() > 0)
	cout << cost << endl;
	//else
	//	cout << "\n";
	*/

	// 小组作业7
	/*
	AdjMatrixGraph<string, int> g = generateAdjMatrixGraph<string, int>();
	int u = 0;
	cin >> u;

	printMatrixGraph<string, int>(g, 0, 0);

	cout << "\n";

	g.shostestPathDijkstra(u);
	*/

	//pause 
	cout << "please enter to continue..." << endl;
	getwchar();
	
	return 0;
}

