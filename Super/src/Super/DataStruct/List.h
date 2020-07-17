#ifndef _List_H_
#define _List_H_

#include <iostream>
using namespace std;
//#ifdef __cplusplus

#include <exception>


//学会迭代器后，补充实现迭代器版本


//1、顺序表
//顺序存储的线性表称为顺序表。表中元素的逻辑顺序与物理顺序相同。
//1.1 静态存储
//在静态分配时，由于数组的大小和空间已经固定，一旦空间占满，再加入新的数据产生溢出，就会导致程序崩溃。
//1.2动态存储
//在动态分配时，存储数组的空间是在程序执行中通过动态存储分配的，一旦已分配空间占满，
//可以另外开辟一块更大的存储空间，用以代换原来的存储空间，从而达到扩充存储数组空间的目的。

//rafactor by lizulin

//1.尾插，2.头插，3.显示，4.尾删，5.头删，6.按位置，7.插按值插，8.按位置删，9.按值删，10.按值查，11.求表长，12.清除数据，13.摧毁该顺序表，14.反转，15.排序(冒泡排序，快速排序)。


//顺序表


//by lizulin 20181201
template <typename T> 
class SeqList
{
    typedef SeqList<T> self;
public:
    SeqList(size_t capacity)
        :m_Capacity(capacity),m_Currpos(0)
    {
          pArray=new T[m_Capacity];
    }
    ~SeqList()
    {
        delete[] pArray;
    }

    //to do:
public:
    //self& SeqList(const SeqList val)
    //{
    //    return *this;
    //}
    //self& operator==(const SeqList val)
    //{
    //     return *this;
    //}


    T& operator[](size_t n)const { return pArray[n];}
    T& at(size_t n)const 
    {
        if (n>=m_Capacity)
        {
            throw std::out_of_range;//return T;
        }
        return pArray[n];
    }
    size_t Capacity() const { return m_Capacity;}
    size_t Length() const { return length;}
    bool IsEmpty()const {return Length()==0;}
    bool IsFull()const {return Length()==Capacity();}
    //to do test
    void PushFront(const T& t)
    {
        if (IsFull()) { return; }

        Insert(0,t);
    }
    void PushBack(const T& t)
    {
        if (IsFull()) { return; }
        Insert(0,length);
    }
    void PopFront()
    {
        if (IsEmpty()){ return; }
        T tmp;
        eraser(0,tmp);
    }
    void PopBack()
    {
        if (IsEmpty()){ return; }
        T tmp;
        eraser(0,length);
    }
    void Insert(size_t pos, T& t)
    {
        if (pos>=m_Capacity)
        {
            throw std::out_of_range;
        }
        size_t n;
        for (n= this->length; n> pos; n--)
        {
            pArray[n] = pArray[n-1];
        }
        pArray[pos]=t;
        this->length++;
    }
    void eraser(size_t pos, T& t)
    {
        if (pos>=m_Capacity)
        {
            return;
        }
        t = pArray[pos];     //对象复制
        size_t n;
        for (n= this->length; n> pos; n--)
        {
            pArray[n-1] = pArray[n];
        }
        this->length--;
        return true;
    }
    void Print();
private:
    T *pArray;
    const size_t m_Capacity;
    size_t length;
};




//单链表
//2、	单链表
//ListNode.h

template<typename T> 
class SingleList;

template<typename T> 
class SingleListNode
{
public:
    SingleListNode():pNext(NULL){}
    SingleListNode(const T item,SingleListNode<T> *next=NULL):m_data(item),pNext(next){}
    ~SingleListNode()
    {
        pNext=NULL;
    }
public:
    T GetData() const {return this->m_data;}
    friend typename SingleList<T>;
    friend ostream& operator<< <T>(ostream& ,SingleListNode<T>&);
    //ostream& operator<<(ostream& os,SingleListNode<T>& out)
    //{
    //    os<<out.m_data;
    //    return os;
    //}
private:
    T m_data;
    SingleListNode *pNext;
};

template<typename T> 
class SingleList
{
public:
    SingleList():head(new SingleListNode<T>()){}
    ~SingleList()
    {
        DestoryAllNode();
        delete head;
    }
public:
    void DestoryAllNode();                                           //make the list empty
    //size_t Length()const {return length;}                     //get the length
    size_t Length()const;                                              //get the length
    SingleListNode<T> *Find(T value,unsigned int n); //find thd nth data which is equal to value
    SingleListNode<T> *Find(unsigned int n);              //find the nth data
    bool Insert(T item,unsigned int n=0);                //insert the data in the nth position
    T Remove(unsigned int n=0);                           //remove the nth data
    void RemoveAll(T item);                    //remove all the data which is equal to item
    //Type Get(int n);                               //get the nth data
    bool Get(T& item,unsigned int n);     //get the nth data
    void Print();                                       //print the list
private:
    SingleListNode<T> *head;
};

template<typename T> 
void SingleList<T>::DestoryAllNode()
{
    SingleListNode<T> *pdel;
    while(head->pNext!=NULL)
    {
        pdel=head->pNext;
        head->pNext=pdel->pNext;
        delete pdel;
    }
}

template<typename T> 
size_t SingleList<T>::Length() const
{
    SingleListNode<T> *pNode=head->pNext;
    size_t Cnt=0;
    while(pNode!=NULL)
    {
        pNode=pNode->pNext;
        Cnt++;
    }
    return Cnt;
}

template<typename T> 
SingleListNode<T>* SingleList<T>::Find(unsigned int n)
{
    if(n<0)
    {
        cout<<"The n is out of boundary"<<endl;
        return NULL;
    }
    SingleListNode<T> *pmove=head->pNext;
    for(int i=0;i<n&&pmove;i++)
    {
        pmove=pmove->pNext;
    }
    if(pmove==NULL)
    {
        cout<<"The n is out of boundary"<<endl;
        return NULL;
    }
    return pmove;
}

template<typename T> 
SingleListNode<T>* SingleList<T>::Find(T value,unsigned int n)
{
    //if(n<1)
    //{
    //    cout<<"The n is illegal"<<endl;
    //    return NULL;
    //}
    SingleListNode<T> *pmove=head;
    int count=0;
    while(count!=n&&pmove)
    {
        pmove=pmove->pNext;
        if(pmove->m_data==value)
        {
            count++;
        }
    }
    //if(pmove==NULL)
    //{
    //    cout<<"can't find the element"<<endl;
    //    return NULL;
    //}
    return pmove;
}

template<typename T> 
bool SingleList<T>::Insert(T item, unsigned int n)
{
    //if(n<0)
    //{
    //    cout<<"The n is illegal"<<endl;
    //    return ;
    //}
    SingleListNode<T> *pmove=head;
    SingleListNode<T> *pnode=new SingleListNode<T>(item);
    //if(pnode==NULL)
    //{
    //    cout<<"Application error!"<<endl;
    //    return 0;
    //}
    for(int i=0;i<n&&pmove;i++)
    {
        pmove=pmove->pNext;
    }
    //if(pmove==NULL)
    //{
    //    cout<<"the n is illegal"<<endl;
    //    return 0;
    //}
    pnode->pNext=pmove->pNext;
    pmove->pNext=pnode;
    return true;
}

template<typename T> 
void SingleList<T>::RemoveAll(T item)
{
    SingleListNode<T> *pmove=head;
    SingleListNode<T> *pdel=head->pNext;
    while(pdel!=NULL)
    {
        if(pdel->m_data==item)
        {
            pmove->pNext=pdel->pNext;
            delete pdel;
            pdel=pmove->pNext;
            continue;
        }
        pmove=pmove->pNext;
        pdel=pdel->pNext;
    }
    return;
}

template<typename T> 
T SingleList<T>::Remove(unsigned int n)
{
    //if(n<0)
    //{
    //    cout<<"can't find the element"<<endl;
    //    exit(1);
    //}
    SingleListNode<T> *pmove=head,*pdel;
    for(int i=0;i<n&&pmove->pNext;i++)
    {
        pmove=pmove->pNext;
    }
    //if(pmove->m_pnext==NULL)
    //{
    //    cout<<"can't find the element"<<endl;
    //    exit(1);
    //}
    pdel=pmove->pNext;
    pmove->pNext=pdel->pNext;
    T temp=pdel->m_data;
    delete pdel;
    return temp;
}

template<typename T> 
bool SingleList<T>::Get(T& item,unsigned int n)
 {
    //if(n<0){
    //    cout<<"The n is out of boundary"<<endl;
    //    exit(1);
    //}
    SingleListNode<T> *pmove=head->pNext;
    for(int i=0;i<n;i++)
    {
        pmove=pmove->pNext;
        if(NULL==pmove)
        {
            cout<<"The n is out of boundary"<<endl;
            //exit(1);
        }
    }
    return pmove->m_data;
}

template<typename T> 
void SingleList<T>::Print()
{
    SingleListNode<T> *pmove=head->pNext;
    cout<<"head";
    while(pmove)
    {
        cout<<"--->"<<pmove->m_data;
        pmove=pmove->pNext;
    }
    cout<<"--->over"<<endl<<endl<<endl;
}


////3、	双向链表 即双向遍历链表
//把常用容器也放到basic


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//一种可以根据所在节点指针快速删除节点的循环链表的实现
template<typename T> class DoubleList;
template<typename T>
class DoubleListNode
{
public:
    T data;
    DoubleListNode<T>* pPrevious;
    DoubleListNode<T>* pNext;
    DoubleListNode()
    {
        pPrevious=NULL;
        pNext=NULL;
    }
    friend class DoubleList<T>;
};

template<typename T>
class DoubleList
{
private:
    DoubleListNode<T>* pHead;  //头节点，始终存在，不存放数据
    DoubleListNode<T>* pTail;     //最尾部节点，没有数据则为空
    size_t length;
public:
    DoubleList();
    ~DoubleList();
    //待实现
    DoubleList(const DoubleList& s);                             //拷贝构造
    DoubleList &operator=(DoubleList& s);                  //赋值运算符的重载

    DoubleListNode<T>* CreateNode();                       //分配内存，构建节点
    void DestoryNode(DoubleListNode<T>* pNode);    //释放
    
    //push元素并且返回node指针
    DoubleListNode<T>* pushBack(const T& data);      //添加节点到链表尾,
    DoubleListNode<T>* pushFront(const T& data);     //添加节点到链表尾

    ////快速版本,作为内存池的容器使用不应该内部再调用new node操作，否则没有意义
    ////用事先申请好的node
    //DoubleListNode<T>* pushBackFast(const T& data);      //添加节点到链表尾,
    //DoubleListNode<T>* pushFrontFast(const T& data);     //添加节点到链表尾
    //DoubleListNode<T>* do_pushBack(const T& data);      //添加节点到链表尾,
    //DoubleListNode<T>* do_pushFront(const T& data);     //添加节点到链表尾

    void PopBack();                                                         //尾删
    void PopFront();                                                        //头删除
    DoubleListNode<T>* search(const T &val) const;    //查找与某个值相同的节点
     size_t Length()const {return length;}                        //取链表长度，非O1操作
     bool empty()const {return length==0;}                     //返回链表是否为空

    T pop();  //弹出链表最后一个节点，并返回值  //失败抛异常?
    bool objectAt(T& data,size_t index);                        //通过index来查找链表中的元素
    bool insert(const T& data,size_t index);                   //插入元素到指定位置的前方
    void ClearAll();
    void ClearAllReverseOrder();                                    //从尾部逆序清空

    //知道某个节点的指针，可以求出前后驱指针，可以快速删除节点
    void removeNodeByItsAddrress(void* pCurr);
    void eraser(DoubleListNode<T>* pNode);  //根据节点指针，清空节点

    //实现类似迭代器遍历的方法
    DoubleListNode<T>* getBegin(){return pHead;}
    DoubleListNode<T>* hasNext(DoubleListNode<T>* pNode);   //判断是否存在下一个并返回，失败返回NULL
    T getData(DoubleListNode<T>* pNode){return pNode->data;}
    void TraverInOrder();                //顺序遍历
    void TraverReverseOrder();       //逆序遍历
    void Reverse();                          //链表逆序
    void print();                             //打印链表的所有元素
};



#if 0
//赋值运算符的重载（传统方法）
    //SList & SList::operator=(const SList& s)    
    //{
    //  if (this != &s)
    //  {
    //      _head = NULL;
    //      _tail = NULL;
    //      LinkNode* tmp = s._head;
    //  do{
    //      PushBack(tmp->_data);
    //      tmp = tmp->_next;
    //       } while (tmp != s._head);
    //  }
    //  return *this; 
    //}
 
    //赋值运算符的重载（高效写法）*
    /*void SList::Swap(SList& s)
    {
    swap(_head, s._head);
    swap(_tail, s._tail);
 
    }
    SList&  SList::operator=(SList &s)
    {
    if (this != &s)
    {
    SList tmp(s);
    Swap(tmp);
    }
    return *this;
    }*/
 
    List&  List::operator=(List &s)     //赋值运算符的重载再优化(推荐写法)
    {
        if (this != &s)
        {
            swap(_head, s._head);
            swap(_tail, s._tail);
        }
        return *this;
    }
#endif


template<typename T> 
DoubleList<T>::DoubleList()
{
    pHead = CreateNode();
    pTail = pHead;    //设计有问题，待续
    //pTail=NULL;     //默认没有尾部

    //头节点不存放实际数据   add
    //pHead->pPrevious=NULL;
    //pHead->pNext=NULL;
    length = 0;
}

template<typename T>
DoubleList<T>::~DoubleList()
{
    ClearAll();  //new add 
    //释放头节点
    DestoryNode(pHead);
}

template<typename T>
DoubleListNode<T> * DoubleList<T>::CreateNode()
{
    DoubleListNode<T>* pNode = (DoubleListNode<T>*)malloc(sizeof(DoubleListNode<T>));
    return pNode;
    //return new DoubleListNode<T>;
}

template<typename T>
void DoubleList<T>::DestoryNode(DoubleListNode<T>* pNode)
{
    //::free(pNode);
    delete pNode;   //论证free是O1操作
}

template<typename T>
DoubleListNode<T>* DoubleList<T>::pushBack(const T& data)
{
    DoubleListNode<T> *pNode = CreateNode();
    pNode->data = data;
    pNode->pPrevious = pTail;
    pNode->pNext=NULL;
    pTail->pNext = pNode;
    pTail = pNode;
    length++;
    return pNode;
}

template<typename T>
DoubleListNode<T>* DoubleList<T>::pushFront(const T& data)
{
    DoubleListNode<T> *pNode = CreateNode();
    //待续
    pNode->data = data;
    DoubleListNode<T> *pFirstOld=pHead->pNext;    //原先head之后第一个
    pNode->pPrevious=pHead;
    pNode->pNext=pFirstOld;

    pFirstOld->pPrevious=pNode;
    pHead->pNext=pNode;
    length ++;
    return pNode;
}

template<typename T>
void DoubleList<T>::PopFront()
{
    DoubleListNode<T> *pNode =pHead->pNext;
    if (pNode!=NULL)
    {
        eraser(pNode);
    }
}

template<typename T>
void DoubleList<T>::PopBack()
{
    DoubleListNode<T>* p = pTail->pPrevious;
    while(p!=NULL&&p!=pHead)   //逆序删除注意，不能删除头
    {
        eraser(pNode);
    }
}


template<typename T>
T DoubleList<T>::pop()
{
    T data;
    DoubleListNode<T>* pNode = pHead->pNext;
    while (pNode->pNext) 
    {
        pNode = pNode->pNext;
    }
    data = pNode->data;
    pTail = pNode->pPrevious;
    pTail->pNext = pNode->pNext;
    length--;
    DestoryNode(pNode);
    pNode = NULL;
    return data;
}

template<typename T>
bool DoubleList<T>::objectAt(T& data,size_t index)
{
    if(index>length)
    {
        return false;
    }
    DoubleListNode<T>* q = pHead;
    for(size_t i=0; i < index;i++)
    {
        q = q->pNext;
    }
    data = q->data;
    return true;
}

template<typename T>
bool DoubleList<T>::insert(const T& data,size_t index)
{
    //if(index<1||index>length)
    //{
    //    return false;
    //}
    if(index>=length)
    {
        return false;
    }
    DoubleListNode<T>* pNode = CreateNode();
    pNode->data = data;
    DoubleListNode<T>* pCurr=pHead;
    DoubleListNode<T>* pNext=pCurr->pNext;

    for(size_t i=0; i < index; i++)
    {
        pCurr = pCurr->pNext;
    }
    pNext=pCurr->pNext;

    //pNode->pPrevious = q->pPrevious;
    //pNode->pNext = q;
    //q->pPrevious->pNext = pNode;
    //q->pPrevious = pNode;
    pCurr->pNext=pNode;
    if (pNext!=NULL)
    {
        pNext->pPrevious=pNode;
    }

    pNode->pPrevious=pCurr;
    pNode->pNext=pNext;
    length ++;
    return true;
}

template<typename T>
DoubleListNode<T>* DoubleList<T>::search(const T &val) const
{
    DoubleListNode<T>* p= pHead->pNext;
    while(p!=NULL)
    {
        if (val==p->data)
        {
            return p;
        }
        p=p->pNext
    }
    return NULL;
}

template<typename T>
void DoubleList<T>::ClearAll()
{
    DoubleListNode<T>* pTmp;
    DoubleListNode<T>* p = pHead->pNext;
     while(p!=NULL)
     {
        pTmp=p;
        p=p->pNext;
        DestoryNode(pTmp);
     }
}

template<typename T>
void DoubleList<T>::ClearAllReverseOrder()
{
    DoubleListNode<T>* pTmp;
    DoubleListNode<T>* p = pTail->pPrevious;
    while(p!=NULL&&p!=pHead)   //逆序删除注意，不能删除头
    {
        pTmp=p;
        p=p->pPrevious;
        DestoryNode(pTmp);
    }
}
template<typename T>
void DoubleList<T>::removeNodeByItsAddrress(void* pCurr)
{
    DoubleListNode<T> *pNodeCurr = (DoubleListNode<T>*)pCurr;
    eraser(pNodeCurr);
}

template<typename T>
void DoubleList<T>::eraser(DoubleListNode<T>* pNode)
{
    DoubleListNode<T> *pPrevious =pNode->pPrevious;
    DoubleListNode<T> *pNext =pNode->pNext;

    //连接前后级
    pPrevious->pNext=pNext;
    if (pNext!=NULL)        //当前节点存在后级节点
    {
        pNext->pPrevious=pPrevious;
    }

    //释放本节点
    DestoryNode(pNode);
}

template<typename T>
DoubleListNode<T>* DoubleList<T>::hasNext(DoubleListNode<T>* pNode) //判断是否有下一个，并返回下一个
{
    if (!pNode)
    {
        return NULL;
    }
    return pNode->pNext;
}

template<typename T>
void DoubleList<T>::TraverInOrder()
{
    DoubleListNode<T> * p=pHead->pNext;
    while(p!=NULL)
    {
        //cout<<p->data<<endl;
        p=p->pNext;
    }
}
template<typename T>
void DoubleList<T>::TraverReverseOrder()
{
    DoubleListNode<T> * p=pTail;
    while(p->pPrevious!=NULL)
    {
        //cout<<p->data<<endl;
        p=p->pPrevious;
    }
}

template<typename T>
void DoubleList<T>::Reverse()   //链表逆置（利用头插新节点的方法）
{
//    if (_head == NULL || _head== _tail)
//    {
//        return;
//    }
//    int ret = Amount();
// 
///*  //  方法一（相当于用头插的方式重新建立链表）
//    _tail = new LinkNode(_head->_data);
//    LinkNode* begin = NULL;
//    LinkNode* tmp = _tail;
//    while (--ret)
//    {
//        LinkNode* del = _head;
//        _head = _head->_next;
//        delete del;           
//        begin = new LinkNode(_head->_data);
//        begin->_next = tmp;
//        tmp->_prev = begin;
//        tmp = begin;
//    }
//    _head = begin;*/
// 
////////  方法二（只是交换对称位置节点的数据）**（高效）
//    LinkNode* begin = _head;
//    LinkNode* end = _tail;
//    while (ret)
//    {
//        if (end->_next == begin)
//            break;
//        ret /= 2;
//        swap(begin->_data, end->_data);
//        begin = begin->_next;
//        end = end->_prev;
//    }
//     
//        /*// 方法三  交换前驱和后继指针
//        swap(_head, _tail);
//        LinkNode* cur = _head;
//        while (cur)
//        {
//            swap(cur->_prev,cur->_next);
//            cur = cur->_next;
//        }
//        */
//         
}


template<typename T>
void DoubleList<T>::print()
{
    DoubleListNode<T> *pNode = pHead->pNext;
    //cout<<"data:";
    while (pNode) 
    {
        //cout<<n->data<<" ";
        pNode = pNode->pNext;
    }
    //cout << endl;
}






//4、	循环链表  //一般指的是循环单向链表  双向循环链表一般不用 优势不明显，空间需求稍大 
//
template<typename T> class CircularList;

template<typename T> 
class CircularListNode
{
private:
    friend class CircularList<T>;
    CircularListNode():m_pnext(NULL){}
    CircularListNode(const T item,CircularListNode<T> *next=NULL):m_data(item),m_pnext(next){}
    ~CircularListNode(){
        m_pnext=NULL;
    }
private:
    T m_data;
    CircularListNode *m_pnext;
};

template<typename T> 
class CircularList
{
public:
    CircularList():head(new CircularListNode<T>())
    {
        head->m_pnext=head;	
    }
    ~CircularList()
    {
        MakeEmpty();
        delete head;
    }
public:
    void MakeEmpty();	//clear the list
    int Length();		//get the length
    CircularListNode<T> *Find(T value,int n);	//find the nth data which is equal to value
    CircularListNode<T> *Find(int n);			//find the nth data
    bool Insert(T item,int n=0);			//insert the data into the nth data of the list
    T Remove(int n=0);					//delete the nth data
    bool RemoveAll(T item);				//delete all the datas which are equal to value
    T Get(int n);	//get the nth data
    void Print();		//print the list

private:
    CircularListNode<T> *head;
};

template<typename T> 
void CircularList<T>::MakeEmpty()
{
    CircularListNode<T> *pdel,*pmove=head;
    while(pmove->m_pnext!=head)
    {
        pdel=pmove->m_pnext;
        pmove->m_pnext=pdel->m_pnext;
        delete pdel;
    }
}

template<typename T> 
int CircularList<T>::Length()
{
    CircularListNode<T> *pmove=head;
    int count=0;
    while(pmove->m_pnext!=head)
    {
        pmove=pmove->m_pnext;
        count++;
    }
    return count;
}

template<typename T> 
CircularListNode<T>* CircularList<T>::Find(int n)
{
    if(n<0){
        cout<<"The n is out of boundary"<<endl;
        return NULL;
    }
    CircularListNode<T> *pmove=head->m_pnext;
    for(int i=0;i<n&&pmove!=head;i++)
    {
        pmove=pmove->m_pnext;
    }
    if(pmove==head){
        cout<<"The n is out of boundary"<<endl;
        return NULL;
    }
    return pmove;
}

template<typename T> 
CircularListNode<T>* CircularList<T>::Find(T value,int n)
{
    if(n<1){
        cout<<"The n is illegal"<<endl;
        return NULL;
    }
    CircularListNode<T> *pmove=head;
    int count=0;
    while(count!=n){
        pmove=pmove->m_pnext;
        if(pmove->m_data==value){
            count++;
        }
        if(pmove==head){
            cout<<"can't find the element"<<endl;
            return NULL;
        }
    }
    return pmove;
}

template<typename T> 
bool CircularList<T>::Insert(T item, int n)
{
    if(n<0){
        cout<<"The n is out of boundary"<<endl;
        return 0;
    }
    CircularListNode<T> *pmove=head;
    CircularListNode<T> *pnode=new CircularListNode<T>(item);
    if(pnode==NULL){
        cout<<"Application error!"<<endl;
        exit(1);
    }
    for(int i=0;i<n;i++){
        pmove=pmove->m_pnext;
        if(pmove==head){
            cout<<"The n is out of boundary"<<endl;
            return 0;
        }
    }
    pnode->m_pnext=pmove->m_pnext;
    pmove->m_pnext=pnode;
    return 1;
}

template<typename T> 
bool CircularList<T>::RemoveAll(T item)
{
    CircularListNode<T> *pmove=head;
    CircularListNode<T> *pdel=head->m_pnext;
    while(pdel!=head){
        if(pdel->m_data==item){
            pmove->m_pnext=pdel->m_pnext;
            delete pdel;
            pdel=pmove->m_pnext;
            continue;
        }
        pmove=pmove->m_pnext;
        pdel=pdel->m_pnext;
    }
    return 1;
}

template<typename T> 
T CircularList<T>::Remove(int n)
{
    if(n<0){
        cout<<"can't find the element"<<endl;
        exit(1);
    }
    CircularListNode<T> *pmove=head,*pdel;
    for(int i=0;i<n&&pmove->m_pnext!=head;i++){
        pmove=pmove->m_pnext;
    }
    if(pmove->m_pnext==head){
        cout<<"can't find the element"<<endl;
        exit(1);
    }
    pdel=pmove->m_pnext;
    pmove->m_pnext=pdel->m_pnext;
    T temp=pdel->m_data;
    delete pdel;
    return temp;
}

template<typename T> 
T CircularList<T>::Get(int n)
{
    if(n<0){
        cout<<"The n is out of boundary"<<endl;
        exit(1);
    }
    CircularListNode<T> *pmove=head->m_pnext;
    for(int i=0;i<n;i++){
        pmove=pmove->m_pnext;
        if(pmove==head){
            cout<<"The n is out of boundary"<<endl;
            exit(1);
        }
    }
    return pmove->m_data;
}

template<typename T> 
void CircularList<T>::Print()
{
    CircularListNode<T> *pmove=head->m_pnext;
    cout<<"head";
    while(pmove!=head){
        cout<<"--->"<<pmove->m_data;
        pmove=pmove->m_pnext;
    }
    cout<<"--->over"<<endl<<endl<<endl;
}


#ifdef UNIT_TEST
int Test_List();
#endif


#endif

