#pragma once

/**
* @author DCMMC
* created on 2017/10/08
*/

#include "sqlist.h"

/*
* Exercise 1
* @param m 1 <= m <= ListLength
* 注释 by DCMMC: 就是把顺序表的前m元素整体移动到表的最后一个元素的后面, 要求: 用尽可能少的辅助空间
*
* 题目描述:
* 目的：使用C++模板设计顺序表的抽象数据类型（ADT）。并在此基础上，使用顺序表ADT的基本操作，设计并实现顺序表的简单算法设计。
* 内容：
* （1）请使用模板设计顺序表的抽象数据类型。（由于该环境目前仅支持单文件的编译，故将所有内容都集中在一个源文件内。
* 在实际的设计中，推荐将抽象类及对应的派生类分别放在单独的头文件中。参考网盘中的ADT原型文件。）
* （2）ADT的简单应用：使用该ADT设计并实现顺序表应用场合的一些简单算法设计。
*
* 应用1：试设计一个算法，用尽可能少的辅助空间将顺序表中前 m 个元素和后 n 个元素进行互换，即将线性表(a1,a2,…,am,b1,b2,…,bn)
* 改变成(b1,b2,…,bn,a1,a2,…,am)。
* 参考函数原型：template<class ElemType>
* void Exchange( SqList<ElemType> &A,int m )；// 本算法实现顺序表中前 m 个元素和后 n 个元素的互换
*
* 输入说明 :
* 第一行：待处理顺序表的长度
* 第二行：待处理顺序表的数据元素（数据元素之间以空格分隔）
* 第三行：逆置位置m
*
* 输出说明 :
* 第一行：待处理顺序表的遍历结果
* 第二行：逆置结果
*
*
*（输入与输出之间以空行分隔）
*
* 输入范例 :
* 10
* 13 5 27 9 32 123 76 98 54 87
* 5
* 输出范例 :
* 13 5 27 9 32 123 76 98 54 87
* 123 76 98 54 87 13 5 27 9 32
*/
template<class T>
void Exchange(SqList<T> &list, int m) {
	//headIndex和midIndex分别为待处理的List的部分的前部分和后部分的第一个元素的位置(1 <= index <= ListLength)
	//整个函数只需要这三个辅助变量再加上一个i
	int headIndex = 1, midIndex = m + 1;
	T aux = list.GetElem(1);

	//额外判断
	if (m == list.ListLength())
		return;

	while (true)
	{
		//计算并判断这两部分的长度
		if (midIndex - headIndex < list.ListLength() - midIndex + 1)
		{
			//如果前部分小于后部分的长度
			//例如 "1 2" "3 4 5 6 7", 就置换成3 4 "1 2" "5 6 7", headIndex和midIndex都会依次递增
			for (int i = 0; i < midIndex - headIndex; i++)
			{
				Exchange<T>(list, headIndex++, midIndex++, aux);
			}
		}
		else if (midIndex - headIndex > list.ListLength() - midIndex + 1)
		{
			//如果前部分大于后部分的长度
			//例如 "1 2 3 4 5" "6 7", 就置换成6 7 "3 4 5" "1 2", 只有headIndex会依次递增
			for (int i = 0; i < list.ListLength() - midIndex + 1; i++)
			{
				Exchange<T>(list, headIndex++, midIndex + i, aux);
			}
		}
		else
		{
			//两部分长度相等
			for (int i = 0; i < midIndex - headIndex; i++)
			{
				Exchange<T>(list, headIndex++, midIndex++, aux);
			}

			return;
		}
	}
}

/*
* Ex1 辅助函数, 用于交换指定两个位置的元素
* @param index0 元素的位置 1 <= index0 <= ListLength
* @param index1 元素的位置 1 <= index0 <= ListLength
* @param auxTmp 暂存元素
*/
template<class T>
inline void Exchange(SqList<T> &list, int index0, int index1, T auxTmp) {
	auxTmp = list.GetElem(index0);
	list.SetElem(index0, list.GetElem(index1));
	list.SetElem(index1, auxTmp);
}

/**
* 题目2
 
应用2：试设计一个算法，实现两个顺序表A、B大小的比较。若 A<B，则返回 -1；若 A=B，则返回 0；若 A>B，则返回 1。
     参考函数原型：template<class ElemType>
                              int ListCompare( SqList<ElemType> &A,SqList<ElemType> &B )；
 
输入说明 :
第一行：顺序表A的长度
第二行：顺序表A的数据元素（数据元素之间以空格分隔）
第三行：顺序表B的长度
第四行：顺序表B的数据元素（数据元素之间以空格分隔）
 
输出说明 :
第一行：比较结果

（输入与输出之间以空行分隔）
输入范例 :
10
13 5 27 9 32 123 76 98 54 87
8
13 5 27 9 32 164 5 8
输出范例 :
13 5 27 9 32 123 76 98 54 87 
13 5 27 9 32 164 5 8 
-1

*/
template<class T>
int ListCompare(SqList<T> &list0, SqList<T> &list1) {
	if (list0.ListLength() == 0) {
		if (list0.ListLength() == list1.ListLength())
			return 0;
		else
			return -1;
	}
	else if (list1.ListLength() == 0) {
		if (list0.ListLength() == list1.ListLength())
			return 0;
		else
			return 1;
	}
		
	for (int i = 0; i < (list0.ListLength() < list1.ListLength() ? list0.ListLength() : list1.ListLength()); i++) {
		if (list0.GetElem(i + 1) != list1.GetElem(i + 1)) {
			return list0.compare(list0.GetElem(i + 1), list1.GetElem(i + 1));
		}
	}

	return 0;
}

/**
* Exercise 3
* TODO 优化效率
* 应用3：试设计一个算法，删除顺序表L中的冗余元素，即使得操作之后的顺序表中只保留操作之前表中所有值都不相同的元素（提纯）。
            参考函数原型：template<class ElemType>
                                     void Purge_Sq( SqList<ElemType> &L )；
 
输入说明 :
第一行：待处理顺序表的长度
第二行：待处理顺序表的数据元素（数据元素之间以空格分隔）
 
输出说明 :
第一行：待处理顺序表的遍历结果
第二行：提纯结果

（输入与输出之间以空行分隔）
 
输入范例 :
10
13 5 13 9 32 51 76 5 9 8
输出范例 :
13 5 13 9 32 51 76 5 9 8 
13 5 9 32 51 76 8 
*/
template<class T>
void Purge_Sq(SqList<T> &list) {
	//暴力解法
	//时间O(N^2) 空间O(N)
	//当前存入最后一个数据的下标
	int index = -1;
	T * results = new T[list.ListLength()];
	for (int i = 1; i <= list.ListLength(); i++)
	{
		T tmp = list.GetElem(i);
		bool flag = false;
		for (int j = index; j > -1; j--)
		{
			if (list.equal(results[j], &tmp))
			{
				flag = true;
				break;
			}
		}
		if (!flag)
			results[++index] = tmp;
	}

	list.SetListLength(index + 1);
	for (int i = 0; i <= index; i++)
	{
		list.SetElem(i + 1, results[i]);
	}
}

/**
* Exercise 4
* 应用4：试设计一个算法，将所有在线性表LB中但不在LA中的数据元素插入到LA中,算法执行之后，线性表LB不再存在。
             参考函数原型：template<class ElemType>
                                      void Union_Sq( SqList<ElemType> &LA,SqList<ElemType> &LB )；
 
输入说明 :
第一行：待处理顺序表LA的长度
第二行：待处理顺序表LA的数据元素（数据元素之间以空格分隔）
第三行：待处理顺序表LB的长度
第四行：待处理顺序表LB的数据元素（数据元素之间以空格分隔）
 
输出说明 :
第一行：顺序表LA的遍历结果
第二行：顺序表LB的遍历结果
第三行：合并后顺序表LA的遍历结果
 
输入范例 :
10
13 5 27 9 32 123 76 98 54 87
10
13 5 13 9 32 51 76 5 54 8
输出范例 :
13 5 27 9 32 123 76 98 54 87 
13 5 13 9 32 51 76 5 54 8 
13 5 27 9 32 123 76 98 54 87 51 8 
*/
template<class ElemType>
void Union_Sq(SqList<ElemType> &LA, SqList<ElemType> &LB)
{
	//暴力解法
	//时间O(n * m)
	//LA中最后一个元素的index
	int lastElemIndex = LA.ListLength();
	for (int i = 1; i <= LB.ListLength(); i++)
	{
		bool flag = false;
		for (int j = 1; j <= LA.ListLength(); j++)
		{

			if (LA.GetElem(j) == LB.GetElem(i))
			{
				flag = true;
				break;
			}
		}
		if (!flag) {
			LA.SetElem(++lastElemIndex, LB.GetElem(i));
			LA.SetListLength(lastElemIndex);
		}
	}

	LB.~SqList();

	//优化版本: 先把LA中的数据快速排序, 然后最LB中每一个元素用二分查找
	//时间 ~ 1.93 * NlogN + MlogN
}

/**
* Exercise 5
* 应用5：试设计一个算法，删除有序顺序表L中的冗余元素，即使得操作之后的顺序表中只保留操作之前表中所有值都不相同的元素，
并保持其有序性。
参考函数原型：
template<class ElemType>
void Purge_Sq_OL( SqList<ElemType> &L )；

输入说明 :
第一行：有序顺序表的长度
第二行：有序顺序表的数据元素（数据元素之间以空格分隔）


输出说明 :
第一行：有序顺序表的遍历结果
第二行：提纯后有序顺序表的遍历结果

（输入与输出之间用空行分隔）

输入范例 :
8
2 3 3 5 5 5 7 11
输出范例 :
2 3 3 5 5 5 7 11 
2 3 5 7 11 

*/
template<class ElemType>
void Purge_Sq_OL(SqList<ElemType> &list)
{
	//提纯的元素个数
	int cnt = 1;
	//上一个(提纯后的)元素的值
	ElemType last = list.GetElem(1);
	for (int i = 2; i <= list.ListLength(); i++)
	{
		if (list.GetElem(i) != last)
		{
			list.SetElem(++cnt, list.GetElem(i));
			last = list.GetElem(i);
		}
	}

	list.SetListLength(cnt);
}

/**
* Exercise 6
应用6：假设以两个元素依值递增有序排列的线性表A和B分别表示两个集合（即同一表中的元素值各不相同），现要求设计一个算法，
另辟空间构成一个线性表C，其元素为A和B中元素的交集，且C中的元素也依值递增有序排列。
参考函数原型：template<class ElemType>
void Intersect_Sq_OL_C( const SqList<ElemType> &A, const SqList<ElemType> &B, SqList<ElemType> &C )；

输入说明 :
第一行：有序顺序表A的长度
第二行：有序顺序表A的数据元素（数据元素之间以空格分隔）
第三行：有序顺序表B的长度
第四行：有序顺序表B的数据元素（数据元素之间以空格分隔）

输出说明 :
第一行：顺序表A的遍历结果
第二行：顺序表B的遍历结果
第三行：顺序表C的遍历结果

输入范例 :
5
1 3 5 7 9
8
1 2 3 4 5 6 9 11
输出范例 :
1 3 5 7 9
1 2 3 4 5 6 9 11
1 3 5 9

*/
template<class ElemType>
void Intersect_Sq_OL_C(const SqList<ElemType> &A, const SqList<ElemType> &B, SqList<ElemType> &C)
{
	//二分查找 
	//O(min(n, m) * log(max(n, m)))
	if (A.ListLength() > B.ListLength())
	{
		int cnt = 0;
		for (int i = 1; i <= B.ListLength(); i++)
		{
			if (binarySearch(A, B.GetElem(i), 1, A.ListLength()) != -1)
			{
				C.SetElem(++cnt, B.GetElem(i));
			}
		}
		C.SetListLength(cnt);
	}
	else
	{
		int cnt = 0;
		for (int i = 1; i <= A.ListLength(); i++)
		{
			if (binarySearch(B, A.GetElem(i), 1, B.ListLength()) != -1)
			{
				C.SetElem(++cnt, A.GetElem(i));
			}
		}
		C.SetListLength(cnt);
	}
}

//二分查找
template<class T>
int binarySearch(const SqList<T> &list, T elem, int lo, int hi) {
	if (lo > hi)
		//没找到
		return -1;

	int mid = lo + (hi - lo) / 2;
	if (list.compare(elem, list.GetElem(mid)) < 0)
		return binarySearch(list, elem, lo, mid - 1);
	else if (list.compare(elem, list.GetElem(mid)) > 0)
		return binarySearch(list, elem, mid + 1, hi);
	else
		return mid;
}