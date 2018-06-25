/**
* 顺序表ADT
*/

#include <iostream>
#include <stdlib.h>
#include "list.h"
#include <algorithm>

using namespace std;

	int MAXLISTSIZE = 2000;

	//模板 ElemType指代顺序表中的元素类型
	template<class ElemType>
	//继承自list<ElemType>
	class SqList:public list<ElemType>
	{
		//私有成员
		private:
			ElemType * elem;   // 存储空间基址
			int length;               // 当前长度
			int listSize;        // 允许的最大存储容量(以sizeof(ElemType)为单位
		//公共成员
		public:
			//构造器原型
			//带默认参数的声明， 如果函数的原型声明和函数体定义分开的情况， 定义中不允许默认参数
			//如果一个函数中有多个默认参数，则形参分布中，默认参数应从右至左逐渐定义。当调用函数时，只能向左匹配参数
			//好处： 默认参数可将一系列简单的重载函数合成为一个
			//这里如果直接调用SqList()就相当于调用了SqList(20)
			//初始化顺序表
			SqList(int ms = 20);

			//析构函数
			//也就是在对象被销毁的时候自动调用
			//删除顺序表
			~SqList(){
				delete [] elem;
			}

			/* 函数原型加定义 */

			int GetListSize() {
				return listSize;
			}
			 
			//将顺序表置为空表
			void ListClear() {
				length = 0;
			}

			//常量成员函数， 说明该函数不会改变所在的对象(中的内容)
			//const成员函数应该在函数原型说明和函数定义中都增加const限定
			//非常量成员函数不能被常量成员对象调用，因为它可能企图修改常量的数据成员
			//返回顺序表的长度
			int ListLength() const {
				return length;
			}

			/* 函数原型声明 */

			//设置顺序表的长度
			bool SetListLength(int len);
			//判断顺序表是否为空表
			bool ListEmpty() const;
			//判断顺序表是否为满表 
			bool ListFull() const;
			//用e返回顺序表的第i个元素
			ElemType& GetElem(int i) const;
			//用e设置顺序表的第i个元素
			bool SetElem(int i, ElemType e);
			//在顺序表的第pos个位置之前插入e元素
			bool ListInsert(int pos,ElemType e);
			//删除顺序表的第pos个位置的元素
			bool ListDelete(int pos, ElemType &e);
			//equal函数，用来判断a和b是否相等
			bool equal(ElemType a, ElemType *b) const;
			//比较两个数值, -1表示a < b, 0 表示等于, 1表示a > b
			int compare(ElemType a, ElemType b) const;
			//按指定条件查找
			int LocateElem(ElemType e);
			//逆置顺序表
			void Invert(int, int); 
			//返回线性表给定数据元素的前驱数据元素的值
			bool PriorElem(ElemType cur_e, ElemType &pri_e); 
			//返回线性表给定数据元素的后继数据元素的值
			bool NextElem(ElemType cur_e, ElemType &nex_e); 
			//销毁线性表
			void ListDestroy(); 
			//遍历顺序表
			int ListTraverse() const;

			ElemType& operator[](std::size_t rhs) {
				return GetElem(rhs + 1);
			}

			const ElemType& operator[](std::size_t rhs) const {
				return GetElem(rhs + 1);
			}

			void exch(int i, int j) {
				if (i < 0 || j < 0 || i >= listSize || j >= listSize)
					return;

				ElemType e = elem[i];
				elem[i] = elem[j];
				elem[j] = e;
			}

			/**
			* 时间复杂度为0(n)、空间复杂度为0(1)的算法
			* 删除后的数组元素与原数组元素不必保持顺序一致。
			* 就是把数组最后面的元素移动覆盖掉被删除的元素
			*/
			void deleteItem(ElemType item) {
				for (int i = 0; i < length; i++) {
					if (elem[i] == item) {
						// 如果末尾的元素(s)与 item 相等, 就一直向前找, 直到遇到了i 
						while (i < length - 1 && length > 0 && length-- && elem[length] == item)
							continue;

						// 如果 length - 1就是i或者类似length是0, i也是0 的情况 
						if (i > length - 1)
							return;
						if (i == length - 1) {
							length--;
							return;
						}

						elem[i] = elem[length];
					}
				}
			}
	};

	/* 定义SqList中的未定义的函数 */

	//构造器
	template<class ElemType>
	SqList<ElemType>::SqList(int maxsize){
		// 构造一个空的线性表L
		if( maxsize == 0 ) 
			maxsize = MAXLISTSIZE;

		if (maxsize > MAXLISTSIZE)
			MAXLISTSIZE = maxsize;

		//开辟空间
		elem = new ElemType[maxsize];

		if(!elem)   
			exit(10);             // 严重错误， 存储分配失败
		length = 0;              // 顺序表的初始长度为0
		listSize = maxsize;      // 该顺序表可以存储元素的最大容量
	}

	template<class ElemType>
	bool SqList<ElemType>::ListEmpty() const{
		 //判断线性表是否为空     
		 if(length == 0) return true;//如果当前元素个数为0说明为空表         
		 return false; 
	}

	template<class ElemType>
	bool SqList<ElemType>::ListFull() const{
		 //判断线性表是否为满     
		 if(length == listSize)
			 return true;//如果当前元素个数为最大值说明为满表         
		 return false; 
	}

	template<class ElemType>
	bool SqList<ElemType>::SetListLength(int len){
		 //判断设置的长度的合法性     
		 if(len < 1 || len > MAXLISTSIZE)
			 return false;
		 length = len;         
		 return true; 
	}

	template<class ElemType>
	ElemType& SqList<ElemType>::GetElem(int i) const{
		if (i < 1 || i > listSize)
		{
			wcout << L"位置i = " << i << L" 不合法(应该在1 ~" << listSize << L"范围内)" << endl;
			exit(i); // 位置不合法
		}
     
		//返回线性表L的第i个元素     
		return *(elem + i - 1);     
	}

	template<class ElemType>
	bool SqList<ElemType>::SetElem(int i, ElemType e){
		 if( i < 1 || i > listSize) 
			 return false; // 位置不合法
		 //设置线性表L的第i个元素     
		 *(elem + i - 1) = e;
		 return true;     
	}

	template<class ElemType>
	void SqList<ElemType>::Invert( int s, int t ){
		 // 本算法将顺序表 中下标自 s 到 t 的元素逆置，即将
		 // （Rs,Rs+1,…,Rt-1,Rt）改变为（Rt,Rt-1,…,Rs+1,Rs）
		 ElemType w; 
		 for ( int k=s; k<=(s+t)/2; k++ ){
			 w = elem[k];
			 elem[k] = elem[t-k+s];
			 elem[t-k+s] = w;
		 } // for
	} // invert 

	template<class ElemType>
	bool SqList<ElemType>::ListInsert(int pos,ElemType e){
		 // 若存储空间不满且1≤pos ≤LengthList(L)+1，则在顺序表L的
		 // 第pos个元素之前插入新的元素e且返回TRUE，否则返回FALSE。
		 if( pos < 1 || pos > listSize) 
			 return false; // 插入位置不合法
		 if( length >= MAXLISTSIZE ) 
			 return false; // 当前存储空间已满，无法插入
		 for( int j = length - 1; j >= pos - 1; --j)  
			 elem[j+1] = elem[j];      // 插入位置及之后的元素后移
		 elem[pos-1] = e;              // 插入e
		 ++length;                     // 表长增1
		 return true;
	} // ListInsert

	template<class ElemType>
	bool SqList<ElemType>::ListDelete(int pos, ElemType &e){
		  // 若1≤pos ≤LengthList(L) ，则以e带回从顺序表L中删除的
		  // 第pos个元素且返回TRUE，否则返回FALSE。
		  if( pos < 1 || pos > length ) 
			  return false;         // 删除位置不合法
		  e = elem[pos-1];
		  for( int j = pos; j <= length; ++j)  
			  elem[j-1] = elem[j];       // 被删除元素之后的元素左移
		  --length;                         // 表长减1
		  return true;
	} // ListDelete

	template<class ElemType>
	int SqList<ElemType>::compare(ElemType a, ElemType b) const {
		return a < b ? -1 : (a == b ? 0 : 1);
	}

	template<class ElemType>
	bool SqList<ElemType>::equal(ElemType a, ElemType *b) const {
		//compare函数，用来判断a和b是否相等     
		if (a == *b)
			return true;
		//如果相等则返回-1     
		else return false;
	}

	template<class ElemType>
	//int SqList<ElemType>::LocateElem(ElemType e,bool (* compare)(ElemType, ElemType *)){
	int SqList<ElemType>::LocateElem(ElemType e){
		//按指定条件查找
		int i;      
		for(i = 0; i < length; i++){          
		   if ((equal(e, elem + i)) == false)
			   continue;
		   //如果e不等于第i个元素就继续查找         
		   else 
			   break;     
		 }      

		 if(i == length) 
			 return -1; //如果搜索完毕都没有找到e这个元素，说明e不是L中的元素返回0
		 else 
			 return i;//如果找到了的话就返回下标 
	}  

	template<class ElemType>
	bool SqList<ElemType>::PriorElem(ElemType cur_e, ElemType &pre_e){
		//查找e的前趋
		if(elem){
			if(LocateElem(cur_e) && LocateElem(cur_e) != 0){
				//如果e是L中的元素，并且e对应的下标不为0
				pre_e =*(elem + LocateElem(cur_e) - 1);
				return true;
			} else 
				return false;
		 }
		 return false;
	}

	template<class ElemType>
	bool SqList<ElemType>::NextElem(ElemType cur_e, ElemType &nex_e){
		//查找e的后继 
		if (elem) {
			if (LocateElem(cur_e) >= 0 && LocateElem(cur_e) <= length - 1) {
				//如果e是L中的元素，并且e对应的下标不为最后元素 
				nex_e = *(elem + LocateElem(cur_e) + 1);
				return true;
			}
			else {
				return false;
			}
		 }

		 return false;
	}

	template<class ElemType>
	int SqList<ElemType>::ListTraverse() const{
			//打印顺序表     
		   for(int i = 0; i < length; i++){         
			  cout << *(elem + i) << " ";
		   }      
		   //cout << endl; 
		   return 1;
	}

	template<class ElemType>
	void SqList<ElemType>::ListDestroy() {
			//销毁顺序表     
			delete elem;
			listSize = 0;
			length = 0;   
	}

	bool less(int a, int b) {
		return a < b;
	}

	class MergeSort {
	private:
		/**
		* Abstract in-place Merge
		* 采用辅助数组的方法(会使用大量的额外空间)来归并两部分结果
		* 也就是归并a[lo...mid]和a[mid + 1...hi]
		* 整体思路就是分别用两个int表示这两部分的下标, 从这两个部分的第一个元素开始往后递推, 分别从两个子序列中选择最小的那个元素作为
		* 放在指定的位置
		* @param aux
		* 		暂存数组
		* @param a
		*		目标数组
		* @param lo
		*		要归并的前半部分的起始下标
		* @param mid
		*		要归并的前半部分的最后一个元素的下标
		* @param hi
		*		要归并的后半部分的最后一个元素的下标
		*/
		////////////// 这里有一个需要注意的地方, C++函数的(类变量的)值传递, 会调用类的构造器复制成函数内部的一个局部变量
		////////////// 副本, 并且在函数退出的时候, 会调用其析构器(因为是局部变量), 如果还想在函数返回之后继续使用作为参数的
		////////////// 类变量或者为了效率高一点(避免不必要的复制和析构), 请使用 passed-by-reference
		static void merge(vector<int>& aux, SqList<int>& a, int lo, int mid, int hi) {
			//先将数据暂存在辅助数组中
			for (int i = lo; i <= hi; i++)
				aux[i] = a[i];

			//i, j分别为两部分的第一个元素的下标
			int i = lo;
			int j = mid + 1;
			//归并
			for (int k = lo; k <= hi; k++) {
				if (i > mid)
					a[k] = (aux[j++] + 0);
				else if (j > hi)
					a[k] = (aux[i++] + 0);
				else if (::less(aux[j], aux[i]))
					a[k] = (aux[j++] + 0);
				else
					a[k] = (aux[i++] + 0);
			}
		}

	public:
		/**
		* 归并排序
		* 采用自底向上的方法非递归的归并排序数组
		*/
		static void mergeSort(SqList<int> a) {
			int length = a.ListLength();
			//创建辅助数组, 只额外分配一次
			vector<int> aux = vector<int>(length, 0);

			for (int sz = 1; sz < length; sz *= 2) {
				for (int lo = 0; lo < length - sz; lo += (2 * sz)) {
					merge(aux, a, lo, lo + sz - 1, min(lo + sz + sz - 1, length - 1));
				}
				a.ListTraverse();
			}
		}
	};


	bool lessThan(int a, int b) {
		return a < b;
	}

	class InsertionSort {
	public:
		/**
		* 插入排序法
		* 时间复杂度比较取决于目标数组, 目标数组越接近于完全正序, 时间复杂度就越低
		* 交换的次数就是逆序数对的对数, 比较的次数就是交换的次数+(N-1)
		* 平均情况下(这里取的是每一个item都是进行到一半就找到了位置), 比较 ~ 1/4N^2, 交换 ~ 1/4N^2
		* 在最坏情况(完全倒序), 比较 ~ 1/2N^2, 交换 ~ 1/2N^2
		* 最好情况(完全正序), 比较 ~ N-1, 交换 0
		* @param a
		* 		要排序的数组
		*/
		static void insertionSort(SqList<int> a) {
			#ifdef DEBUG
			a.ListTraverse();
			#endif

			for (int i = 1; i < a.ListLength(); i++) {
				//找到a[i]前面的序列(前面的序列都是由小到大排序好了的)中, 在a[i]应该所在的位置插入(这里通过一个一个交换来实现, 如果用
				//链表效率肯定会高一些 见Ex 2.1.25)
				for (int j = i; j > 0 && ::lessThan(a[j], a[j - 1]); j--) {
					a.exch(j, j - 1);
				}
				#ifdef DEBUG
				a.ListTraverse();
				#endif
			}
		}
	};

	class ShellSort {
	public:
		/**
		* 希尔排序
		* 说实话, 这里用的希尔排序 Donald Shell 的 Increment Sequence, 基本对于 InsertionSort没有多大提升
		* 一般来说推荐使用 Donald Knuth 的 h = h*3 + 1 作为 Increment Sequence
		*/
		static void shellSort(SqList<int>& a) {
			int h = 5; // 5, 3, 1 
			while (h >= 1) {
				//以当前h做h-sorting
				for (int i = h; i < a.ListLength(); i++) {
					for (int j = i; j >= h && (::lessThan(a[j], a[j - h])); j -= h)
						a.exch(j, j - h);
				}
				#ifdef DEBUG
				cout << h << "\n";
				a.ListTraverse();
				#endif

				h -= 2;
			}
		}
	};

	class HeapSort {
	public:
		/**
		* 堆排序
		*/
		static void heapSort(SqList<int>& a) {
			int N = a.ListLength();

			//phase 1, Heap construction
			//可以从第二个元素开始, 从左往右进行swim, 当前指针之前的元素就是heap-ordered的了, 后面的元素是待构造的元素, 指针到最后
			//一个元素的时候, 整个数组就是heap-ordered的了, 时间消耗O(NlogN)
			//一个更加高效的方法是从右往左进行sink, 从数组的floor(N/2)位置开始(N对应的是heap的最后一个元素, 而N/2代表最后一个元素的父节点,
			//这个结点也就是最后一个拥有子节点的结点了, 后面的结点都是最后一个level的)进行sink, 最开始这几个sink相当于reheapifying
			//三个元素的subheap, 然后逐渐reheapifying 7个元素的heaps... (结合heap的树形表示), 这样sink最后一个元素相当于reheapifying
			//整个数组代表的tree, 这时候就已经是heap-ordered的了.
			//sink-based heap construction在最坏情况下的交换次数为N, 比较次数为2N(因为每次sink调用要比较两次), 这个可以通过heap的树形图
			//来理解, N个元素的heap的binary tree的`!$\lfloor \log N \rfloor + 1$`层, N/2这个位置是在倒数第二层(也就是
			//`!$\lfloor \log N \rfloor$`层的最后一个有子结点的结点, 简称为k层), 然后最多有`!$2^k$`个有三个元素的subheap需要sink(也
			//就是最后一层全满的情况下), 遍历完这些最多3个元素的subheap之后需要遍历`!$2^{k - 1}$`个最多7个元素的subheap(也就是三层的树),
			//... 一直到最后sink整个树, 加入每次都需要交换, 则最多需要`!$2^k \sum_{i = 0}^{i = k} \frac {i + 1} {2^i} \dot{=} N$`
			//例如一个127个元素的heap, 需要sink 32个大小为3的subheaps, 16个大小为7的subheaps, 8个大小为15的subheaps, 4个大小为31的subheaps,
			//2个大小为63的subheaps, 一个大小为127的heap, 最坏情况下的交换次数就是32*1 + 16*2 + 8*3 + 4*4 + 2*5 + 1*5 = 120.
			//建一个大根堆
			for (int k = N / 2; k > 0; k--)
				sink(a, k, N);

			#ifdef DEBUG
			a.ListTraverse();
			cout << "\n";
			#endif 

			//phase 2, Sortdown
			//因为Heap construction之后数组中的元素是按照heap binary tree的顺序存储的, 所以为了调整为由小到大的顺序, 需要将前面的较大的
			//元素移动到数组的后面去, 第一次把第一个元素(也就是最大的元素)和最后一个元素交换, 这样pq[N]就是最大的元素了, 然后在sink(1, N - 1)
			//把最后一个元素(这叫已排序的部分)之前的元素重新reheapifying, 然后在把第一个元素和第N - 1个元素交换... 这样就可以使整个数组变成由小到大的
			//顺序了, 按照前面证明的sink的比较次数为2logN, 可以得到Sortdown的比较次数为2NlogN
			while (N > 1) {
				a.exch(0, --N);
				sink(a, 1, N);
				#ifdef DEBUG
				a.ListTraverse();
				#endif
			}

			//所以总的比较次数要少于2NlogN + 2N, 交换次数少于NlogN + N
		}

		/**
		* Top-down reheapofying (sink)
		* O(logN)
		* 通过不断与其较大的子节点进行交换直到两个子节点的key都要小于等于该node, 或者是已经到了heap的bottom
		* k节点的两个子节点的index分别为2k和2k + 1
		* @param a 目标数组
		* @param k 要操作的node的位置 0 <= k <= N
		* @param N binart heap的最后一个元素的index
		*/
		static void sink(SqList<int>& a, int k, int N) {
			//其左边那个子节点要存在, 不然的话就已经到了heap的bottom了
			while (k * 2 <= N) {
				int j = 2 * k;
				if (j < N && (::less(a[j - 1], a[j])))
					++j;
				if (!::less(a[k - 1], a[j - 1]))
					break;
				//debug
				//cout << "heapify a[" << k << "] with a[" << j << "]\n";
				a.exch(k - 1, j - 1);
				k = j;
			}
		}
	};

	class BubbleSort {
	public:
		/**
		* 冒泡排序
		* @param a 要排序的数组
		*/
		static void bubbleSort(SqList<int>& a) {
			// inner loop的左端点为0, 右端点从n - 1开始, 每次inner loop都会减去1
			// 并且需要有一个boolean变量用来记录上一轮inner loop是否有交换行为, 如果没有, 说明剩下的inner loop都不需要了, 因为未排序
			// 的已经是有序的了. 所以冒泡排序的排序趟数跟数组初始顺序有关.
			// inner loop每次需要相邻的大小相反的都会交换, 这样能确保inner loop的最右边元素就是待排序部分的最大元素
			// out loop 为从0..n-1或n-1到0并且上一轮inner loop无交换行为

			// changed初始为true, 记录上一轮inner loop是否有交换
			bool changed = true;
			for (int i = 0; i < a.ListLength() - 1 && changed; i++) {
				changed = false;
				for (int j = 0; j < a.ListLength() - i - 1; j++)
					if (::less(a[j + 1], a[j])) {
						changed = true;
						a.exch(j, j + 1);
					}

				#ifdef DEBUG
				a.ListTraverse();
				#endif
			}
		}
	};

	class SelectionSort {
	public:
		/**
		* 选择排序法
		* 时间复杂度不太取决于目标数组, 反正效率都不高
		* 一次一次的找到每一轮的最小值并放到这一轮的第一个位置
		* ~ 1/2N^2(固定这么多次) compare, ~ N(最差情况下) exchange
		* 数据交换这一方面的开销小是优点, 总的效率低的缺点
		*/
		static void selectionSort(SqList<int>& a) {
			for (int i = 0; i < a.ListLength() - 1; i++) {
				int min = i;

				for (int j = i + 1; j < a.ListLength(); j++)
					if (::less(a[j], a[min]))
						min = j;

				a.exch(i, min);

				#ifdef DEBUG
				a.ListTraverse();
				#endif
			}
		}
	};

	class QuickSort {
	public:
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
		static void quickSort(SqList<int>& a) {
			#ifdef DEBUG
			a.ListTraverse();
			#endif
			quickSort(a, 0, a.ListLength() - 1);
		}

		/**
		* 快速排序指定部分
		* @param a
		* 		要排序的数组
		* @param lo
		*		要排序的范围的下界
		* @param hi
		* 		要排序的范围的上界
		*/
		static void quickSort(SqList<int>& a, int lo, int hi) {
			if (lo >= hi)
				return;

			//分区的分界位置
			int j = partition(a, lo, hi);

			#ifdef DEBUG
			cout << j << " " << a[j] << "\n";
			a.ListTraverse();
			#endif

			quickSort(a, lo, j - 1);
			quickSort(a, j + 1, hi);
		}

	private:
		/**
		* 原地分区(In-place Partition)
		* @param a
		*		要分区的数组
		* @param lo
		*		分区范围的下界
		* @param hi
		*		分区范围的上界
		*/
		static int partition(SqList<int>& a, int lo, int hi) {
			int pivotkey = a[lo];

			while (lo < hi) {
				while (lo < hi && a[hi] >= pivotkey)
					--hi;
				a.exch(lo, hi);
				while (lo < hi && a[lo] <= pivotkey)
					++lo;
				a.exch(lo, hi);
			}

			return lo;
		}

	public:
		/**
		* 用最少的时间来将数组中的所有值为负数的元素移到全部正数值元素前边 (其实怎么都是O(n), 不过是让常数因子小一点)
		* 采用类似于 QuickSort的算法
		*/
		static void rearrange(SqList<int>& a) {
			int lo = 0, hi = a.ListLength() - 1;

			#ifdef DEBUG
			a.ListTraverse();
			#endif

			while (lo < hi) {
				while (lo < hi && a[hi] >= 0)
					--hi;
				a.exch(lo, hi);
				while (lo < hi && a[lo] <= 0)
					++lo;
				a.exch(lo, hi);
			}

			#ifdef DEBUG
			a.ListTraverse();
			#endif
		}
	};

	class BinaryInsertionSort {
	public:
		/**
		* 折半插入排序
		*/
		static void binaryInsertionSort(SqList<int>& a) {
			#ifdef DEBUG
			a.ListTraverse();
			#endif

			for (int i = 1; i < a.ListLength(); i++) {
				int lo = 0, hi = i - 1;
				int m = 0;
				//折半查找适合的位置
				while (lo <= hi) {
					m = (lo + hi) / 2;
					if (a[i] < a[m])
						hi = m - 1;
					else
						lo = m + 1;
				}

				int tmp = a[i];
				//后移
				for (int j = i - 1; j >= hi + 1; --j)
					a[j + 1] = a[j];
				a[hi + 1] = tmp;

				#ifdef DEBUG
				a.ListTraverse();
				#endif
			}
		}
	};
