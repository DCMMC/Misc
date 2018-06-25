#pragma once

#include "Queue.h"
#include <bitset>

/**
* 队列练习题
*/

/**
* Ex 1 杨辉三角
*/
void YangHuiTriangle(Queue<int> queue, int n) {
	queue.enqueue(1);
	cout << "1" << endl;
	int tmp = 0;
	for (int i = 2; i <= n; i++) {
		cout << "1 ";
		queue.enqueue(1);

		for (int j = 0; j < i - 2; j++) {
			tmp = queue.dequeue();
			tmp += queue.peek();
			queue.enqueue(tmp);
			cout << tmp << " ";
		}

		cout << queue.dequeue();
		queue.enqueue(1);

		cout << endl;
	}
}

/**
* Ex 2 有点难度
* 农夫过河
* 用Queue来实现广度优先搜索(Breadth First Search)
* 在状态空间中搜索路径: 找到一条路径(相邻状态间转移合法), 从开始状态到结束状态, 途中不经过不安全状态.
* 用位向量来表示状态: 四位二进制数从高位到低位分别表示狼, 羊, 白菜和农夫是否在北岸(i.e. 1表示在北岸, 0表示在南岸) (所以易得
* 有2^4 = 16中状态)
* 
* 状态 0000 -> 状态 1111
* 队列moveTo记录可达但是向前试探的状态
* 数组route记录各个状态的路径, 其中route[i]记录状态i的路径的上一个状态 -1表示从没有访问过
*/
enum 
{
	FARMER = 8, CABBAGE = 2, GOAT = 1, WOLF = 4
};
/* 返回值1表示在北岸, 0表示在南岸 */
int goat(int position) {
	return (position & GOAT) != 0;
}

int cabbage(int position) {
	return (position & CABBAGE) != 0;
}

int farmer(int position) {
	return (position & FARMER) != 0;
}

int wolf(int position) {
	return (position & WOLF) != 0;
}

/* 安全性判断 */
bool safe(int position) {
	if ((goat(position) == cabbage(position)) && (goat(position) != farmer(position)))
		return 0; //羊吃白菜
	if ((goat(position) == wolf(position)) && (goat(position) != farmer(position)))
		return 0; //狼吃羊
	return 1;
}

void farmerProblem() {
	int route[16] = { 0 };
	for (int i = 1; i < 16; i++)
		route[i] = -1;

	Queue<int> moveTo;
	//初始状态
	moveTo.enqueue(0x00);

	//队列中还有没访问的状态, 并且结束状态1111还没有被访问过 
	while (!moveTo.isEmpty() && (route[15] == -1)) {
		int status = moveTo.dequeue();

		//依次遍历羊, 白菜, 狼, 农民去另外一边河岸的状态
		for (int mover = 1; mover <= 8; mover <<= 1) {
			//如果mover与农夫同一侧
			if (farmer(status) == ((status & mover) != 0)) {
				//过河后的状态
				int new_status = status ^ (FARMER | mover);
				//如果该状态没有访问过并且安全
				if (route[new_status] == -1 && safe(new_status)) {
					route[new_status] = status;
					moveTo.enqueue(new_status);
				}
			}
		}
	}

	//print
	int index = 15;
	cout << "1111" << endl;
	while (route[index] != 0) {
		cout << bitset<4>(route[index]) << endl;
		index = route[index];
	}
	cout << "0000" << endl;

}


/**
* Ex3
应用：假设某国特工收到一串事先经过加密处理的密码数字（每位数字的范围为0-9），要求按照预先设定的规则将其解码，恢复到原始的状态，方可使用。解码规则如下：首先将第1位数删除，紧接着将第2位数放到这串数的末尾，再将第3位数删除，再将第4位数放到这串数的末尾，再将第5位数删除……直到剩下最后一位数，将最后一位数也删除。最后，按照刚才删除的顺序，把所有这些删除的数，按照删除的顺序重新连在一起就是原始的密码数字。

例如：给定的一串数字是“631758924”， 经过解码处理后，得到原始的密码数字为“615947283”。接收到的数字串内如存在空格，空格不计入位数。

要求设计一个算法，使用顺序队列，设计并实现按照以上规则进行解码的算法。为简单起见，解码结果直接输出即可，无需保存。

参考函数原型：
template<class ElemType>
void decode(SqQueue<ElemType> &S, string &code);

输入说明 :

第一行：待解码的乱码数字（数字串之间可以有空格，不影响解码结果）
输出说明 :

第一行：解码后的原始密码
输入范例 :


6  317 5 8924
输出范例 :


615947283

*/
void decode(string &code) {
	Queue<int> queue;

	for (string::iterator itr = code.begin(); itr != code.end(); itr++) {
		queue.enqueue((*itr) - '0');
	}

	while (!queue.isEmpty()) {
		cout << queue.dequeue();
		if (!queue.isEmpty())
			queue.enqueue(queue.dequeue());
	}
}


/**
* Ex4
应用：某部队进行新兵队列训练，将新兵从一开始按顺序依次编号，并排成一行横队，训练的规则如下：
从头开始一至二报数，凡报到二的出列，剩下的向小序号方向靠拢，再从头开始进行一至三报数，凡报到三的出列，
剩下的向小序号方向靠拢，继续从头开始进行一至二报数。。。，以后从头开始轮流进行一至二报数、一至三报数直到剩下的人
数不超过三人为止。
要求设计一个算法，使用链队列或顺序队列（只能使用1个队列），设计并实现打印输出剩下的新兵最初的编号的算法。
注意：本题允许有多个测试数据组，新兵总人数不超过5000。由于士兵可能的总人数较多，顺序队列需预先申请的存储空间较大，
建议使用链队列。（样例程序使用的是链队列。）

输入说明 :

第一行：组数N，
第二行-第N+1行：每组的新兵人数
输出说明 :

第一行：
.
.      剩下的新兵最初的编号，编号之间有用","分隔。
.
第N行：

输入范例 :


2
20
40
输出范例 :


1,7,19
1,19,37

*/
void queuetraining(int n) {
	Queue<int> queue;

	for (int i = 1; i <= n; i++)
		queue.enqueue(i);
	//报到out的时候出列
	unsigned out = 2, size = 0;
	int tmp = 0;


	while ( (size = queue.getSize()) > 3 ) {
		for (unsigned i = 1; i <= size; i++) {
			tmp = queue.dequeue();
			if (i % out != 0)
				queue.enqueue(tmp);
		}
		//转置
		size = queue.getSize();
		for (unsigned i = 0; i < size; i++)
			queue.enqueue(queue.dequeue());

		out = (out == 2) ? 3 : 2;
	}

	//print
	std::string tmpStr("");
	for (Queue<int>::iterator itr = queue.begin(); itr != queue.end(); ++itr)
		tmpStr += (SSTR(*itr) + ",");
	tmpStr = tmpStr.substr(0, tmpStr.length() - 1);
	cout << tmpStr << endl;
}

/**
*
* Ex 5

应用：猴子选大王是一个小游戏，其规则是：假设n个猴子围成一堆，取数字m为将要被淘汰的数字，从第一个猴子开始数，数到数字m时，则该猴子出列，被淘汰，然后从被淘汰的猴子的下一个开始数，再数到数字m时，这个猴子也被淘汰，从下一个开始数，依次进行，直到剩下一个猴子结束。该猴子即猴子大王。

要求设计一个算法，使用链队列，设计并实现按照以上规则进行选择猴王的算法。为简单起见，直接输出猴王的序号即可，无需保存。

参考函数原型：
template<class ElemType>
void monkey_king( LinkQueue<ElemType> S, int n, int m );
输入说明 :

第一行：猴子的数目n
第二行：淘汰数字m
输出说明 :

第一行：猴王的序号
输入范例 :


30
4
输出范例 :


6

*/

void monkey_king(int n, int m) {
	//就是一个约瑟夫环的问题
	//推荐还是用数学方法而不是模拟方法(就算是模拟方法也已经在Circle LinkedList那里的习题Cover到了)
	int s = 0;
	for (int i = 2; i <= n; i++)
		s = (s + m) % i;

	cout << (s + 1) << endl;
}