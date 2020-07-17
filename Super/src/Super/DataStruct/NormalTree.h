#ifndef _NormalTree_H_
#define _NormalTree_H_

//��ͨ�����Ƕ�������һ���ڵ�������������ӽڵ�


//��ͨ����ʵ�֣��ֵܺ��ӱ�ʾ�����������ݣ���ѯ���ݣ�ǰ�����򣬺������
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

//��ͨ����ʵ��
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
    NormalTree::NormalTree(T Value):pRoot(NULL)   //�������͹��캯������ͬʱ����
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
    //  �ͷ��ڴ�
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

    //parentValue���ýڵ�ĸ���㣻Value���ýڵ��ֵ
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

    //�ݹ����:ʵ�ּ�

    //ǰ����� ���ȸ��ڵ㣬�����ӽڵ㣬���Һ��ӽ�㣩 
    void NormalTree::Preorder( pNormalTreeNode  pNode )
    {
        if (pNode == NULL)
            return;
        cout << " " << pNode->data << " ";

        Preorder( pNode->pFirstChild );
        Preorder( pNode->pNextBrother );
    }

    //������� �������ӽ�㣬����ڵ㣬���Һ��ӽ�㣩
    void Inorder(pNormalTreeNode pNode)
    {
        if ( pNode == NULL )
            return;

        Inorder( pNode->pFirstChild );
        cout << " " << pNode->data << " ";
        Inorder( pNode->pNextBrother );
    }
    //������� ���������ӽڵ㣬���Һ��ӽ�㣬�ٸ��ڵ㣩
    void NormalTree::postorder( pNormalTreeNode  pNode )
    {
        if ( pNode == NULL )
            return;

        postorder( pNode->pFirstChild );
        postorder( pNode->pNextBrother );
        cout << " " << pNode->data << " ";
    }
    //�ǵݹ�汾�� �ݲ�ʵ��
};


#ifdef UNIT_TEST

int Test_NormalTree();
#endif


#endif







