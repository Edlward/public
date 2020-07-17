#ifndef _BinarySearchTree_H_
#define _BinarySearchTree_H_

#include <iomanip>
#include <iostream>
using namespace std;


//
//   �����ο�--����  �������������Լ����������������򡢺������ �� ������������������������ͼ�⣩   ��ȫ




#if 0
//12������������
//   ThreadNode.h

template<typename Type> class ThreadTree;
template<typename Type> class ThreadInorderIterator;

template<typename Type> 
class ThreadNode
{
public:
    friend class ThreadTree<Type>;
    friend class ThreadInorderIterator<Type>;
    ThreadNode():m_nleftthread(1),m_nrightthread(1)
    {
        m_pleft=this;
        m_pright=this;
    }
    ThreadNode(const Type item):m_data(item),m_pleft(NULL),
        m_pright(NULL),m_nleftthread(0),m_nrightthread(0)
    {
    }
private:
    Type m_data;
    ThreadNode<Type> *m_pleft,*m_pright;
    int m_nleftthread,m_nrightthread;
};


template<typename Type> 
class ThreadTree
{
public:
    friend class ThreadInorderIterator<Type>;
    ThreadTree():m_proot(new ThreadNode<Type>()){}
private:
    ThreadNode<Type> *m_proot;
};

template<typename Type> 
class ThreadInorderIterator
{
public:
    ThreadInorderIterator(ThreadTree<Type> &tree):m_ptree(tree),m_pcurrent(tree.m_proot)
    {
        //InThread(m_ptree.m_proot->m_pleft,m_ptree.m_proot);
    }

    ThreadNode<Type> *First();
    ThreadNode<Type> *Prior();
    ThreadNode<Type> *Next();

    void Print();
    void Print(ThreadNode<Type> *start, int n=0);
    void InOrder();
    void InsertLeft(ThreadNode<Type> *left);
    void InsertRight(ThreadNode<Type> *right);
    ThreadNode<Type> *GetParent(ThreadNode<Type> *current);

private:
    ThreadTree<Type> &m_ptree;
    ThreadNode<Type> *m_pcurrent;
    void InThread(ThreadNode<Type> *current,ThreadNode<Type> *pre);
};

template<typename Type> 
void ThreadInorderIterator<Type>::InThread(ThreadNode<Type> *current, ThreadNode<Type> *pre)
{
        if(current!=m_ptree.m_proot)
        {
            InThread(current->m_pleft,pre);
            if(current->m_pleft==NULL)
            {
                current->m_pleft=pre;
                current->m_nleftthread=1;
            }
            if(pre->m_pright==NULL)
            {
                pre->m_pright=current;
                pre->m_nrightthread=1;
            }

            pre=current;
            InThread(current->m_pright,pre);
        }
}

template<typename Type> 
ThreadNode<Type>* ThreadInorderIterator<Type>::First()
{
    while(m_pcurrent->m_nleftthread==0)
    {
        m_pcurrent=m_pcurrent->m_pleft;
    }
    return m_pcurrent;
}

template<typename Type> 
ThreadNode<Type>* ThreadInorderIterator<Type>::Prior()
{
    ThreadNode<Type> *pmove=m_pcurrent->m_pleft;
    if(0==m_pcurrent->m_nleftthread)
    {
        while(0==pmove->m_nrightthread)
        {
            pmove=pmove->m_pright;
        }
    }
    m_pcurrent=pmove;
    if(m_pcurrent==m_ptree.m_proot)
    {
        return NULL;
    }
    return m_pcurrent;
}

template<typename Type> 
ThreadNode<Type>* ThreadInorderIterator<Type>::Next()
{
    ThreadNode<Type> *pmove=m_pcurrent->m_pright;
    if(0==m_pcurrent->m_nrightthread)
    {
        while(0==pmove->m_nleftthread)
        {
            pmove=pmove->m_pleft;
        }
    }
    m_pcurrent=pmove;
    if(m_pcurrent==m_ptree.m_proot)
    {
        return NULL;
    }
    return m_pcurrent;
}

template<typename Type> 
void ThreadInorderIterator<Type>::InOrder()
{
    ThreadNode<Type> *pmove=m_ptree.m_proot;
    while(pmove->m_pleft!=m_ptree.m_proot)
    {
        pmove=pmove->m_pleft;
    }
    m_pcurrent=pmove;
    cout<<"root";
    while(pmove!=m_ptree.m_proot&&pmove)
    {
        cout<<"--->"<<pmove->m_data;
        pmove=this->Next();
    }
    cout<<"--->end";
}

template<typename Type> 
void ThreadInorderIterator<Type>::InsertLeft(ThreadNode<Type> *left)
{
    left->m_pleft=m_pcurrent->m_pleft;
    left->m_nleftthread=m_pcurrent->m_nleftthread;
    left->m_pright=m_pcurrent;
    left->m_nrightthread=1;
    m_pcurrent->m_pleft=left;
    m_pcurrent->m_nleftthread=0;
    if(0==left->m_nleftthread)
    {
        m_pcurrent=left->m_pleft;
        ThreadNode<Type> *temp=First();
        temp->m_pright=left;
    }
    m_pcurrent=left;
}

template<typename Type> 
void ThreadInorderIterator<Type>::InsertRight(ThreadNode<Type> *right)
{
    right->m_pright=m_pcurrent->m_pright;
    right->m_nrightthread=m_pcurrent->m_nrightthread;
    right->m_pleft=m_pcurrent;
    right->m_nleftthread=1;
    m_pcurrent->m_pright=right;
    m_pcurrent->m_nrightthread=0;
    if(0==right->m_nrightthread)
    {
        m_pcurrent=right->m_pright;
        ThreadNode<Type> *temp=First();
        temp->m_pleft=right;
    }
    m_pcurrent=right;
}

template<typename Type> 
ThreadNode<Type>* ThreadInorderIterator<Type>::GetParent(ThreadNode<Type> *current)
{
        ThreadNode<Type> *pmove=current;
        while(0==pmove->m_nleftthread)
        {
            pmove=pmove->m_pleft;
        }
        pmove=pmove->m_pleft;
        if(pmove==m_ptree.m_proot)
        {
            if(pmove->m_pleft==current)
            {
                return NULL;
            }
        }
        if(pmove->m_pright==current)
        {
            return pmove;
        }
        pmove=pmove->m_pright;
        while(pmove->m_pleft!=current)
        {
            pmove=pmove->m_pleft;
        }
        return pmove;
}

template<typename Type> 
void ThreadInorderIterator<Type>::Print(ThreadNode<Type> *start, int n)
{
    if(start->m_nleftthread&&start->m_nrightthread)
    {
        for(int i=0;i<n;i++)
        {
            cout<<"     ";
        }
        if(n>=0){
            cout<<start->m_data<<"--->"<<endl;
        }
        return;
    }
    if(start->m_nrightthread==0)
    {
        Print(start->m_pright,n+1);
    }
    for(int i=0;i<n;i++)
    {
        cout<<"     ";
    }
    if(n>=0)
    {
        cout<<start->m_data<<"--->"<<endl;
    }
    if(start->m_nleftthread==0)
    {
        Print(start->m_pleft,n+1);
    }
}

template<typename Type> 
void ThreadInorderIterator<Type>::Print()
{
    Print(m_ptree.m_proot->m_pleft);
}
#endif










enum PointTag
{
    Link,
    Thread
};

template<typename T>
struct ThrBinTreeNode
{
    T data;                     //����Ԫ��
    ThrBinTreeNode<T> *leftChild;  //ָ�����ӵ�ָ��
    ThrBinTreeNode<T> *rightChild; //ָ���Һ��ӵ�ָ��
    PointTag LTag,RTag;                 //���־���ұ�־
    //��������
    ThrBinTreeNode();
    ThrBinTreeNode(const T &val,ThrBinTreeNode<T> *lChild=NULL,ThrBinTreeNode<T> *rChild=NULL);
    ThrBinTreeNode<T> &operator =(const ThrBinTreeNode<T> &copy);
};


template<typename T>
ThrBinTreeNode<T>::ThrBinTreeNode()
{
    leftChild=rightChild=NULL;
    LTag=RTag=Link;               //�˴���ʼ��ΪLink�������ں���
}

template<typename T>
ThrBinTreeNode<T>::ThrBinTreeNode(const T &val,ThrBinTreeNode<T> *lChild,ThrBinTreeNode<T> *rChild)
{
    data=val;
    LTag=RTag=Link;               //��ʼ��ΪLink
    leftChild=lChild;
    rightChild=rChild;
}

template<typename T>
ThrBinTreeNode<T> &ThrBinTreeNode<T>::operator =(const ThrBinTreeNode<T> &copy)
{
    data=copy.data;
    leftChild=copy.leftChild;
    rightChild=copy.leftChild;
    LTag=copy.LTag;
    RTag=copy.RTag;
}


template<typename T>
class ThrBinTree
{
protected:
    //���ݳ�Ա
    ThrBinTreeNode<T> *root;
    //��������
    ThrBinTreeNode<T> *CopyTreeHelp(const ThrBinTreeNode<T> *r);//���ƶ�����
    void DestroyHelp(ThrBinTreeNode<T> *&r);//����rΪ���Ķ�����
    void PreThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre);   //����������
    void InThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre);   //����������
    void CreateBTreeHelp(ThrBinTreeNode<T> *&r,T pre[],T in[],int,int,int,int);//��������������й�����
public:
    ThrBinTree(){root=NULL}//�޲ι���
    ThrBinTree(ThrBinTreeNode<T> *r){ root=r;}//������rΪ���Ķ�����
    virtual ~ThrBinTree();//��ָ���Զ����鹹���������鹹
    ThrBinTree<T> &CreateBTree(T pre[],T in[],int n); //������
    void PreTreading();//����������
    void InTreading();//����������
    void PreOrderTraverse_Thr(void (*visit) (const T &))const;                       //�������
    void InOrderTraverse_Thr(void (*visit) (const T &))const;                       //�������
    void LevelOrder(void (*visit) (const T &))const;
    ThrBinTree<T> &operator =(const ThrBinTree<T> &copy);//���ظ�ֵ�����
};
//Copy
template<typename T>
ThrBinTreeNode<T> *ThrBinTree<T>::CopyTreeHelp(const ThrBinTreeNode<T> *r)
{
    ThrBinTreeNode<T> *cur;
    if(r==NULL)   cur=NULL;
    else
    {
        ThrBinTreeNode<T> *lChild=CopyTreeHelp(r->leftChild);//����������
        ThrBinTreeNode<T> *rChild=CopyTreeHelp(r->rightChild);//����������
        cur=new ThrBinTreeNode<T>(r->data,lChild,rChild);
        //���Ƹ��ڵ�
    }
    return cur;
}

template<typename T>
void ThrBinTree<T>::InThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre)//�������
{
    if(p)
    {
        if(p->LTag==Link)
            InThreadingHelp(p->leftChild,pre);    //������������.
        if(!p->leftChild)                 //���ӿ� 
        {
            p->LTag=Thread;
            p->leftChild=pre;
        }
        if(!pre->rightChild)             //ǰ�� ���Һ��ӿ�
        {
            pre->RTag=Thread;
            pre->rightChild=p;
        }
        pre=p;
        if(p->RTag==Link)
            InThreadingHelp(p->rightChild,pre);  //������������
    }

}

template<typename T>
void ThrBinTree<T>::PreThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre)//�������
{
    if(p)
    {
        if(!p->leftChild)                 //���ӿ� 
        {
            p->LTag=Thread;
            p->leftChild=pre;
        }
        if(!pre->rightChild)             //ǰ�� ���Һ��ӿ�
        {
            pre->RTag=Thread;
            pre->rightChild=p;
        }
        pre=p;
        if(p->LTag==Link)
            PreThreadingHelp(p->leftChild,pre);    //������������.
        if(p->RTag==Link)
            PreThreadingHelp(p->rightChild,pre);  //������������
    }

}

template<typename T>
void ThrBinTree<T>::DestroyHelp(ThrBinTreeNode<T> *&r)
{
    if(r!=NULL)
    {
        if(r->LTag==Link)
            DestroyHelp(r->leftChild);
        if(r->RTag==Link)
            DestroyHelp(r->rightChild);
        delete r;
        r=NULL;
    }
}

//�鹹
template<typename T>
ThrBinTree<T>::~ThrBinTree()
{
    DestroyHelp(root);
}

template<typename T>
//ThrΪͷ�ڵ㣬TΪ��.
void ThrBinTree<T>::InTreading()
{
    ThrBinTreeNode<T> *pre=root;
    InThreadingHelp(root,pre);    //����������
    if(pre->rightChild==NULL)
        pre->RTag=Thread;       //�������һ���ڵ�
}

template<typename T>
void ThrBinTree<T>::PreTreading()
{
    ThrBinTreeNode<T> *pre=root;
    PreThreadingHelp(root,pre);    //����������
    if(pre->rightChild==NULL)
        pre->RTag=Thread;       //�������һ���ڵ�
}

template<typename T>
void print(const T &e )
{
    cout<<e<<" ";
}

template<typename T>
void ThrBinTree<T>::InOrderTraverse_Thr(void (*visit) (const T &))const
{
    visit=print;
    if(root!=NULL){
        ThrBinTreeNode<T> *p=root;            //pָ���
        while(p->LTag==Link)  p=p->leftChild;     //�����������½�
        while(p)
        {
            (*visit)(p->data);       //���ʽ��Ԫ��
            if(p->RTag==Thread)      //�Һ���Ϊ��������pָ����
            {
                p=p->rightChild;
            }
            else    //�Һ��Ӵ���
            {
                p=p->rightChild;     //�����Һ���
                while(p->LTag==Link)
                    p=p->leftChild;
            }
        }
    }
}

template<typename T>
void ThrBinTree<T>::PreOrderTraverse_Thr(void (*visit) (const T &))const
{
    visit=print;
    if(root!=NULL){
        ThrBinTreeNode<T> *p=root;        //pָ���
        while(p)
        {
            (*visit)(p->data);
            if(p->RTag==Thread)
                p=p->rightChild;
            else
            {
                if(p->LTag==Link)
                    p=p->leftChild;
                else
                    p=p->rightChild;
            }
        }
    }
}

template<typename T>
void ThrBinTree<T>::CreateBTreeHelp(ThrBinTreeNode<T> *&r,
                                               T pre[],T in[],
                                               int preLeft,int preRight,int inLeft,int inRight)

{
    if(preLeft>preRight||inLeft>inRight)
        r=NULL;
    else
    {
        r=new ThrBinTreeNode<T>(pre[preLeft]);//���ɸ����
        int mid=inLeft;
        while(in[mid]!=pre[preLeft])
            mid++;
        CreateBTreeHelp(r->leftChild,pre,in,preLeft+1,preLeft+mid-inLeft,inLeft,mid-1);        //����������������Լ���ͼ�ֹ�ʵ��һ�顣
        CreateBTreeHelp(r->rightChild,pre,in,preLeft+mid-inLeft+1,preRight,mid+1,inRight);
    }
}

template<typename T>
//������
ThrBinTree<T>& ThrBinTree<T>::CreateBTree(T pre[],T in[],int n)
{
    ThrBinTreeNode<T> *r;   //��
    CreateBTreeHelp(r,pre,in,0,n-1,0,n-1);
    //return BinaryTree<ElemType>(r);//��������ô���أ�Error:��Ӧ�÷��ؾֲ������ĵ�ַ
    *this = ThrBinTree<T>(r);
    return *this;
}

#include<queue>
template<typename T>
void ThrBinTree<T>::LevelOrder(void (*visit) (const T &))const
{    //����ʵ��
    visit=print;
    queue<ThrBinTreeNode<T> *> q;
    ThrBinTreeNode<T> *t=root;
    if(t!=NULL) q.push(t);                //���ǿգ����
    while(!q.empty())                //�Ӳ���
    {
        t=q.front();
        q.pop();                    //����
        (*visit)(t->data);          //һ��һ����б������޷���⻹���Լ���ͼʵ��һ��
        if(t->leftChild)
            q.push(t->leftChild);  //��������
        if(t->rightChild)           
            q.push(t->rightChild); //�����Һ���
    }

}

//operator =
template<typename T>
ThrBinTree<T> &ThrBinTree<T>::operator=(const ThrBinTree<T> &copy)
{
    if(&copy!=this)
    {
        DestroyHelp(root);
        root=CopyTreeHelp(copy.root);
    }
    return *this;
}


#ifdef UNIT_TEST
int Test_ThreadTree();
#endif

#endif


