#pragma once

#include <cstdlib>

#ifndef SSTR_FLAG
#define SSTR_FLAG
#include <sstream>
//std::dec sets the basefield format flag for the str stream to decimal(十进制)
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif // !SSTR_FLAG

#include <iostream>
/**
* DHU OJ 顺序栈练习题
* @author DCMMC
* created on 2017/10/28
*/

#include "Stack.h"

/**
* Exercise 1
* 逆置输出
*/
//个人认为这道题目不需要模板, T就是char, 要啥模板
void Invert_Input(Stack<char> stack) {
	//input
	std::string str;

	std::getline(std::cin, str);

	for (char c : str) {
		stack.push(c);
	}

	for (char c : stack) {
		std::cout << c;
	}
	std::cout << std::endl;
}

/**
* Ex2
* 进制转换
*/
void Conversion(Stack<char> &stack, int data, int formate) {
	if (data == 0)
		std::cout << 0 << std::endl;

	while (data) {
		char remainder = (char) (data % formate);
		stack.push(remainder > 9 ? remainder - 10 + 'A' : remainder + '0');
		data /= formate;
	}

	//c++11
	for (char c : stack) {
		std::cout << c;
	}

	std::cout << std::endl;
}

/**
* Ex 3 括弧匹配检验
*/
bool Matching(Stack<char> &stack) {
	//input
	std::string str;
	std::getline(std::cin, str);

	bool hasParentheses = false;

	//c++11
	for (char c : str) {
		switch (c)
		{
		case '(' :
			hasParentheses = true;
			//push stack
			stack.push('(');
			break;
		case ')' :
			//pop stack
			try {
				stack.pop();
			}
			catch (const NullStackException& e) {
				return false;
			}
			break;
		default:
			break;
		}
	}

	return !hasParentheses || stack.getSize() == 0;
}

/**
* Ex 4
* 中缀式转换后缀式
*/
std::string infixToSuffix(Stack<std::string> &ops, std::string infix) {
	//没有对infix的合法性做检查! 可以用前面的Matching来检查括号
	//可以用map存储一下操作符的优先级, 这样可以扩展以便支持更多操作符的表达式
	//这里只支持 + - * / ( )
	//C++没有string的switch真难受=,=

	//cin read from infix
	std::istringstream stream(infix);
	//backup original stream
	streambuf* cin_backup = std::cin.rdbuf(stream.rdbuf());

	std::string element;
	std::string op;

	std::string suffix;

	while (std::cin >> element) {
		if (element == "(") {
			ops.push(element);
		}
		else if (element == ")") {
			//执行运算直到遇到"("并去掉对应的那个"("
			//因为这时候如果括号里面有多个表达式, 必定是优先级一直在增大的, e.g. ( 1 + 2 * 3 ) -> 1 2 3 * +
			while ((op = ops.pop()) != "(") {
				suffix += op + " ";
			}
		}
		else if (element == "+" || element == "-") {
			if (ops.getSize() > 0) 
			{
				if ((op = ops.pop()) == "+" || op == "-" || op == "*" || op == "/") {
					//如果前面有(一个)优先级相同或者更高的操作符, 就处理掉
					// e.g. 1 * 2 + 3 and 1 + 2 + 3
					suffix += op + " ";
				}
				else
				{
					ops.push(op);
				}
			}

			ops.push(element);
		}
		else if (element == "/" || element == "*") {
			if (ops.getSize() > 0)
			{	
				if ((op = ops.pop()) == "/" || op == "*") {
					//如果前面有(一个)优先级相同的操作符, 就处理掉
					//前面如果优先级低的操作符就不管他
					suffix += op + " ";
				}
				else {
					ops.push(op);
				}
			}
			
			ops.push(element);
		}
		else {
			//数值
			suffix += element + " ";
		}
	}

	//最后, 还会剩下一些(这里一般是一个或者两个)操作符
	while (ops.getSize() > 0) {
		suffix += ops.pop() + " ";
	}

	//去掉最后面的一个括号
	suffix = suffix.substr(0, suffix.length() - 1);

	delete cin_backup;

	return suffix;
}

/**
* Ex 5
* 应用后缀式进行二元表达式四则运算
*/
double calc_suffix(std::string infix) {
	Stack<std::string> stack;
	std::string postfix = infixToSuffix(stack, infix);

	//output
	std::cout << postfix << "\n" << std::endl;

	//calculate
	std::string token;

	//cin read from infix
	std::istringstream stream(postfix);
	//backup original stream
	streambuf* cin_backup = std::cin.rdbuf(stream.rdbuf());

	double num1 = 0.0, num2 = 0.0;
	while (std::cin >> token) {
		if (token == "+") {
			num2 = atof(stack.pop().c_str());
			num1 = atof(stack.pop().c_str());
			
			stack.push(SSTR(num1 + num2));
		}
		else if (token == "-") {
			num2 = atof(stack.pop().c_str());
			num1 = atof(stack.pop().c_str());

			stack.push(SSTR(num1 - num2));
		}
		else if (token == "*") {
			num2 = atof(stack.pop().c_str());
			num1 = atof(stack.pop().c_str());

			stack.push(SSTR(num1 * num2));
		}
		else if (token == "/") {
			num2 = atof(stack.pop().c_str());
			num1 = atof(stack.pop().c_str());

			stack.push(SSTR(num1 / num2));
		}
		else {
			//数值
			stack.push(token);
		}
	}

	delete cin_backup;
	return atof(stack.pop().c_str());
}


/**
* Ex 6 火车调度
* @param trains 代表初始的车厢排列序列，车厢和车厢之间以空格分隔。
* @return 调度结果。
*/
std::string trainScheduling(std::string trains) {
	std::string result;

	Stack<std::string> HStack;
	//cin read from infix
	std::istringstream stream(trains);
	//backup original stream
	streambuf* cin_backup = std::cin.rdbuf(stream.rdbuf());

	std::string train;

	while (std::cin >> train) {
		if (train.at(0) == 'S') {
			result += train + " ";
		}
		else
			HStack.push(train);
	}

	//c++11
	for (std::string str : HStack) {
		result += str + " ";
	}

	delete cin_backup;
	
	return result.substr(0, result.length() - 1);
}

/**
* Ex 7辅助类
*/
class Grid {
public:
	int x = 0, y = 0;
	//分别代表四个方向的连通性(已经被尝试过不能到达的endGrid的原来open的那些方向也会被标记为close), 0表示close, 1表示open
	//index 0 ~ 3 分别是上右下左
	int opens[4] = { 0 };

	Grid() {

	}

	Grid(int x, int y) {
		this->x = x;
		this->y = y;
	}

	bool operator==(Grid grid) {
		return x == grid.x && y == grid.y;
	}

	bool operator!=(Grid grid) {
		return !(operator==(grid));
	}
};


void checkOpens(int **maze, Grid &grid, int row, int col) {
	//上
	if (grid.x != 0 && maze[grid.x - 1][grid.y] == 0)
		grid.opens[0] = 1;

	//下
	if (grid.x != row - 1 && maze[grid.x + 1][grid.y] == 0)
		grid.opens[2] = 1;

	//左
	if (grid.y != 0 && maze[grid.x][grid.y - 1] == 0)
		grid.opens[3] = 1;

	//右
	if (grid.y != col - 1 && maze[grid.x][grid.y + 1] == 0)
		grid.opens[1] = 1;
}

Grid getOpenGrid(int index, Grid grid) {
	switch (index)
	{
	case 0: return Grid(grid.x - 1, grid.y);
	case 2: return Grid(grid.x + 1, grid.y);
	case 3: return Grid(grid.x, grid.y - 1);
	case 1: return Grid(grid.x, grid.y + 1);
	default:
		return Grid(-1, -1);
	}
}

/**
* 获取grid的下一个开的方向的Grid(不是lastGrid)
*/
int getNextOpenGridIndex(Grid grid, Grid lastGrid) {
	for (int i = 0; i < 4; i++) {
		if (grid.opens[i] && getOpenGrid(i, grid) != lastGrid) {
			return i;
		}
	}

	//allClosed
	return -1;
}

/**
* 定位lastGrid在grid的哪个方向上
*/
int localLastGrid(Grid grid, Grid lastGrid) {
for (int i = 0; i < 4; i++) {
	if (grid.opens[i] && getOpenGrid(i, grid) == lastGrid) {
		return i;
	}
}

//allClosed
return -1;
}

/**
* 判断grid是否跟Path有相交
*/
bool crossPath(Grid &grid, Stack<Grid> &path) {
	//c++11
	for (Grid g : path) {
		if (grid == g)
			return true;
	}

	return false;
}

/**
* Ex 7 迷宫问题
* 这题有点像凸包问题(ConvexHull), 回溯法, 现在实现的方法是暴力算法, 希望能够改进一下
* @parma maze 迷宫的节点图, 0表示连通, 1标志封闭
*/
std::string maze_path(int **maze, int inRow, int inCol, int outRow, int outCol, int row, int col) {
	if (maze[inRow][inCol] == 1 || maze[outRow][outCol] == 1)
		return "No Path";

	Stack<Grid> path;

	const Grid &allClosed = Grid(-1, -1);

	//先把allClose作为下面这个workGrid的lastGrid
	path.push(allClosed);

	Grid workGrid = Grid(outRow, outCol);

	checkOpens(maze, workGrid, row, col);

	Grid endGrid = Grid(inRow, inCol);

	while (true) {
		if (workGrid == endGrid) {
			path.push(workGrid);
			break;
		}

		Grid tmpGrid = getOpenGrid(getNextOpenGridIndex(workGrid, path.peek()), workGrid);
		if (tmpGrid == allClosed || crossPath(tmpGrid, path)) {
			path.push(workGrid);
			//回溯
			while (!path.isEmpty()) {
				tmpGrid = path.pop();
				int index = getNextOpenGridIndex(tmpGrid, path.peek());
				tmpGrid.opens[index] = 0;
				if (index != -1) {
					workGrid = getOpenGrid(index, tmpGrid);
					path.push(tmpGrid);
					if (crossPath(workGrid, path)) {
						//相交的情况
						continue;
					}
					else
					{
						checkOpens(maze, workGrid, row, col);
						workGrid.opens[localLastGrid(workGrid, tmpGrid)] = 0;
						break;
					}
				}
			}

			if (path.isEmpty()) {
				//失败
				return "No Path";
			}
		}
		else
		{
			//关闭
			workGrid.opens[getNextOpenGridIndex(workGrid, path.peek())] = 0;
			checkOpens(maze, tmpGrid, row, col);
			tmpGrid.opens[localLastGrid(tmpGrid, workGrid)] = 0;
			path.push(workGrid);
			workGrid = tmpGrid;
		}
	}

	//generate output path
	std::string result_path;
	int cnt = 0;

	//check
	Grid last = *new Grid(-1, -1);
	Grid allClose = *new Grid(-1, -1);

	//c++ 11
	for (Grid g : path) {
		//assert debug
		if (last == allClose || g == allClose) {
			last = g;
		}
		else
		{
			checkOpens(maze, g, row, col);
			checkOpens(maze, last, row, col);
			if (localLastGrid(last, g) == -1)
				cout << "WA!" << endl;
			last = g;
		}

		result_path += "(" + SSTR(g.x) + "," + SSTR(g.y) + ")->";

		if (++cnt == 4) {
			cnt = 0;
			result_path += "\n";
		}
	}

	//最后这11个字符是->(-1,-1)->\0
	if (result_path.at(result_path.length() - 1) == '\n')
		result_path = result_path.substr(0, result_path.length() - 12);
	else
		result_path = result_path.substr(0, result_path.length() - 11);

	return result_path;
}

/**
* Ex 8 回文判断
* 个人认为这个函数(~3n time, ~3n space 是失败的)
*/
bool parlindrome_judge(std::string str1, std::string str2) {
	Stack<char> str1Reverse;
	//c++11
	for (char c : str1) {
		str1Reverse.push(c);
	}

	
	int index = 0;
	const char *str2Ptr = str2.c_str();
	//c++11
	for (char c : str1Reverse) {
		if (c != str2Ptr[index++])
			return false;
	}

	return true;
}

/**
* Ex 9 递归求解最大值
* @param n 表示比较区域为前n个元素
*/
int MaxRecusive(int values[], int n) {
	//递归三要素: base case, address subproblems, cannot overlap

	//base case
	if (n == 1)
		return values[0];

	//尾递归
	int max_n_minus_one = MaxRecusive(values, n - 1);

	return values[n - 1] > max_n_minus_one ? values[n - 1] : max_n_minus_one;
}

/**
* Ex10 递归按中点优先的顺序遍历线性表
* aka 中序遍历链表
@param lo 从0开始
@param hi
*/
template<class T>
void List_Order_Recursive(LinkedList<T> &list, int lo, int hi) {
	//base case
	if (lo > hi) {
		return;
	}

	int mid = lo + (hi - lo) / 2;
	std::cout << list.get(mid) << " ";

	//recursive call, 尾递归
	List_Order_Recursive(list, lo, mid - 1);
	List_Order_Recursive(list, mid + 1, hi);
}

/**
* 递归思想的应用: 背包问题
* 背包里面可以放置重量为s的物品, 现有n件物品, 重量分别为w_1, w_2, ..., w_n. 问能否从中选出若干件的重量之和正好是s.
*
* 背包问题的函数为 int knap(int s, int n) 其中 s >= 0, n >= 1, 返回1表示有解, 0表示无解
* 如果有解: 1. 如果选得物品包括w_n, 则knap(s - w_n, n - 1) = knap(s, n)
* 2. 如果选的物品不包括w_n, 则 knap(s, n - 1) = knap(s, n)
*/
int w[] = {1, 4, 5, 9, 2, 1}; //s = 8, n = 6
int knap(int s, int n) {
	if (s == 0)
		//有解
		return 1;
	else if (s < 0 || (s > 0 && n < 1))
		//无解
		return 0;
	else if (knap(s - w[n - 1], n - 1) == 1) {
		//选的物品包括w_n
		printf("n = %d, w[%d] = %d\n", n, n - 1, w[n - 1]);
		return 1;
	}
	else
		//选的物品不包括w_n
		return knap(s, n - 1);
}