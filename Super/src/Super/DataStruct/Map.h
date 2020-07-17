
#ifndef _Map_H_
#define _Map_H_

//#pragma once

#include <utility>  
#include<iostream>
using namespace std;



enum COLOR
{
    RED,
    BLACK 
};

template<class K, class V>
struct RBTreeNode
{
    RBTreeNode<K, V>* pParent;
    RBTreeNode<K, V>* pLeft;
    RBTreeNode<K, V>* pRight;
    pair<K, V> m_Value;
    COLOR _color;
    RBTreeNode(const K& key = K(), const V& value = V(), COLOR color = RED)
        : pLeft(NULL)
        , pRight(NULL)
        , pParent(NULL)
        , m_Value(key, value)
        , _color(color)
    {

    }
};


template<class K, class V>
class RBTreeiterator
{
    typedef RBTreeNode<K, V> Node;
    typedef Node* PNode;
    typedef RBTreeiterator<K, V> Self;
public:
    RBTreeiterator(PNode pNode = NULL)
        : pNode(pNode)
    {}
    RBTreeiterator(const Self& s)
        : pNode(s.pNode)
    {}
    pair<K, V>& operator*()
    {
        return pNode->m_Value;
    }
    pair<K, V>* operator->()
    {
        return &(operator*());
    }

    Self& operator++()
    {
        RBTreeItIncrement();
        return *this;
    }

    Self operator++(int)
    {
        Self temp(*this);
        RBTreeItIncrement();
        return temp;
    }

    Self& operator--()
    {
        RBTreeItDecrement();
        return *this;
    }

    Self operator--(int)
    {
        Self temp(*this);
        RBTreeItDecrement();
        return temp;
    }

    bool operator==(const Self& s)
    {
        return pNode == s.pNode;
    }

    bool operator!=(const Self& s)
    {
        return pNode != s.pNode;
    }

private:
    void RBTreeItIncrement()
    {
        if (pNode->pRight)
        {
            pNode = pNode->pRight;
            while (pNode->pLeft)
            {
                pNode = pNode->pLeft;
            }
        }
        else
        {
            PNode pParent = pNode->pParent;
            while (pParent->pRight == pNode)
            {
                pNode = pParent;
                pParent = pNode->pParent;
            }

            // 如果树的根节点没有右孩子的情况且迭代器起始位置在根节点
            if (pNode->pRight != pParent)
            {
                pNode = pParent;
            }
        }
    }

    void RBTreeItDecrement()
    {
        if (pNode->pParent->pParent == pNode && RED == pNode->_color)
        {
            pNode = pNode->pRight;
        }
        else if (pNode->pLeft)
        {
            // 在当前节点左子树中找最大的结点
            pNode = pNode->pLeft;
            while (pNode->pRight)
            {
                pNode = pNode->pRight;
            }
        }
        else
        {
            PNode pParent = pNode;
            while (pParent->pLeft == pNode)
            {
                pNode = pParent;
                pParent = pNode->pParent;
            }

            pNode = pParent;
        }
    }

private:
    PNode pNode;
};


template<class K, class V>
class RBTree{
    //typedef RBTreeiterator<K, V> Iterator;
    typedef RBTreeNode<K, V> Node;
    typedef Node* PNode;
public:
    typedef RBTreeiterator<K, V> Iterator;
public:
    RBTree()
        :pHead(new Node)
    {}
    Iterator Begin()
    {
        return Iterator(pHead->pLeft);
    }
    Iterator End()
    {
        return Iterator(pHead);
    }
    PNode& GetRoot()
    {
        return pHead->pParent;
    }
    pair<Iterator,bool> InsertUnique(const pair<K, V>& value)
    {
        PNode& _pRoot = GetRoot();
        PNode newNode = NULL;
        if (NULL == _pRoot)
        {
            newNode = _pRoot = new Node(value.first, value.second, BLACK);
            _pRoot->pParent = pHead;
        }
        else
        {
            PNode pCur = _pRoot;
            PNode pParent = pCur;
            while (pCur)
            {
                if (pCur->m_Value.first < value.first)
                {
                    pParent = pCur;
                    pCur = pCur->pRight;
                }
                else if (pCur->m_Value.first>value.first)
                {
                    pParent = pCur;
                    pCur = pCur->pLeft;
                }
                else
                {
                    return pair<Iterator,bool>(Iterator(pCur),false);
                }
            }
            newNode = pCur = new Node(value.first, value.second);
            if (value.first < pParent->m_Value.first)
            {
                pParent->pLeft = pCur;
            }
            else
            {
                pParent->pRight = pCur;
            }
            pCur->pParent = pParent;
            while (pParent&& pParent->_color == RED)
            {
                PNode grandParent = pParent->pParent;
                if (pParent == grandParent->pLeft)
                {
                    PNode pUncle = grandParent->pRight;
                    if (pUncle&&pUncle->_color == RED)
                    {
                        pParent->_color = BLACK;
                        pUncle->_color = BLACK;
                        grandParent->_color = RED;
                        grandParent = pCur;
                        pParent = pCur->pParent;
                    }
                    else
                    {
                        if (pCur == pParent->pRight)
                        {
                            rotateL(pParent);
                            swap(pCur, pParent);
                        }
                        grandParent->_color = RED;
                        pParent->_color = BLACK;
                        rotateR(grandParent);
                    }
                }
                else{
                    PNode pUncle = grandParent->pLeft;
                    if (pUncle&&pUncle->_color == RED)
                    {
                        pParent->_color = BLACK;
                        pUncle->_color = BLACK;
                        grandParent->_color = RED;
                        grandParent = pCur;
                        pParent = pCur->pParent;
                    }
                    else 
                    {
                        if (pCur == pParent->pLeft)
                        {
                            rotateR(pParent);
                            swap(pCur, pParent);
                        }
                        grandParent->_color = RED;
                        pParent->_color = BLACK;
                        rotateL(grandParent);
                    }
                }
            }
        }

        _pRoot->_color = BLACK;
        pHead->pLeft = MostLeft();
        pHead->pRight = MostRight();
        return make_pair(Iterator(newNode),true);
    }

    bool Empty()const
    {
        return NULL == GetRoot();
    }

    size_t Size()const
    {
        size_t count = 0;
        Iterator it = Iterator(pHead->pLeft);
        Iterator ed = Iterator(pHead);
        while (it != ed)
        {
            ++count;
            ++it;
        }
        return count;
    }

    void InOrder()
    {
        cout << "InOrder:";
        _InOrder(GetRoot());
        cout << endl;
    }
    bool IsRBTree()
    {
        PNode& _pRoot = GetRoot();
        if (NULL == _pRoot)
        {
            return true;
        }
        if (RED == _pRoot->_color)
        {
            cout << "根节点为红色违反性质2" << endl;
            return false;
        }
        // 统计单条路径中黑色结点的个数
        size_t blackCount = 0;
        PNode pCur = _pRoot;
        while (pCur)
        {
            if (BLACK == pCur->_color)
            {
                ++blackCount;
            }
            pCur = pCur->pLeft;
        }
        size_t pathCount = 0;
        return _IsRBTree(_pRoot, pathCount, blackCount);
    }
private:
    void rotateL(PNode pParent)
    {
        PNode pSubR = pParent->pRight;
        PNode pSubRL = pSubR->pLeft;
        pParent->pRight = pSubRL;
        if (pSubRL)
        {
            pSubRL->pParent = pParent;
        }
        pSubR->pLeft = pParent;
        PNode pPParent = pParent->pParent;
        pParent->pParent = pSubR;
        pSubR->pParent = pPParent;
        if (pHead == pPParent)
        {
            GetRoot() = pSubR;
        }
        else
        {
            if (pPParent->pLeft == pParent)
            {
                pPParent->pLeft = pSubR;
            }
            else
            {
                pPParent->pRight = pSubR;
            }
        }
    }
    void rotateR(PNode pParent)
    {
        PNode pSubL = pParent->pLeft;
        PNode pSubLR = pSubL->pRight;
        pParent->pLeft = pSubLR;
        if (pSubLR)
        {
            pSubLR->pParent = pParent;
        }
        pSubL->pRight = pParent;
        PNode pPParent = pParent->pParent;
        pParent->pParent = pSubL;
        pSubL->pParent = pPParent;
        if (pHead == pPParent)
        {
            GetRoot() = pSubL;
        }
        else
        {
            if (pPParent->pLeft == pParent)
            {
                pPParent->pLeft = pSubL;
            }
            else
            {
                pPParent->pRight = pSubL;
            }
        }
    }
    void _InOrder(PNode pRoot)
    {
        if (pRoot)
        {
            _InOrder(pRoot->pLeft);
            cout << "<" << pRoot->m_Value.first << "," << pRoot->m_Value.second << ">";
            _InOrder(pRoot->pRight);
        }
    }

    bool _IsRBTree(PNode pRoot, size_t n, size_t blackCount)
    {
        if (NULL == pRoot)
        {
            return true;
        }
        if (BLACK == pRoot->_color)
        {
            ++n;
        }
        PNode pParent = pRoot->pParent;
        if (pParent && RED == pRoot->_color && RED == pParent->_color)
        {
            cout << "有连在一起的红色结点违反性质3" << endl;
            return false;
        }
        if (NULL == pRoot->pLeft && NULL == pRoot->pRight){
            if (n != blackCount){
                cout << "路径中黑色结点个数不同违反性质4" << endl;
                return false;
            }
        }
        return _IsRBTree(pRoot->pLeft, n, blackCount) && _IsRBTree(pRoot->pRight, n, blackCount);
    }
    PNode MostLeft()
    {
        PNode pCur = GetRoot();
        if (NULL == pCur)
        {
            return NULL;
        }
        while (pCur->pLeft)
        {
            pCur = pCur->pLeft;
        }
        return pCur;
    }
    PNode MostRight()
    {
        PNode pCur = GetRoot();
        if (NULL == pCur)
        {
            return NULL;
        }
        while (pCur->pRight)
        {
            pCur = pCur->pRight;
        }
        return pCur;
    }
private:
    PNode pHead;
};





//Map.h文件：



//#include"RBTree.h"
#include<string>

template<class K,class V>
class Map{
public:
    typedef pair<K, V> valueType;
    typename typedef RBTree<K, V>::Iterator Iterator;
public:
    Map():mTree()
    {
    }
    pair<Iterator, bool> Insert(const valueType& v)
    {
        return mTree.InsertUnique(v);
    }

    bool Empty()const
    {
        return mTree.Empty();
    }

    size_t Size()const
    {
        return mTree.Size();
    }

    V& operator[](const K& key)
    {
        Iterator ret = mTree.InsertUnique(pair<K, V>(key, V())).first;
        return (*ret).second;
    }

    Iterator Begin()
    {
        return mTree.Begin();
    }

    Iterator End()
    {
        return mTree.End();
    }

private:
    RBTree<K, V> mTree;
};



#ifdef UNIT_TEST
int Test_Map();
#endif

#endif

