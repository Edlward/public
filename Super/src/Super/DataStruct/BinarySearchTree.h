#ifndef _BinarySearchTree_H_
#define _BinarySearchTree_H_

//二叉查找树(Binary Search Tree)，又被称为二叉搜索树。
//在二叉查找树中：
//(01) 若任意节点的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
//(02) 任意节点的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
//(03) 任意节点的左、右子树也分别为二叉查找树。
//(04) 没有键值相等的节点（no duplicate nodes）。


//二叉树binary tree和二叉搜索树 binary search tree的区别??
//差别在于BST一定是二叉树，并且结点信息是关键码（可能还带有记录的索引），
//并且BST中关键码无重复，左子树如果存在，其所有的关键码一定小于根，
//右子树如果存在，其所有关键码一定大于根，左右子树自然也是BST，因此有特性中序遍历序列单调递增


#include <iomanip>
#include <iostream>
using namespace std;

template <class T>
class BSTNode
{
    public:
        T key;                    // 关键字(键值)
        BSTNode *left;      //左孩子
        BSTNode *right;    //右孩子
        BSTNode *parent;  //父结点
        //int num;//相同值的结点出现的次数,如果不考虑重复元素的话，该成员可以省去  
        //暂不实现-- 具体参见CSDN博文 数据结构与算法——二叉查找树类的C++实现

        BSTNode(T value, BSTNode *p, BSTNode *l, BSTNode *r):
            key(value),parent(p),left(l),right(r) 
        {

        }
};

template <class T>
class BSTree 
{
    private:
        BSTNode<T> *pRoot;    // 根结点
    public:
        BSTree();
        ~BSTree();

        // 前序遍历"二叉树"
        void preOrder();
        // 中序遍历"二叉树"
        void inOrder();
        // 后序遍历"二叉树"
        void postOrder();

        // (递归实现)查找"二叉树"中键值为key的节点
        BSTNode<T>* search(T key);
        // (非递归实现)查找"二叉树"中键值为key的节点
        BSTNode<T>* iterativeSearch(T key);

        // 查找最小结点：返回最小结点的键值。
        T findMin();
        // 查找最大结点：返回最大结点的键值。
        T findMax();

        // 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
        BSTNode<T>* successor(BSTNode<T> *x);
        // 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
        BSTNode<T>* predecessor(BSTNode<T> *x);

        // 将结点(key为节点键值)插入到二叉树中
        void insert(T key);

        // 删除结点(key为节点键值)
        void remove(T key);

        // 销毁二叉树
        void destroy();

        // 打印二叉树
        void print();
    private:
        // 前序遍历"二叉树"
        void preOrder(BSTNode<T>* pNode) const;
        // 中序遍历"二叉树"
        void inOrder(BSTNode<T>* pNode) const;
        // 后序遍历"二叉树"
        void postOrder(BSTNode<T>* pNode) const;

        // (递归实现)查找"二叉树x"中键值为key的节点
        BSTNode<T>* search(BSTNode<T>* x, T key) const;
        // (非递归实现)查找"二叉树x"中键值为key的节点
        BSTNode<T>* iterativeSearch(BSTNode<T>* x, T key) const;

        // 查找最小结点：返回pNode为根结点的二叉树的最小结点。 递归实现
        BSTNode<T>* findMin(BSTNode<T>* pNode);
        // 查找最大结点：返回pNode为根结点的二叉树的最大结点。 递归实现
        BSTNode<T>* findMax(BSTNode<T>* pNode);

        // 将结点(z)插入到二叉树(pNode)中
        void insert(BSTNode<T>* &pNode, BSTNode<T>* z);

        // 删除二叉树(pNode)中的结点(z)，并返回被删除的结点
        BSTNode<T>* remove(BSTNode<T>* &pNode, BSTNode<T> *z);

        // 销毁二叉树
        void destroy(BSTNode<T>* &pNode);

        // 打印二叉树
        void print(BSTNode<T>* pNode, T key, int direction);


       //add by lizulin 
	   //当x找到时返回真，否则返回假 
        bool contains(const T &x) const
        {
            return contains(x,pRoot);
        }
	    // 通过遍历的方法查找x是否在树（或子树）t中
	    bool contains(const T &x, BSTNode<T>* t) const
	    {
		    if (t == NULL) // 遍历中未找到元素的中止条件
			    return false;
		    else if (x < t->key)
			    return contains(x, t->left);
		    else if (t->key < x)
			    return contains(x, t->right);
		    else // 如果 x 不大于 也 不小于t所指的节点中的元素，则x==t->element
			    return true;
        }
        //判断树是否为空
        bool isEmpty() const
        {
            return pRoot== NULL;
        }

     //复制子树
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


//前序遍历"二叉树"
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


//中序遍历"二叉树"
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


//后序遍历"二叉树"
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


//(递归实现)查找"二叉树x"中键值为key的节点
//查找过程： 
//    1、若待查找的元素值和根节点相同，则返回根节点。 
//    2、若待查找的元素值小于根节点的元素值，则递归从根节点的左子树中查找。 
//    3、若待查找的元素值大于根节点的元素值，则递归从根节点的右子树中查找。
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


//(非递归实现)查找"二叉树x"中键值为key的节点
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


//查找最小结点：返回pNode为根结点的二叉树的最小结点。
//获取最小元素的节点： 
//    1、如果根节点无左子树，则返回根节点。 
//    2、依次查询跟节点的左子树节点，返回左子树的最后一个左节点。
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
 

//查找最大结点：返回pNode为根结点的二叉树的最大结点。
//获取最大元素的节点： 
//    1、如果根节点无右子树，则返回根节点。 
//    2、依次查询根节点的右子树节点，返回右子树的最后一个右节点。
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


//找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
template <class T>
BSTNode<T>* BSTree<T>::successor(BSTNode<T> *x)
{
    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return findMin(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->right))
    {
        x = y;
        y = y->parent;
    }

    return y;
}
 

//找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
template <class T>
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T> *x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return findMax(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    BSTNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

//将结点插入到二叉树中参数说明：pNode 二叉树的根结点  z 插入的结点
//插入过程： 
//    1、若当前二叉树为空，则插入的元素为根节点。 
//    2、若插入的元素值小于根节点值，则递归从根节点的左子树中找到可插入位置。 
//    3、若插入的元素值大于根节点值，则递归从根节点的右子树中找到可插入位置。
//
template <class T>
void BSTree<T>::insert(BSTNode<T>* &pNode, BSTNode<T>* z)
{
    BSTNode<T> *y = NULL;
    BSTNode<T> *x = pNode;

    // 查找z的插入位置
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


// 将结点(key为节点键值)插入到二叉树中  参数说明：    pNode 二叉树的根结点    key 插入结点的键值
template <class T>
void BSTree<T>::insert(T key)
{
    BSTNode<T> *z=NULL;

    // 如果新建结点失败，则返回。
    if ((z=new BSTNode<T>(key,NULL,NULL,NULL)) == NULL)
        return ;

    insert(pRoot, z);
}


// 删除结点(z)，并返回被删除的结点参数说明：    pNode 二叉树的根结点   z 删除的结点
//删除过程： 
//    1、待删除节点Z为叶子节点，则直接删除该节点。修改父节点的指针。 
//    2、待删除节点Z为单支节点（只有左子树或者右子树），让Z的子树与Z的父节点相连，删除节点Z。 
//    3、待删除节点Z左右子树都不为空。 
//    方法一：找到Z的后继节点y，因为y一定没有左子树，所以可以直接删除y，并让y的父亲节点成为y的右子树的父亲节点。用y替换Z。 
//    方法二：找到Z的前驱节点x，x一定没有右子树，所以可以直接删除x，并让x的父亲节点成为x的左子树的父亲节点。用x替换Z。
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

//删除结点(z)，并返回被删除的结点参数说明：  pNode 二叉树的根结点   z 删除的结点
template <class T>
void BSTree<T>::remove(T key)
{
    BSTNode<T> *z, *node; 

    if ((z = search(pRoot, key)) != NULL)
        if ( (node = remove(pRoot, z)) != NULL)
            delete node;
}

//销毁二叉树
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


//打印"二叉查找树"
//key        -- 节点的键值 
//direction  --  0，表示该节点是根节点;-1，表示该节点是它的父结点的左孩子;1，表示该节点是它的父结点的右孩子。


template <class T>
void BSTree<T>::print(BSTNode<T>* pNode, T key, int direction)
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
void BSTree<T>::print()
{
    if (pRoot != NULL)
        print(pRoot, pRoot->key, 0);
}


#ifdef UNIT_TEST
int Test_BinarySearch();
#endif


#endif




