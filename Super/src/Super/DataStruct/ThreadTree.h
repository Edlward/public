#ifndef _BinarySearchTree_H_
#define _BinarySearchTree_H_

#include <iomanip>
#include <iostream>
using namespace std;


//
//   继续参考--博文  二叉树线索化以及线索化的先序、中序、后序遍历 和 后序线索化二叉树及遍历（图解）   补全




#if 0
//12、线索二叉树
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
    T data;                     //数据元素
    ThrBinTreeNode<T> *leftChild;  //指向左孩子的指针
    ThrBinTreeNode<T> *rightChild; //指向右孩子的指针
    PointTag LTag,RTag;                 //左标志，右标志
    //函数构造
    ThrBinTreeNode();
    ThrBinTreeNode(const T &val,ThrBinTreeNode<T> *lChild=NULL,ThrBinTreeNode<T> *rChild=NULL);
    ThrBinTreeNode<T> &operator =(const ThrBinTreeNode<T> &copy);
};


template<typename T>
ThrBinTreeNode<T>::ThrBinTreeNode()
{
    leftChild=rightChild=NULL;
    LTag=RTag=Link;               //此处初始化为Link，即存在孩子
}

template<typename T>
ThrBinTreeNode<T>::ThrBinTreeNode(const T &val,ThrBinTreeNode<T> *lChild,ThrBinTreeNode<T> *rChild)
{
    data=val;
    LTag=RTag=Link;               //初始化为Link
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
    //数据成员
    ThrBinTreeNode<T> *root;
    //辅助函数
    ThrBinTreeNode<T> *CopyTreeHelp(const ThrBinTreeNode<T> *r);//复制二叉树
    void DestroyHelp(ThrBinTreeNode<T> *&r);//销毁r为根的二叉树
    void PreThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre);   //先序线索化
    void InThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre);   //中序线索化
    void CreateBTreeHelp(ThrBinTreeNode<T> *&r,T pre[],T in[],int,int,int,int);//用中序和先序序列构造树
public:
    ThrBinTree(){root=NULL}//无参构造
    ThrBinTree(ThrBinTreeNode<T> *r){ root=r;}//建立以r为根的二叉树
    virtual ~ThrBinTree();//有指针自定义虚构，且用虚虚构
    ThrBinTree<T> &CreateBTree(T pre[],T in[],int n); //构造树
    void PreTreading();//先序线索化
    void InTreading();//中序线索化
    void PreOrderTraverse_Thr(void (*visit) (const T &))const;                       //先序遍历
    void InOrderTraverse_Thr(void (*visit) (const T &))const;                       //中序遍历
    void LevelOrder(void (*visit) (const T &))const;
    ThrBinTree<T> &operator =(const ThrBinTree<T> &copy);//重载赋值运算符
};
//Copy
template<typename T>
ThrBinTreeNode<T> *ThrBinTree<T>::CopyTreeHelp(const ThrBinTreeNode<T> *r)
{
    ThrBinTreeNode<T> *cur;
    if(r==NULL)   cur=NULL;
    else
    {
        ThrBinTreeNode<T> *lChild=CopyTreeHelp(r->leftChild);//复制左子树
        ThrBinTreeNode<T> *rChild=CopyTreeHelp(r->rightChild);//复制右子树
        cur=new ThrBinTreeNode<T>(r->data,lChild,rChild);
        //复制根节点
    }
    return cur;
}

template<typename T>
void ThrBinTree<T>::InThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre)//中序遍历
{
    if(p)
    {
        if(p->LTag==Link)
            InThreadingHelp(p->leftChild,pre);    //线索化左子树.
        if(!p->leftChild)                 //左孩子空 
        {
            p->LTag=Thread;
            p->leftChild=pre;
        }
        if(!pre->rightChild)             //前驱 的右孩子空
        {
            pre->RTag=Thread;
            pre->rightChild=p;
        }
        pre=p;
        if(p->RTag==Link)
            InThreadingHelp(p->rightChild,pre);  //线索化右子树
    }

}

template<typename T>
void ThrBinTree<T>::PreThreadingHelp(ThrBinTreeNode<T> *p,ThrBinTreeNode<T> *&pre)//先序遍历
{
    if(p)
    {
        if(!p->leftChild)                 //左孩子空 
        {
            p->LTag=Thread;
            p->leftChild=pre;
        }
        if(!pre->rightChild)             //前驱 的右孩子空
        {
            pre->RTag=Thread;
            pre->rightChild=p;
        }
        pre=p;
        if(p->LTag==Link)
            PreThreadingHelp(p->leftChild,pre);    //线索化左子树.
        if(p->RTag==Link)
            PreThreadingHelp(p->rightChild,pre);  //线索化右子树
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

//虚构
template<typename T>
ThrBinTree<T>::~ThrBinTree()
{
    DestroyHelp(root);
}

template<typename T>
//Thr为头节点，T为根.
void ThrBinTree<T>::InTreading()
{
    ThrBinTreeNode<T> *pre=root;
    InThreadingHelp(root,pre);    //中序线索化
    if(pre->rightChild==NULL)
        pre->RTag=Thread;       //处理最后一个节点
}

template<typename T>
void ThrBinTree<T>::PreTreading()
{
    ThrBinTreeNode<T> *pre=root;
    PreThreadingHelp(root,pre);    //先序线索化
    if(pre->rightChild==NULL)
        pre->RTag=Thread;       //处理最后一个节点
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
        ThrBinTreeNode<T> *p=root;            //p指向根
        while(p->LTag==Link)  p=p->leftChild;     //左走至最左下角
        while(p)
        {
            (*visit)(p->data);       //访问结点元素
            if(p->RTag==Thread)      //右孩子为线索，则p指向后继
            {
                p=p->rightChild;
            }
            else    //右孩子存在
            {
                p=p->rightChild;     //遍历右孩子
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
        ThrBinTreeNode<T> *p=root;        //p指向根
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
        r=new ThrBinTreeNode<T>(pre[preLeft]);//生成根结点
        int mid=inLeft;
        while(in[mid]!=pre[preLeft])
            mid++;
        CreateBTreeHelp(r->leftChild,pre,in,preLeft+1,preLeft+mid-inLeft,inLeft,mid-1);        //这里如果不懂建议自己画图手工实现一遍。
        CreateBTreeHelp(r->rightChild,pre,in,preLeft+mid-inLeft+1,preRight,mid+1,inRight);
    }
}

template<typename T>
//构造树
ThrBinTree<T>& ThrBinTree<T>::CreateBTree(T pre[],T in[],int n)
{
    ThrBinTreeNode<T> *r;   //根
    CreateBTreeHelp(r,pre,in,0,n-1,0,n-1);
    //return BinaryTree<ElemType>(r);//，不能这么返回，Error:不应该返回局部变量的地址
    *this = ThrBinTree<T>(r);
    return *this;
}

#include<queue>
template<typename T>
void ThrBinTree<T>::LevelOrder(void (*visit) (const T &))const
{    //队列实现
    visit=print;
    queue<ThrBinTreeNode<T> *> q;
    ThrBinTreeNode<T> *t=root;
    if(t!=NULL) q.push(t);                //根非空，入队
    while(!q.empty())                //队不空
    {
        t=q.front();
        q.pop();                    //出队
        (*visit)(t->data);          //一层一层进行遍历，无法理解还是自己画图实现一遍
        if(t->leftChild)
            q.push(t->leftChild);  //遍历左孩子
        if(t->rightChild)           
            q.push(t->rightChild); //遍历右孩子
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


