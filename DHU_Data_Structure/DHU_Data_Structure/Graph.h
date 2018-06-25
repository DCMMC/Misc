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

// ! ���Ƽ�������, �� lib �о�����Ҫʹ�� using, ��������Ⱦ include ��
// ! ��ͷ�ļ���Դ�ļ��������ռ�
using namespace std;

//ͼ��

//ͼ���ڽӱ�ADT
//VerT��EdgeT�ֱ��ʾ����洢��ֵ�ͱߴ洢��ֵ������
template <typename VerT = int, typename EdgeT = bool>
class AdjListGraph {
private:
	/* Inner classes */
	//�����
	template<typename T = bool>
	struct ArcNode {
		//�û���ָ��Ķ����λ��
		//��0��ʼ
		int adjVer;
		//��һ�����Ľ��ָ��
		ArcNode *nextArc;
		//���ϵ�Ȩ��
		T weight;

		ArcNode(int adjVer, T weight, ArcNode<T> *nextArc = nullptr) {
			this->adjVer = adjVer;
			this->nextArc = nextArc;
			this->weight = weight;
		}
	};

	//������
	//T��ʾ�����ֵ������, E��ʾ����ֵ������
	template<typename T = int, typename E = bool>
	struct VerNode {
		//�������
		int index;
		//�����ֵ
		T data;
		//ָ���һ�������ڸö���Ļ���ָ��
		ArcNode<E> *firstArc;

		VerNode(int index, T data, ArcNode<E> *firstArc = nullptr) {
			this->index = index;
			this->data = data;
			this->firstArc = firstArc;
		}
	};

	/* Fields */
	//�ڽӱ�, �洢�Ÿ�������
	vector<VerNode<VerT, EdgeT> *> *adjList;
	//����ͱߵĸ���
	int verNum, arcNum;

public:
	//ͼ������ DG������ͼ��, DN����������, UDG������ͼ��, UDN����������
	enum graphTypes: int { DG, DN, UDG, UDN };

private:
	//ͼ������
	graphTypes kind;
	//��¼�Ƿ񱻷��ʹ�
	vector<bool> visited;

	////////////// ���� Kruskal ��С�������㷨 ///////////////////////
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
	* ����һ��ֻ�ж���û�бߵ�ͼ(����ͼ���п���)
	* @param verSize �������
	* @param type ͼ������
	* @param vers ���㼯��
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
	* ���ڽӾ���Ͷ��㼯����һ��ͼ����(UDG����DG����UDN����DN)
	* ���edges��bool����, ���ʾ��Ȩͼ(UDG��DG)
	************** UNCHECKED: ����У��edges�Ƿ���edges[vers.size][vers.size] ***********************
	* @param vers ���㼯��
	* @param eSize �ߵ�����
	* @param edges �ߵ��ڽӾ���
	* @param disconnectedNotation �ڽӾ����б�ʾδ���ӵ�����ֵ, �������Ȩͼ(i.e. bool ����)�г���0��ʾΪδ
	* ����, �� EdgeTΪintʱ���� INT_MAX����ʾ
	*/
	AdjListGraph(const vector<VerT> vers, const int eSize, const EdgeT **edges,
		const EdgeT &disconnectedNotation) {
		arcNum = eSize;
		adjList = new vector< VerNode<VerT, EdgeT> *>();
		for (typename vector<VerT>::iterator itr = vers.begin(); itr != vers.end(); itr++)
			adjList->push_back(new VerNode<VerT, EdgeT>(adjList->size(), *itr));
		verNum = adjList->size();
		ArcNode<EdgeT> *lastArc = nullptr;
		//�Ȱ�kind�ٶ�Ϊ UDG��UDN
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
	* �Ӹ����Ķ��㼯�ͱ߼�����һ��ͼ����
	* @param type ͼ������
	* @param vers ���㼯
	* @param edges �߼���(���������ͼ, ÿ�����û�β�Ķ����index�ͻ�ͷ�Ķ����index��ʾ, e.g. <0, 2> 
	* ��ʾһ������0ָ�򶥵�2�Ļ�), ע��: �߼��ϱ��밴��˳����, �������ж���0Ϊ��β�ı�, Ȼ�󶥵�1, ������
	*/
	AdjListGraph(const graphTypes type, const vector<VerT> vers, const vector< pair<int, int> > edges,
		const vector<EdgeT> weights = vector<EdgeT>() ) {
		//check type
		if (weights.size() == 0 && edges.size() != 0) {
			//UDG��DG
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
			//UDN��DN
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
		//����ĵ�����
		typename vector< VerNode<VerT, EdgeT> *>::iterator iVer = adjList->begin();
		//Ȩ�ĵ�����
		typename vector<EdgeT>::const_iterator iWeight = weights.begin();
		//��һ����ͬ��β����ıߵĻ�ͷ�����index
		int lastTailIndex = -1;
		for (typename vector< pair<int, int> >::const_iterator itr = edges.begin();
			itr != edges.end(); itr++) {
			if (lastTailIndex != (*itr).first) {
				if (lastTailIndex != -1)
					iVer = adjList->begin() + (*itr).first;
				lastTailIndex = (*itr).first;
			}

			// ���� �ߺͶ����
			weightPairs.push_back(pair< pair<int, int>, EdgeT>(*itr, *iWeight));
			
			//���߲����ڸö����firstArc֮ǰ
			ArcNode<EdgeT> *node = new ArcNode<EdgeT>((*itr).second, (kind == UDN || kind == DN)
				? *iWeight++ : 0, (*iVer)->firstArc);
			(*iVer)->firstArc = node;
	
			//���������ͼ, ����Ҫ��������������һ������
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

	//�ж��Ƿ�Ϊ��
	bool isEmpty() const {
		return !verNum & !arcNum;
	}

	//���رߵĸ���
	int getArcNum() const {
		return arcNum;
	}

	//���ض���ĸ���
	int getVerNum() const {
		return verNum;
	}

	//����ͼ������
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

	//���ض��㼯(˳��������ʱ���˳��һ��)
	//��Ȼ����vers��local variable, �洢��stack��, �������ص�ʱ����ȸ���һ��vers�������ز�������ԭ����vers
	//����һ������retuan a local object with reference, ��Ȼ���᷵��һ�������ٵĶ��������
	vector<VerT> getVertexs() const {
		vector<VerT> vers;
		for (typename vector<VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); itr != adjList->end(); itr++) {
			vers.push_back((*itr)->data);
		}
		return vers;
	}

	//���ն��㼯��˳�����ÿ��������ڽӱ�(���а�����ͷ�Ķ����index�ͻ���Ȩ��(�������Ȩͼ����false)
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

	//���һ������
	AdjListGraph<VerT, EdgeT> &addVertex(const VerT t) {
		adjList->push_back(new VerNode<VerT, EdgeT>(adjList->size(), t));
		return *this;
	}

	//���ض����index
	//���û���ҵ��򷵻�-1
	int getVertexIndex(VerT v) const {
		//����
		int i = 0;
		for (typename vector< VerNode<VerT, EdgeT> *>::iterator itr = adjList->begin(); itr != adjList->end(); itr++, i++) {
			if (v == (*itr)->data)
				return i;
		}
		return -1;
	}

	//ɾ�������������ı�
	VerT *removeVertexByIndex(const int index) {
		if (index < 0 || index >= adjList->size())
			////////////// throw an exception ///////////////
			return nullptr;

		VerT *removedT = nullptr;
		vector< VerNode<VerT, EdgeT> *> *newAdjList = new vector< VerNode<VerT, EdgeT> *>();
		//����ɾ����֮������ı�
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


	//����G��ָ������(indexΪbase�Ķ���)����һ���ڽӶ����λ�� 
	//������ͼ�о��Ǿ����ڽӱ����������һ��ָ��Ķ���
	//��������ͼ��, ����Ҫ�����ж��Ƿ��� u -> v �� v-> u�����, Ҳ�����Ƿ����ͬһ���ߵ����
	int getNextAdjVerIndex(const int verIndex, const int baseIndex = -1) const {
		if (verIndex < 0 || verIndex >= adjList->size())
			return -1;
		
		VerNode<VerT, EdgeT> *node = (*adjList)[verIndex];
		if (baseIndex == -1) {
			// �����Ƕ���ж��е��鷳, ��Ϊ����ͼ���뻹Ҫ�����Ƿ���ͬһ���ߵ����
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

	//����һ����Ȩ�ı�
	bool addArc(const int tailIndex, const int headIndex) {
		return addArc(tailIndex, headIndex, false);
	}

	//����һ����
	bool addArc(const int tailIndex, const int headIndex, const EdgeT t) {
		if (tailIndex < 0 || headIndex < 0 || tailIndex >= adjList->size() || headIndex >= adjList->size()) {
			return false;
		}

		//�ж��Ƿ��Ѿ�����
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

	//ɾ����
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

	//��û� <tail, head> ��weight, ����nullptr��ʾδ�ҵ�����������
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

	//�ж��Ƿ��� u->v�Ļ�(��)
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

	//���������Ƿ����ڽӶ��� (u -> v �� v -> u)
	bool isAdjVers(const int u, const int v) const {
		if (u < 0 || v < 0 || u >= verNum || v >= verNum)
			return false;

		return isArcAdjVers(u, v) || isArcAdjVers(v, u);
	}

	//����
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

	//���
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

	//�Ƿ���·��
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

	// ����BFS����������(�����޻�ͼ)
	// sorted�����洢��������������
	// ����false˵������ͼ�д��ڻ�·
	bool topologicalSort(vector<VerT> &sorted) const {
		if (kind == UDG || kind == UDN) {
			//����Ҫ����ͼ
			return false;
		}

		//sorted must empty
		if (sorted.size() > 0)
			return false;

		//�洢inDegreeΪ0�Ķ����index
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
			//������q.dequeueָ��Ķ�������-1, ������Ϊ0, ����ջ
			for (ArcNode<EdgeT> *arc = (*adjList)[t]->firstArc; arc; arc = arc->nextArc) {
				if (!(--inDegrees[arc->adjVer]))
					q.enqueue(arc->adjVer);
			}
		}

		//���cntС�ڶ�����, ˵������ͼ�л�
		if (cnt < verNum)
			return false;
		else
			return true;
	}

	//ͼ�������л�·���ж�
	bool hasCircle() const {
		if (kind == DN || kind == DG) {
			//����ͼ 
			return topologicalSort(vector<VerT>());
		}

		//����ͼ�Ļ�·�ж��鷳һ��, ������������������, ����DFS
		//������visitedColor���鴢��ÿһ�������״̬��Ϣ, ���� 'W' ��ɫ��ʾ����δ������, 'G' ��ɫ��ʾ���ڶ���
		//��, 'B' ��ɫ��ʾ�Ѿ������ʹ��˲��ҳ�������
		vector<char> visitedColor = vector<char>(verNum, 'W');
		//�ڽ���������ֿ��ܣ��ڣ�'B'�����ף�'W'�����ң�'G'������Ϊ��ɫ��㣬
		//˵����DFS����֮ǰ�ѷ��ʹ��ģ����Բ��ܣ������ظ����ʣ������ǰ�ɫ�������ӣ�
		//��ɫȾ�ң�'G'�������л�ɫ���������ý���Ѵ����ڶ����У�˵���л�·
	
		for (int i = 0; i < verNum; i++) {
			if (visitedColor[i] == 'W' && DFS_Circle(i, visitedColor))
				return true;
		}

		return false;
	}

private:
	/**
	* ����������ͼ�л��жϵ�DFS�ĵݹ麯��
	* @param last �ݹ���ϴε��õ�ʱ���i, �����ж��Ƿ�����Ϊ����ͼ��˫����, �ٴ�ָ������һ���ݹ���õ�ʱ���õ�i
	* @param i �ݹ鿪ʼ�Ķ�������
	* @param visitedColor ��������ûһ�������״̬��Ϣ��vector
	* @return ����л�·, i.e. ��������һ���ڽӶ�����Ǹ�DFS·���еĽ��Ļ�, �ͷ���true, ����false
	*/
	bool DFS_Circle(const int i, vector<char> &visitedColor, const int last = -1) const {
		visitedColor[i] = 'G';
		int w = getNextAdjVerIndex(i);
		if (w == -1)
			return false;
		for (; w != -1; w = getNextAdjVerIndex(i, w)) {
			// �ϴεݹ���þ��� last -> i ����wһ��������last
			if ( (visitedColor[w] == 'W' && DFS_Circle(w, visitedColor, i)) 
				|| (visitedColor[w] == 'G' && w != last))
				return true;
			visitedColor[i] = 'B';
		}
		return false;
	}

	// ����Ԫ��, ������Ԫ�ر������, �������� С����ҵ6
	// P.S. DHU OJ ʲôʱ��֧�� C++11 ��, �ö�Ԫ��������Ԫ�������鷳
	typedef pair< pair<pair<int, int>, EdgeT>, int> WeightPairIndex;

	/**
	* Kruskal �㷨������
	* ���ڿ�������, ���ձߵ�Ȩֵ��С��������
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
	* ԭ�ط���(In-place Partition)
	* @param a
	*		Ҫ����������
	* @param lo
	*		������Χ���½�
	* @param hi
	*		������Χ���Ͻ�
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
	* ��������ָ������
	* @param a
	* 		Ҫ���������
	* @param lo
	*		Ҫ����ķ�Χ���½�
	* @param hi
	* 		Ҫ����ķ�Χ���Ͻ�
	*/
	template <typename T>
	static void quickSort(vector<T> &a, int lo, int hi) {
		if (lo >= hi)
			return;

		//�����ķֽ�λ��
		int j = partition<T>(a, lo, hi);

		quickSort<T>(a, lo, j - 1);
		quickSort<T>(a, j + 1, hi);
	}

	/**
	* ��������
	* �ŵ�: ʵ�ֱȽϼ�, ÿһ�ֵıȽϴ������ǹ̶���N + 1, ʱ�临�ӶȺͿռ临�Ӷȶ��൱����, ��ȻQuicksort��һ������µıȽ�
	* ����(1.39N logN)����mergesort()(1/2N logN ~ N logN), ����Quicksort�����ƶ�(exch)�Ĵ����൱��, ���������˵Quicksort����ӿ�.
	* ȱ��: �ܶ�С��ϸ�����׵������ص�������ʧ, ��ʱ�������ﵽ��N^2��ʱ�临�Ӷ�
	* �㷨����:
	* �����������, ÿһ��j����������subarray���м�λ��, Ҳ����ÿ�ζ��ܶ���, ������mergesortһģһ��, ʱ�临�Ӷ�Ϊ ~ NlogN
	*
	* Quicksortƽ��ʹ�� ~ 2NlnN�ıȽϴ���(�Լ�1/6�ĵĽ���)
	* Prooof.
	* �����������, ÿһ��j����������subarray���м�λ��, Ҳ����ÿ�ζ��ܶ���, ������mergesortһģһ��, ʱ�临�Ӷ�Ϊ ~ NlogN
	*
	* Quicksort����N����ͬ������ƽ��ʹ�� ~ 2NlogN (`!$\dot {=} 1.39N \log N$`)�ıȽϴ���(�Լ�1/6�ĵĽ���)
	*
	**Prooof.**
	*
	* ��`!$C_N$`Ϊ����N��(��ɢ��)items����Ҫ��ƽ���Ƚϴ���, �׵�`!$C_0 = C_1 = 0$`, ���Ҷ���`!$N > 1$`, ��һ�µݹ��ϵ:
	*
	* ```mathjax!
	* $$C_N = N + 1 + \frac {\left(C_0 + C_1 + \cdot \cdot \cdot + C_{N - 2} + C_{N - 1} \right)} {N} +
	* \frac {\left(C_{N - 1} + C_{N - 2} + \cdot \cdot \cdot + C_{1} + C_{0} \right)} {N}$$
	* ```
	*
	* N + 1��ÿһ������Ĺ̶��Ƚϴ���, �ڶ�����������left subarray��ƽ���Ƚϴ���, ��������������right subarray��ƽ���Ƚϴ���.
	*
	* �ֽ�`!$C_N$`��`!$C_{N - 1}$`��ʽ���, �õ�`!$NC_N = 2N + (N + 1)C_{N - 1}$`, ����ͬ����N(N + 1), �õ�
	* `!$\frac {C_N} {N + 1} = \frac {2} {N + 1} + \frac {C_{N - 1}} {N}$`, ��`!$\lambda_N =  \frac {C_N} {N + 1}$`,
	* ���Ե��Ƶõ�
	*
	* `!$C_N = 2(N + 1) \cdot \left( \sum_{i = 3}^{N + 1} \frac {1} {i} \right)$` ~ `!$2N\ln N$`
	*
	* ֤��.
	*
	* > ����������֤�����������Ʋ������Ӹ���.
	*
	* > �������ظ���ֵ�����, ׼ȷ�ķ������Ӻܶ�, �������ѱ���ƽ���Ƚϴ���������`!$C_N$`, ���潫���ж�����������Ż�.
	*
	* @param a
	* 		Ҫ���������
	*/
	template <typename T>
	static void quickSort( vector<T> &a ) {
		quickSort<T>(a, 0, a.size() - 1);
	}


	/**
	* ȷ��ͼ�Ƿ���(ǿ)��ͨͼ, ���� BFS ԭ��
	* @return ����Ƿ�(ǿ)��ͨͼ, �ͷ��� false
	*/
	bool isConnectedGraph(const int v = 0) {
		visited = vector<bool>(verNum, false);

		Queue<int> q;
		int visitedCnt = 0;

		// ���ǶԶ��� v ����BFS, ������������Ķ���С��ͼ���ܶ������, ��˵���Ƿ�(ǿ)��ͨͼ.
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
	* ��С������ -- Kruskal
	* @param mstTree ��С�������Ĵ���
	* @return ͼ�Ƿ��򷵻� false
	*/
	bool miniSpanTreeKruskal(vector< pair<pair<VerT, VerT>, EdgeT> > &mstTree) {
		// �ж�ͼ�Ƿ�Ϸ�
		if (kind == graphTypes::UDG || kind == graphTypes::DG || !isConnectedGraph()) {
			// ��ʵ���������һ������ͨͼ, û��Ҫ����� 9 ���س�
			for (int i = 0; i < 5; i++)
				cout << "\n";
			return false;
		}

		// �����߼���vector
		// Ϊ��С��ϰ�� 6 ����Ҫ, ���һ��
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
		// ���ձߵ�Ȩֵ��С��������
		// O(e log e) in time
		quickSort< WeightPairIndex >(weightPairsIndexVector);
		// P.S. Ϊʲô�Ͳ����� C++ STL �� QuickSort(�Ż�����)��
		//sort(weightPairsIndexVector.begin(), weightPairsIndexVector.end(),
		//	AdjListGraph<VerT, EdgeT>::compareWeightPairs);

		// Ϊ��С��ϰ�� 6 ����Ҫ, �����һ��
		cout << "\n";
		for (int i = 0; i < weightPairsIndexVector.size(); i++)
			cout << weightPairsIndexVector[i].first.second << " ";
		cout << "\n";
		for (int i = 0; i < weightPairsIndexVector.size(); i++)
			cout << weightPairsIndexVector[i].second << " ";
		cout << "\n\n";

		// ���� Union-Find
		UnionFind subGraphs = UnionFind(arcNum);
		// reset mstTree
		mstTree.clear();

		// �߸���Ϊ 0 �����
		if (weightPairsIndexVector.size() <= 0) {
			return true;
		}

		// ���ձߵ�Ȩֵ��С����, ���Խ��߼��� T, ��� T ��û���γɻ�·, ���߼��� T
		// ��������, ������һ��
		// �����жϻ�·ʹ�� Weighted Quick Union-Find with Path Compression ( ~ c(m + log* e)
		mstTree.push_back(pair< pair<VerT, VerT>, EdgeT >(pair<VerT, VerT>(
			((*adjList)[weightPairsIndexVector[0].first.first.first])->data,
			((*adjList)[weightPairsIndexVector[0].first.first.second])->data), weightPairsIndexVector[0].first.second));
		subGraphs.quickUnion(weightPairsIndexVector[0].first.first.first,
			weightPairsIndexVector[0].first.first.second);

		// �ܵ�ʱ�临�Ӷ�: e log* e
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

		// ���������㷨��ʱ�临�Ӷ� O(e log* e)
		return true;
	}
};

/************** ��Ϊʱ��ִ�, ����û�����κεĶ���ĳ���OJ���췶Χ�Ŀ���, ���Ը�class����û���ճ�ʹ�� *****************/
//ͼ���ڽӾ���ADT
//VerT��EdgeT�ֱ��ʾ����洢��ֵ�ͱߴ洢��ֵ������
template <typename VerT = int, typename EdgeT = bool>
class AdjMatrixGraph {
private:
	/* Fields */
	// �������ͱ���
	int verNum, arcNum;
	// �ڽӾ���
	// matrixEdges[i][j] ��ʾ�� <i, j> (i.e., i -> j) 
	vector< vector<EdgeT> > matrixEdges;
	//�����ֵ
	vector<VerT> vers;
	//�ޱ߱��
	EdgeT noEdge;
	string GraphKind;   //ͼ�������־
	vector<bool> visited;

public:
	// Ĭ�Ϲ�����
	/////////////////////// ����ȫ�� ////////////////////////////
	AdjMatrixGraph() {

	}

	//���캯������һ��ֻ�н��û�бߵ�ͼ��4�������ĺ��壺ͼ�����͡�����������ֵ���ڽӾ����б�ʾ����û�бߵı��
	//����Ȩͼ��0����Ȩͼ������������� 
	AdjMatrixGraph(const string &kd, int vSize, const vector<VerT> d, const EdgeT noEdgeFlag) {
		GraphKind = kd;

		matrixEdges = vector< vector<VerT> >(vSize, vector<VerT>(vSize, noEdgeFlag));

		vers = d;
		noEdge = noEdgeFlag;
		
		verNum = vSize;
		arcNum = 0;
	}
	
	//���캯������һ����Ȩͼ��5�������ĺ��壺ͼ�����͡����������������㼯�ͱ߼� 
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

	//���캯������һ����Ȩͼ��7�������ĺ��壺ͼ�����͡���������������ޱ߱�ǡ���㼯���߼���Ȩ��
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

	//��ȡ���㼯
	vector<VerT> getVers() const {
		return vers;
	}

	//��ȡ�ڽӾ���
	vector< vector<EdgeT> > getAdjMatrix() const {
		return matrixEdges;
	}

	//�ж�ͼ�շ�
	bool GraphisEmpty() { return verNum == 0; }  

	string GetGraphKind() { return GraphKind; }

	//ȡ��G��ָ�������ֵ 
	bool GetVer(int u, EdgeT &data) {
		if (u < 0 || u > verNum)
			return false;

		data = vers[u];
		return true;
	}

	//����G��ָ������u�ĵ�һ���ڽӶ����λ�򣨶��㼯������������G��û���ڽӶ��㣬�򷵻�-1 
	int GetFirstAdjVex(int u) {
		if (u < 0 || u >= verNum)
			return -1;

		for (int i = 0; i < matrixEdges[u].size(); i++) {
			if (matrixEdges[u][i] != noEdge)
				return i;
		}

		return -1;
	}
	
	//����G��ָ������u����һ���ڽӶ��㣨�����v����λ�򣨶��㼯������������G��û���ڽӶ��㣬�򷵻�-1
	int GetNextAdjVex(int u, int v) {
		if (u < 0 || u >= verNum || v < 0 || v > verNum)
			return -1;

		for (int i = v + 1; i < matrixEdges[u].size(); i++) {
			if ( matrixEdges[u][i] != noEdge)
				return i;
		}

		return -1;
	}
	
	//��G��ָ�����㸳ֵ 
	bool PutVer(int u, EdgeT data) {
		if (u < 0 || u >= verNum)
			return false;

		vers[u] = data;
		return true;
	}

	//��G�����һ������ 
	bool InsertVer(const VerT &data) {
		vers.push_back(data);
		verNum++;
		for (typename vector< vector<EdgeT> >::iterator i = matrixEdges.begin(); i != matrixEdges.end(); i++)
			(*i).push_back(noEdge);
		matrixEdges.push_back(vector<EdgeT>(verNum, noEdge));

		return true;
	}

	//����G��ָ�������λ��
	int LocateVer(VerT data) {
		for (int i = 0; i < verNum; i++)
			if (vers[i] == data)
				return i;
		return -1;
	}

	//����ڽӾ���
	bool PrintMatrix() {
		for (typename vector< vector<EdgeT> >::iterator i = matrixEdges.begin(); i != matrixEdges.end(); i++) {
			for (typename vector<EdgeT>::iterator j = (*i).begin(); j != (*i).end(); j++)
				cout << (*j) << " ";
			cout << "\n";
		}
		return true;
	}

	//ȡ�õ�ǰ������
	int GetVerNum() { return verNum; }   

	//ȡ�õ�ǰ���� 
	int GetEdgeNum() { return arcNum; }  


	//��Ȩͼ����һ����
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

	//��G��ɾ��һ������
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

	//ɾ��һ����
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

	//DFS��������ǲ��֣�
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

	//ȷ�����������Ƿ����·��, ԭ��ΪDFS
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

	//BFS����
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

	//����
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

	//(��)��
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

	//�ж����������Ƿ��ڽ�
	bool isAdjVers(const int u, const int v) const {
		return matrixEdges[u][v] != noEdge;
	}

	// ����BFS����������(�����޻�ͼ)
	// sorted�����洢��������������
	// ����false˵������ͼ�д��ڻ�·
	bool topologicalSort(vector<VerT> &sorted) const {
		if (GraphKind == "UDG" || GraphKind == "UDN") {
			//����Ҫ����ͼ
			return false;
		}

		//sorted must empty
		if (sorted.size() > 0)
			return false;

		//�洢inDegreeΪ0�Ķ����index
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
			//������q.dequeueָ��Ķ�������-1, ������Ϊ0, ����ջ
			for (int i = 0; i < verNum; i++) {
				if (matrixEdges[t][i] != noEdge && !(--inDegrees[i]))
					q.enqueue(i);
			}
		}

		//���cntС�ڶ�����, ˵������ͼ�л�
		if (cnt < verNum)
			return false;
		else
			return true;
	}

	//ͼ�������л�·���ж�
	bool hasCircle() {
		if (GraphKind == "DN" || GraphKind == "DG") {
			//����ͼ 
			return topologicalSort(vector<VerT>());
		}

		//����ͼ�Ļ�·�ж��鷳һ��, ������������������, ����DFS
		//������visitedColor���鴢��ÿһ�������״̬��Ϣ, ���� 'W' ��ɫ��ʾ����δ������, 'G' ��ɫ��ʾ���ڶ���
		//��, 'B' ��ɫ��ʾ�Ѿ������ʹ��˲��ҳ�������
		vector<char> visitedColor = vector<char>(verNum, 'W');
		//�ڽ���������ֿ��ܣ��ڣ�'B'�����ף�'W'�����ң�'G'������Ϊ��ɫ��㣬
		//˵����DFS����֮ǰ�ѷ��ʹ��ģ����Բ��ܣ������ظ����ʣ������ǰ�ɫ�������ӣ�
		//��ɫȾ�ң�'G'�������л�ɫ���������ý���Ѵ����ڶ����У�˵���л�·

		for (int i = 0; i < verNum; i++) {
			if (visitedColor[i] == 'W' && DFS_Circle(i, visitedColor))
				return true;
		}

		return false;
	}

private:
	/**
	* ����������ͼ�л��жϵ�DFS�ĵݹ麯��
	* @param last �ݹ���ϴε��õ�ʱ���i, �����ж��Ƿ�����Ϊ����ͼ��˫����, �ٴ�ָ������һ���ݹ���õ�ʱ���õ�i
	* @param i �ݹ鿪ʼ�Ķ�������
	* @param visitedColor ��������ûһ�������״̬��Ϣ��vector
	* @return ����л�·, i.e. ��������һ���ڽӶ�����Ǹ�DFS·���еĽ��Ļ�, �ͷ���true, ����false
	*/
	bool DFS_Circle(const int i, vector<char> &visitedColor, const int last = -1) {
		visitedColor[i] = 'G';
		int w = GetFirstAdjVex(i);
		if (w == -1)
			return false;
		for (; w != -1; w = GetNextAdjVex(i, w)) {
			// �ϴεݹ���þ��� last -> i ����wһ��������last
			if ((visitedColor[w] == 'W' && DFS_Circle(w, visitedColor, i))
				|| (visitedColor[w] == 'G' && w != last))
				return true;
			visitedColor[i] = 'B';
		}
		return false;
	}

	/**
	* Prim �㷨��������
	* ʱ�临�Ӷ� O(n)
	* �� U ���ҵ��� v_index ��������v_indexΪ��ͷ����Ȩ��С�Ķ��� �����ظö����index, �Ҳ����ͷ��� <-1, 0>
	* @return pair<�����index, Ȩ>
	*/
	pair<int, int> minCostPrim(int index, const set<int> &U) {
		set<int>::const_iterator i = U.begin(), last = U.begin();
		while (i != U.end() && noEdge == matrixEdges[*i][index])
			last = i++;
		// ����û��һ���� index ������ U �еĶ���
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
	* Prim �㷨��������
	* �ҵ� closedges ��(V - U���㼯��)��С�� lowcost �� index, ���� ����Ӧ��ֵ���� msetVers �� lowcosts
	* @param closedges ��¼ V - U �ж����� U ���ڽӵĶ���Ļ���Ȩֵ��С�� U �ж����index
	* @param mstVers U �ж��㼯��
	* @param verPairs Ҫ�������С�������ı߼�
	* @param lowcosts ��С�������Ķ��㼯
	*/
	void findMiniCost(vector< pair<int, EdgeT> > &closedges, set<int> &mstVers,
			vector< pair<VerT, VerT> > &verPairs, vector<EdgeT> &lowcosts) {
		// closedges �� first С��0��Ҫô��û���� U �ж����ڽӵıߵ�, Ҫô���� ���㱾����� U ��
		int i = 0;
		while (i < closedges.size() && closedges[i++].first < 0)
			continue;
		// �϶�����һ������ V - U ���� U ����������Ķ���
		// min.first.first Ϊ U �ж���, min.first.second Ϊ V - U �ж���
		pair< pair<int, int>, EdgeT> min =
			pair< pair<int, int>, EdgeT>(pair<int, int>(closedges[--i].first, i),
				matrixEdges[closedges[i].first][i]);

		for (; i < closedges.size(); i++) {
			if (closedges[i].first >= 0 && matrixEdges[closedges[i].first][i] < min.second)
				min = pair< pair<int, int>, EdgeT>(pair<int, int>(closedges[i].first, i),
					matrixEdges[closedges[i].first][i]);
		}

		// ����min.first.second����index������
		verPairs[min.first.second] = pair<int, int>(vers[min.first.first], vers[min.first.second]);
		lowcosts[min.first.second] = min.second;
		// �� mstVers �м���Ԫ��
		mstVers.insert(min.first.second);
		// �� closedges �ж�Ӧ�Ķ�����Ϊ <-2, 0>
		closedges[min.first.second] = pair<int, int>(-2, EdgeT());
	}

	/**
	* Dijkstra ���·���㷨��������
	* int Ϊ ���·��Ȩֵ
	*/
	struct HeapComparator {
		int operator()( const int &lhs, const int &rhs ) {
			return lhs - rhs;
		}
	};

	/**
	* Dijkstra���·���㷨��������
	* ·�����
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
	* ȷ��ͼ�Ƿ���(ǿ)��ͨͼ, ���� BFS ԭ��
	* @return ����Ƿ�(ǿ)��ͨͼ, �ͷ��� false
	*/
	bool isConnectedGraph(const int v = 0) {
		visited = vector<bool>(verNum, false);

		Queue<int> q;
		int visitedCnt = 0;

		// ���ǶԶ��� v ����BFS, ������������Ķ���С��ͼ���ܶ������, ��˵���Ƿ�(ǿ)��ͨͼ.
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
	* ��С�������㷨 -- Prim
	* @param u ��ʼ������� 0 (inclusive) ~ vertex Count (exclusive)
	* @param adjvers ��С�������ıߵĶ����
	* @param lowcosts ��С�������ıߵ�ֵ
	* @return ���ͼ�� UDG �� DG �� ��(ǿ)��ͨͼ�Ļ�, ���� false
	*/
	bool miniSpanTreePrim(const int u, vector< pair<VerT, VerT> > &verPairs, vector<EdgeT> &lowcosts) {
		// �ж�ͼ�Ƿ�Ϸ�
		if (u < 0 || u >= verNum || GraphKind == "UDG" || GraphKind == "DG" || !isConnectedGraph(u))
			return false;

		// �������
		verPairs.clear();
		lowcosts.clear();
		// Ԥ�ȷ��� verNum ���ռ�
		verPairs.insert(verPairs.begin(), this->verNum, pair<VerT, VerT>());
		lowcosts.insert(lowcosts.begin(), this->verNum, 0);

		// V ����Ϊͼ�����ж��㼯
		// U ���ϱ�ʾ��С��������ǰ�Ķ��㼯(Ҳ���ǵ�ǰ mstVers �ļ���), ��ʼ��Ϊ�ռ�
		// closedges[i] �����洢���� ��� i \in V - U, ������ֵΪ pair<��v_iΪ��ͷ���һ�β���� U ����СȨֵ�Ļ��Ļ�βindex, 
		// first ����������Ȩ> (����������Ļ�), <-1, 0> (���û���������������Ļ�) (������ DN Ϊһ�����), 
		// ��� i \in U, �Ǿ��� pair<-2, 0>
		// P.S. ��ôϣ������ C++17 ��ģ�������Ƶ��� =,=
		vector< pair<int, EdgeT> > closedges = vector< pair<int, int> >(verNum, pair<int, EdgeT>(-1, EdgeT()));
		// U �ж��㼯
		set<int> mstVers;
		// ��ʼ��
		// mstVers �ʼ�� u
		mstVers.insert(u);
		for (int i = 0; i < verNum; i++) {
			if (u == i)
				closedges[i] = pair<int, int>(-2, EdgeT());
			else
				closedges[i] = minCostPrim(i, mstVers);
		}
		
		// ѭ������Ϊ verNum - 1
		for (int i = 0; i < verNum - 1; i++) {
			findMiniCost(closedges, mstVers, verPairs, lowcosts);
			for (int i = 0; i < verNum; i++) {
				// �����ж������� V - U �Ľ��� minCostPrim
				if (closedges[i].first > -2)
					closedges[i] = minCostPrim(i, mstVers);
			}
		}

		// �� verPairs lowcosts �� index Ϊ u ��Ԫ��ɾ��
		verPairs.erase(verPairs.begin() + u);
		lowcosts.erase(lowcosts.begin() + u);

		return true;
	}

	/**
	* ���·���㷨 -- Dijkstra
	* �㷨˼���е������� Prim ��С�������㷨
	* @param u ��ʼ������� 0 (inclusive) ~ vertex Count (exclusive)
	*/
	bool shostestPathDijkstra(const int src) {
		// ��� EdgeT ���� int, �ͷ��� false
		if (typeid(EdgeT) != typeid(int)) {
			cerr << "EdgeT expected int." << endl;
			return false;
		}

		// ��� u �Ϸ���
		if (src < 0 || src >= verNum) {
			cerr << "u illegal!" << endl;
			return false;
		}

		// ���ͼ�Ƿ���(ǿ)��ͨͼ
		if (!isConnectedGraph(src)) {
			cerr << "Grapg is NOT Connected Graph!" << endl;
			return false;
		}

		// dist ��¼��Դ�㵽�ö�������·������(����Դ���ʼ��Ϊ0, ���������Դ��ֱ������, ��Ϊ��ߵ�Ȩֵ, ������ʼ��Ϊ INFINITE)
		vector<int> dist = vector<int>(verNum, noEdge);

		// ��ʾ��V_u�����յ�����·���ϣ��˶����ǰһ��������(1��ʾ��һ������, ������Ŵ�1��ʼ)������V_u��ĳ�յ���·����
		// ����0��Ϊ��ǰһ�������š�
		vector<int> pre = vector<int>(verNum, 0);

		// �������i(i��0��ʼ)�ڵ�ǰ���·����(Shortest Path Tree)����, sptSet[i] Ϊ true
		vector<bool> sptSet = vector<bool>(verNum, false); 				

		// MinPQ ���ȶ���
		IndexMinPQ< int, HeapComparator> pq(verNum, new int(noEdge));

		// ��¼�Ѿ�����Ķ������, �� verNum - 1 ʱ��ʾ���������еĶ�����.
		int cnt = 0;

		// ��Դ�����Դ��ֱ�������Ķ������ sptSet, ���Ҹ��� pq
		// O(N * log N)
		dist[src] = 0;
		sptSet[src] = true;
		for (int w = GetFirstAdjVex(src); w > -1; w = GetNextAdjVex(src, w)) {
			dist[w] = matrixEdges[src][w];
			pre[w] = src + 1;
			// �ı���е�ֵ, O(logN)
			pq.change(w, new int(dist[w]));
		}

		// ������ĿҪ��, ���
		dist[src] = 0;
		for (int i = 0; i < dist.size(); i++)
			cout << (pre[i] == (src + 1) ? dist[i] : noEdge) << " ";
		cout << "\n";
		for (int i = 0; i < pre.size(); i++) {
			cout << (pre[i] == (src + 1) ? (src + 1) : 0) << " ";
		}

		cout << "\n\n";

		// �� cnt ������Ϊ��ֹ����, Ҳ���Խ� pq �д���sptSet�Ķ���ɾ��, �� pq �Ƿ�Ϊ������Ϊ����, ���������˷�����
		// O(N * N * log N)
		while (cnt < verNum) {
			// ����ǰ��С��������� sptSet
			int w = pq.minIndex();
			sptSet[w] = true;
			cnt++;
			// ������ sptSet �Ķ����� pq ��ֵ��Ϊ INT_MAX
			pq.change(w, new int(noEdge));
			for (int v = GetFirstAdjVex(w); v > -1; v = GetNextAdjVex(w, v)) {
				if (!sptSet[v] && dist[v] > dist[w] + matrixEdges[w][v]) {
					dist[v] = dist[w] + matrixEdges[w][v];
					pre[v] = w + 1;
					// �ı���е�ֵ, O(logN)
					pq.change(v, new int(dist[v]));
				}
			}
		}

		// ������ĿҪ��, ���
		for (int i = 0; i < dist.size(); i++)
			cout << dist[i] << " ";
		cout << "\n";
		for (int i = 0; i < pre.size(); i++)
			cout << pre[i] << " ";
		cout << "\n\n";
		searchPath(src, dist, pre);

		// �����ܵ� Time Complexity Ϊ O(N * N * log N) (���Թ���), �ܵ� Space Complexity Ϊ O(N) (����ϵ���е��, ���Ż�)
		return true;
	}
};

#endif // !Graph