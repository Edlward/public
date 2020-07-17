#ifndef _NormalTree_H_
#define _NormalTree_H_

//普通二叉树，递归遍历，非递归遍历实现，逐步补全


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


//二叉树类
template <class  T>
class BinaryTree
{
private:
    BinaryNode<T> *pRoot;  //根结点
public:
    BinaryTree();	//构造空结点
    BinaryTree(const T preList[], const int size, int index, const T end);	//先序构造
    ~BinaryTree();

    //数组初始化构建二叉树
    BinaryNode<T>* CreateBiTree(const T preList[], const int size, int &index, const T end);	    //先序创建二叉树
    void ClearBiTree(BinaryNode<T> *root);          //销毁二叉树

    //插入法构建二叉树
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
    void preOrder() //前序遍历递归外部调用
    {
        preOrder_(pRoot);
    }
    void inOrder()    //中序遍历递归外部调用
    {
        inOrder_(pRoot);
    }
    void posOrder() //后序遍历递归外部调用
    {
        posOrder_(pRoot);
    }
private:
    //前序遍历递归内部实现  次序为：根结点——左结点——右结点。
    void preOrder_(BinaryNode<T>*  pNode)
    {
        if(pNode)
        {
            std::cout<<pNode->data;
            preOrder_(pNode->leftChild);
            preOrder_(pNode->rightChild);
        }
    }
    //中序遍历递归内部实现 次序为：左结点——根结点——右结点
    void inOrder_(BinaryNode<T>*  pNode)
    {
        if(pNode)
        {
            inOrder(pNode->leftChild);
            std::cout<<pNode->data;
            inOrder(pNode->rightChild);
        }
    }
    //后序遍历递归内部实现 次序为：左结点——右结点——根结点。
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
    void PreOrderNonRescursion();	    //先序遍历，非递归
    void InOrderNonRescursion();	//中序遍历，非递归
    void PostOrderNonRescursion();	//后序遍历，非递归

    //层次遍历(广度优先遍历)（剑指Offer上有涉及）
    void DfsTravel();
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
//非递归的实现：
//前序遍历的访问顺序为：根，左和右。对于任一结点，其可看做根结点，因此可直接访问。访问完后，若其左孩子不为空，则按照相同规则访问它的左子树；访问完左子树，再访问它的右子树。处理过程如下：
//对于任一结点P：
//step 1:访问结点p，并将结点p入栈。
//step 2:判断结点p的左孩子是否为空。若为空，则取栈顶结点并出栈，将栈顶元素的右孩子设为当前的结点p，循环至step 1。
//若不为空，则将p的左孩子设为当前结点p。
//step 3:直到p为NULL，并且栈为空，则遍历结束。

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

//2 中序遍历
//非递归法：
//根据中序遍历的顺序，对于任一结点，先访问其左孩子。而左孩子又可以看做一个根结点，然后继续访问左孩子，直到遇到的左孩子结点为空，则停止访问。然后访问右孩子。
//处理过程如下：
//对于任一结点p：
//step 1:若其左孩子不为空，则将p入栈，并将p的左孩子置为当前的p。然后对当前结点p再进行相同的处理。
//step 2:若其左孩子为空，则取栈顶元素并进行出栈操作，访问该栈顶结点，然后将栈顶结点的右孩子置为当前的P结点。
//step 3:直到p为NULL并且栈为空则遍历结束。

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


//后序遍历
//非递归遍历：要保证根结点在左孩子和右孩子访问之后才能访问。因此对于任一结点p，先将其入栈。
//    若p不存在左孩子和右孩子，则可以直接访问它。或者p存在左孩子或者右孩子，但是左孩子和右孩子都已经被访问过了，则可以直接访问该结点。
//    若非上述两种情况，则将右孩子和左孩子依次入栈。这样可以保证每次取栈顶元素时，左孩子在右孩子前面被访问，根结点在左孩子和右孩子访问之后被访问。

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

    //写法2 可以参考
#if 0
    stack<BinaryTreeNode*> s;
    BinaryTreeNode *cur;
    BinaryTreeNode *pre=NULL;
    s.push(pRoot);//根结点入栈
    while(!s.empty())
    {
        cur=s.top();
        if((cur->left==NULL&&cur->right==NULL)||(pre!=NULL&&(pre==cur->left||pre==cur->right)))
        {
            //左孩子和右孩子同时为空，或者当前结点的左孩子或右孩子已经遍历过了
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


//四、层次遍历（剑指Offer上有涉及）
//    题目：从上到下打印二叉树的每个节点，同层的节点按照从左向右打印。
//    解析：即分层遍历二叉树。利用广度优先遍历的思想，遍历树或者有向图，都可在队列中完成。
//step1:把起始节点放入队列。
//step2:每次从队头取出节点，遍历（输出）。接下来，把从该节点能到达的节点（子树或者其他）都依次放入队列。
//重复step2，直到队列中的节点为空。
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


