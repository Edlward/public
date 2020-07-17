#ifndef _NormalTree_H_
#define _NormalTree_H_

//普通树，非二叉树，一个节点可以有任意多个子节点


//普通树的实现，兄弟孩子表示法，插入数据，查询数据，前序，中序，后序遍历
//typedef int ElementDataType;
//typedef T ElementDataType;
#include <iostream>
using namespace std;


template<typename T> class NormalTreeNode;
template<typename T> class NormalTree;

template<typename T>
class NormalTreeNode
{
public:
    //typedef NormalTreeNode<T>*  pNormalTreeNode;
    friend class NormalTree<T>;
    //typedef T ElementDataType;
    T data;
    NormalTreeNode<T>*  pFirstChild;
    NormalTreeNode<T>* pNextBrother;
    NormalTreeNode()
    {
        pFirstChild = NULL;
        pNextBrother = NULL;
    }
    NormalTreeNode(T Value )
    {
        data = Value;
        pFirstChild = NULL;
        pNextBrother = NULL;
    }
}; 

//typedef struct NormalTreeNode* NormalTreeNode*;

//普通树的实现
template<typename T>
class NormalTree
{
public:
    NormalTreeNode<T>* pRoot;
public:
    typedef NormalTreeNode<T >*  pNormalTreeNode;
    //NormalTree::NormalTree()//:pRoot(NULL)
    //{
    //    //pRoot = NULL;
    //}
    NormalTree::NormalTree(T Value):pRoot(NULL)   //两种类型构造函数不能同时声明
    {
        pRoot = new NormalTreeNode<T>(Value);
        //if (pRoot==NULL )
        //    return;
    }
    NormalTree::~NormalTree()
    {
        //if (pRoot == NULL )
        //    return;
        FreeMemory( pRoot );
    }
    //  释放内存
    void NormalTree::FreeMemory( pNormalTreeNode  pNode )
    {
        if ( pNode == NULL )
            return;

        if ( pNode->pFirstChild != NULL )
            FreeMemory( pNode->pFirstChild );

        if ( pNode->pNextBrother != NULL )
            FreeMemory( pNode->pNextBrother );

        delete pNode;
        pNode = NULL;
    }

    //parentValue：该节点的父结点；Value：该节点的值
    void NormalTree::Insert( T parentValue, T Value )
    {
        if ( pRoot == NULL )
            return;

        pNormalTreeNode  pFindNode = Search( pRoot, parentValue );
        if ( pFindNode == NULL )
            return;

        if ( pFindNode->pFirstChild == NULL )
        {
            pFindNode->pFirstChild = new NormalTreeNode<T>( Value );
            return;
        }
        else
        {
            InsertBrother( pFindNode->pFirstChild, Value );
            return;
        }
    }

    void NormalTree::InsertBrother( pNormalTreeNode  pBrotherNode, T Value )
    {
        if ( pBrotherNode->pNextBrother != NULL )
        {
            InsertBrother( pBrotherNode->pNextBrother, Value );
        }
        else
        {
            pBrotherNode->pNextBrother = new NormalTreeNode<T>( Value );
            return;
        }
    }

    pNormalTreeNode  NormalTree::Search(pNormalTreeNode  pNode, T Value )
    {
        if ( pNode == NULL )
            return NULL;

        if ( pNode->data == Value )
            return pNode;

        if ( pNode->pFirstChild == NULL && pNode->pNextBrother == NULL )
            return NULL;
        else
        {
            if ( pNode->pFirstChild != NULL )
            {
                pNormalTreeNode  pNodeTemp = Search( pNode->pFirstChild, Value );
                if ( pNodeTemp != NULL )
                {
                    return pNodeTemp;
                }
                else
                {
                    return Search( pNode->pNextBrother, Value );
                }
            }
            else
            {
                return Search( pNode->pNextBrother, Value );
            }
        }
    }

    //递归遍历:实现简单

    //前序遍历 （先根节点，后左孩子节点，再右孩子结点） 
    void NormalTree::Preorder( pNormalTreeNode  pNode )
    {
        if (pNode == NULL)
            return;
        cout << " " << pNode->data << " ";

        Preorder( pNode->pFirstChild );
        Preorder( pNode->pNextBrother );
    }

    //中序遍历 （先左孩子结点，后根节点，再右孩子结点）
    void Inorder(pNormalTreeNode pNode)
    {
        if ( pNode == NULL )
            return;

        Inorder( pNode->pFirstChild );
        cout << " " << pNode->data << " ";
        Inorder( pNode->pNextBrother );
    }
    //后序遍历 （先做孩子节点，后右孩子结点，再根节点）
    void NormalTree::postorder( pNormalTreeNode  pNode )
    {
        if ( pNode == NULL )
            return;

        postorder( pNode->pFirstChild );
        postorder( pNode->pNextBrother );
        cout << " " << pNode->data << " ";
    }
    //非递归版本： 暂不实现
};


#ifdef UNIT_TEST

int Test_NormalTree();
#endif


#endif







