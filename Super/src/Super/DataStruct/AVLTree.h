#ifndef _AVL_TREE_HPP_
#define _AVL_TREE_HPP_


//AVL树的"前序遍历"、"中序遍历"、"后序遍历"、"最大值"、"最小值"、"查找"、"打印"、"销毁"等接口与"二叉查找树"基本一样


#include <iomanip>
#include <iostream>
using namespace std;


template <class T>
class AVLTreeNode
{
    public:
        T key;                              //关键字(键值)
        int height;                       //高度
        AVLTreeNode *left;        //左孩子
        AVLTreeNode *right;      //右孩子
        AVLTreeNode(T value, AVLTreeNode *l, AVLTreeNode *r):
            key(value), height(0),left(l),right(r) 
        {

        }
};

template <class T>
class AVLTree
{
    private:
        AVLTreeNode<T> *pRoot;    // 根结点
    public:
        AVLTree();
        ~AVLTree();
        // 获取树的高度
        int height();
        // 获取树的高度
        int max(int a, int b);

        // 前序遍历"AVL树"
        void preOrder();
        // 中序遍历"AVL树"
        void inOrder();
        // 后序遍历"AVL树"
        void postOrder();

        // (递归实现)查找"AVL树"中键值为key的节点
        AVLTreeNode<T>* search(T key);
        // (非递归实现)查找"AVL树"中键值为key的节点
        AVLTreeNode<T>* iterativeSearch(T key);

        // 查找最小结点：返回最小结点的键值。
        T findMin();
        // 查找最大结点：返回最大结点的键值。
        T findMax();

        // 将结点(key为节点键值)插入到AVL树中
        void insert(T key);

        // 删除结点(key为节点键值)
        void remove(T key);

        // 销毁AVL树
        void destroy();

        // 打印AVL树
        void print();
    private:
        // 获取树的高度
        int height(AVLTreeNode<T>* pNode) ;

        // 前序遍历"AVL树"
        void preOrder(AVLTreeNode<T>* pNode) const;
        // 中序遍历"AVL树"
        void inOrder(AVLTreeNode<T>* pNode) const;
        // 后序遍历"AVL树"
        void postOrder(AVLTreeNode<T>* pNode) const;

        // (递归实现)查找"AVL树x"中键值为key的节点
        AVLTreeNode<T>* search(AVLTreeNode<T>* x, T key) const;
        // (非递归实现)查找"AVL树x"中键值为key的节点
        AVLTreeNode<T>* iterativeSearch(AVLTreeNode<T>* x, T key) const;

        // 查找最小结点：返回pNode为根结点的AVL树的最小结点。
        AVLTreeNode<T>* findMin(AVLTreeNode<T>* pNode);
        // 查找最大结点：返回pNode为根结点的AVL树的最大结点。
        AVLTreeNode<T>* findMax(AVLTreeNode<T>* pNode);

        // LL：左左对应的情况(左单旋转)。
        AVLTreeNode<T>* leftLeftRotation(AVLTreeNode<T>* k2);

        // RR：右右对应的情况(右单旋转)。
        AVLTreeNode<T>* rightRightRotation(AVLTreeNode<T>* k1);

        // LR：左右对应的情况(左双旋转)。
        AVLTreeNode<T>* leftRightRotation(AVLTreeNode<T>* k3);

        // RL：右左对应的情况(右双旋转)。
        AVLTreeNode<T>* rightLeftRotation(AVLTreeNode<T>* k1);

        // 将结点(z)插入到AVL树(pNode)中
        AVLTreeNode<T>* insert(AVLTreeNode<T>* &pNode, T key);

        // 删除AVL树(pNode)中的结点(z)，并返回被删除的结点
        AVLTreeNode<T>* remove(AVLTreeNode<T>* &pNode, AVLTreeNode<T>* z);

        // 销毁AVL树
        void destroy(AVLTreeNode<T>* &pNode);

        // 打印AVL树
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


// 获取树的高度
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


//比较两个值的大小

template <class T>
int AVLTree<T>::max(int a, int b) 
{
    return a>b ? a : b;
}


//前序遍历"AVL树"
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


//中序遍历"AVL树"
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

//后序遍历"AVL树"
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


//(递归实现)查找"AVL树x"中键值为key的节点
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


//(非递归实现)查找"AVL树x"中键值为key的节点

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


//查找最小结点：返回pNode为根结点的AVL树的最小结点。
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
 

//查找最大结点：返回pNode为根结点的AVL树的最大结点。

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

//LL：左左对应的情况(左单旋转)。返回值：旋转后的根节点

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



//RR：右右对应的情况(右单旋转)。返回值：旋转后的根节点
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

//LR：左右对应的情况(左双旋转)。返回值：旋转后的根节点
template <class T>
AVLTreeNode<T>* AVLTree<T>::leftRightRotation(AVLTreeNode<T>* k3)
{
    k3->left = rightRightRotation(k3->left);

    return leftLeftRotation(k3);
}


// RL：右左对应的情况(右双旋转)。返回值：旋转后的根节点

template <class T>
AVLTreeNode<T>* AVLTree<T>::rightLeftRotation(AVLTreeNode<T>* k1)
{
    k1->right = leftLeftRotation(k1->right);

    return rightRightRotation(k1);
}


//将结点插入到AVL树中，并返回根节点 参数说明:pNode AVL树的根结点   key 插入的结点的键值  返回值： 根节点
template <class T>
AVLTreeNode<T>* AVLTree<T>::insert(AVLTreeNode<T>* &pNode, T key)
{
    if (pNode == NULL) 
    {
        // 新建节点
        pNode = new AVLTreeNode<T>(key, NULL, NULL);
        if (pNode==NULL)
        {
            cout << "ERROR: create avltree node failed!" << endl;
            return NULL;
        }
    }
    else if (key < pNode->key) // 应该将key插入到"pNode的左子树"的情况
    {
        pNode->left = insert(pNode->left, key);
        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
        if (height(pNode->left) - height(pNode->right) == 2)
        {
            if (key < pNode->left->key)
                pNode = leftLeftRotation(pNode);
            else
                pNode = leftRightRotation(pNode);
        }
    }
    else if (key > pNode->key) // 应该将key插入到"pNode的右子树"的情况
    {
        pNode->right = insert(pNode->right, key);
        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
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
        cout << "添加失败：不允许添加相同的节点！" << endl;
    }

    pNode->height = max( height(pNode->left), height(pNode->right)) + 1;

    return pNode;
}

template <class T>
void AVLTree<T>::insert(T key)
{
    insert(pRoot, key);
}


// 删除结点(z)，返回根节点参数说明： pNode AVL树的根结点  z 待删除的结点  返回值： 根节点

template <class T>
AVLTreeNode<T>* AVLTree<T>::remove(AVLTreeNode<T>* &pNode, AVLTreeNode<T>* z)
{
    // 根为空 或者 没有要删除的节点，直接返回NULL。
    if (pNode==NULL || z==NULL)
        return NULL;

    if (z->key < pNode->key)        // 待删除的节点在"pNode的左子树"中
    {
        pNode->left = remove(pNode->left, z);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。
        if (height(pNode->right) - height(pNode->left) == 2)
        {
            AVLTreeNode<T> *r =  pNode->right;
            if (height(r->left) > height(r->right))
                pNode = rightLeftRotation(pNode);
            else
                pNode = rightRightRotation(pNode);
        }
    }
    else if (z->key > pNode->key)// 待删除的节点在"pNode的右子树"中
    {
        pNode->right = remove(pNode->right, z);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。
        if (height(pNode->left) - height(pNode->right) == 2)
        {
            AVLTreeNode<T> *l =  pNode->left;
            if (height(l->right) > height(l->left))
                pNode = leftRightRotation(pNode);
            else
                pNode = leftLeftRotation(pNode);
        }
    }
    else    // pNode是对应要删除的节点。
    {
        // pNode的左右孩子都非空
        if ((pNode->left!=NULL) && (pNode->right!=NULL))
        {
            if (height(pNode->left) > height(pNode->right))
            {
                // 如果pNode的左子树比右子树高；
                // 则(01)找出pNode的左子树中的最大节点
                //   (02)将该最大节点的值赋值给pNode。
                //   (03)删除该最大节点。
                // 这类似于用"pNode的左子树中最大节点"做"pNode"的替身；
                // 采用这种方式的好处是：删除"pNode的左子树中最大节点"之后，AVL树仍然是平衡的。
                AVLTreeNode<T>* max = findMax(pNode->left);
                pNode->key = max->key;
                pNode->left = remove(pNode->left, max);
            }
            else
            {
                // 如果pNode的左子树不比右子树高(即它们相等，或右子树比左子树高1)
                // 则(01)找出pNode的右子树中的最小节点
                //   (02)将该最小节点的值赋值给pNode。
                //   (03)删除该最小节点。
                // 这类似于用"pNode的右子树中最小节点"做"pNode"的替身；
                // 采用这种方式的好处是：删除"pNode的右子树中最小节点"之后，AVL树仍然是平衡的。
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


//销毁AVL树
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

//打印二叉查找树"key 节点的键值 direction 0表示是根节点;-1表示是它的父结点的左孩子; 1表示是它的父结点的右孩子。

template <class T>
void AVLTree<T>::print(AVLTreeNode<T>* pNode, T key, int direction)
{
    if(pNode != NULL)
    {
        if(direction==0)    // pNode是根节点
            cout << setw(2) << pNode->key << " is root" << endl;
        else                // pNode是分支节点
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


