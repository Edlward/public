#ifndef _AVL_TREE_HPP_
#define _AVL_TREE_HPP_


//AVL����"ǰ�����"��"�������"��"�������"��"���ֵ"��"��Сֵ"��"����"��"��ӡ"��"����"�Ƚӿ���"���������"����һ��


#include <iomanip>
#include <iostream>
using namespace std;


template <class T>
class AVLTreeNode
{
    public:
        T key;                              //�ؼ���(��ֵ)
        int height;                       //�߶�
        AVLTreeNode *left;        //����
        AVLTreeNode *right;      //�Һ���
        AVLTreeNode(T value, AVLTreeNode *l, AVLTreeNode *r):
            key(value), height(0),left(l),right(r) 
        {

        }
};

template <class T>
class AVLTree
{
    private:
        AVLTreeNode<T> *pRoot;    // �����
    public:
        AVLTree();
        ~AVLTree();
        // ��ȡ���ĸ߶�
        int height();
        // ��ȡ���ĸ߶�
        int max(int a, int b);

        // ǰ�����"AVL��"
        void preOrder();
        // �������"AVL��"
        void inOrder();
        // �������"AVL��"
        void postOrder();

        // (�ݹ�ʵ��)����"AVL��"�м�ֵΪkey�Ľڵ�
        AVLTreeNode<T>* search(T key);
        // (�ǵݹ�ʵ��)����"AVL��"�м�ֵΪkey�Ľڵ�
        AVLTreeNode<T>* iterativeSearch(T key);

        // ������С��㣺������С���ļ�ֵ��
        T findMin();
        // ��������㣺���������ļ�ֵ��
        T findMax();

        // �����(keyΪ�ڵ��ֵ)���뵽AVL����
        void insert(T key);

        // ɾ�����(keyΪ�ڵ��ֵ)
        void remove(T key);

        // ����AVL��
        void destroy();

        // ��ӡAVL��
        void print();
    private:
        // ��ȡ���ĸ߶�
        int height(AVLTreeNode<T>* pNode) ;

        // ǰ�����"AVL��"
        void preOrder(AVLTreeNode<T>* pNode) const;
        // �������"AVL��"
        void inOrder(AVLTreeNode<T>* pNode) const;
        // �������"AVL��"
        void postOrder(AVLTreeNode<T>* pNode) const;

        // (�ݹ�ʵ��)����"AVL��x"�м�ֵΪkey�Ľڵ�
        AVLTreeNode<T>* search(AVLTreeNode<T>* x, T key) const;
        // (�ǵݹ�ʵ��)����"AVL��x"�м�ֵΪkey�Ľڵ�
        AVLTreeNode<T>* iterativeSearch(AVLTreeNode<T>* x, T key) const;

        // ������С��㣺����pNodeΪ������AVL������С��㡣
        AVLTreeNode<T>* findMin(AVLTreeNode<T>* pNode);
        // ��������㣺����pNodeΪ������AVL��������㡣
        AVLTreeNode<T>* findMax(AVLTreeNode<T>* pNode);

        // LL�������Ӧ�����(����ת)��
        AVLTreeNode<T>* leftLeftRotation(AVLTreeNode<T>* k2);

        // RR�����Ҷ�Ӧ�����(�ҵ���ת)��
        AVLTreeNode<T>* rightRightRotation(AVLTreeNode<T>* k1);

        // LR�����Ҷ�Ӧ�����(��˫��ת)��
        AVLTreeNode<T>* leftRightRotation(AVLTreeNode<T>* k3);

        // RL�������Ӧ�����(��˫��ת)��
        AVLTreeNode<T>* rightLeftRotation(AVLTreeNode<T>* k1);

        // �����(z)���뵽AVL��(pNode)��
        AVLTreeNode<T>* insert(AVLTreeNode<T>* &pNode, T key);

        // ɾ��AVL��(pNode)�еĽ��(z)�������ر�ɾ���Ľ��
        AVLTreeNode<T>* remove(AVLTreeNode<T>* &pNode, AVLTreeNode<T>* z);

        // ����AVL��
        void destroy(AVLTreeNode<T>* &pNode);

        // ��ӡAVL��
        void print(AVLTreeNode<T>* pNode, T key, int direction);
};


template <class T>
AVLTree<T>::AVLTree():pRoot(NULL)
{
}

template <class T>
AVLTree<T>::~AVLTree() 
{
    destroy(pRoot);
}


// ��ȡ���ĸ߶�
template <class T>
int AVLTree<T>::height(AVLTreeNode<T>* pNode) 
{
    if (pNode != NULL)
        return pNode->height;

    return 0;
}

template <class T>
int AVLTree<T>::height() 
{
    return height(pRoot);
}


//�Ƚ�����ֵ�Ĵ�С

template <class T>
int AVLTree<T>::max(int a, int b) 
{
    return a>b ? a : b;
}


//ǰ�����"AVL��"
template <class T>
void AVLTree<T>::preOrder(AVLTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        cout<< pNode->key << " " ;
        preOrder(pNode->left);
        preOrder(pNode->right);
    }
}

template <class T>
void AVLTree<T>::preOrder() 
{
    preOrder(pRoot);
}


//�������"AVL��"
template <class T>
void AVLTree<T>::inOrder(AVLTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        inOrder(pNode->left);
        cout<< pNode->key << " " ;
        inOrder(pNode->right);
    }
}

template <class T>
void AVLTree<T>::inOrder() 
{
    inOrder(pRoot);
}

//�������"AVL��"
template <class T>
void AVLTree<T>::postOrder(AVLTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        postOrder(pNode->left);
        postOrder(pNode->right);
        cout<< pNode->key << " " ;
    }
}

template <class T>
void AVLTree<T>::postOrder() 
{
    postOrder(pRoot);
}


//(�ݹ�ʵ��)����"AVL��x"�м�ֵΪkey�Ľڵ�
template <class T>
AVLTreeNode<T>* AVLTree<T>::search(AVLTreeNode<T>* x, T key) const
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template <class T>
AVLTreeNode<T>* AVLTree<T>::search(T key) 
{
    return search(pRoot, key);
}


//(�ǵݹ�ʵ��)����"AVL��x"�м�ֵΪkey�Ľڵ�

template <class T>
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(AVLTreeNode<T>* x, T key) const
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
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(T key)
{
    return iterativeSearch(pRoot, key);
}


//������С��㣺����pNodeΪ������AVL������С��㡣
template <class T>
AVLTreeNode<T>* AVLTree<T>::findMin(AVLTreeNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->left != NULL)
        pNode = pNode->left;
    return pNode;
}

template <class T>
T AVLTree<T>::findMin()
{
    AVLTreeNode<T> *p = findMin(pRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}
 

//��������㣺����pNodeΪ������AVL��������㡣

template <class T>
AVLTreeNode<T>* AVLTree<T>::findMax(AVLTreeNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->right != NULL)
        pNode = pNode->right;
    return pNode;
}

template <class T>
T AVLTree<T>::findMax()
{
    AVLTreeNode<T> *p = findMax(pRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}

//LL�������Ӧ�����(����ת)������ֵ����ת��ĸ��ڵ�

template <class T>
AVLTreeNode<T>* AVLTree<T>::leftLeftRotation(AVLTreeNode<T>* k2)
{
    AVLTreeNode<T>* k1;

    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = max( height(k2->left), height(k2->right)) + 1;
    k1->height = max( height(k1->left), k2->height) + 1;

    return k1;
}



//RR�����Ҷ�Ӧ�����(�ҵ���ת)������ֵ����ת��ĸ��ڵ�
template <class T>
AVLTreeNode<T>* AVLTree<T>::rightRightRotation(AVLTreeNode<T>* k1)
{
    AVLTreeNode<T>* k2;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = max( height(k1->left), height(k1->right)) + 1;
    k2->height = max( height(k2->right), k1->height) + 1;

    return k2;
}

//LR�����Ҷ�Ӧ�����(��˫��ת)������ֵ����ת��ĸ��ڵ�
template <class T>
AVLTreeNode<T>* AVLTree<T>::leftRightRotation(AVLTreeNode<T>* k3)
{
    k3->left = rightRightRotation(k3->left);

    return leftLeftRotation(k3);
}


// RL�������Ӧ�����(��˫��ת)������ֵ����ת��ĸ��ڵ�

template <class T>
AVLTreeNode<T>* AVLTree<T>::rightLeftRotation(AVLTreeNode<T>* k1)
{
    k1->right = leftLeftRotation(k1->right);

    return rightRightRotation(k1);
}


//�������뵽AVL���У������ظ��ڵ� ����˵��:pNode AVL���ĸ����   key ����Ľ��ļ�ֵ  ����ֵ�� ���ڵ�
template <class T>
AVLTreeNode<T>* AVLTree<T>::insert(AVLTreeNode<T>* &pNode, T key)
{
    if (pNode == NULL) 
    {
        // �½��ڵ�
        pNode = new AVLTreeNode<T>(key, NULL, NULL);
        if (pNode==NULL)
        {
            cout << "ERROR: create avltree node failed!" << endl;
            return NULL;
        }
    }
    else if (key < pNode->key) // Ӧ�ý�key���뵽"pNode��������"�����
    {
        pNode->left = insert(pNode->left, key);
        // ����ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
        if (height(pNode->left) - height(pNode->right) == 2)
        {
            if (key < pNode->left->key)
                pNode = leftLeftRotation(pNode);
            else
                pNode = leftRightRotation(pNode);
        }
    }
    else if (key > pNode->key) // Ӧ�ý�key���뵽"pNode��������"�����
    {
        pNode->right = insert(pNode->right, key);
        // ����ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
        if (height(pNode->right) - height(pNode->left) == 2)
        {
            if (key > pNode->right->key)
                pNode = rightRightRotation(pNode);
            else
                pNode = rightLeftRotation(pNode);
        }
    }
    else //key == pNode->key)
    {
        cout << "���ʧ�ܣ������������ͬ�Ľڵ㣡" << endl;
    }

    pNode->height = max( height(pNode->left), height(pNode->right)) + 1;

    return pNode;
}

template <class T>
void AVLTree<T>::insert(T key)
{
    insert(pRoot, key);
}


// ɾ�����(z)�����ظ��ڵ����˵���� pNode AVL���ĸ����  z ��ɾ���Ľ��  ����ֵ�� ���ڵ�

template <class T>
AVLTreeNode<T>* AVLTree<T>::remove(AVLTreeNode<T>* &pNode, AVLTreeNode<T>* z)
{
    // ��Ϊ�� ���� û��Ҫɾ���Ľڵ㣬ֱ�ӷ���NULL��
    if (pNode==NULL || z==NULL)
        return NULL;

    if (z->key < pNode->key)        // ��ɾ���Ľڵ���"pNode��������"��
    {
        pNode->left = remove(pNode->left, z);
        // ɾ���ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
        if (height(pNode->right) - height(pNode->left) == 2)
        {
            AVLTreeNode<T> *r =  pNode->right;
            if (height(r->left) > height(r->right))
                pNode = rightLeftRotation(pNode);
            else
                pNode = rightRightRotation(pNode);
        }
    }
    else if (z->key > pNode->key)// ��ɾ���Ľڵ���"pNode��������"��
    {
        pNode->right = remove(pNode->right, z);
        // ɾ���ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
        if (height(pNode->left) - height(pNode->right) == 2)
        {
            AVLTreeNode<T> *l =  pNode->left;
            if (height(l->right) > height(l->left))
                pNode = leftRightRotation(pNode);
            else
                pNode = leftLeftRotation(pNode);
        }
    }
    else    // pNode�Ƕ�ӦҪɾ���Ľڵ㡣
    {
        // pNode�����Һ��Ӷ��ǿ�
        if ((pNode->left!=NULL) && (pNode->right!=NULL))
        {
            if (height(pNode->left) > height(pNode->right))
            {
                // ���pNode�����������������ߣ�
                // ��(01)�ҳ�pNode���������е����ڵ�
                //   (02)�������ڵ��ֵ��ֵ��pNode��
                //   (03)ɾ�������ڵ㡣
                // ����������"pNode�������������ڵ�"��"pNode"������
                // �������ַ�ʽ�ĺô��ǣ�ɾ��"pNode�������������ڵ�"֮��AVL����Ȼ��ƽ��ġ�
                AVLTreeNode<T>* max = findMax(pNode->left);
                pNode->key = max->key;
                pNode->left = remove(pNode->left, max);
            }
            else
            {
                // ���pNode��������������������(��������ȣ�������������������1)
                // ��(01)�ҳ�pNode���������е���С�ڵ�
                //   (02)������С�ڵ��ֵ��ֵ��pNode��
                //   (03)ɾ������С�ڵ㡣
                // ����������"pNode������������С�ڵ�"��"pNode"������
                // �������ַ�ʽ�ĺô��ǣ�ɾ��"pNode������������С�ڵ�"֮��AVL����Ȼ��ƽ��ġ�
                AVLTreeNode<T>* min = findMax(pNode->right);
                pNode->key = min->key;
                pNode->right = remove(pNode->right, min);
            }
        }
        else
        {
            AVLTreeNode<T>* tmp = pNode;
            pNode = (pNode->left!=NULL) ? pNode->left : pNode->right;
            delete tmp;
        }
    }

    return pNode;
}

template <class T>
void AVLTree<T>::remove(T key)
{
    AVLTreeNode<T>* z; 

    if ((z = search(pRoot, key)) != NULL)
        pRoot = remove(pRoot, z);
}


//����AVL��
template <class T>
void AVLTree<T>::destroy(AVLTreeNode<T>* &pNode)
{
    if (pNode==NULL)
        return ;

    if (pNode->left != NULL)
        destroy(pNode->left);
    if (pNode->right != NULL)
        destroy(pNode->right);

    delete pNode;
}

template <class T>
void AVLTree<T>::destroy()
{
    destroy(pRoot);
}

//��ӡ���������"key �ڵ�ļ�ֵ direction 0��ʾ�Ǹ��ڵ�;-1��ʾ�����ĸ���������; 1��ʾ�����ĸ������Һ��ӡ�

template <class T>
void AVLTree<T>::print(AVLTreeNode<T>* pNode, T key, int direction)
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
void AVLTree<T>::print()
{
    if (pRoot != NULL)
        print(pRoot, pRoot->key, 0);
}

#ifdef UNIT_TEST
int Test_AVLTree();
#endif



#endif


