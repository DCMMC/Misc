#pragma once

#include "BinaryTree.h"
#include <string>

using namespace std;

/**
* Tree的练习
* @author DCMMC
* Create on 2017/11/25
*/

/**
* Binary Tree ADT Ex 1 & 2
* 基本操作1：二叉树的二叉链表存储形式的建立，完成后将其加入到二叉树的ADT基本操作集中。
要求设计一个递归算法，将二叉树转化为二叉链表的存储形式。
初始条件：definition给出二叉树T的定义（先序序列。无孩子或指针为空的情形，算法通过特殊分隔符识别（输入））。
输出：按definition构造二叉树的二叉链表。
注意：由于测试数据的显示需建立在二叉树的遍历基础上。因此，请在设计好二叉树的三种遍历算法之后（基本操作2），再进行测试。
*/
bool operate(const char c) {
	cout << c << " ";

	return true;
}

bool operateString(const string s) {
	cout << s << " ";

	return true;
}

bool operateHuffmanWeight(HuffmanItem<char> h) {
	cout << h.weight << " ";

	return true;
}

void ADT_Tree_Ex1(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);
	tree.preOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.inOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.sufOrderRecursiveTraverseOp(operate);
}

//Ex 5
void ADT_Tree_Ex5(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	char toFind = 0;
	cin >> toFind;
	char *parentItem = tree.getParentItem(toFind);

	if (parentItem)
		cout << *parentItem << endl;
	else
		cout << "NULL" << endl;
}

//Ex 3
void ADT_Tree_Ex3(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	tree.layerTraverse(operate);
}

//Ex 6
void ADT_Tree_Ex6(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	char toFind = 0;
	cin >> toFind;
	char *item = tree.findItem(toFind);

	if (item)
		cout << *item << endl;
	else
		cout << "NULL" << endl;
}

//Ex 7
void ADT_Tree_Ex7(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	char item = 0;
	cin >> item;

	cout << tree.getBinaryTreeHeight(item) << endl;

	tree.~BinaryTree();

}


//Ex 8
void ADT_Tree_Ex8(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	char item = 0;
	cin >> item;
	int flag = 0;
	cin >> flag;

	char *result = tree.getChildItem(item, flag);

	if (result)
		cout << *result << endl;
	else
		cout << "NULL" << endl;
}

//Ex 9
void ADT_Tree_Ex9(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);

	char item = 0;
	cin >> item;
	int flag = 0;
	cin >> flag;

	cout << (tree.childTreeDestroy(item, flag) ? "true" : "false") << endl;
	tree.preOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.inOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.sufOrderRecursiveTraverseOp(operate);
	cout << endl;
}


//Ex 10
void ADT_Tree_Ex10(BinaryTree<char> &tree, BinaryTree<char> &subTree) {
	char item = 0;
	bool flag = 0;

	cin >> item >> flag;

	tree.preOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.inOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.sufOrderRecursiveTraverseOp(operate);
	cout << "\n" << endl;

	subTree.preOrderRecursiveTraverseOp(operate);
	cout << endl;
	subTree.inOrderRecursiveTraverseOp(operate);
	cout << endl;
	subTree.sufOrderRecursiveTraverseOp(operate);
	cout << "\n" << endl;

	if (tree.ex10(subTree, item, flag)) {
		cout << "success" << endl;
		tree.preOrderRecursiveTraverseOp(operate);
		cout << endl;
		tree.inOrderRecursiveTraverseOp(operate);
		cout << endl;
		tree.sufOrderRecursiveTraverseOp(operate);
		cout << endl;
	}
	else
		cout << "failed" << endl;
}

//Ex 11
void ADT_Tree_Ex11(const char notation, const char *str) {
	char item = 0, newValue = 0;

	cin >> item >> newValue;

	BinaryTree<char> tree(str, notation);
	tree.preOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.inOrderRecursiveTraverseOp(operate);
	cout << endl;
	tree.sufOrderRecursiveTraverseOp(operate);
	cout << "\n" << endl;

	if (tree.replaceItem(item, newValue)) {
		tree.preOrderRecursiveTraverseOp(operate);
		cout << endl;
		tree.inOrderRecursiveTraverseOp(operate);
		cout << endl;
		tree.sufOrderRecursiveTraverseOp(operate);
		cout << endl;
	}
	else
		cout << "false" << endl;
}

//Ex 16
void ADT_Tree_Ex16(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);
	cout << tree.getSize() << endl;
}


//Ex 19
void ADT_Tree_Ex19(const char notation, const char *str) {
	BinaryTree<char> tree(str, notation);
	cout << tree.getBinaryTreeHeight(tree.getRootNodeItem()) << endl;
}