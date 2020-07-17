#ifndef _Heap_H_
#define _Heap_H_

#include <iostream>
using namespace std;

#ifdef __cplusplus


//#include "BinaryTree.h"
//#include "MinHeap.h"

#include "Tree.h"
#include "Heap.h"

//编译不通过，暂时屏蔽

#if 0

template<typename Type> void Huffman(Type *elements, int n, BinaryTree<Type> &tree){
    BinaryTree<Type> first, second;
    BinaryTree<Type> node[20];
    for (int i=0; i<n; i++){
        node[i].m_proot = new BinTreeNode<Type>(elements[i]);
    }
    MinHeap<BinaryTree<Type> > heap(node, n);
    for (int i=0; i<n-1; i++){
        heap.DeleteMin(first);
        heap.DeleteMin(second);

        //using the first and the second minimize element create new tree
        if (first.m_proot->GetData() == second.m_proot->GetData()){
            tree = *(new BinaryTree<Type>(second, first));
        }
        else {
            tree = *(new BinaryTree<Type>(first, second));
        }
        heap.Insert(tree);
    }
}
#endif


#endif

#ifdef UNIT_TEST
int Test_Huffman();
#endif

#endif