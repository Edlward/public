#pragma once


#include <iostream>
#include <stack>
#include <queue>

using namespace std;

//�����
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


//��������
template <class  T>
class BinaryTree
{
private:
    BinaryNode<T>* pRoot;  //�����
public:
    BinaryTree();	//����ս��
    BinaryTree(const T preList[], const int size, int index, const T end);	//������
    ~BinaryTree();

    BinaryNode<T>* CreateBiTree(const T preList[], const int size, int &index, const T end);	//���򴴽�������
    void ClearBiTree(BinaryNode<T> *root);  //���ٶ�����
    void PreOrderNonRescursion();	//����������ǵݹ�
    void InOrderNonRescursion();	//����������ǵݹ�
    void PostOrderNonRescursion();	//����������ǵݹ�
};

//�������
template <class T>
BinaryTree<T>::BinaryTree()
{
    pRoot = NULL;
}

//�����������
template <class T>
BinaryTree<T>::BinaryTree(const T preList[], const int size, int index, const T end)
{
    pRoot = CreateBiTree(preList, size, index, end);
}

//����
template <class T>
BinaryTree<T>::~BinaryTree()
{
    ClearBiTree(pRoot);
}

//���򴴽�������
template <class T>
BinaryNode<T>* BinaryTree<T>::CreateBiTree(const T preList[],const int size, int &index,const T end)  //�ر�ע�⣺index���������ã�������������++index�����������ı�
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

//���ٶ�����
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

//�ǵݹ����

//1 ǰ�����
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

//2 �������
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


//�������
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