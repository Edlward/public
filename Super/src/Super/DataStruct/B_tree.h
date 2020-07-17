#ifndef _NormalTree_H_
#define _NormalTree_H_

//��ͨ���������ݹ�������ǵݹ����ʵ�֣��𲽲�ȫ


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
    BinaryNode()
    {
        data = 0;
        leftChild = NULL;
        rightChild = NULL;
    }
    BinaryNode(T val)
    {
        data = val;
        leftChild = NULL;
        rightChild = NULL;
    }
private:
};


//��������
template <class  T>
class BinaryTree
{
private:
    BinaryNode<T> *pRoot;  //�����
public:
    BinaryTree();	//����ս��
    BinaryTree(const T preList[], const int size, int index, const T end);	//������
    ~BinaryTree();

    //�����ʼ������������
    BinaryNode<T>* CreateBiTree(const T preList[], const int size, int &index, const T end);	    //���򴴽�������
    void ClearBiTree(BinaryNode<T> *root);          //���ٶ�����

    //���뷨����������
    void inertNode(T value)
    {
        BinaryNode<T>*  pNode=pRoot;
        if (pNode==NULL)
        {
            return;
        }
        if(value<=pNode->data)
        {
            if(!pNode->leftChild)
            {
                pNode->leftChild=new BinaryNode<T>(value);
            }
            else
            {
                inertNode(pNode->leftChild,value);
            }
        }
        else
        {
            if(!pNode->rightChild)
            {
                pNode->rightChild=new BinaryNode<T> *(value);
            }
            else
            {
                inertNode(pNode->rightChild,value);
            }
        }
    }

public:
    void preOrder() //ǰ������ݹ��ⲿ����
    {
        preOrder_(pRoot);
    }
    void inOrder()    //��������ݹ��ⲿ����
    {
        inOrder_(pRoot);
    }
    void posOrder() //��������ݹ��ⲿ����
    {
        posOrder_(pRoot);
    }
private:
    //ǰ������ݹ��ڲ�ʵ��  ����Ϊ������㡪�����㡪���ҽ�㡣
    void preOrder_(BinaryNode<T>*  pNode)
    {
        if(pNode)
        {
            std::cout<<pNode->data;
            preOrder_(pNode->leftChild);
            preOrder_(pNode->rightChild);
        }
    }
    //��������ݹ��ڲ�ʵ�� ����Ϊ�����㡪������㡪���ҽ��
    void inOrder_(BinaryNode<T>*  pNode)
    {
        if(pNode)
        {
            inOrder(pNode->leftChild);
            std::cout<<pNode->data;
            inOrder(pNode->rightChild);
        }
    }
    //��������ݹ��ڲ�ʵ�� ����Ϊ�����㡪���ҽ�㡪������㡣
    void posOrder_(BinaryNode<T>*  pNode)
    {
        if(pNode)
        {
            posOrder_(pNode->leftChild);
            posOrder_(pNode->rightChild);
            std::cout<<pNode->data;
        }
    }

public:
    void PreOrderNonRescursion();	    //����������ǵݹ�
    void InOrderNonRescursion();	//����������ǵݹ�
    void PostOrderNonRescursion();	//����������ǵݹ�

    //��α���(������ȱ���)����ָOffer�����漰��
    void DfsTravel();
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
//�ǵݹ��ʵ�֣�
//ǰ������ķ���˳��Ϊ����������ҡ�������һ��㣬��ɿ�������㣬��˿�ֱ�ӷ��ʡ���������������Ӳ�Ϊ�գ�������ͬ����������������������������������ٷ�������������������������£�
//������һ���P��
//step 1:���ʽ��p���������p��ջ��
//step 2:�жϽ��p�������Ƿ�Ϊ�ա���Ϊ�գ���ȡջ����㲢��ջ����ջ��Ԫ�ص��Һ�����Ϊ��ǰ�Ľ��p��ѭ����step 1��
//����Ϊ�գ���p��������Ϊ��ǰ���p��
//step 3:ֱ��pΪNULL������ջΪ�գ������������

template <class T>
void BinaryTree<T>::PreOrderNonRescursion()
{
    BinaryNode<T>* cur = pRoot;
    if(cur == NULL)
    {
        cout << "NULL" << endl;
        return;
    }

    stack<BinaryNode<T> *> Stack;
    while ((cur != NULL) || (!Stack.empty()))
    {
        while (cur != NULL)
        {
            cout << cur->data << " "; 
            Stack.push(cur);
            cur = cur->leftChild;
        }
        BinaryNode<T> *top = Stack.top();
        Stack.pop();
        cur = top->rightChild;
        cur->data;
    }
    cout << endl;
}

//2 �������
//�ǵݹ鷨��
//�������������˳�򣬶�����һ��㣬�ȷ��������ӡ��������ֿ��Կ���һ������㣬Ȼ������������ӣ�ֱ�����������ӽ��Ϊ�գ���ֹͣ���ʡ�Ȼ������Һ��ӡ�
//����������£�
//������һ���p��
//step 1:�������Ӳ�Ϊ�գ���p��ջ������p��������Ϊ��ǰ��p��Ȼ��Ե�ǰ���p�ٽ�����ͬ�Ĵ���
//step 2:��������Ϊ�գ���ȡջ��Ԫ�ز����г�ջ���������ʸ�ջ����㣬Ȼ��ջ�������Һ�����Ϊ��ǰ��P��㡣
//step 3:ֱ��pΪNULL����ջΪ�������������

template<class T>
void BinaryTree<T>::InOrderNonRescursion()
{
    BinaryNode<T> *cur = pRoot;
    stack<BinaryNode<T> *> Stack;

    while ((cur != NULL) || (!Stack.empty()))
    {
        while (cur != NULL)
        {
            Stack.push(cur);
            cur = cur->leftChild;
        }
        BinaryNode<T> *top = Stack.top();
        cout << top->data << " ";
        Stack.pop();
        cur = top->rightChild;
    }
    cout << endl;
}


//�������
//�ǵݹ������Ҫ��֤����������Ӻ��Һ��ӷ���֮����ܷ��ʡ���˶�����һ���p���Ƚ�����ջ��
//    ��p���������Ӻ��Һ��ӣ������ֱ�ӷ�����������p�������ӻ����Һ��ӣ��������Ӻ��Һ��Ӷ��Ѿ������ʹ��ˣ������ֱ�ӷ��ʸý�㡣
//    ��������������������Һ��Ӻ�����������ջ���������Ա�֤ÿ��ȡջ��Ԫ��ʱ���������Һ���ǰ�汻���ʣ�����������Ӻ��Һ��ӷ���֮�󱻷��ʡ�

template<class T>
void BinaryTree<T>::PostOrderNonRescursion()
{
    BinaryNode<T> *cur = pRoot;
    BinaryNode<T> *pre = NULL;

    stack<BinaryNode<T> *> Stack;

    while ((cur != NULL) || (!Stack.empty()))
    {
        while (cur != NULL)
        {
            Stack.push(cur);
            cur = cur->leftChild;
        }
        BinaryNode<T> *top = Stack.top();
        if((top->rightChild == NULL) || (pre == top->rightChild))
        {
            cout << top->data << " ";
            pre = top;
            Stack.pop();
        }
        else
        {
            cur = top->rightChild;
        }

    }
    cout << endl;

    //д��2 ���Բο�
#if 0
    stack<BinaryTreeNode*> s;
    BinaryTreeNode *cur;
    BinaryTreeNode *pre=NULL;
    s.push(pRoot);//�������ջ
    while(!s.empty())
    {
        cur=s.top();
        if((cur->left==NULL&&cur->right==NULL)||(pre!=NULL&&(pre==cur->left||pre==cur->right)))
        {
            //���Ӻ��Һ���ͬʱΪ�գ����ߵ�ǰ�������ӻ��Һ����Ѿ���������
            cout<<cur->value<<" ";
            s.pop();
            pre=cur;
        }
        else
        {
            if(cur->right!=NULL)
                s.push(cur->right);
            if(cur->left!=NULL)
                s.push(cur->left);
        }
    }
#endif

}


//�ġ���α�������ָOffer�����漰��
//    ��Ŀ�����ϵ��´�ӡ��������ÿ���ڵ㣬ͬ��Ľڵ㰴�մ������Ҵ�ӡ��
//    ���������ֲ���������������ù�����ȱ�����˼�룬��������������ͼ�������ڶ�������ɡ�
//step1:����ʼ�ڵ������С�
//step2:ÿ�δӶ�ͷȡ���ڵ㣬����������������������ѴӸýڵ��ܵ���Ľڵ㣨�������������������η�����С�
//�ظ�step2��ֱ�������еĽڵ�Ϊ�ա�
template<class T>
void BinaryTree<T>::DfsTravel()
{
    if(pRoot == NULL)
        return;
    std::deque<BinaryNode<T>> deque;
    deque.push_back(pRoot);
    while(deque.size())
    {
        BinaryNode<T>* pNode = deque.front();
        deque.pop_front();

        printf("%d ", pNode->m_nValue);

        if(pNode->leftChild)
            deque.push_back(pNode->leftChild);

        if(pNode->rightChild)
            deque.push_back(pNode->rightChild);
    }
}


#ifdef UNIT_TEST
int Test_BinaryTree();
#endif

#endif


