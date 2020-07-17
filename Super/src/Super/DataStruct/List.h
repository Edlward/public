#ifndef _List_H_
#define _List_H_

#include <iostream>
using namespace std;
//#ifdef __cplusplus

#include <exception>


//ѧ��������󣬲���ʵ�ֵ������汾


//1��˳���
//˳��洢�����Ա��Ϊ˳�������Ԫ�ص��߼�˳��������˳����ͬ��
//1.1 ��̬�洢
//�ھ�̬����ʱ����������Ĵ�С�Ϳռ��Ѿ��̶���һ���ռ�ռ�����ټ����µ����ݲ���������ͻᵼ�³��������
//1.2��̬�洢
//�ڶ�̬����ʱ���洢����Ŀռ����ڳ���ִ����ͨ����̬�洢����ģ�һ���ѷ���ռ�ռ����
//�������⿪��һ�����Ĵ洢�ռ䣬���Դ���ԭ���Ĵ洢�ռ䣬�Ӷ��ﵽ����洢����ռ��Ŀ�ġ�

//rafactor by lizulin

//1.β�壬2.ͷ�壬3.��ʾ��4.βɾ��5.ͷɾ��6.��λ�ã�7.�尴ֵ�壬8.��λ��ɾ��9.��ֵɾ��10.��ֵ�飬11.�����12.������ݣ�13.�ݻٸ�˳���14.��ת��15.����(ð�����򣬿�������)��


//˳���


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
        t = pArray[pos];     //������
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




//������
//2��	������
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


////3��	˫������ ��˫���������
//�ѳ�������Ҳ�ŵ�basic


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//һ�ֿ��Ը������ڽڵ�ָ�����ɾ���ڵ��ѭ�������ʵ��
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
    DoubleListNode<T>* pHead;  //ͷ�ڵ㣬ʼ�մ��ڣ����������
    DoubleListNode<T>* pTail;     //��β���ڵ㣬û��������Ϊ��
    size_t length;
public:
    DoubleList();
    ~DoubleList();
    //��ʵ��
    DoubleList(const DoubleList& s);                             //��������
    DoubleList &operator=(DoubleList& s);                  //��ֵ�����������

    DoubleListNode<T>* CreateNode();                       //�����ڴ棬�����ڵ�
    void DestoryNode(DoubleListNode<T>* pNode);    //�ͷ�
    
    //pushԪ�ز��ҷ���nodeָ��
    DoubleListNode<T>* pushBack(const T& data);      //��ӽڵ㵽����β,
    DoubleListNode<T>* pushFront(const T& data);     //��ӽڵ㵽����β

    ////���ٰ汾,��Ϊ�ڴ�ص�����ʹ�ò�Ӧ���ڲ��ٵ���new node����������û������
    ////����������õ�node
    //DoubleListNode<T>* pushBackFast(const T& data);      //��ӽڵ㵽����β,
    //DoubleListNode<T>* pushFrontFast(const T& data);     //��ӽڵ㵽����β
    //DoubleListNode<T>* do_pushBack(const T& data);      //��ӽڵ㵽����β,
    //DoubleListNode<T>* do_pushFront(const T& data);     //��ӽڵ㵽����β

    void PopBack();                                                         //βɾ
    void PopFront();                                                        //ͷɾ��
    DoubleListNode<T>* search(const T &val) const;    //������ĳ��ֵ��ͬ�Ľڵ�
     size_t Length()const {return length;}                        //ȡ�����ȣ���O1����
     bool empty()const {return length==0;}                     //���������Ƿ�Ϊ��

    T pop();  //�����������һ���ڵ㣬������ֵ  //ʧ�����쳣?
    bool objectAt(T& data,size_t index);                        //ͨ��index�����������е�Ԫ��
    bool insert(const T& data,size_t index);                   //����Ԫ�ص�ָ��λ�õ�ǰ��
    void ClearAll();
    void ClearAllReverseOrder();                                    //��β���������

    //֪��ĳ���ڵ��ָ�룬�������ǰ����ָ�룬���Կ���ɾ���ڵ�
    void removeNodeByItsAddrress(void* pCurr);
    void eraser(DoubleListNode<T>* pNode);  //���ݽڵ�ָ�룬��սڵ�

    //ʵ�����Ƶ����������ķ���
    DoubleListNode<T>* getBegin(){return pHead;}
    DoubleListNode<T>* hasNext(DoubleListNode<T>* pNode);   //�ж��Ƿ������һ�������أ�ʧ�ܷ���NULL
    T getData(DoubleListNode<T>* pNode){return pNode->data;}
    void TraverInOrder();                //˳�����
    void TraverReverseOrder();       //�������
    void Reverse();                          //��������
    void print();                             //��ӡ���������Ԫ��
};



#if 0
//��ֵ����������أ���ͳ������
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
 
    //��ֵ����������أ���Чд����*
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
 
    List&  List::operator=(List &s)     //��ֵ��������������Ż�(�Ƽ�д��)
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
    pTail = pHead;    //��������⣬����
    //pTail=NULL;     //Ĭ��û��β��

    //ͷ�ڵ㲻���ʵ������   add
    //pHead->pPrevious=NULL;
    //pHead->pNext=NULL;
    length = 0;
}

template<typename T>
DoubleList<T>::~DoubleList()
{
    ClearAll();  //new add 
    //�ͷ�ͷ�ڵ�
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
    delete pNode;   //��֤free��O1����
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
    //����
    pNode->data = data;
    DoubleListNode<T> *pFirstOld=pHead->pNext;    //ԭ��head֮���һ��
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
    while(p!=NULL&&p!=pHead)   //����ɾ��ע�⣬����ɾ��ͷ
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
    while(p!=NULL&&p!=pHead)   //����ɾ��ע�⣬����ɾ��ͷ
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

    //����ǰ��
    pPrevious->pNext=pNext;
    if (pNext!=NULL)        //��ǰ�ڵ���ں󼶽ڵ�
    {
        pNext->pPrevious=pPrevious;
    }

    //�ͷű��ڵ�
    DestoryNode(pNode);
}

template<typename T>
DoubleListNode<T>* DoubleList<T>::hasNext(DoubleListNode<T>* pNode) //�ж��Ƿ�����һ������������һ��
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
void DoubleList<T>::Reverse()   //�������ã�����ͷ���½ڵ�ķ�����
{
//    if (_head == NULL || _head== _tail)
//    {
//        return;
//    }
//    int ret = Amount();
// 
///*  //  ����һ���൱����ͷ��ķ�ʽ���½�������
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
////////  ��������ֻ�ǽ����Գ�λ�ýڵ�����ݣ�**����Ч��
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
//        /*// ������  ����ǰ���ͺ��ָ��
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






//4��	ѭ������  //һ��ָ����ѭ����������  ˫��ѭ������һ�㲻�� ���Ʋ����ԣ��ռ������Դ� 
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

