#ifndef _Queue_H_
#define _Queue_H_

#include <iostream>
using namespace std;

#ifdef __cplusplus



//7.顺序队列

template<typename T> 
class SeqQueue
{
public:
    SeqQueue(int sz):m_nrear(0),m_nfront(0),m_ncount(0),m_nMaxSize(sz)
    {
        m_pelements=new T[sz];
        //if(m_pelements==NULL)
        //{
        //    cout<<"Application Error!"<<endl;
        //    exit(1);
        //}
    }
    ~SeqQueue()
    {
        delete[] m_pelements;
    }
    void MakeEmpty();               //make the queue empty
    bool IsEmpty();
    bool IsFull();
    bool Append(const T item);   //insert data
    T Delete();                  //delete data
    T Get();                     //get data
    void Print();                   //print the queue

private:
    int m_nrear;
    int m_nfront;
    int m_ncount;
    int m_nMaxSize;
    T *m_pelements;

};

template<typename T> 
void SeqQueue<T>::MakeEmpty()
{
    this->m_ncount=0;
    this->m_nfront=0;
    this->m_nrear=0;
}

template<typename T> 
bool SeqQueue<T>::IsEmpty()
{
    return m_ncount==0;
}

template<typename T> 
bool SeqQueue<T>::IsFull()
{
    return m_ncount==m_nMaxSize;
}

template<typename T> 
bool SeqQueue<T>::Append(const T item)
{
    if(IsFull())
    {
        cout<<"The queue is full!"<<endl;
        return 0;
    }
    m_pelements[m_nrear]=item;
    m_nrear=(m_nrear+1)%m_nMaxSize;
    m_ncount++;
    return 1;
}

template<typename T> 
T SeqQueue<T>::Delete()
{
    if(IsEmpty())
    {
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    T temp=m_pelements[m_nfront];
    m_nfront=(m_nfront+1)%m_nMaxSize;
    m_ncount--;
    return temp;
}

template<typename T> 
T SeqQueue<T>::Get()
{
    if(IsEmpty())
    {
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    return m_pelements[m_nfront];
}

template<typename T> 
void SeqQueue<T>::Print()
{
    cout<<"front";
    for(int i=0;i<m_ncount;i++)
    {
        cout<<"--->"<<m_pelements[(m_nfront+i+m_nMaxSize)%m_nMaxSize];
    }
    cout<<"--->rear"<<endl<<endl<<endl;
}





//8、链式队列
template<typename T> class LinkQueue;

template<typename T> 
class LinkQueueNode
{
private:
    friend class LinkQueue<T>;
    LinkQueueNode(const T item,LinkQueueNode<T> *next=NULL)
        :m_data(item),m_pnext(next){}
private:
    T m_data;
    LinkQueueNode<T> *m_pnext;
};



template<typename T> 
class LinkQueue
{
public:
    LinkQueue():m_prear(NULL),m_pfront(NULL){}
    ~LinkQueue(){
        MakeEmpty();
    }
    void Append(const T item);   //insert data
    T Delete();                  //delete data
    T GetFront();                //get data
    void MakeEmpty();               //make the queue empty
    void Print();                   //print the queue

    bool IsEmpty() const{
        return m_pfront==NULL;
    }

private:
    LinkQueueNode<T> *m_prear,*m_pfront;
};

template<typename T> 
void LinkQueue<T>::MakeEmpty()
{
    LinkQueueNode<T> *pdel;
    while(m_pfront)
    {
        pdel=m_pfront;
        m_pfront=m_pfront->m_pnext;
        delete pdel;
    }
}

template<typename T> 
void LinkQueue<T>::Append(const T item)
{
    if(m_pfront==NULL)
    {
        m_pfront=m_prear=new LinkQueueNode<T>(item);
    }
    else
    {
        m_prear=m_prear->m_pnext=new LinkQueueNode<T>(item);
    }
}

template<typename T> 
T LinkQueue<T>::Delete()
{
    if(IsEmpty())
    {
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    LinkQueueNode<T> *pdel=m_pfront;
    T temp=m_pfront->m_data;
    m_pfront=m_pfront->m_pnext;
    delete pdel;
    return temp;
}

template<typename T> 
T LinkQueue<T>::GetFront()
{
    if(IsEmpty()){
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    return m_pfront->m_data;
}

template<typename T> 
void LinkQueue<T>::Print()
{
    LinkQueueNode<T> *pmove=m_pfront;
    cout<<"front";
    while(pmove)
    {
        cout<<"--->"<<pmove->m_data;
        pmove=pmove->m_pnext;
    }
    cout<<"--->rear"<<endl<<endl<<endl;
}







//9、优先级队列
//QueueNode.h

template<typename T,typename Cmp> class PriorityQueue;

template<typename T,typename Cmp>
class PriorityQueueNode
{
private:
    friend class PriorityQueue<T,Cmp>;
    PriorityQueueNode(const T item,PriorityQueueNode<T,Cmp> *next=NULL)
        :m_data(item),m_pnext(next){}
private:
    T m_data;
    PriorityQueueNode<T,Cmp> *m_pnext;
};

//Compare.h
    template<typename T> 
    class Compare
    {	//处理一般比较大小
    public:
        static bool lt(T item1,T item2);
    };

template<typename T> 
bool Compare<T>::lt(T item1, T item2)
{
    return item1<item2;
}

struct SpecialData
{
    friend ostream& operator<<(ostream& ,SpecialData &);
    int m_ntenor;
    int m_npir;
};

ostream& operator<<(ostream& os,SpecialData &out)
{
    os<<out.m_ntenor<<"   "<<out.m_npir;
    return os;
}

class SpecialCmp
{		//处理特殊比较大小,用户可添加适当的类
public:
    static bool lt(SpecialData item1,SpecialData item2);
};

bool SpecialCmp::lt(SpecialData item1, SpecialData item2)
{
    return item1.m_npir<item2.m_npir;
}




template<typename T,typename Cmp> 
class PriorityQueue
{	//Cmp is Designed for compare
public:
    PriorityQueue():m_prear(NULL),m_pfront(NULL){}
    ~PriorityQueue(){
        MakeEmpty();
    }

    void MakeEmpty();               //make the queue empty
    void Append(const T item);   //insert data
    T Delete();                  //delete data
    T GetFront();                //get data
    void Print();                   //print the queue

    bool IsEmpty() const
    {           
        return m_pfront==NULL;
    }


private:
    PriorityQueueNode<T,Cmp> *m_prear,*m_pfront;
};

template<typename T,typename Cmp> 
void PriorityQueue<T,Cmp>::MakeEmpty()
{
    PriorityQueueNode<T,Cmp> *pdel;
    while(m_pfront)
    {
        pdel=m_pfront;
        m_pfront=m_pfront->m_pnext;
        delete pdel;
    }
}

template<typename T,typename Cmp> 
void PriorityQueue<T,Cmp>::Append(const T item)
{
    if(m_pfront==NULL)
    {
        m_pfront=m_prear=new PriorityQueueNode<T,Cmp>(item);
    }
    else
    {
        m_prear=m_prear->m_pnext=new PriorityQueueNode<T,Cmp>(item);
    }
}

template<typename T,typename Cmp> 
T PriorityQueue<T,Cmp>::Delete()
{
    if(IsEmpty())
    {
        cout<<"There is no elements!"<<endl;
        exit(1);
    }
    PriorityQueueNode<T,Cmp> *pdel=m_pfront,*pmove=m_pfront;
    while(pmove->m_pnext)
    {  //get the minimize priority's data

        //cmp:: lt is used for compare the two data, if the front one 
        //      is less than the back, then return 1
        if(Cmp::lt(pmove->m_pnext->m_data,pdel->m_pnext->m_data)){
            pdel=pmove;
        }
        pmove=pmove->m_pnext;
    }

    pmove=pdel;
    pdel=pdel->m_pnext;
    pmove->m_pnext=pdel->m_pnext;
    T temp=pdel->m_data;
    delete pdel;
    return temp;
}

template<typename T,typename Cmp> 
T PriorityQueue<T,Cmp>::GetFront()
{
    if(IsEmpty())
    {
        cout<<"There is no elements!"<<endl;
        exit(1);
    }
    PriorityQueueNode<T,Cmp> *pdel=m_pfront,*pmove=m_pfront->m_pnext;
    while(pmove)
    {   //get the minimize priority's data
        if(Cmp::lt(pmove->m_data,pdel->m_data))
        {
            pdel=pmove;
        }
        pmove=pmove->m_pnext;
    }
    return pdel->m_data;
}

template<typename T,typename Cmp> 
void PriorityQueue<T,Cmp>::Print()
{
    PriorityQueueNode<T,Cmp> *pmove=m_pfront;
    cout<<"front";

    while(pmove)
    {
        cout<<"--->"<<pmove->m_data;
        pmove=pmove->m_pnext;
    }

    cout<<"--->rear"<<endl<<endl<<endl;
}



#endif


#ifdef UNIT_TEST
int Test_Queue();
#endif



#endif

