#pragma once


#include <iostream>
#include <stack>
#include <queue>

using namespace std;

//结点类
template <class T>
class BinaryNode
{
public:
    T data;
    BinaryNode<T> *leftChild;
    BinaryNode<T> *rightChild;

    BinaryNode();
    BinaryNode(T val);
private:

};

template <class T>
BinaryNode<T>::BinaryNode()
{
    data = 0;
    leftChild = NULL;
    rightChild = NULL;
}

template <class T>
BinaryNode<T>::BinaryNode(T val)
{
    data = val;
    leftChild = NULL;
    rightChild = NULL;
}


//二叉树类
template <class  T>
class BinaryTree
{
private:
    BinaryNode<T>* pRoot;  //根结点
public:
    BinaryTree();	//构造空结点
    BinaryTree(const T preList[], const int size, int index, const T end);	//先序构造
    ~BinaryTree();

    BinaryNode<T>* CreateBiTree(const T preList[], const int size, int &index, const T end);	//先序创建二叉树
    void ClearBiTree(BinaryNode<T> *root);  //销毁二叉树
    void PreOrderNonRescursion();	//先序遍历，非递归
    void InOrderNonRescursion();	//中序遍历，非递归
    void PostOrderNonRescursion();	//后序遍历，非递归
};

//构造空树
template <class T>
BinaryTree<T>::BinaryTree()
{
    pRoot = NULL;
}

//先序构造二叉树
template <class T>
BinaryTree<T>::BinaryTree(const T preList[], const int size, int index, const T end)
{
    pRoot = CreateBiTree(preList, size, index, end);
}

//析构
template <class T>
BinaryTree<T>::~BinaryTree()
{
    ClearBiTree(pRoot);
}

//先序创建二叉树
template <class T>
BinaryNode<T>* BinaryTree<T>::CreateBiTree(const T preList[],const int size, int &index,const T end)  //特别注意：index必须用引用，否则函数的两个++index将不会正常改变
{
    BinaryNode<T>* root = NULL;

    if((index < size) && (preList[index] != end))
    {

        root = new BinaryNode<T>();

        root->data = preList[index];
        root->leftChild = CreateBiTree(preList, size, ++index, end);
        root->rightChild = CreateBiTree(preList, size, ++index, end);
    }
    return root;
}

//销毁二叉树
template <class T>
void BinaryTree<T>::ClearBiTree(BinaryNode<T>* root)
{
    BinaryNode<T> *tmp = root;

    if(tmp == NULL)
    {
        return;
    }
    ClearBiTree(tmp->leftChild);
    ClearBiTree(tmp->rightChild);

    delete tmp;
    tmp = NULL;
}

//非递归遍历

//1 前序遍历
template <class T>
void BinaryTree<T>::PreOrderNonRescursion()
{
    BinaryNode<T>* cur = pRoot;
    stack<BinaryNode<T> *> biTreeStack;

    if(cur == NULL)
    {
        cout << "NULL" << endl;
        return;
    }

    while ((cur != NULL) || (!biTreeStack.empty()))
    {
        while (cur != NULL)
        {
            cout << cur->data << " "; 
            biTreeStack.push(cur);
            cur = cur->leftChild;
        }
        BinaryNode<T> *top = biTreeStack.top();
        biTreeStack.pop();
        cur = top->rightChild;
        cur->data;
    }
    cout << endl;
}

//2 中序遍历
template<class T>
void BinaryTree<T>::InOrderNonRescursion()
{
    BinaryNode<T> *cur = pRoot;
    stack<BinaryNode<T> *> biTreeStack;

    while ((cur != NULL) || (!biTreeStack.empty()))
    {
        while (cur != NULL)
        {
            biTreeStack.push(cur);
            cur = cur->leftChild;
        }
        BinaryNode<T> *top = biTreeStack.top();
        cout << top->data << " ";
        biTreeStack.pop();
        cur = top->rightChild;
    }
    cout << endl;
}


//后序遍历
template<class T>
void BinaryTree<T>::PostOrderNonRescursion()
{
    BinaryNode<T> *cur = pRoot;
    BinaryNode<T> *priview = NULL;

    stack<BinaryNode<T> *> biTreeStack;

    while ((cur != NULL) || (!biTreeStack.empty()))
    {
        while (cur != NULL)
        {
            biTreeStack.push(cur);
            cur = cur->leftChild;
        }

        BinaryNode<T> *top = biTreeStack.top();

        if((top->rightChild == NULL) || (priview == top->rightChild))
        {
            cout << top->data << " ";
            priview = top;
            biTreeStack.pop();
        }
        else
        {
            cur = top->rightChild;
        }

    }
    cout << endl;
}


int main()
{	
    int preList[] = {1, 2 , 0, 22, 0, 0, 3, 0, 33, 0, 0};
    //int preList[] = {1, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0};
    BinaryTree<int> *tree = new BinaryTree<int>(preList, 12, 0, 0);
    tree->PreOrderNonRescursion();
    tree->InOrderNonRescursion();
    tree->PostOrderNonRescursion();
    delete tree;

    return 0;
}