#ifndef _BinarySearchTree_H_
#define _BinarySearchTree_H_

//���������(Binary Search Tree)���ֱ���Ϊ������������
//�ڶ���������У�
//(01) ������ڵ�����������գ��������������н���ֵ��С�����ĸ�����ֵ��
//(02) ����ڵ�����������գ��������������н���ֵ���������ĸ�����ֵ��
//(03) ����ڵ����������Ҳ�ֱ�Ϊ�����������
//(04) û�м�ֵ��ȵĽڵ㣨no duplicate nodes����


//������binary tree�Ͷ��������� binary search tree������??
//�������BSTһ���Ƕ����������ҽ����Ϣ�ǹؼ��루���ܻ����м�¼����������
//����BST�йؼ������ظ���������������ڣ������еĹؼ���һ��С�ڸ���
//������������ڣ������йؼ���һ�����ڸ�������������ȻҲ��BST���������������������е�������


#include <iomanip>
#include <iostream>
using namespace std;

template <class T>
class BSTNode
{
    public:
        T key;                    // �ؼ���(��ֵ)
        BSTNode *left;      //����
        BSTNode *right;    //�Һ���
        BSTNode *parent;  //�����
        //int num;//��ֵͬ�Ľ����ֵĴ���,����������ظ�Ԫ�صĻ����ó�Ա����ʡȥ  
        //�ݲ�ʵ��-- ����μ�CSDN���� ���ݽṹ���㷨����������������C++ʵ��

        BSTNode(T value, BSTNode *p, BSTNode *l, BSTNode *r):
            key(value),parent(p),left(l),right(r) 
        {

        }
};

template <class T>
class BSTree 
{
    private:
        BSTNode<T> *pRoot;    // �����
    public:
        BSTree();
        ~BSTree();

        // ǰ�����"������"
        void preOrder();
        // �������"������"
        void inOrder();
        // �������"������"
        void postOrder();

        // (�ݹ�ʵ��)����"������"�м�ֵΪkey�Ľڵ�
        BSTNode<T>* search(T key);
        // (�ǵݹ�ʵ��)����"������"�м�ֵΪkey�Ľڵ�
        BSTNode<T>* iterativeSearch(T key);

        // ������С��㣺������С���ļ�ֵ��
        T findMin();
        // ��������㣺���������ļ�ֵ��
        T findMax();

        // �ҽ��(x)�ĺ�̽�㡣��������"������������ֵ���ڸý��"��"��С���"��
        BSTNode<T>* successor(BSTNode<T> *x);
        // �ҽ��(x)��ǰ����㡣��������"������������ֵС�ڸý��"��"�����"��
        BSTNode<T>* predecessor(BSTNode<T> *x);

        // �����(keyΪ�ڵ��ֵ)���뵽��������
        void insert(T key);

        // ɾ�����(keyΪ�ڵ��ֵ)
        void remove(T key);

        // ���ٶ�����
        void destroy();

        // ��ӡ������
        void print();
    private:
        // ǰ�����"������"
        void preOrder(BSTNode<T>* pNode) const;
        // �������"������"
        void inOrder(BSTNode<T>* pNode) const;
        // �������"������"
        void postOrder(BSTNode<T>* pNode) const;

        // (�ݹ�ʵ��)����"������x"�м�ֵΪkey�Ľڵ�
        BSTNode<T>* search(BSTNode<T>* x, T key) const;
        // (�ǵݹ�ʵ��)����"������x"�м�ֵΪkey�Ľڵ�
        BSTNode<T>* iterativeSearch(BSTNode<T>* x, T key) const;

        // ������С��㣺����pNodeΪ�����Ķ���������С��㡣 �ݹ�ʵ��
        BSTNode<T>* findMin(BSTNode<T>* pNode);
        // ��������㣺����pNodeΪ�����Ķ�����������㡣 �ݹ�ʵ��
        BSTNode<T>* findMax(BSTNode<T>* pNode);

        // �����(z)���뵽������(pNode)��
        void insert(BSTNode<T>* &pNode, BSTNode<T>* z);

        // ɾ��������(pNode)�еĽ��(z)�������ر�ɾ���Ľ��
        BSTNode<T>* remove(BSTNode<T>* &pNode, BSTNode<T> *z);

        // ���ٶ�����
        void destroy(BSTNode<T>* &pNode);

        // ��ӡ������
        void print(BSTNode<T>* pNode, T key, int direction);


       //add by lizulin 
	   //��x�ҵ�ʱ�����棬���򷵻ؼ� 
        bool contains(const T &x) const
        {
            return contains(x,pRoot);
        }
	    // ͨ�������ķ�������x�Ƿ���������������t��
	    bool contains(const T &x, BSTNode<T>* t) const
	    {
		    if (t == NULL) // ������δ�ҵ�Ԫ�ص���ֹ����
			    return false;
		    else if (x < t->key)
			    return contains(x, t->left);
		    else if (t->key < x)
			    return contains(x, t->right);
		    else // ��� x ������ Ҳ ��С��t��ָ�Ľڵ��е�Ԫ�أ���x==t->element
			    return true;
        }
        //�ж����Ƿ�Ϊ��
        bool isEmpty() const
        {
            return pRoot== NULL;
        }

     //��������
	BSTNode<T> * clone(BSTNode<T> *t) const
	{
		if (t == nullptr)
			return nullptr;
		return new BSTNode<T>(t->key, clone(t->left), clone(t->right));
	}
};


template <class T>
BSTree<T>::BSTree():pRoot(NULL)
{
}

template <class T>
BSTree<T>::~BSTree() 
{
    destroy();
}


//ǰ�����"������"
template <class T>
void BSTree<T>::preOrder(BSTNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        cout<< pNode->key << " " ;
        preOrder(pNode->left);
        preOrder(pNode->right);
    }
}

template <class T>
void BSTree<T>::preOrder() 
{
    preOrder(pRoot);
}


//�������"������"
template <class T>
void BSTree<T>::inOrder(BSTNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        inOrder(pNode->left);
        cout<< pNode->key << " " ;
        inOrder(pNode->right);
    }
}

template <class T>
void BSTree<T>::inOrder() 
{
    inOrder(pRoot);
}


//�������"������"
template <class T>
void BSTree<T>::postOrder(BSTNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        postOrder(pNode->left);
        postOrder(pNode->right);
        cout<< pNode->key << " " ;
    }
}

template <class T>
void BSTree<T>::postOrder() 
{
    postOrder(pRoot);
}


//(�ݹ�ʵ��)����"������x"�м�ֵΪkey�Ľڵ�
//���ҹ��̣� 
//    1���������ҵ�Ԫ��ֵ�͸��ڵ���ͬ���򷵻ظ��ڵ㡣 
//    2���������ҵ�Ԫ��ֵС�ڸ��ڵ��Ԫ��ֵ����ݹ�Ӹ��ڵ���������в��ҡ� 
//    3���������ҵ�Ԫ��ֵ���ڸ��ڵ��Ԫ��ֵ����ݹ�Ӹ��ڵ���������в��ҡ�
//
template <class T>
BSTNode<T>* BSTree<T>::search(BSTNode<T>* x, T key) const
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template <class T>
BSTNode<T>* BSTree<T>::search(T key) 
{
    search(pRoot, key);
}


//(�ǵݹ�ʵ��)����"������x"�м�ֵΪkey�Ľڵ�
template <class T>
BSTNode<T>* BSTree<T>::iterativeSearch(BSTNode<T>* x, T key) const
{
    while ((x!=NULL) && (x->key!=key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

template <class T>
BSTNode<T>* BSTree<T>::iterativeSearch(T key)
{
    iterativeSearch(pRoot, key);
}


//������С��㣺����pNodeΪ�����Ķ���������С��㡣
//��ȡ��СԪ�صĽڵ㣺 
//    1��������ڵ������������򷵻ظ��ڵ㡣 
//    2�����β�ѯ���ڵ���������ڵ㣬���������������һ����ڵ㡣
//
template <class T>
BSTNode<T>* BSTree<T>::findMin(BSTNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->left != NULL)
        pNode = pNode->left;
    return pNode;
}

template <class T>
T BSTree<T>::findMin()
{
    BSTNode<T> *p = findMin(pRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}
 

//��������㣺����pNodeΪ�����Ķ�����������㡣
//��ȡ���Ԫ�صĽڵ㣺 
//    1��������ڵ������������򷵻ظ��ڵ㡣 
//    2�����β�ѯ���ڵ���������ڵ㣬���������������һ���ҽڵ㡣
template <class T>
BSTNode<T>* BSTree<T>::findMax(BSTNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->right != NULL)
        pNode = pNode->right;
    return pNode;
}

template <class T>
T BSTree<T>::findMax()
{
    BSTNode<T> *p = findMax(pRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}


//�ҽ��(x)�ĺ�̽�㡣��������"������������ֵ���ڸý��"��"��С���"��
template <class T>
BSTNode<T>* BSTree<T>::successor(BSTNode<T> *x)
{
    // ���x�����Һ��ӣ���"x�ĺ�̽��"Ϊ "�����Һ���Ϊ������������С���"��
    if (x->right != NULL)
        return findMin(x->right);

    // ���xû���Һ��ӡ���x���������ֿ��ܣ�
    // (01) x��"һ������"����"x�ĺ�̽��"Ϊ "���ĸ����"��
    // (02) x��"һ���Һ���"�������"x����͵ĸ���㣬���Ҹø����Ҫ��������"���ҵ������"��͵ĸ����"����"x�ĺ�̽��"��
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->right))
    {
        x = y;
        y = y->parent;
    }

    return y;
}
 

//�ҽ��(x)��ǰ����㡣��������"������������ֵС�ڸý��"��"�����"��
template <class T>
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T> *x)
{
    // ���x�������ӣ���"x��ǰ�����"Ϊ "��������Ϊ���������������"��
    if (x->left != NULL)
        return findMax(x->left);

    // ���xû�����ӡ���x���������ֿ��ܣ�
    // (01) x��"һ���Һ���"����"x��ǰ�����"Ϊ "���ĸ����"��
    // (01) x��"һ������"�������"x����͵ĸ���㣬���Ҹø����Ҫ�����Һ���"���ҵ������"��͵ĸ����"����"x��ǰ�����"��
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

//�������뵽�������в���˵����pNode �������ĸ����  z ����Ľ��
//������̣� 
//    1������ǰ������Ϊ�գ�������Ԫ��Ϊ���ڵ㡣 
//    2���������Ԫ��ֵС�ڸ��ڵ�ֵ����ݹ�Ӹ��ڵ�����������ҵ��ɲ���λ�á� 
//    3���������Ԫ��ֵ���ڸ��ڵ�ֵ����ݹ�Ӹ��ڵ�����������ҵ��ɲ���λ�á�
//
template <class T>
void BSTree<T>::insert(BSTNode<T>* &pNode, BSTNode<T>* z)
{
    BSTNode<T> *y = NULL;
    BSTNode<T> *x = pNode;

    // ����z�Ĳ���λ��
    while (x != NULL)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y==NULL)
        pNode = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
}


// �����(keyΪ�ڵ��ֵ)���뵽��������  ����˵����    pNode �������ĸ����    key ������ļ�ֵ
template <class T>
void BSTree<T>::insert(T key)
{
    BSTNode<T> *z=NULL;

    // ����½����ʧ�ܣ��򷵻ء�
    if ((z=new BSTNode<T>(key,NULL,NULL,NULL)) == NULL)
        return ;

    insert(pRoot, z);
}


// ɾ�����(z)�������ر�ɾ���Ľ�����˵����    pNode �������ĸ����   z ɾ���Ľ��
//ɾ�����̣� 
//    1����ɾ���ڵ�ZΪҶ�ӽڵ㣬��ֱ��ɾ���ýڵ㡣�޸ĸ��ڵ��ָ�롣 
//    2����ɾ���ڵ�ZΪ��֧�ڵ㣨ֻ������������������������Z��������Z�ĸ��ڵ�������ɾ���ڵ�Z�� 
//    3����ɾ���ڵ�Z������������Ϊ�ա� 
//    ����һ���ҵ�Z�ĺ�̽ڵ�y����Ϊyһ��û�������������Կ���ֱ��ɾ��y������y�ĸ��׽ڵ��Ϊy���������ĸ��׽ڵ㡣��y�滻Z�� 
//    ���������ҵ�Z��ǰ���ڵ�x��xһ��û�������������Կ���ֱ��ɾ��x������x�ĸ��׽ڵ��Ϊx���������ĸ��׽ڵ㡣��x�滻Z��
//
template <class T>
BSTNode<T>* BSTree<T>::remove(BSTNode<T>* &pNode, BSTNode<T> *z)
{
    BSTNode<T> *x=NULL;
    BSTNode<T> *y=NULL;

    if ((z->left == NULL) || (z->right == NULL) )
        y = z;
    else
        y = successor(z);

    if (y->left != NULL)
        x = y->left;
    else
        x = y->right;

    if (x != NULL)
        x->parent = y->parent;

    if (y->parent == NULL)
        pNode = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    if (y != z) 
        z->key = y->key;

    return y;
}

//ɾ�����(z)�������ر�ɾ���Ľ�����˵����  pNode �������ĸ����   z ɾ���Ľ��
template <class T>
void BSTree<T>::remove(T key)
{
    BSTNode<T> *z, *node; 

    if ((z = search(pRoot, key)) != NULL)
        if ( (node = remove(pRoot, z)) != NULL)
            delete node;
}

//���ٶ�����
template <class T>
void BSTree<T>::destroy(BSTNode<T>* &pNode)
{
    if (pNode==NULL)
        return ;

    if (pNode->left != NULL)
        return destroy(pNode->left);
    if (pNode->right != NULL)
        return destroy(pNode->right);

    delete pNode;
    pNode=NULL;
}

template <class T>
void BSTree<T>::destroy()
{
    destroy(pRoot);
}


//��ӡ"���������"
//key        -- �ڵ�ļ�ֵ 
//direction  --  0����ʾ�ýڵ��Ǹ��ڵ�;-1����ʾ�ýڵ������ĸ���������;1����ʾ�ýڵ������ĸ������Һ��ӡ�


template <class T>
void BSTree<T>::print(BSTNode<T>* pNode, T key, int direction)
{
    if(pNode != NULL)
    {
        if(direction==0)    // pNode�Ǹ��ڵ�
            cout << setw(2) << pNode->key << " is root" << endl;
        else                // pNode�Ƿ�֧�ڵ�
            cout << setw(2) << pNode->key << " is " << setw(2) << key << "'s "  << setw(12) << (direction==1?"right child" : "left child") << endl;

        print(pNode->left, pNode->key, -1);
        print(pNode->right,pNode->key,  1);
    }
}

template <class T>
void BSTree<T>::print()
{
    if (pRoot != NULL)
        print(pRoot, pRoot->key, 0);
}


#ifdef UNIT_TEST
int Test_BinarySearch();
#endif


#endif




