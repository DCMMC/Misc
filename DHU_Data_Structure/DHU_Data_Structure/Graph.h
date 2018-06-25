#pragma once

#ifndef Graph

#include <vector>
#include <set>
#include <utility>
#include <stdexcept>
#include "UnionFind.h"
#include "MinPQ.h"
#include "Queue.h"
#include "Stack.h"

// ! 不推荐的做法, 在 lib 中尽量不要使用 using, 这样会污染 include 了
// ! 该头文件的源文件的命名空间
using namespace std;

//图论

//图的邻接表ADT
//VerT和EdgeT分别表示顶点存储的值和边存储的值的类型
template <typename VerT = int, typename EdgeT = bool>
class AdjListGraph {
private:
	/* Inner classes */
	//弧结点
	template<typename T = bool>
	struct ArcNode {
		//该弧所指向的顶点的位置
		//从0开始
		int adjVer;
		//下一条弧的结点指针
		ArcNode *nextArc;
		//弧上的权重
		T weight;

		ArcNode(int adjVer, T weight, ArcNode<T> *nextArc = nullptr) {
			this->adjVer = adjVer;
			this->nextArc = nextArc;
			this->weight = weight;
		}
	};

	//顶点结点
	//T表示顶点的值的类型, E表示边上值的类型
	template<typename T = int, typename E = bool>
	struct VerNode {
		//顶点序号
		int index;
		//顶点的值
		T data;
		//指向第一条依附于该顶点的弧的指针
		ArcNode<E> *firstArc;

		VerNode(int index, T data, ArcNode<E> *firstArc = nullptr) {
			this->index = index;
			this->data = data;
			this->firstArc = firstArc;
		}
	};

	/* Fields */
	//邻接表, 存储着各个顶点
	vector<VerNode<VerT, EdgeT> *> *adjList;
	//顶点和边的个数
	int verNum, arcNum;

public:
	//图的种类 DG（有向图）, DN（有向网）, UDG（无向图）, UDN（无向网）
	enum graphTypes: int { DG, DN, UDG, UDN };

private:
	//图的类型
	graphTypes kind;
	//记录是否被访问过
	vector<bool> visited;

	////////////// 用于 Kruskal 最小生成树算法 ///////////////////////
	vector< pair< pair<int, int>, EdgeT> > weightPairs;

public:

	/* constructors */
	//constructor an empty graph
	AdjListGraph() {
		verNum = 0;
		arcNum = 0;
		kind = DN;
		adjList = nullptr;
	}

	/**
	* 构造一个只有顶点没有边的图(四种图都有可能)
	* @param verSize 顶点个数
	* @param type 图的类型
	* @param vers 顶点集合
	*/
	AdjListGraph(const graphTypes type, const vector<VerT> vers) {
		kind = type;
		adjList = new vector< VerNode<VerT, EdgeT> *>();
		for (typename vector<VerT>::const_iterator itr = vers.begin(); itr != vers.end(); itr++)
			adjList->push_back(new VerNode<VerT>(adjList->size(), *itr));
		verNum = adjList->size();
		arcNum = 0;
	}

	/**
	* 从邻接矩阵和顶点集构造一个图或网(UDG或者DG或者UDN或者DN)
	* 如果edges是bool数组, 则表示无权图(UDG或DG)
	************** UNCHECKED: 不会校验edges是否是edges[vers.size][vers.size] ***********************
	* @param vers 顶点集合
	* @param eSize 边的条数
	* @param edges 边的邻接矩阵
	* @param disconnectedNotation 邻接矩阵中表示未连接的特殊值, 如果是无权图(i.e. bool 数组)中常用0表示为未
	* 连接, 而 EdgeT为int时常用 INT_MAX来表示
	*/
	AdjListGraph(const vector<VerT> vers, const int eSize, const EdgeT **edges,
		const EdgeT &disconnectedNotation) {
		arcNum = eSize;
		adjList = new vector< VerNode<VerT, EdgeT> *>();
		for (typename vector<VerT>::iterator itr = vers.begin(); itr != vers.end(); itr++)
			adjList->push_back(new VerNode<VerT, EdgeT>(adjList->size(), *itr));
		verNum = adjList->size();
		ArcNode<EdgeT> *lastArc = nullptr;
		//先把kind假定为 UDG或UDN
		if (typeid(EdgeT) == typeid(bool))
			kind = UDG;
		else
			kind = UDN;
		for (int i = 0; i < verNum; i++) {
			for (int j = 0; j < verNum; j++) {
				if (i < j && edges[i][j] != edges[j][i]) 
				{
					if (kind == UDG)
						kind = DG;
					else
						kind = DN;
				}
				if (edges[i][j] != disconnectedNotation)
				{
					//arcNum++;
					if (!lastArc) {
						adjList[i]->firstArc = new ArcNode<EdgeT>(i, edges[i][j]);
						lastArc = adjList[i]->firstArc;
					}
					else {
						lastArc->nextArc = new ArcNode<EdgeT>(i, edges[i][j]);
						lastArc = lastArc->nextArc;
					}
				}
			}
			lastArc = nullptr;
		}
	}

	/**
	* 从给定的顶点集和边集构造一个图或网
	* @param type 图的类型
	* @param vers 顶点集
	* @param edges 边集合(如果是有向图, 每条边用弧尾的顶点的index和弧头的顶点的index表示, e.g. <0, 2> 
	* 表示一条顶点0指向顶点2的弧), 注意: 边集合必须按照顺序来, 先是所有顶点0为弧尾的边, 然后顶点1, 依次来
	*/
	AdjListGraph(const graphTypes type, const vector<VerT> vers, const vector< pair<int, int> > edges,
		const vector<EdgeT> weights = vector<EdgeT>() ) {
		//check type
		if (weights.size() == 0 && edges.size() != 0) {
			//UDG或DG
			if (type == UDN || type == DN) {
				/////// throw an exception ////////
				cerr << "error graph type" << endl;
				verNum = 0;
				arcNum = 0;
				adjList = nullptr;
				return;
			}
			else
				kind = type;
		}
		else
		{
			//UDN或DN
			if (type == UDG || type == DG) {
				/////////// throw an exception ////////
				cerr << "error graph type" << endl;
				verNum = 0;
				arcNum = 0;
				adjList = nullptr;
				return;
			}
			else
				kind = type;
		}
		
		//check weight
		if ((kind == UDN || kind == DN) && weights.size() != edges.size()) {
			///////// throw an exception ///////
			cerr << "size of weights must equals to size of edges" << endl;
			verNum = 0;
			arcNum = 0;
			adjList = nullptr;
			return;
		}

		verNum = vers.size();
		arcNum = edges.size();
	
		adjList = new vector< VerNode<VerT, EdgeT> *>();
		for (typename vector<VerT>::const_iterator itr = vers.begin(); itr != vers.end(); itr++)
			adjList->push_back(new VerNode<VerT, EdgeT>(adjList->size(), *itr));
		//顶点的迭代器
		typename vector< VerNode<VerT, EdgeT> *>::iterator iVer = adjList->begin();
		//权的迭代器
		typename vector<EdgeT>::const_iterator iWeight = weights.begin();
		//上一条不同弧尾顶点的边的弧头顶点的index
		int lastTailIndex = -1;
		for (typename vector< pair<int, int> >::const_iterator itr = edges.begin();
			itr != edges.end(); itr++) {
			if (lastTailIndex != (*itr).first) {
				if (lastTailIndex != -1)
					iVer = adjList->begin() + (*itr).first;
				lastTailIndex = (*itr).first;
			}

			// 插入 边和顶点对
			weightPairs.push_back(pair< pair<int, int>, EdgeT>(*itr, *iWeight));
			
			//将边插入在该顶点的firstArc之前
			ArcNode<EdgeT> *node = new ArcNode<EdgeT>((*itr).second, (kind == UDN || kind == DN)
				? *iWeight++ : 0, (*iVer)->firstArc);
			(*iVer)->firstArc = node;
	
			//如果是无向图, 还需要反过来插入另外一个方向
			if (kind == UDG || kind == UDN) {
				node = new ArcNode<EdgeT>((*iVer)->index, (kind == UDN || kind == DN)
					? *(iWeight - 1) : 0, ((*adjList)[(*itr).second])->firstArc);
				((*adjList)[(*itr).second])->firstArc = node;
			}
		}
	}

	////////////////// TODO //////////////////////
	~AdjListGraph() {
		verNum = 0;
		arcNum = 0;
	}

	//判断是否为空
	bool isEmpty() const {
		return !verNum & !arcNum;
	}

	//返回边的个数
	int getArcNum() const {
		return arcNum;
	}

	//返回顶点的个数
	int getVerNum() const {
		return verNum;
	}

	//返回图的类型
	graphTypes getGraphType() const {
		return kind;
	}

	string getGrapgTypeString() const {
		switch (kind)
		{
		case graphTypes::DG:
			return "DG";
		case graphTypes::DN:
			return "DN";
		case graphTypes::UDG:
			return "UDG";
		case graphTypes::UDN:
			return "UDN";
		default:
			return "unknown graph type";
		}
	}

	string getGraphTypeName() const {
		switch (kind)
		{
		case UDG: return "UDG";
		case DG: return "DG";
		case UDN: return "UDN";
		case DN: return "DN";
		default:
			return "error kind of graph";
		}
	}

	//返回顶点集(顺序和输入的时候的顺序一致)
	//虽然这里vers是local variable, 存储在stack上, 不过返回的时候会先复制一个vers用来返回并且销毁原来的vers
	//所以一定不用retuan a local object with reference, 不然将会返回一个被销毁的对象的引用
	vector<VerT> getVertexs() const {
		vector<VerT> vers;
		for (typename vector<VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); itr != adjList->end(); itr++) {
			vers.push_back((*itr)->data);
		}
		return vers;
	}

	//按照顶点集的顺序输出每个顶点的邻接表(其中包括弧头的顶点的index和弧的权重(如果是无权图就是false)
	vector< vector< pair<int, EdgeT> > > getAdjLists() const {
		vector< vector< pair<int, EdgeT> > > adjLists;
		for (typename vector< VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); itr != adjList->end(); itr++) {
			ArcNode<EdgeT> *arc = (*itr)->firstArc;
			vector< pair<int, EdgeT> > list;
			while (arc) {
				list.push_back( pair<int, EdgeT>(arc->adjVer, arc->weight));
				arc = arc->nextArc;
			}
			adjLists.push_back(list);
		}
		return adjLists;
	}

	//添加一个顶点
	AdjListGraph<VerT, EdgeT> &addVertex(const VerT t) {
		adjList->push_back(new VerNode<VerT, EdgeT>(adjList->size(), t));
		return *this;
	}

	//返回顶点的index
	//如果没有找到则返回-1
	int getVertexIndex(VerT v) const {
		//遍历
		int i = 0;
		for (typename vector< VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); itr != adjList->end(); itr++, i++) {
			if (v == (*itr)->data)
				return i;
		}
		return -1;
	}

	//删除顶点和相关联的边
	VerT *removeVertexByIndex(const int index) {
		if (index < 0 || index >= adjList->size())
			////////////// throw an exception ///////////////
			return nullptr;

		VerT *removedT = nullptr;
		vector< VerNode<VerT, EdgeT> *> *newAdjList = new vector< VerNode<VerT, EdgeT> *>();
		//遍历删除与之相关联的边
		for (typename vector< VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); 
				itr != adjList->end(); itr++) {
			if (itr - adjList->begin() != index) {
				if ((*itr)->index > index)
					((*itr)->index)--;
				ArcNode<EdgeT> *arc = (*itr)->firstArc, *lastArc = nullptr;
				while (arc) {
					if (arc->adjVer == index) {
						if (lastArc) {
							lastArc->nextArc = arc->nextArc;
						}
						else
						{
							(*itr)->firstArc = arc->nextArc;
						}
						lastArc = arc;
						arc = arc->nextArc;
						delete arc;
						if (kind == DG || kind == DN)
							arcNum--;
						continue;
					}
					else if (arc->adjVer > index)
						(arc->adjVer)--;
					
					lastArc = arc;
					arc = arc->nextArc;
				}
				newAdjList->push_back(*itr);
			}
			else
			{
				removedT = &((*itr)->data);
				ArcNode<EdgeT> *arcTmp = (*itr)->firstArc, *tmp;
				while (arcTmp) {
					arcNum--;
					tmp = arcTmp;
					arcTmp = arcTmp->nextArc;
					delete tmp;
				}
				//remove vertex
				delete (*itr);
				verNum--;
			}
		}

		adjList = newAdjList;
		return removedT;
	}


	//返回G中指定顶点(index为base的顶点)的下一个邻接顶点的位序 
	//在有向图中就是就是邻接表中链表的下一个指向的顶点
	//而在无向图中, 还需要额外判断是否是 u -> v 且 v-> u的情况, 也就是是否就是同一条边的情况
	int getNextAdjVerIndex(const int verIndex, const int baseIndex = -1) const {
		if (verIndex < 0 || verIndex >= adjList->size())
			return -1;
		
		VerNode<VerT, EdgeT> *node = (*adjList)[verIndex];
		if (baseIndex == -1) {
			// 这里的嵌套判断有点麻烦, 因为无向图必须还要考虑是否是同一条边的情况
			return node->firstArc 
				? node->firstArc->adjVer 
				: -1;
		}
		ArcNode<EdgeT> *arc = node->firstArc;
		while (arc) {
			if (arc->adjVer == baseIndex)
				return arc->nextArc 
				? arc->nextArc->adjVer
				: -1;
			arc = arc->nextArc;
		}
		return -1;
	}

	//插入一条无权的边
	bool addArc(const int tailIndex, const int headIndex) {
		return addArc(tailIndex, headIndex, false);
	}

	//插入一条边
	bool addArc(const int tailIndex, const int headIndex, const EdgeT t) {
		if (tailIndex < 0 || headIndex < 0 || tailIndex >= adjList->size() || headIndex >= adjList->size()) {
			return false;
		}

		//判断是否已经存在
		ArcNode<EdgeT> *arc = (*adjList)[tailIndex]->firstArc;
		bool duplicate = false;
		while (arc) {
			if (arc->adjVer == headIndex) {
				duplicate = true;
				break;
			}
			arc = arc->nextArc;
		}

		if (duplicate)
			return false;

		arcNum++;
		arc = new ArcNode<EdgeT>(headIndex, (kind == UDG || kind == DG) ? false : t, 
			(*adjList)[tailIndex]->firstArc);
		(*adjList)[tailIndex]->firstArc = arc;
		if (kind == UDG || kind == UDN) {
			arc = new ArcNode<EdgeT>(tailIndex, (kind == UDG || kind == DG) ? false : t,
				(*adjList)[headIndex]->firstArc);
			(*adjList)[headIndex]->firstArc = arc;
		}
		return true;
	}

	//删除边
	bool removeArc(const int tail, const int head) {
		//check
		if (tail < 0 || head < 0 || tail >= verNum || head >= verNum) {
			return false;
		}

		bool success = false;
		ArcNode<EdgeT> *arc = (*adjList)[tail]->firstArc, *last = nullptr;
		while (arc) {
			if (arc->adjVer == head) {
				if (last) {
					last->nextArc = arc->nextArc;
				}
				else
				{
					(*adjList)[tail]->firstArc = arc->nextArc;
				}
				delete arc;
				arcNum--;
				success = true;
				break;
			}
			last = arc;
			arc = arc->nextArc;
		}
		

		if (kind == UDG || kind == UDN) {
			arc = (*adjList)[head]->firstArc;
			last = nullptr;
			while (arc) {
				if (arc->adjVer == tail) {
					if (last) {
						last->nextArc = arc->nextArc;
					}
					else
					{
						(*adjList)[head]->firstArc = arc->nextArc;
					}
					delete arc;
					break;
				}
				last = arc;
				arc = arc->nextArc;
			}
		}

		return success;
	}

	//DFS
	void DFSTrverse(bool(*op)(VerT &t), const int beginIndex = 0) {
		visited = vector<bool>(verNum, false);

		for (int v = beginIndex; v < verNum + beginIndex; v++) {
			if (!visited[v % verNum])
				DFS(v % verNum, op);
		}
	}

	void DFS(const int verIndex, bool (*op)(VerT &t)) {
		visited[verIndex] = true;
		op((*adjList)[verIndex]->data);
		for (int w = getNextAdjVerIndex(verIndex); w >= 0; w = getNextAdjVerIndex(verIndex, w)) {
			if (!visited[w])
				DFS(w, op);
		}
	}

	vector<VerT> DFSTraverse(const int beginIndex = 0) {
		visited = vector<bool>(verNum, false);
		vector<VerT> result;

		for (int v = beginIndex; v < verNum + beginIndex; v++) {
			if (!visited[v % verNum])
				DFS(v % verNum, result);
		}
		return result;
	}

	void DFS(const int verIndex, vector<VerT> &vec) {
		visited[verIndex] = true;
		vec.push_back((*adjList)[verIndex]->data);
		for (int w = getNextAdjVerIndex(verIndex); w != -1; w = getNextAdjVerIndex(verIndex, w)) {
			if (!visited[w])
				DFS(w, vec);
		}
	}

	//BFS
	void BFSTraverse(bool(*op)(VerT &t), const int beginIndex = 0) {
		visited = vector<bool>(verNum, false);
		Queue<int> q;

		for (int v = beginIndex; v < verNum + beginIndex; v++) {
			if (!visited[v % verNum]) {
				visited[v % verNum] = true;
				q.enqueue(v % verNum);
				op((*adjList)[v]->data);
				while (!q.isEmpty()) {
					int u = q.dequeue();
					for (int w = getNextAdjVerIndex(u); w != -1; w = getNextAdjVerIndex(u, w)) {
						if (!visited[w]) {
							visited[w] = true;
							op((*adjList)[w]->data);
							q.enqueue(w);
						}
					}
				}
			}
		}
	}

	vector<VerT> BFSTraverse(const int beginIndex = 0) {
		visited = vector<bool>(verNum, false);
		Queue<int> q;
		vector<VerT> result;

		for (int v = beginIndex; v < verNum + beginIndex; v++) {
			if (!visited[v % verNum]) {
				visited[v % verNum] = true;
				q.enqueue(v % verNum);
				result.push_back((*adjList)[v]->data);
				while (!q.isEmpty()) {
					int u = q.dequeue();
					for (int w = getNextAdjVerIndex(u); w != -1; w = getNextAdjVerIndex(u, w)) {
						if (!visited[w]) {
							visited[w] = true;
							result.push_back((*adjList)[w]->data);
							q.enqueue(w);
						}
					}
				}
			}
		}

		return result;
	}

	//获得弧 <tail, head> 的weight, 返回nullptr表示未找到或者有问题
	EdgeT *getWeight(const int tail, const int head) const {
		if (tail < 0 || head < 0 || tail >= verNum || head >= verNum || kind == UDG || kind == DG) {
			return nullptr;
		}

		ArcNode<EdgeT> *arc = (*adjList)[tail]->firstArc;
		while (arc) {
			if (arc->adjVer == head) {
				return &(arc->weight);
			}
			arc = arc->nextArc;
		}
		return nullptr;
	}

	//判断是否有 u->v的弧(边)
	bool isArcAdjVers(const int u, const int v) const {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum)
			return false;

		ArcNode<EdgeT> *arc = (*adjList)[u]->firstArc;
		while (arc) {
			if (arc->adjVer == v)
				return true;
			arc = arc->nextArc;
		}
		return false;
	}

	//两个顶点是否是邻接顶点 (u -> v 或 v -> u)
	bool isAdjVers(const int u, const int v) const {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum)
			return false;

		return isArcAdjVers(u, v) || isArcAdjVers(v, u);
	}

	//出度
	int getOutDegree(const int index) const {
		if (index < 0 || index >= verNum)
			return -1;
		int outDegree = 0;
		ArcNode<EdgeT> *arc = (*adjList)[index]->firstArc;
		while (arc) {
			outDegree++;
			arc = arc->nextArc;
		}
		return outDegree;
	}

	//入度
	int getInDegree(const int index) const {
		if (index < 0 || index >= verNum || kind == UDG || kind == UDN)
			return -1;
		int inDegreee = 0;
		for (typename vector< VerNode<VerT, EdgeT> *>::iterator i = adjList->begin(); i != adjList->end(); i++) {
			ArcNode<EdgeT> *arc = (*i)->firstArc;
			while (arc) {
				if (arc->adjVer == index)
					inDegreee++;
				arc = arc->nextArc;
			}
		}
		return inDegreee;
	}

	//是否有路径
	bool hasPath(const int u, const int v) {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum)
			return false;

		visited = vector<bool>(verNum, false);
		vector<VerT> result;

		DFS(u, result);
		for (typename vector<VerT>::iterator i = result.begin(); i != result.end(); i++)
			if ((*i) == (*adjList)[v]->data)
				return true;
		return false;
	}

	// 基于BFS的拓扑排序(有向无环图)
	// sorted用来存储拓扑排序后的序列
	// 返回false说明有向图中存在回路
	bool topologicalSort(vector<VerT> &sorted) const {
		if (kind == UDG || kind == UDN) {
			//必须要有向图
			return false;
		}

		//sorted must empty
		if (sorted.size() > 0)
			return false;

		//存储inDegree为0的顶点的index
		Queue<int> q;
		vector<int> inDegrees = vector<int>(verNum, 0);
		for (int i = 0; i < verNum; i++) {
			if ((inDegrees[i] = getInDegree(i)) == 0)
				q.enqueue(i);
		}
		int cnt = 0;
		
		while (!q.isEmpty()) {
			int t = q.dequeue();
			sorted.push_back((*adjList)[t]->data);
			++cnt;
			//将所有q.dequeue指向的顶点的入度-1, 如果入度为0, 则入栈
			for (ArcNode<EdgeT> *arc = (*adjList)[t]->firstArc; arc; arc = arc->nextArc) {
				if (!(--inDegrees[arc->adjVer]))
					q.enqueue(arc->adjVer);
			}
		}

		//如果cnt小于顶点数, 说明有向图有环
		if (cnt < verNum)
			return false;
		else
			return true;
	}

	//图（网）中回路的判断
	bool hasCircle() const {
		if (kind == DN || kind == DG) {
			//有向图 
			return topologicalSort(vector<VerT>());
		}

		//无向图的回路判断麻烦一点, 不过类似于拓扑排序, 采用DFS
		//首先用visitedColor数组储存每一个顶点的状态信息, 其中 'W' 白色表示顶点未被访问, 'G' 灰色表示还在队列
		//中, 'B' 黑色表示已经被访问过了并且出队列了
		vector<char> visitedColor = vector<char>(verNum, 'W');
		//邻近结点有三种可能：黑（'B'）、白（'W'）、灰（'G'）。若为黑色结点，
		//说明是DFS序列之前已访问过的，忽略不管（避免重复访问）；若是白色结点则入队，
		//颜色染灰（'G'）；若有灰色结点则表明该结点已存在于队列中，说明有回路
	
		for (int i = 0; i < verNum; i++) {
			if (visitedColor[i] == 'W' && DFS_Circle(i, visitedColor))
				return true;
		}

		return false;
	}

private:
	/**
	* 用来做无向图有环判断的DFS的递归函数
	* @param last 递归的上次调用的时候的i, 用来判断是否是因为无向图的双向性, 再次指向了上一个递归调用的时候用的i
	* @param i 递归开始的顶点的序号
	* @param visitedColor 用来保存没一个顶点的状态信息的vector
	* @return 如果有环路, i.e. 遇到了下一个邻接顶点就是该DFS路径中的结点的话, 就返回true, 否则false
	*/
	bool DFS_Circle(const int i, vector<char> &visitedColor, const int last = -1) const {
		visitedColor[i] = 'G';
		int w = getNextAdjVerIndex(i);
		if (w == -1)
			return false;
		for (; w != -1; w = getNextAdjVerIndex(i, w)) {
			// 上次递归调用就是 last -> i 所以w一定不能是last
			if ( (visitedColor[w] == 'W' && DFS_Circle(w, visitedColor, i)) 
				|| (visitedColor[w] == 'G' && w != last))
				return true;
			visitedColor[i] = 'B';
		}
		return false;
	}

	// 类三元组, 第三个元素保存序号, 仅仅用于 小组作业6
	// P.S. DHU OJ 什么时候支持 C++11 啊, 用二元组来当三元组用真麻烦
	typedef pair< pair<pair<int, int>, EdgeT>, int> WeightPairIndex;

	/**
	* Kruskal 算法辅助类
	* 用于快速排序, 按照边的权值由小到大排序
	*/
	static int compareWeightPairs(const WeightPairIndex &i, const WeightPairIndex &j) {
		return i.first.second - j.first.second;
	}

	template <typename T>
	static void exch(vector<T> &a, const int &i, const int &j) {
		T tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}

	/**
	* 原地分区(In-place Partition)
	* @param a
	*		要分区的数组
	* @param lo
	*		分区范围的下界
	* @param hi
	*		分区范围的上界
	*/
	template <typename T>
	static int partition(vector<T> &a, int lo, int hi) {
		T pivotkey = a[lo];

		while (lo < hi) {
			while (lo < hi && compareWeightPairs(a[hi], pivotkey) >= 0)
				--hi;
			exch<T>(a, lo, hi);
			while (lo < hi && compareWeightPairs(a[lo], pivotkey) <= 0)
				++lo;
			exch<T>(a, lo, hi);
		}

		return lo;
	}

public:
	/**
	* 快速排序指定部分
	* @param a
	* 		要排序的数组
	* @param lo
	*		要排序的范围的下界
	* @param hi
	* 		要排序的范围的上界
	*/
	template <typename T>
	static void quickSort(vector<T> &a, int lo, int hi) {
		if (lo >= hi)
			return;

		//分区的分界位置
		int j = partition<T>(a, lo, hi);

		quickSort<T>(a, lo, j - 1);
		quickSort<T>(a, j + 1, hi);
	}

	/**
	* 快速排序
	* 优点: 实现比较简单, 每一轮的比较次数都是固定的N + 1, 时间复杂度和空间复杂度都相当优秀, 虽然Quicksort在一般情况下的比较
	* 次数(1.39N logN)大于mergesort()(1/2N logN ~ N logN), 不过Quicksort数据移动(exch)的次数相当少, 所以相对来说Quicksort会更加快.
	* 缺点: 很多小的细节容易导致严重的性能损失, 有时候甚至达到了N^2的时间复杂度
	* 算法分析:
	* 在最理想情况, 每一次j都是正好在subarray的中间位置, 也就是每次都能二分, 这样和mergesort一模一样, 时间复杂度为 ~ NlogN
	*
	* Quicksort平均使用 ~ 2NlnN的比较次数(以及1/6的的交换)
	* Prooof.
	* 在最理想情况, 每一次j都是正好在subarray的中间位置, 也就是每次都能二分, 这样和mergesort一模一样, 时间复杂度为 ~ NlogN
	*
	* Quicksort排序N个不同的数字平均使用 ~ 2NlogN (`!$\dot {=} 1.39N \log N$`)的比较次数(以及1/6的的交换)
	*
	**Prooof.**
	*
	* 设`!$C_N$`为排序N个(分散的)items所需要的平均比较次数, 易得`!$C_0 = C_1 = 0$`, 而且对于`!$N > 1$`, 有一下递归关系:
	*
	* ```mathjax!
	* $$C_N = N + 1 + \frac {\left(C_0 + C_1 + \cdot \cdot \cdot + C_{N - 2} + C_{N - 1} \right)} {N} +
	* \frac {\left(C_{N - 1} + C_{N - 2} + \cdot \cdot \cdot + C_{1} + C_{0} \right)} {N}$$
	* ```
	*
	* N + 1是每一轮排序的固定比较次数, 第二部分是排序left subarray的平均比较次数, 第三部分是排序right subarray的平均比较次数.
	*
	* 又将`!$C_N$`与`!$C_{N - 1}$`两式相减, 得到`!$NC_N = 2N + (N + 1)C_{N - 1}$`, 左右同除以N(N + 1), 得到
	* `!$\frac {C_N} {N + 1} = \frac {2} {N + 1} + \frac {C_{N - 1}} {N}$`, 令`!$\lambda_N =  \frac {C_N} {N + 1}$`,
	* 所以递推得到
	*
	* `!$C_N = 2(N + 1) \cdot \left( \sum_{i = 3}^{N + 1} \frac {1} {i} \right)$` ~ `!$2N\ln N$`
	*
	* 证毕.
	*
	* > 交换次数的证法与上面类似不过更加复杂.
	*
	* > 对于有重复数值的情况, 准确的分析复杂很多, 不过不难表明平均比较次数不大于`!$C_N$`, 后面将会有对这种情况的优化.
	*
	* @param a
	* 		要排序的数组
	*/
	template <typename T>
	static void quickSort( vector<T> &a ) {
		quickSort<T>(a, 0, a.size() - 1);
	}


	/**
	* 确认图是否是(强)连通图, 利用 BFS 原理
	* @return 如果是非(强)连通图, 就返回 false
	*/
	bool isConnectedGraph(const int v = 0) {
		visited = vector<bool>(verNum, false);

		Queue<int> q;
		int visitedCnt = 0;

		// 就是对顶点 v 进行BFS, 如果被遍历到的顶点小于图的总顶点个数, 就说明是非(强)连通图.
		q.enqueue(v);
		visited[v] = true;
		while (!q.isEmpty()) {
			int u = q.dequeue();
			visitedCnt++;
			int w = getNextAdjVerIndex(u);
			if (w != -1) {
				do {
					if (!visited[w]) {
						q.enqueue(w);
						visited[w] = true;
					}
				} while ((w = getNextAdjVerIndex(u, w)) != -1);
			}
		}

		return visitedCnt == verNum;
	}

	/**
	* 最小生成树 -- Kruskal
	* @param mstTree 最小生成树的代价
	* @return 图非法则返回 false
	*/
	bool miniSpanTreeKruskal(vector< pair<pair<VerT, VerT>, EdgeT> > &mstTree) {
		// 判断图是否合法
		if (kind == graphTypes::UDG || kind == graphTypes::DG || !isConnectedGraph()) {
			// 其实本来这就是一个非连通图, 没必要输出这 9 个回车
			for (int i = 0; i < 5; i++)
				cout << "\n";
			return false;
		}

		// 创建边集的vector
		// 为了小组习题 6 的需要, 输出一下
		vector< WeightPairIndex > weightPairsIndexVector;
		for (int i = 0; i < weightPairs.size(); i++) {
			weightPairsIndexVector.push_back(WeightPairIndex(weightPairs[i], i));

			cout << weightPairs[i].second << " ";
		}

		cout << "\n";
		for (int i = 0; i < weightPairs.size(); i++)
			cout << i << " ";
		cout << "\n";

		// quick sort weightPairs
		// 按照边的权值由小到大排序
		// O(e log e) in time
		quickSort< WeightPairIndex >(weightPairsIndexVector);
		// P.S. 为什么就不能用 C++ STL 的 QuickSort(优化过的)呢
		//sort(weightPairsIndexVector.begin(), weightPairsIndexVector.end(),
		//	AdjListGraph<VerT, EdgeT>::compareWeightPairs);

		// 为了小组习题 6 的需要, 再输出一下
		cout << "\n";
		for (int i = 0; i < weightPairsIndexVector.size(); i++)
			cout << weightPairsIndexVector[i].first.second << " ";
		cout << "\n";
		for (int i = 0; i < weightPairsIndexVector.size(); i++)
			cout << weightPairsIndexVector[i].second << " ";
		cout << "\n\n";

		// 创建 Union-Find
		UnionFind subGraphs = UnionFind(arcNum);
		// reset mstTree
		mstTree.clear();

		// 边个数为 0 的情况
		if (weightPairsIndexVector.size() <= 0) {
			return true;
		}

		// 按照边的权值由小到大, 尝试将边加入 T, 如果 T 中没有形成环路, 将边加入 T
		// 否则舍弃, 尝试下一个
		// 这里判断回路使用 Weighted Quick Union-Find with Path Compression ( ~ c(m + log* e)
		mstTree.push_back(pair< pair<VerT, VerT>, EdgeT >(pair<VerT, VerT>(
			((*adjList)[weightPairsIndexVector[0].first.first.first])->data,
			((*adjList)[weightPairsIndexVector[0].first.first.second])->data), weightPairsIndexVector[0].first.second));
		subGraphs.quickUnion(weightPairsIndexVector[0].first.first.first,
			weightPairsIndexVector[0].first.first.second);

		// 总的时间复杂度: e log* e
		// for loop time complexity is O(e) (e is ArcNum)
		for (int i = 1; i < weightPairsIndexVector.size(); i++) {
			// ~ c ( 2 + log* e) in time
			if (!subGraphs.isConnected(weightPairsIndexVector[i].first.first.first, 
				weightPairsIndexVector[i].first.first.second)) {
				subGraphs.quickUnion(weightPairsIndexVector[i].first.first.first,
					weightPairsIndexVector[i].first.first.second);
				mstTree.push_back(pair<pair<VerT, VerT>, EdgeT>(
					pair<VerT, VerT>(
						((*adjList)[weightPairsIndexVector[i].first.first.first])->data,
						((*adjList)[weightPairsIndexVector[i].first.first.second])->data
						), 
					weightPairsIndexVector[i].first.second));
			}
		}

		// 所以整个算法的时间复杂度 O(e log* e)
		return true;
	}
};

/************** 因为时间仓促, 所以没有做任何的额外的超出OJ考察范围的考虑, 所以该class基本没法日常使用 *****************/
//图的邻接矩阵ADT
//VerT和EdgeT分别表示顶点存储的值和边存储的值的类型
template <typename VerT = int, typename EdgeT = bool>
class AdjMatrixGraph {
private:
	/* Fields */
	// 顶点数和边数
	int verNum, arcNum;
	// 邻接矩阵
	// matrixEdges[i][j] 表示弧 <i, j> (i.e., i -> j) 
	vector< vector<EdgeT> > matrixEdges;
	//顶点的值
	vector<VerT> vers;
	//无边标记
	EdgeT noEdge;
	string GraphKind;   //图的种类标志
	vector<bool> visited;

public:
	// 默认构造器
	/////////////////////// 不安全的 ////////////////////////////
	AdjMatrixGraph() {

	}

	//构造函数构造一个只有结点没有边的图。4个参数的含义：图的类型、结点数、结点值和邻接矩阵中表示结点间没有边的标记
	//（无权图：0，有权图：输入参数定） 
	AdjMatrixGraph(const string &kd, int vSize, const vector<VerT> d, const EdgeT noEdgeFlag) {
		GraphKind = kd;

		matrixEdges = vector< vector<VerT> >(vSize, vector<VerT>(vSize, noEdgeFlag));

		vers = d;
		noEdge = noEdgeFlag;
		
		verNum = vSize;
		arcNum = 0;
	}
	
	//构造函数构造一个无权图。5个参数的含义：图的类型、结点数、边数、结点集和边集 
	AdjMatrixGraph(const string &kd, int vSize, int eSize, const vector<VerT> d, const vector< pair<int, int> > e) {
		GraphKind = kd;

		matrixEdges = vector< vector<bool> >(vSize, vector<bool>(vSize, false));

		vers = d;
		noEdge = false;
		verNum = vSize;
		arcNum = eSize;

		for (vector<pair<int, int> >::const_iterator i = e.begin(); i != e.end(); i++) {
			matrixEdges[(*i).first][(*i).second] = true;
			if (kd == "UDG")
				matrixEdges[(*i).second][(*i).first] = true;
		}
	}

	//构造函数构造一个有权图。7个参数的含义：图的类型、结点数、边数、无边标记、结点集、边集、权集
	AdjMatrixGraph(const string &kd, int vSize, int eSize, const EdgeT noEdgeFlag,
		const vector<VerT> d, const vector< pair<int, int> > e, const vector<EdgeT> w) {
		GraphKind = kd;

		matrixEdges = vector< vector<EdgeT> >(vSize, vector<EdgeT>(vSize, noEdgeFlag));

		vers = d;
		noEdge = noEdgeFlag;
		verNum = vSize;
		arcNum = eSize;

		typename vector<EdgeT>::const_iterator j = w.begin();
		for (vector< pair<int, int> >::const_iterator i = e.begin(); i != e.end() ; i++, j++) {
			matrixEdges[(*i).first][(*i).second] = *j;
			if (kd == "UDN")
				matrixEdges[(*i).second][(*i).first] = *j;
		}
	}

	~AdjMatrixGraph() {

	}

	//获取顶点集
	vector<VerT> getVers() const {
		return vers;
	}

	//获取邻接矩阵
	vector< vector<EdgeT> > getAdjMatrix() const {
		return matrixEdges;
	}

	//判断图空否
	bool GraphisEmpty() { return verNum == 0; }  

	string GetGraphKind() { return GraphKind; }

	//取得G中指定顶点的值 
	bool GetVer(int u, EdgeT &data) {
		if (u < 0 || u > verNum)
			return false;

		data = vers[u];
		return true;
	}

	//返回G中指定顶点u的第一个邻接顶点的位序（顶点集）。若顶点在G中没有邻接顶点，则返回-1 
	int GetFirstAdjVex(int u) {
		if (u < 0 || u >= verNum)
			return -1;

		for (int i = 0; i < matrixEdges[u].size(); i++) {
			if (matrixEdges[u][i] != noEdge)
				return i;
		}

		return -1;
	}
	
	//返回G中指定顶点u的下一个邻接顶点（相对于v）的位序（顶点集）。若顶点在G中没有邻接顶点，则返回-1
	int GetNextAdjVex(int u, int v) {
		if (u < 0 || u >= verNum || v < 0 || v > verNum)
			return -1;

		for (int i = v + 1; i < matrixEdges[u].size(); i++) {
			if ( matrixEdges[u][i] != noEdge)
				return i;
		}

		return -1;
	}
	
	//对G中指定顶点赋值 
	bool PutVer(int u, EdgeT data) {
		if (u < 0 || u >= verNum)
			return false;

		vers[u] = data;
		return true;
	}

	//往G中添加一个顶点 
	bool InsertVer(const VerT &data) {
		vers.push_back(data);
		verNum++;
		for (typename vector< vector<EdgeT> >::iterator i = matrixEdges.begin(); i != matrixEdges.end(); i++)
			(*i).push_back(noEdge);
		matrixEdges.push_back(vector<EdgeT>(verNum, noEdge));

		return true;
	}

	//返回G中指定顶点的位置
	int LocateVer(VerT data) {
		for (int i = 0; i < verNum; i++)
			if (vers[i] == data)
				return i;
		return -1;
	}

	//输出邻接矩阵
	bool PrintMatrix() {
		for (typename vector< vector<EdgeT> >::iterator i = matrixEdges.begin(); i != matrixEdges.end(); i++) {
			for (typename vector<EdgeT>::iterator j = (*i).begin(); j != (*i).end(); j++)
				cout << (*j) << " ";
			cout << "\n";
		}
		return true;
	}

	//取得当前顶点数
	int GetVerNum() { return verNum; }   

	//取得当前边数 
	int GetEdgeNum() { return arcNum; }  


	//无权图插入一条边
	bool Insert_Edge(int u, int v) {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum || GraphKind == "UDN" || GraphKind == "DN")
			return false;

		if (!matrixEdges[u][v]) {
			matrixEdges[u][v] = true;
			arcNum++;
		}
		else
			return false;

		if (GraphKind == "UDG") {
			if (!matrixEdges[v][u]) {
				matrixEdges[v][u] = true;
			}
			else
				return false;
		}
			
		return true;
	}

	//往G中删除一个顶点
	bool DeleteVer(const VerT &data) {
		int index = LocateVer(data);
		if (-1 == index)
			return false;

		vector<VerT> vs = vector<VerT>();
		for (int i = 0; i < vers.size(); i++)
			if (i != index)
				vs.push_back(vers[i]);
		vers = vs;
		verNum--;

		vector< vector<EdgeT> > aux = vector< vector<EdgeT> >();
		for (typename vector< vector<EdgeT> >::iterator i = matrixEdges.begin(); i != matrixEdges.end(); i++)
			if (i - matrixEdges.begin() != index) {
				vector<EdgeT> tmp = vector<EdgeT>();
				for (int j = 0; j < i->size(); j++) {
					if (j != index)
						tmp.push_back((*i)[j]);
					else if ((*i)[j] != noEdge)
						arcNum--;
				}
				aux.push_back(tmp);
			}
			else
			{
				for (int j = 0; j < (*i).size(); j++)
					if ((*i)[j] != noEdge)
						arcNum--;
			}
				
		matrixEdges = aux;
		return true;
	}

	//删除一条边
	bool Delete_Edge(int u, int v) {
		if (u < 0 || v < 0 || v >= verNum || u >= verNum || matrixEdges[u][v] == noEdge) {
			return false;
		}

		matrixEdges[u][v] = noEdge;
		arcNum--;
		if (GraphKind == "UDG" || GraphKind == "UDN") {
			matrixEdges[v][u] = noEdge;
		}
		return true;
	}

	//DFS遍历（外壳部分）
	void DFS_Traverse(int u) {
		visited = vector<bool>(verNum, false);

		vector<VerT> vec;
		for (int i = u; i < u + verNum; i++) {
			if (!visited[i % verNum])
				DFS(i % verNum, vec);
		}

		if (vec.size() > 0) {
			cout << vec[0];
			for (int i = 1; i < vec.size(); i++)
				cout << "->" << vec[i];
		}
	}

	void DFS(const int i, vector<VerT> &vec) {
		visited[i] = true;
		vec.push_back(vers[i]);

		for (int w = GetFirstAdjVex(i); w != -1; w = GetNextAdjVex(i, w)) {
			if (!visited[w])
				DFS(w, vec);
		}
	}

	//确定两个顶点是否存在路径, 原理为DFS
	bool CheckRoute(const int u, const int v) {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum)
			return false;

		return CheckRoute(u, v, vector<bool>(verNum, false));
	}

	bool CheckRoute(const int i, const int v, vector<bool> vted) {
		vted[i] = true;
		if (v == i)
			return true;
		for (int w = GetFirstAdjVex(i); w != -1; w = GetNextAdjVex(i, w)) {
			if (!vted[w])
				if (CheckRoute(w, v, vted))
					return true;
		}
		return false;
	}

	//BFS遍历
	void BFS_Traverse(int u) {
		visited = vector<bool>(verNum, false);

		Queue<int> q;
		vector<VerT> v;
		
		for (int i = 0; i < verNum; i++) {
			if (!visited[i]) {
				q.enqueue(i);
				visited[i] = true;
				while (!q.isEmpty()) {
					int u = q.dequeue();
					v.push_back(vers[u]);
					int w = GetFirstAdjVex(u);
					if (w != -1) {
						do {
							if (!visited[w]) {
								q.enqueue(w);
								visited[w] = true;
							}
						} while ((w = GetNextAdjVex(u, w)) != -1);
					}
				}
			}
		}

		if (v.size() > 0) {
			cout << v[0];
			for (int i = 1; i < v.size(); i++)
				cout << "->" << v[i];
		}
	}

	//出度
	int getOutDegree(const int u) const {
		if (u < 0 || u >= verNum)
			return -1;

		int d = 0;
		for (int i = 0; i < verNum; i++) {
			if (matrixEdges[u][i] != noEdge)
				d++;
		}

		return d;
	}

	//(入)度
	int getInDegree(const int u) const {
		if (u < 0 || u >= verNum)
			return -1;

		int d = 0;
		for (int i = 0; i < verNum; i++) {
			if (matrixEdges[i][u] != noEdge)
				d++;
		}

		return d;
	}

	//判断两个顶点是否邻接
	bool isAdjVers(const int u, const int v) const {
		return matrixEdges[u][v] != noEdge;
	}

	// 基于BFS的拓扑排序(有向无环图)
	// sorted用来存储拓扑排序后的序列
	// 返回false说明有向图中存在回路
	bool topologicalSort(vector<VerT> &sorted) const {
		if (GraphKind == "UDG" || GraphKind == "UDN") {
			//必须要有向图
			return false;
		}

		//sorted must empty
		if (sorted.size() > 0)
			return false;

		//存储inDegree为0的顶点的index
		Queue<int> q;
		vector<int> inDegrees = vector<int>(verNum, 0);
		for (int i = 0; i < verNum; i++) {
			if ((inDegrees[i] = getInDegree(i)) == 0)
				q.enqueue(i);
		}
		int cnt = 0;

		while (!q.isEmpty()) {
			int t = q.dequeue();
			sorted.push_back(vers[t]);
			++cnt;
			//将所有q.dequeue指向的顶点的入度-1, 如果入度为0, 则入栈
			for (int i = 0; i < verNum; i++) {
				if (matrixEdges[t][i] != noEdge && !(--inDegrees[i]))
					q.enqueue(i);
			}
		}

		//如果cnt小于顶点数, 说明有向图有环
		if (cnt < verNum)
			return false;
		else
			return true;
	}

	//图（网）中回路的判断
	bool hasCircle() {
		if (GraphKind == "DN" || GraphKind == "DG") {
			//有向图 
			return topologicalSort(vector<VerT>());
		}

		//无向图的回路判断麻烦一点, 不过类似于拓扑排序, 采用DFS
		//首先用visitedColor数组储存每一个顶点的状态信息, 其中 'W' 白色表示顶点未被访问, 'G' 灰色表示还在队列
		//中, 'B' 黑色表示已经被访问过了并且出队列了
		vector<char> visitedColor = vector<char>(verNum, 'W');
		//邻近结点有三种可能：黑（'B'）、白（'W'）、灰（'G'）。若为黑色结点，
		//说明是DFS序列之前已访问过的，忽略不管（避免重复访问）；若是白色结点则入队，
		//颜色染灰（'G'）；若有灰色结点则表明该结点已存在于队列中，说明有回路

		for (int i = 0; i < verNum; i++) {
			if (visitedColor[i] == 'W' && DFS_Circle(i, visitedColor))
				return true;
		}

		return false;
	}

private:
	/**
	* 用来做无向图有环判断的DFS的递归函数
	* @param last 递归的上次调用的时候的i, 用来判断是否是因为无向图的双向性, 再次指向了上一个递归调用的时候用的i
	* @param i 递归开始的顶点的序号
	* @param visitedColor 用来保存没一个顶点的状态信息的vector
	* @return 如果有环路, i.e. 遇到了下一个邻接顶点就是该DFS路径中的结点的话, 就返回true, 否则false
	*/
	bool DFS_Circle(const int i, vector<char> &visitedColor, const int last = -1) {
		visitedColor[i] = 'G';
		int w = GetFirstAdjVex(i);
		if (w == -1)
			return false;
		for (; w != -1; w = GetNextAdjVex(i, w)) {
			// 上次递归调用就是 last -> i 所以w一定不能是last
			if ((visitedColor[w] == 'W' && DFS_Circle(w, visitedColor, i))
				|| (visitedColor[w] == 'G' && w != last))
				return true;
			visitedColor[i] = 'B';
		}
		return false;
	}

	/**
	* Prim 算法辅助函数
	* 时间复杂度 O(n)
	* 从 U 中找到与 v_index 相连并以v_index为弧头并且权最小的顶点 并返回该顶点的index, 找不到就返回 <-1, 0>
	* @return pair<顶点的index, 权>
	*/
	pair<int, int> minCostPrim(int index, const set<int> &U) {
		set<int>::const_iterator i = U.begin(), last = U.begin();
		while (i != U.end() && noEdge == matrixEdges[*i][index])
			last = i++;
		// 可能没有一个与 index 相连的 U 中的顶点
		if (i == U.end() && noEdge == matrixEdges[*last][index])
			return pair<int, int>(-1, 0);

		i = last;
		pair<int, int> min = pair<int, int>(*i, matrixEdges[*i][index]);

		for (; i != U.end(); i++) {
			if (matrixEdges[*i][index] != noEdge && matrixEdges[*i][index] < min.second)
				min = pair<int, int>(*i, matrixEdges[*i][index]);
		}

		return min;
	}

	/**
	* Prim 算法辅助函数
	* 找到 closedges 中(V - U顶点集中)最小的 lowcost 的 index, 并且 将对应的值放入 msetVers 和 lowcosts
	* @param closedges 记录 V - U 中顶点与 U 中邻接的顶点的弧的权值最小的 U 中顶点的index
	* @param mstVers U 中顶点集合
	* @param verPairs 要输出的最小生成树的边集
	* @param lowcosts 最小生成树的顶点集
	*/
	void findMiniCost(vector< pair<int, EdgeT> > &closedges, set<int> &mstVers,
			vector< pair<VerT, VerT> > &verPairs, vector<EdgeT> &lowcosts) {
		// closedges 中 first 小于0的要么是没有于 U 中顶点邻接的边的, 要么就是 顶点本身就在 U 的
		int i = 0;
		while (i < closedges.size() && closedges[i++].first < 0)
			continue;
		// 肯定会有一个属于 V - U 的与 U 有相连顶点的顶点
		// min.first.first 为 U 中顶点, min.first.second 为 V - U 中顶点
		pair< pair<int, int>, EdgeT> min =
			pair< pair<int, int>, EdgeT>(pair<int, int>(closedges[--i].first, i),
				matrixEdges[closedges[i].first][i]);

		for (; i < closedges.size(); i++) {
			if (closedges[i].first >= 0 && matrixEdges[closedges[i].first][i] < min.second)
				min = pair< pair<int, int>, EdgeT>(pair<int, int>(closedges[i].first, i),
					matrixEdges[closedges[i].first][i]);
		}

		// 按照min.first.second所在index插入结果
		verPairs[min.first.second] = pair<int, int>(vers[min.first.first], vers[min.first.second]);
		lowcosts[min.first.second] = min.second;
		// 从 mstVers 中加入元素
		mstVers.insert(min.first.second);
		// 把 closedges 中对应的顶点置为 <-2, 0>
		closedges[min.first.second] = pair<int, int>(-2, EdgeT());
	}

	/**
	* Dijkstra 最短路径算法辅助函数
	* int 为 最短路径权值
	*/
	struct HeapComparator {
		int operator()( const int &lhs, const int &rhs ) {
			return lhs - rhs;
		}
	};

	/**
	* Dijkstra最短路径算法辅助函数
	* 路径输出
	*/
	void searchPath(const int &src, const vector<int> &dist, const vector<int> &pre) const {
		Stack<int> s;
		s.push(src);
		for (int i = 0; i < dist.size(); i++) {
			if (i == src)
				continue;

			int j = i;
			s.push(i);
			while (pre[j] != src + 1) {
				s.push(pre[j] - 1);
				j = pre[j] - 1;
			}
			// out
			if (s.getSize() > 1) {
				j = s.pop();
				cout << "<(" << src << "," << vers[src] << ")," << "(" << j << "," << vers[j] << ")";
			}
			while (s.getSize() > 1) {
				j = s.pop();
				cout << ",(" << j << "," << vers[j] << ")";
			}
			cout << ">," << dist[i] << "\n";
		}
	}

public:
	/**
	* 确认图是否是(强)连通图, 利用 BFS 原理
	* @return 如果是非(强)连通图, 就返回 false
	*/
	bool isConnectedGraph(const int v = 0) {
		visited = vector<bool>(verNum, false);

		Queue<int> q;
		int visitedCnt = 0;

		// 就是对顶点 v 进行BFS, 如果被遍历到的顶点小于图的总顶点个数, 就说明是非(强)连通图.
		q.enqueue(v);
		visited[v] = true;
		while (!q.isEmpty()) {
			int u = q.dequeue();
			visitedCnt++;
			int w = GetFirstAdjVex(u);
			if (w != -1) {
				do {
					if (!visited[w]) {
						q.enqueue(w);
						visited[w] = true;
					}
				} while ((w = GetNextAdjVex(u, w)) != -1);
			}
		}

		return visitedCnt == verNum;
	}

	/**
	* 最小生成树算法 -- Prim
	* @param u 初始顶点序号 0 (inclusive) ~ vertex Count (exclusive)
	* @param adjvers 最小生成树的边的顶点对
	* @param lowcosts 最小生成树的边的值
	* @return 如果图是 UDG 或 DG 或 非(强)连通图的话, 返回 false
	*/
	bool miniSpanTreePrim(const int u, vector< pair<VerT, VerT> > &verPairs, vector<EdgeT> &lowcosts) {
		// 判断图是否合法
		if (u < 0 || u >= verNum || GraphKind == "UDG" || GraphKind == "DG" || !isConnectedGraph(u))
			return false;

		// 清空容器
		verPairs.clear();
		lowcosts.clear();
		// 预先分配 verNum 个空间
		verPairs.insert(verPairs.begin(), this->verNum, pair<VerT, VerT>());
		lowcosts.insert(lowcosts.begin(), this->verNum, 0);

		// V 集合为图的所有顶点集
		// U 集合表示最小生成树当前的顶点集(也就是当前 mstVers 的集合), 初始化为空集
		// closedges[i] 用来存储顶点 如果 i \in V - U, 并且其值为 pair<以v_i为弧头并且弧尾属于 U 的最小权值的弧的弧尾index, 
		// first 中那条弧的权> (如果有这样的弧), <-1, 0> (如果没有这样符合条件的弧) (这里以 DN 为一般情况), 
		// 如果 i \in U, 那就是 pair<-2, 0>
		// P.S. 多么希望用上 C++17 的模板类型推导啊 =,=
		vector< pair<int, EdgeT> > closedges = vector< pair<int, int> >(verNum, pair<int, EdgeT>(-1, EdgeT()));
		// U 中顶点集
		set<int> mstVers;
		// 初始化
		// mstVers 最开始有 u
		mstVers.insert(u);
		for (int i = 0; i < verNum; i++) {
			if (u == i)
				closedges[i] = pair<int, int>(-2, EdgeT());
			else
				closedges[i] = minCostPrim(i, mstVers);
		}
		
		// 循环次数为 verNum - 1
		for (int i = 0; i < verNum - 1; i++) {
			findMiniCost(closedges, mstVers, verPairs, lowcosts);
			for (int i = 0; i < verNum; i++) {
				// 对所有顶点属于 V - U 的进行 minCostPrim
				if (closedges[i].first > -2)
					closedges[i] = minCostPrim(i, mstVers);
			}
		}

		// 把 verPairs lowcosts 中 index 为 u 的元素删除
		verPairs.erase(verPairs.begin() + u);
		lowcosts.erase(lowcosts.begin() + u);

		return true;
	}

	/**
	* 最短路径算法 -- Dijkstra
	* 算法思想有点类似于 Prim 最小生成树算法
	* @param u 初始顶点序号 0 (inclusive) ~ vertex Count (exclusive)
	*/
	bool shostestPathDijkstra(const int src) {
		// 如果 EdgeT 不是 int, 就返回 false
		if (typeid(EdgeT) != typeid(int)) {
			cerr << "EdgeT expected int." << endl;
			return false;
		}

		// 检查 u 合法性
		if (src < 0 || src >= verNum) {
			cerr << "u illegal!" << endl;
			return false;
		}

		// 检查图是否是(强)连通图
		if (!isConnectedGraph(src)) {
			cerr << "Grapg is NOT Connected Graph!" << endl;
			return false;
		}

		// dist 记录从源点到该顶点的最短路径长度(除了源点初始化为0, 如果顶点与源点直接相连, 就为其边的权值, 其他初始化为 INFINITE)
		vector<int> dist = vector<int>(verNum, noEdge);

		// 表示从V_u到各终点的最短路径上，此顶点的前一顶点的序号(1表示第一个顶点, 顶点序号从1开始)；若从V_u到某终点无路径，
		// 则用0作为其前一顶点的序号。
		vector<int> pre = vector<int>(verNum, 0);

		// 如果顶点i(i从0开始)在当前最短路径树(Shortest Path Tree)上面, sptSet[i] 为 true
		vector<bool> sptSet = vector<bool>(verNum, false); 				

		// MinPQ 优先队列
		IndexMinPQ< int, HeapComparator> pq(verNum, new int(noEdge));

		// 记录已经处理的顶点个数, 到 verNum - 1 时表示处理完所有的顶点了.
		int cnt = 0;

		// 把源点和与源点直接相连的顶点加入 sptSet, 并且更新 pq
		// O(N * log N)
		dist[src] = 0;
		sptSet[src] = true;
		for (int w = GetFirstAdjVex(src); w > -1; w = GetNextAdjVex(src, w)) {
			dist[w] = matrixEdges[src][w];
			pre[w] = src + 1;
			// 改变堆中的值, O(logN)
			pq.change(w, new int(dist[w]));
		}

		// 按照题目要求, 输出
		dist[src] = 0;
		for (int i = 0; i < dist.size(); i++)
			cout << (pre[i] == (src + 1) ? dist[i] : noEdge) << " ";
		cout << "\n";
		for (int i = 0; i < pre.size(); i++) {
			cout << (pre[i] == (src + 1) ? (src + 1) : 0) << " ";
		}

		cout << "\n\n";

		// 用 cnt 个数作为终止条件, 也可以将 pq 中存入sptSet的顶点删除, 用 pq 是否为空来作为条件, 不过这样浪费性能
		// O(N * N * log N)
		while (cnt < verNum) {
			// 将当前最小的这个加入 sptSet
			int w = pq.minIndex();
			sptSet[w] = true;
			cnt++;
			// 将放入 sptSet 的顶点在 pq 的值改为 INT_MAX
			pq.change(w, new int(noEdge));
			for (int v = GetFirstAdjVex(w); v > -1; v = GetNextAdjVex(w, v)) {
				if (!sptSet[v] && dist[v] > dist[w] + matrixEdges[w][v]) {
					dist[v] = dist[w] + matrixEdges[w][v];
					pre[v] = w + 1;
					// 改变堆中的值, O(logN)
					pq.change(v, new int(dist[v]));
				}
			}
		}

		// 按照题目要求, 输出
		for (int i = 0; i < dist.size(); i++)
			cout << dist[i] << " ";
		cout << "\n";
		for (int i = 0; i < pre.size(); i++)
			cout << pre[i] << " ";
		cout << "\n\n";
		searchPath(src, dist, pre);

		// 所以总的 Time Complexity 为 O(N * N * log N) (粗略估计), 总的 Space Complexity 为 O(N) (并且系数有点大, 待优化)
		return true;
	}
};

#endif // !Graph