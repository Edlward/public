#ifndef _Stack_H_
#define _Stack_H_

#include <stdexcept>


//5、	顺序栈--基于动态数组实现
//SeqStack.h
#include <iostream>
using namespace std;

#ifdef __cplusplus

    template<typename T> 
    class SeqStack
    {
    public:
        SeqStack(unsigned int sz):m_ntop(-1),m_nMaxSize(sz)
        {
            m_pelements=new T[sz];
            //if(m_pelements==NULL)
            //{
            //    cout<<"Application Error!"<<endl;
            //    //exit(1);
            //}
        }
        ~SeqStack()
        {
            delete[] m_pelements;
        }
    public:
        void Push(const T item); //push data
        T Pop();                 //pop data
        T GetTop() const;  //get data
        void Print();                 //print the stack
        void MakeEmpty()       //make the stack empty
        {           
            m_ntop=-1;
        }
        bool IsEmpty() const
        {
            return m_ntop==-1;
        }
        bool IsFull() const
        {
            return m_ntop==m_nMaxSize-1;
        }

    private:
        int m_ntop;
        T *m_pelements;
        unsigned int m_nMaxSize;
};


template<typename T> 
void SeqStack<T>::Push(const T item)
{
    if(IsFull())
    {
        cout<<"The stack is full!"<<endl;
        return;
    }
    m_pelements[++m_ntop]=item;
}

template<typename T> 
T SeqStack<T>::Pop()
{
    if(IsEmpty())
    {
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    return m_pelements[m_ntop--];
}

template<typename T> 
T SeqStack<T>::GetTop() const
{
    if(IsEmpty())
    {
        cout<<"There is no element!"<<endl;
        exit(1);
    }
    return m_pelements[m_ntop];
}

template<typename T> 
void SeqStack<T>::Print()
{
    cout<<"bottom";
    for(int i=0;i<=m_ntop;i++)
    {
        cout<<"--->"<<m_pelements[i];
    }
    cout<<"--->top"<<endl<<endl<<endl;
}
#endif




//6、链式栈--基于链表实现
template<typename T> class LinkStack;

template<typename T> 
class LinkStackNode
{
private:
    friend class LinkStack<T>;
    LinkStackNode(T dt,LinkStackNode<T> *next=NULL):m_data(dt),m_pnext(next){} 
private:
    T m_data;
    LinkStackNode<T> *m_pnext;
};


template<typename T> class LinkStack
{
public:
    LinkStack():m_ptop(NULL){}
    ~LinkStack()
    {
        MakeEmpty();
    }
public:
    void MakeEmpty();           //make the stack empty
    void Push(const T item); //push the data
    T Pop();                 //pop the data
    T GetTop() const;        //get the data
    void Print();               //print the stack
    bool IsEmpty() const
    {
        return m_ptop==NULL;
    }
private:
    LinkStackNode<T> *m_ptop;
};

template<typename T> 
void LinkStack<T>::MakeEmpty()
{
    LinkStackNode<T> *pmove;
    while(m_ptop!=NULL)
    {
        pmove=m_ptop;
        m_ptop=m_ptop->m_pnext;
        delete pmove;
    }
}

template<typename T> 
void LinkStack<T>::Push(const T item)
{
    m_ptop=new LinkStackNode<T>(item,m_ptop);
}

template<typename T> 
T LinkStack<T>::GetTop() const
{
    if(IsEmpty())
    {
        cout<<"There is no elements!"<<endl;
        exit(1);
    }
    return m_ptop->m_data;
}

template<typename T> 
T LinkStack<T>::Pop()
{
    if(IsEmpty())
    {
        cout<<"There is no elements!"<<endl;
        exit(1);
    }
    LinkStackNode<T> *pdel=m_ptop;
    m_ptop=m_ptop->m_pnext;
    T temp=pdel->m_data;
    delete pdel;
    return temp;
}

template<typename T> 
void LinkStack<T>::Print()
{
    LinkStackNode<T> *pmove=m_ptop;
    cout<<"buttom";
    while(pmove!=NULL)
    {
        cout<<"--->"<<pmove->m_data;
        pmove=pmove->m_pnext;
    }
    cout<<"--->top"<<endl<<endl<<endl;
}



//基于deque的stack 的实现
//1.特征：先进后出 。只能插入，删除，访问栈顶的元素 ！！
//2.实现
//可以用 vector ,list ,deque 来实现。但是 vector和deque 的实现要比list实现的性能好

#include<iostream>
#include<deque>
using namespace std;
template<class T ,class cont = deque<T> >  //类模板
class StackBydqueue
{
    cont tt ;
public:
    T & top()
    {
        return tt.front();
    }
    void pop( )
    {
        tt.pop_front() ;
    }
    void push(T  temp)
    {
        tt.push_front(temp);
    }
    int size()
    {
        return tt.size();
    }
};

#ifdef UNIT_TEST
int Test_Stack();
#endif


#endif


