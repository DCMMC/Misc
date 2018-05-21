template<class ElemType>
//顶层接口
class list
{
   public:
      //虚函数，可以被基类覆盖的方法(这里的覆盖和Java类似)
      //只有在通过基类指针或引用间接指向派生类子类型时多态性才会起作用
      //假设pBaseClass是基类指针，pSubClass是派生类指针，pSubClass的所有函数调用都只是调用自己的函数，和多态性无关；
      //pBaseClass的函数调用如果有virtual则根据多态性调用派生类的，如果没有virtual则是正常的静态函数调用，
      //还是调用基类的
      
      //下面函数声明后面紧跟赋值0是纯虚函数： 只是提供了一个可被子类型改写的接口，它本身并不能通过虚拟机制被调用
      //包含（或继承）一个或多个纯虚拟函数的类被编译器识别为抽象基类(类似Java的abstract class 或者　interface)
      virtual void ListClear() = 0; //将线性表置为空表
      virtual bool ListEmpty() const = 0; //判断线性表是否为空表
      virtual int ListLength() const = 0; //得到线性表的长度
      virtual bool SetListLength(int length) = 0; //设置线性表的长度
      virtual ElemType& GetElem(int i) const = 0; //用e返回线性表的第i个元素
      virtual bool SetElem(int i,ElemType e) = 0; //设置线性表的第i个元素
      virtual bool ListInsert(int pos,ElemType e) = 0; //在线性表的第pos个位置之前插入e元素
      virtual bool ListDelete(int pos, ElemType &e) = 0; //删除线性表的第pos个位置的元素
      virtual int LocateElem(ElemType e) = 0; //按指定条件查找
      virtual void Invert(int, int) = 0; //逆置线性表 
      virtual bool PriorElem(ElemType cur_e, ElemType &pri_e) = 0; //返回线性表给定数据元素的前驱数据元素的值
      virtual bool NextElem(ElemType cur_e, ElemType &nex_e) = 0; //返回线性表给定数据元素的后继数据元素的值
      virtual int ListTraverse() const = 0; //遍历线性表
      virtual void ListDestroy() = 0; //销毁线性表 

      //虚析构函数
      //如果基类的析构函数不是虚析构，当我们用delete来释放基类指针(它其实指向的是派生类的对象实例)占用的内存的时候，
      //只有基类的析构函数被调用，而派生类的析构函数不会被调用，这就可能引起内存泄露。
      virtual ~list(){}; 
};
 
