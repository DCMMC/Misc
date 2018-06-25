#pragma once

#include "DLList.h"

//fuck C++98 DHU OJ
#ifndef SSTR_FLAG
#define SSTR_FLAG
#include <sstream>
//std::dec sets the basefield format flag for the str stream to decimal(十进制)
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif // !SSTR_FLAG


/**
* Exercise 7
*
应用1：假设有一个带头结点的单链表A，现要求设计一个算法，实现单链表的就地逆置，即利用原表的存储空间实现表中
前m 个元素和后n 个元素的互换。
参考函数原型：
template<class ElemType>
void Exchange_L( LinkList<ElemType> &L, int m );

输入说明 :
第一行：待逆置单链表的长度
第二行：待逆置单链表的数据元素（数据元素之间以空格分隔）
第三行：逆置位置m

输出说明 :
第一行：待逆置单链表的遍历结果
第二行：逆置后单链表的遍历结果

（输入与输出之间以空行分隔）
输入范例 :
10
13 5 27 9 32 123 76 98 54 87
5
输出范例 :
13 5 27 9 32 123 76 98 54 87
123 76 98 54 87 13 5 27 9 32

*/
template<class T>
void Exchange_L(LinkedList<T> &list, int m) {
	//不对m做正确性检验

	if (m == list.getSize() - 1)
		return;

	//虽然这里用到了last node, 不过转换到Single Linked List的话, 只需要放心的去掉关于lastNode的语句就好啦
	//所以也算是只用单链表的思维来解题了, 实在懒得为单链表再去实现一个ADT了 :)
	//为了避免编译器对这个内部类无法识别， 最好（按照标准应该是必须）加入typename关键字， 不过我在VS里面的编译器不需要指定
	typename LinkedList<T>::Node *newFirst = list.getNode(m), *newLast = list.getNode(m - 1);

	(list.getNode(list.getSize() - 1))->next = list.getNode(0);
	list.getNode(0)->previous = list.getNode(list.getSize() - 1);

	newFirst->previous = NULL;
	newLast->next = NULL;

	list.setFirstNode(newFirst);
	list.setLastNode(newLast);
}

/**
* Exercise 8
* 应用2：假设线性表A=(a1,a2,...,am)，线性表B=(b1,b2,...,bn)，现要求设计一个算法，使用带头结点的单链表存储结构，
将这两个链表连接在一起（即令单链表B的首元结点连在单链表A的尾结点之后），且使用A和B的原存储空间。单链表B不再单独存在。
输入中的单链表的长度仅作为建表使用，不得在该算法中利用。
参考函数原型：
template<class ElemType>
void Linklist_Contact( LinkList<ElemType> &A, LinkList<ElemType> &B );


输入说明 :

第一行：单链表A的长度
第二行：单链表A的数据元素（数据元素之间以空格分隔）
第三行：单链表B的长度
第四行：单链表B的数据元素（数据元素之间以空格分隔）

输出说明 :

第一行：单链表A的遍历结果
第二行：单链表B的遍历结果
第三行：连接后单链表A的遍历结果

（输入与输出之间以空行分隔）

输入范例 :


10
13 5 27 9 32 123 76 98 54 87
5
1 3 7 8 11
输出范例 :


13 5 27 9 32 123 76 98 54 87 
1 3 7 8 11 
13 5 27 9 32 123 76 98 54 87 1 3 7 8 11 
*/
template<class ElemType>
void Linklist_Contact(LinkedList<ElemType> &A, LinkedList<ElemType> &B) {
	typename LinkedList<ElemType>::Node * node = A.getNode(0);
	typename LinkedList<ElemType>::Node *lastNodeOfA, *firstNodeOfB = B.getNode(0);

	while (node->next)
		node = node->next;
	lastNodeOfA = node;

	lastNodeOfA->next = firstNodeOfB;
	firstNodeOfB->previous = lastNodeOfA;
	A.setSize(A.getSize() + B.getSize());
}


/**
* Exercise 9
* 应用3：假设线性表A=(a1,a2,...,am)，线性表B=(b1,b2,...,bn)，现要求设计一个算法，使用带头结点的单链表存储结构，
按照下列规则合并A、B为线性表C。即：A和B的元素间隔排列，且使用A和B的原存储空间，且B不再单独存在。输入中的单链表的长度不
得在该算法中利用，仅作为建表使用。
参考函数原型：
template<class ElemType>
void Merge_L( LinkList<ElemType> &A, LinkList<ElemType> &B );

输入说明 :

第一行：单链表A的长度
第二行：单链表A的数据元素（数据元素之间以空格分隔）
第三行：单链表B的长度
第四行：单链表B的数据元素（数据元素之间以空格分隔）

输出说明 :

第一行：单链表A的遍历结果
第二行：单链表B的遍历结果
第三行：交叉归并后单链表A的遍历结果

（输入与输出之间以空行分隔）

输入范例 :


10
13 5 27 9 32 123 76 98 54 87
5
1 3 7 8 11
输出范例 :


13 5 27 9 32 123 76 98 54 87 
1 3 7 8 11 
13 1 5 3 27 7 9 8 32 11 123 76 98 54 87 

*/
template<class T>
void Merge_L(LinkedList<T> &A, LinkedList<T> &B) {
	typename LinkedList<T>::Node *node = A.getNode(0), *nodeOfB = B.getNode(0), *tmpNode, *tmpNode1;

	while (node->next) {
		tmpNode = node->next;

		if (nodeOfB) {
			tmpNode1 = nodeOfB->next;
			node->next = nodeOfB;
			nodeOfB->previous = node;
			nodeOfB->next = tmpNode;
			tmpNode->previous = nodeOfB;
			nodeOfB = tmpNode1;
		}
		else
			break;

		node = tmpNode;
	}

	if (nodeOfB) {
		node->next = nodeOfB;
		nodeOfB->previous = node;
	}

	A.setSize(A.getSize() + B.getSize());
}

/**
* Exercise 10
* 应用4：假设线性表 A和B，分别由单链表 La和Lb 存储，其中数据元素按值非递减有序排列（即已经有序），现要求设计一个算法，
将A和B归并为一个线性表 , 其数据元素仍按值非递减排列。要求使用A和B的原存储空间，且B不再单独存在。输入中的单链表的长度不得
在归并算法中利用，仅作为建表使用。
参考函数原型：
template<class ElemType>
void Merge_L_Order( LinkList<ElemType> &A, LinkList<ElemType> &B );

输入说明 :

第一行：单链表A的长度
第二行：单链表A的数据元素（数据元素之间以空格分隔）
第三行：单链表B的长度
第四行：单链表B的数据元素（数据元素之间以空格分隔）

输出说明 :

第一行：单链表A的遍历结果
第二行：单链表B的遍历结果
第三行：归并后单链表A的遍历结果

输入范例 :


4
3 5 8 11
5
2 6 8 9 11
输出范例 :


3 5 8 11 
2 6 8 9 11 
2 3 5 6 8 8 9 11 11 
*/
template<class T>
void Merge_L_Order(LinkedList<T> &A, LinkedList<T> &B) {
	try {
		typename LinkedList<T>::Node *nodeA = A.getNode(0), *nodeB = B.getNode(0), *tmpNode;

		if (B.get(0) < A.get(0)) {
			A.setFirstNode(nodeB);

			while (nodeB->item < nodeA->item) {
				nodeB = nodeB->next;
			}

			//让B前部分中比A的first node还小的node的最后一个接在A的first node的前面
			nodeB->previous->next = nodeA;
			nodeA->previous = nodeB->previous;

			//这时候的nodeB->item >= nodeA->item
		}

		while (nodeB) {
			while (nodeA->next && nodeB->item > nodeA->item) {
				nodeA = nodeA->next;
			}

			if (!(nodeA->next)) {
				if (nodeB->item > nodeA->item) {
					//将nodeB接在last node of A的后面
					nodeA->next = nodeB;
					nodeB->previous = nodeA;

					return;
				}
			}

			//这时候如果nodeB->item <= nodeA->item就回溯到nodeA上一个, 使得正好nodeA->item < nodeB->item
			if (nodeB->item <= nodeA->item) {
				//这时候nodeB->item <= nodeA->item且nodeA->previous->item < nodeB->item
				//所以将nodeB插入在nodeA和nodeA->previous之间
				nodeA->previous->next = nodeB;
				nodeB->previous = nodeA->previous;
				tmpNode = nodeB->next;
				nodeB->next = nodeA;
				nodeA->previous = nodeB;
				nodeA = nodeB;
				nodeB = tmpNode;
			}
	
		}

		A.setSize(A.getSize() + B.getSize());
	}
	catch (std::length_error) {
		cout << "getNode的参数错误." << endl;

		//可能是有个list是空的, check
		if (A.getSize() == 0) {
			A.setFirstNode(B.getNode(0));
			return;
		}
		else if (B.getSize() == 0)
			return;
		else
			exit(-1);
	}
}

/**
* Exercise 11
应用5：试设计一个算法，删除有序单链表A中的冗余元素，即使得操作之后的单链表中只保留操作之前表中所有值都不相同的元素，
并保持其有序性。要求利用原表中的结点，并释放A表中冗余的结点空间。
参考函数原型：
template<class ElemType>
void Purge_Lk_OL( LinkList<ElemType> &A );

输入说明 :
第一行：单链表A的长度
第二行：单链表A的数据元素（数据元素之间以空格分隔）

输出说明 :
第一行：单链表A的遍历结果
第二行：提纯后单链表A的遍历结果

（输入与输出之间以空行分隔）

输入范例 :
7
3 3 5 5 8 11 11
输出范例 :
3 3 5 5 8 11 11
3 5 8 11
*/
template<class T>
void Purge_Lk_OL(LinkedList<T> &list) {
	try {
		typename LinkedList<T>::Node *node = list.getNode(0), *last = list.getNode(0);

		int cnt = 0;

		if (!(node->next)) {
			//说明只有一个元素在list
			return;
		}

		node = node->next;
		while (node) {
			if (node->item != last->item) {
				//将node接在last的后面
				last->next = node;
				node->previous = last;
				last = node;
				cnt++;
			}

			if (!(node->next)) {
				last->previous->next = node;
				node->previous = last->previous;
				list.setSize(++cnt);
				return;
			}

			node = node->next;
		}

		list.setSize(++cnt);
	}
	catch (length_error) {
		cout << "getNode function call error" << endl;
	}
}

/**
* Exercise 12
应用6：假设2个线性表分别由单链表A和B存储，其中数据元素按值非递减有序排列（即已经有序），现要求设计一个算法，
将A和B归并为一个线性表 , 其数据元素仍按值非递减排列，且表中不存在值相同的元素。要求使用A和B的原存储空间，且B不再单独存在。
输入中的单链表的长度不得在归并算法中利用，仅作为建表使用。
参考函数原型：
template<class ElemType>
void Merge_L_Pure_Order( LinkList<ElemType> &A, LinkList<ElemType> &B );

输入说明 :
第一行：有序单链表A的长度
第二行：有序单链表A的数据元素（数据元素之间以空格分隔）
第三行：有序单链表B的长度
第四行：有序单链表B的数据元素（数据元素之间以空格分隔）

输出说明 :
第一行：单链表A的遍历结果
第二行：单链表B的遍历结果
第三行：归并提纯后单链表A的遍历结果

（输入与输出之间以空行分隔）

输入范例 :
8
3 3 5 5 8 8 11 11
7
2 2 6 8 8 9 11
输出范例 :
3 3 5 5 8 8 11 11
2 2 6 8 8 9 11
2 3 5 6 8 9 11

*/
template<class T>
void Merge_L_Pure_Order(LinkedList<T> &A, LinkedList<T> &B) {
	Purge_Lk_OL(A);
	Purge_Lk_OL(B);
	Merge_L_Order(A, B);
	Purge_Lk_OL(A);
}

/**
* Exercise 13
应用1：假设2个线性表分别由带头结点的循环链表A和B存储。现要求设计一个算法，将A和B归并为一个线性表 。
要求例如循环链表的特点，使用A和B的原存储空间，且合并后B成为一空表。
输入中的单链表的长度不得在归并算法中利用，仅作为建表使用。
参考函数原型：
template<class ElemType>
void Merge_Cur_Linklist( CirLinkList<ElemType> &A, CirLinkList<ElemType> &B );

输入说明 :
第一行：循环链表A的长度
第二行：循环链表A的数据元素（数据元素之间以空格分隔）
第三行：循环链表B的长度
第四行：循环链表B的数据元素（数据元素之间以空格分隔）

输出说明 :
第一行：循环链表A的遍历结果
第二行：循环链表B的遍历结果
第三行：归并后循环链表A的遍历结果

输入范例 :
3
1 2 3
4
5 6 7 8
输出范例 :
1 2 3
5 6 7 8
1 2 3 5 6 7 8
*/
template<class ElemType>
void Merge_Cur_Linklist(LinkedList<ElemType> &A, LinkedList<ElemType> &B) {
	//特殊情况处理
	if (B.getSize() == 0)
		return;

	if (A.getSize() == 0) {
		A.setFirstNode(B.getNode(0));
		return;
	}

	//循环链表就是环状链表嘛, 就是把tail的next指向head就OK了
	//这里将这两个DDList伪造成两个环状链表
	//这里面调用的getSize()不计入题目限定条件中, 只作为环状链表构建的代码
	//虽然这里空链表直接在上面就return不会生成环状链表了
	A.getNode(A.getSize() == 0 ? 0 : (A.getSize() - 1))->next = A.getNode(0);
	B.getNode(B.getSize() == 0 ? 0 : (B.getSize() - 1))->next = B.getNode(0);

	typename LinkedList<ElemType>::Node *tailA = A.getNode(0), *headA = A.getNode(0),
		*tailB = B.getNode(0), *headB = B.getNode(0);

	//找到A的tail node
	while (tailA->next != headA) {
		tailA = tailA->next;
	}

	//找到B的tail node
	while (tailB->next != headB) {
		tailB = tailB->next;
	}

	tailA->next = headB;
	tailB->next = headA;
}

/**
* Exercise 13
* 用于遍历输出伪环状链表的函数
*/
template<typename T>
void traversePseudoCircleLinkedList(LinkedList<T> &pseudoCircleLinkedList) {
	typename LinkedList<T>::Node *head = pseudoCircleLinkedList.getNode(0), *node = pseudoCircleLinkedList.getNode(0);

	while (node->next != head) {
		cout << node->item << " ";
		node = node->next;
	}

	cout << node->item << " " << endl;
}

/**
* Exercise 14
应用1：假设2个稀疏一元多项式分别由带头结点的有序单链表A和B存储（指数项递增有序）。现要求设计一个算法，
实现稀疏一元多项式的加减法计算。要求使用A和B的原存储空间，且计算后B不再单独存在。
输入中的单链表的长度不得在计算算法中利用，仅作为建表使用。
注意：加/减法计算后，如某一项的结果系数为0，则该项要从多项式链表中删除。
参考函数原型：
template<class ElemType1，class ElemType2>
void Add_Poly( poly_LinkList<ElemType> &A, poly_LinkList<ElemType> &B, int add_sub );

输入说明 :
第一行：加/减法选择（0：加法  1：减法）
第二行：一元多项式A的项数
第三行：一元多项式A的各项的系数（系数之间以空格分隔）
第四行：一元多项式A的各项的指数（指数之间以空格分隔）
第五行：一元多项式B的项数
第六行：一元多项式B的各项的系数（系数之间以空格分隔）
第七行：一元多项式B的各项的指数（指数之间以空格分隔）

输出说明 :
第一行：多项式A的第一项的系数、指数（以空格分隔）
第一行：多项式A的第二项的系数、指数（以空格分隔）
...
第n行：多项式A的第n项的系数、指数（以空格分隔） （假设多项式A的项数为n）

第一行：多项式B的第一项的系数、指数（以空格分隔）
第一行：多项式B的第二项的系数、指数（以空格分隔）
...
第m行：多项式B的第m项的系数、指数（以空格分隔） （假设多项式B的项数为m）

第一行：加/减法计算后，结果多项式A的第一项的系数、指数（以空格分隔）
第一行：加/减法计算后，结果多项式A的第二项的系数、指数（以空格分隔）
...
第p行：加/减法计算后，结果多项式A的第n项的系数、指数（以空格分隔） （假设结果多项式的项数为p）

（输入与输出之间以空行分隔，多项式之间以空行分割）

输入范例 :
1
6
7 3 -22 9 5 -8
0 1 7 8 17 100
3
8 22 -9
1 7 8
输出范例 :
7 0
3 1
-22 7
9 8
5 17
-8 100

8 1
22 7
-9 8

7 0
-5 1
-44 7
18 8
5 17
-8 100

*/

//Ex 14辅助类
class CoefficientAndExponent {
public:
	int coefficient = 0;
	int exponent = 0;

	CoefficientAndExponent() {

	}

	CoefficientAndExponent(int coefficient) {
		this->coefficient = coefficient;
	}

	CoefficientAndExponent(int coefficient, int exponent) {
		this->coefficient = coefficient;
		this->exponent = exponent;
	}

	~CoefficientAndExponent() {

	}
};

//operate中0表示加法, 1表示减法
void Add_Poly(LinkedList<CoefficientAndExponent> &A, LinkedList<CoefficientAndExponent> &B, bool operate) {
	//法二: 用STL set容器, 如果有已存在的值的时候就进行运算

	//priint
	typename LinkedList<CoefficientAndExponent>::Node *nodeA = A.getNode(0), *nodeB = B.getNode(0), *tmpNode;

	while (nodeA) {
		cout << (nodeA->item).coefficient << " " << (nodeA->item).exponent << " " << endl;
		nodeA = nodeA->next;
	}

	cout << endl;

	while (nodeB) {
		cout << (nodeB->item).coefficient << " " << (nodeB->item).exponent << " " << endl;
		nodeB = nodeB->next;
	}

	cout << endl;

	nodeA = A.getNode(0), nodeB = B.getNode(0);

	//记录A中添加的元素的个数
	int cnt = 0;
	
	if (B.get(0)->exponent < A.get(0)->exponent) {
		A.setFirstNode(nodeB);

		while (nodeB->next && (nodeB->item).exponent < (nodeA->item).exponent) {
			cnt++;
			//如果是减去就系数取负
			if (operate)
				(nodeB->item).coefficient = -(nodeB->item).coefficient;
			nodeB = nodeB->next;
		}

		//如果nodeB是最后一个元素了而且nodeB的指数比nodeA的指数还小
		if (nodeB->next && (nodeB->item).exponent < (nodeA->item).exponent) {
			cnt++;
			//如果是减去就系数取负
			if (operate)
				(nodeB->item).coefficient = -(nodeB->item).coefficient;

			nodeA->previous->next = nodeB;
			nodeB->previous = nodeA->previous;
			nodeB->next = nodeA;
			nodeA->previous = nodeB;
			A.setSize(A.getSize() + cnt);

			//print new list
			nodeA = A.getNode(0);

			while (nodeA) {
				cout << (nodeA->item).coefficient << " " << (nodeA->item).exponent << " " << endl;
				nodeA = nodeA->next;
			}
			return;
		}

		//让B前部分中比A的first node还小的node的最后一个接在A的first node的前面
		nodeB->previous->next = nodeA;
		nodeA->previous = nodeB->previous;

		//这时候的nodeB->item >= nodeA->item
	}

	while (nodeB) {
		while (nodeA->next && (nodeB->item).exponent > (nodeA->item).exponent) {
			nodeA = nodeA->next;
		}

		//如果到了A的最后一个元素了
		if (!(nodeA->next)) {
			if ((nodeB->item).exponent > (nodeA->item).exponent) {
				//将nodeB接在last node of A的后面
				nodeA->next = nodeB;
				nodeB->previous = nodeA;

				while (nodeB) {
					cnt++;
					if (operate)
						(nodeB->item).coefficient = -(nodeB->item).coefficient;
					nodeB = nodeB->next;
				}

				A.setSize(A.getSize() + cnt);

				//print new list
				nodeA = A.getNode(0);

				while (nodeA) {
					cout << (nodeA->item).coefficient << " " << (nodeA->item).exponent << endl;
					nodeA = nodeA->next;
				}
				return;
			}
			else if ((nodeB->item).exponent == (nodeA->item).exponent) {
				(nodeA->item).coefficient = operate ? ((nodeA->item).coefficient - (nodeB->item).coefficient)
					: ((nodeA->item).coefficient + (nodeB->item).coefficient);

				if ((nodeA->item).coefficient == 0) {
					nodeB = nodeB->next;
					nodeA->previous->next = nodeB;
					nodeB->previous = nodeA->previous;
					nodeB->next = nodeA->next;
					nodeA->next->previous = nodeB;
					cnt--;
					delete nodeA;
				}
				else {
					//应该不用这两行被注释的东西
					//nodeA->next = nodeB;
					//nodeB->previous = nodeA;
					nodeB = nodeB->next;
				}

				while (nodeB) {
					cnt++;
					if (operate)
						(nodeB->item).coefficient = -(nodeB->item).coefficient;
					nodeB = nodeB->next;
				}

				A.setSize(A.getSize() + cnt);

				//print new list
				nodeA = A.getNode(0);

				while (nodeA) {
					cout << (nodeA->item).coefficient << " " << (nodeA->item).exponent << endl;
					nodeA = nodeA->next;
				}
				return;
			}
		}

		//这时候如果nodeB->item < nodeA->item就回溯到nodeA上一个, 使得正好nodeA->item < nodeB->item
		if ((nodeB->item).exponent < (nodeA->item).exponent) {
			//这时候nodeB->item < nodeA->item且nodeA->previous->item < nodeB->item
			//所以将nodeB插入在nodeA和nodeA->previous之间
			if (operate)
				(nodeB->item).coefficient = -(nodeB->item).coefficient;

			nodeA->previous->next = nodeB;
			nodeB->previous = nodeA->previous;
			tmpNode = nodeB->next;
			nodeB->next = nodeA;
			nodeA->previous = nodeB;
			nodeA = nodeB;
			nodeB = tmpNode;
			cnt++;
		}
		else if ((nodeB->item).exponent == (nodeA->item).exponent) {
			(nodeA->item).coefficient = operate ? ((nodeA->item).coefficient - (nodeB->item).coefficient)
				: ((nodeA->item).coefficient + (nodeB->item).coefficient);

			if ((nodeA->item).coefficient == 0) {
				tmpNode = nodeA->next;
				nodeA->previous->next = nodeA->next;
				nodeA->next->previous = nodeA->previous;
				cnt--;
				delete nodeA;
				nodeA = tmpNode;
			}
				
			nodeB = nodeB->next;
		}

	}

	A.setSize(A.getSize() + cnt);

	//print new list
	nodeA = A.getNode(0);

	while (nodeA) {
		cout << (nodeA->item).coefficient << " " << (nodeA->item).exponent << endl;
		nodeA = nodeA->next;
	}
}

/**
* Exercise 15
* 应用：假设2个任意长度的整数x、y分别由带头结点的单链表A和B存储，现要求设计一个算法，实现任意长的整数进行加法运算。
参考函数原型：
template<class ElemType>
void Long_Int_Add( LinkList<ElemType> &A, LinkList<ElemType> &B, string &result, const int &len_A, const int &len_B );

辅助函数原型：
（1）从长整数的低位开始拆分（4位为一组，即不超过9999的非负整数），依次存放在单链表的每个结点的数据域中；
头结点的数据域存放正负数标志（正数或0：1，负数：-1）。
template<class ElemType> 
void Input_Int_Division( LinkList<ElemType> &L, string &str, int &length );  
（length：长整数分割后的block数，恰为存储用单链表的长度）
（2）计算结果中间位格式控制
string Int_String( int result );
（3）两个长整数的绝对值大小比较（x>y 返回值为1；x<y 返回值为2；x=y 返回值为0；）
template<class ElemType>
int Two_LongNum_Compare( LinkList<ElemType> &A, LinkList<ElemType> &B, const int &len_A, const int &len_B );
（4）单链表ADT基本操作：单链表的逆置（实际计算时，鉴于单链表的顺序查找的特性，存储在单链表中的长整数需逆置，
由原始的高位到低位排列，逆置为低位到高位降序排列。）
template<class ElemType>
void LinkList<ElemType>::ListReverse();

输入说明 :

第一行：长整数x
第二行：长整数y
输出说明 :

第一行：格式化后的长整数x（从低位到高位每4位用空格分开）
第二行：格式化后的长整数y（从低位到高位每4位用空格分开）
第三行：空行
第四行：格式化后的计算结果（从低位到高位每4位用空格分开）

（输入与输出之间用一空行分隔）
输入范例 :


-53456467576846547658679870988098
435643754856985679
输出范例 :


-5345 6467 5768 4654 7658 6798 7098 8098
43 5643 7548 5698 5679

-5345 6467 5768 4611 2014 9250 1400 2419

*/
void Long_Int_Add(LinkedList<int> &A, LinkedList<int> &B, string &result, const int &len_A, const int &len_B) {
	//compare absolute value
	//0表示绝对值相等, 1表示|A| > |B|, -1表示|A| < |B|
	int compare = 0;
	if (len_A > len_B) {
		compare = 1;
	}
	else if (len_A < len_B) {
		compare = -1;
	}
	else {
		//一样长就开始从前往后比较大小
		typename LinkedList<int>::Node *nodeA = A.getNode(1), *nodeB = B.getNode(1);

		while (nodeA && nodeA->item == nodeB->item) {
			nodeA = nodeA->next;
			nodeB = nodeB->next;
		}

		if (!nodeA)
			compare = 0;
		else
			compare = nodeA->item > nodeB->item ? 1 : (nodeA->item < nodeB->item ? -1 : 0);
	}

	//calculate
	switch (compare)
	{
	case 1:
	{
		typename LinkedList<int>::Node *nodeA = A.getNode(A.getSize() - 1), *nodeB = B.getNode(B.getSize() - 1);
		//|A| > |B|
		if ( *(A.get(0)) * (*(B.get(0))) == -1) {
			//异号, 减法, 也就是nodeA.get(0) * (|A| - |B|)
			while (nodeB->previous) {
				if (nodeA->item < nodeB->item) {
					//向前借位
					//这种情况不可能发生在nodeA(1)

					//如果前面是0, 一直往前借位
					typename LinkedList<int>::Node *nodeTmp = nodeA->previous;
					while (!(nodeTmp->item)) {
						nodeTmp->item = 9999;
						nodeTmp = nodeTmp->previous;
					}
					--(nodeTmp->item);

					if (nodeTmp->item == 0 && !(nodeTmp->previous->previous)) {
						//一种很特殊的情况， 也就是正好向第一位借位之后变成了0
						nodeTmp->previous->next = nodeTmp->next;
						//delete tmp1;
					}
					nodeA->item = nodeA->item + 10000 - nodeB->item;
				}
				else {
					nodeA->item = nodeA->item - nodeB->item;
				}
				nodeA = nodeA->previous;
				nodeB = nodeB->previous;
			}
		}
		else {
			//同号就是简单的加法(其实并不简单...)
			while (nodeB->previous) {
				int tmp = nodeA->item + nodeB->item;

				typename LinkedList<int>::Node *nodeAtmp = nodeA;

				while (tmp / 10000) {
					//向前进位
					if (!(nodeAtmp->previous->previous)) {
						//如果是node1, 就在node0后面插入一个进位node
						A.add(0, tmp / 10000);
						nodeAtmp->item = tmp % 10000;
						tmp /= 10000;
					}
					else {
						nodeAtmp->item = tmp % 10000;
						nodeAtmp->previous->item += (tmp / 10000);
						tmp = nodeAtmp->previous->item;
					}
					nodeAtmp = nodeAtmp->previous;
				}

				if (nodeAtmp->previous)
					//tmp <= 9999
					nodeAtmp->item = tmp;

				nodeA = nodeA->previous;
				nodeB = nodeB->previous;
			}
		}
		break;
	}
	case 0:
	{
		if ( (*(A.get(0))) * (*(B.get(0))) == -1) {
			//异号且绝对值相等
			result = "\n0";
			return;
		}
		else {
			//times 2
			typename LinkedList<int>::Node *nodeA = A.getNode(A.getSize() - 1);

			while (nodeA->previous) {
				int tmp = nodeA->item * 2;
				while (tmp / 10000) {
					//向前进位
					if (!(nodeA->previous->previous)) {
						//如果是node1, 就在node0后面插入一个进位node
						A.add(0, tmp / 10000);
						nodeA->item = tmp % 10000;
						tmp /= 10000;
					}
					else {
						nodeA->item = tmp % 10000;
						nodeA->previous->item += (tmp / 10000);
						tmp = nodeA->previous->item;
					}
					nodeA = nodeA->previous;
				}

				if (!(nodeA->previous))
					break;

				//tmp <= 9999
				nodeA->item = tmp;

				nodeA = nodeA->previous;
			}
		}
		break;
	}
	case -1:
	{
		typename LinkedList<int>::Node *nodeA = A.getNode(A.getSize() - 1), *nodeB = B.getNode(B.getSize() - 1);
		//|A| < |B|
		if ( (*(A.get(0))) * (*(B.get(0))) == -1) {
			//异号, 减法, 也就是nodeA.get(0) * (|A| - |B|)
			while (nodeA->previous) {
				if (nodeB->item < nodeA->item) {
					//向前借位
					//这种情况不可能发生在nodeA(1)
					//如果前面是0, 一直往前借位
					typename LinkedList<int>::Node *nodeTmp = nodeB->previous;
					while (!(nodeTmp->item)) {
						nodeTmp->item = 9999;
						nodeTmp = nodeTmp->previous;
					}
					--(nodeTmp->item);
					if (nodeTmp->item == 0 && !(nodeTmp->previous->previous)) {
						//一种很特殊的情况， 也就是正好向第一位借位之后变成了0
						nodeTmp->previous->next = nodeTmp->next;
						//delete tmp1;
					}
					nodeB->item = nodeB->item + 10000 - nodeA->item;
				}
				else {
					nodeB->item = nodeB->item - nodeA->item;
				}
				nodeB = nodeB->previous;
				nodeA = nodeA->previous;
			}

			A = B;
		}
		else {
			//同号就是简单的加法(其实并不简单...)
			while (nodeB->previous) {
				int tmp = nodeA->item + nodeB->item;

				typename LinkedList<int>::Node *nodeAtmp = nodeA;

				while (tmp / 10000) {
					//向前进位
					if (!(nodeAtmp->previous->previous)) {
						//如果是node1, 就在node0后面插入一个进位node
						A.add(0, tmp / 10000);
						nodeAtmp->item = tmp % 10000;
						tmp /= 10000;
					}
					else {
						nodeAtmp->item = tmp % 10000;
						nodeAtmp->previous->item += (tmp / 10000);
						tmp = nodeAtmp->previous->item;
					}
					nodeAtmp = nodeAtmp->previous;
				}

				if (nodeAtmp->previous)
					//tmp <= 9999
					nodeAtmp->item = tmp;

				nodeA = nodeA->previous;
				nodeB = nodeB->previous;
			}
		}
	}
	default:
		break;
	}

	//generate result string (in A)
	typename LinkedList<int>::Node *node = A.getNode(1);

	cout << endl;

	if ( (*A.getFirst()) == -1) {
		result += "-";
	}


	//去掉前面的0
	while (node && node->item == 0) {
		node = node->next;
	}

	if (node)
	{

		if (node->next) {
			char fuck[5];
			snprintf(fuck, sizeof(fuck), "%d", node->item);
			result += fuck;
			result += " ";
			//(result += to_string(node->item)) += " ";
			node = node->next;
		}
		else
		{
			return;
		}
		
	}
	else {
		result += "0";
		return;
	}

	while (node->next) {
		char tmp2[6];
		int fuck = node->item;
		//debug
		//printf("%d\n", fuck);
		//辣鸡sprintf()毁一生, 有时候正常有时候不正常 
		snprintf(tmp2, sizeof(tmp2), "%4.4d", fuck);

		string str = string(tmp2);
		result += str;
		result += " ";
		node = node->next;
	}

	if (node) {
		char tmp2[6];
		int fuck = node->item;
		//debug
		//辣鸡sprintf()毁一生, 有时候正常有时候不正常 
		snprintf(tmp2, sizeof(tmp2), "%4.4d", fuck);
		//debug
		//printf("%s\n", tmp);
		string str = string(tmp2);
		result += str;
	}

}

/**
* Ex16
问题描述 :

目的：使用C++模板设计循环链表的抽象数据类型（ADT）。并在此基础上，使用循环链表ADT的基本操作，设计并实现单链表的简单算法设计。
内容：（1）请使用模板设计循环链表的抽象数据类型。（由于该环境目前仅支持单文件的编译，故将所有内容都集中在一个源文件内。在实际的设计中，推荐将抽象类及对应的派生类分别放在单独的头文件中。参考网盘中的单链表ADT原型文件，自行设计循环链表的ADT。）

（2）ADT的简单应用：使用该ADT设计并实现循环链表应用场合的一些简单算法设计。
应用2：编号为1,2,...,n的n个人按顺时针方向围坐一圈，每人持有一个密码（正整数）。现在给定一个随机数m>0，从编号为1的人开始，按顺时针方向1开始顺序报数，报到m时停止。报m的人出圈，同时留下他的密码作为新的m值，从他在顺时针方向上的下一个人开始，重新从1开始报数，如此下去，直至所有的人全部出列为止。请在使用循环链表ADT的基础上，设计一个算法求出圈的顺序（以编号表示）。限定人数至少为1.
参考函数原型：
template<class ElemType>
void Joseph(CirLinkList<ElemType> &A, int m);

//约瑟夫环专用结点类型
struct node{
int number;
int code;
};
输入说明 :

第一行：人数n
第二行：第一个人所持的密码
第三行：第二个人所持的密码
...
第n+1行：第n个人所持的密码
第n+2行：给定的随机数m
输出说明 :

第一行至第n行：建立的循环链表的遍历结果（一个结点占据1行）
第n+1行：空行
第n+2行：出圈的顺序（编号与编号之间以“->”分隔）
输入范例 :


7
3
8
1
22
4
9
15
5
输出范例 :


1 3
2 8
3 1
4 22
5 4
6 9
7 15

5->2->6->7->4->3->1
*/
class Person {
public:
	int index;
	int key;

	Person() {

	}

	Person(int index, int key) {
		this->index = index;
		this->key = key;
	}
};

void joseph(LinkedList<Person>& list, int m) {
	//循环链表化
	list.getNode(list.getSize() == 0 ? 0 : (list.getSize() - 1))->next = list.getNode(0);
	list.getNode(0)->previous = list.getNode(list.getSize() == 0 ? 0 : (list.getSize() - 1));

	//记录报号数
	int cnt = 1;

	LinkedList<Person>::Node *node = list.getNode(0);

	while (list.getSize() > 0) {
		if (cnt == m) {
			node->previous->next = node->next;
			node->next->previous = node->previous;
			cout << node->item.index;
			m = node->item.key;

			list.setSize(list.getSize() - 1);
			if (list.getSize() != 0)
				cout << "->";

			//LinkedList<Person>::Node *tmp = node->next;
			//free
			//delete node;

			//node = tmp;

			node = node->next;

			cnt = 1;
		}
		else
		{
			cnt++;
			node = node->next;
		}
	}
}