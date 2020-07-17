#include "B_tree.h"


#ifdef UNIT_TEST

int Test_BinaryTree()
{	
    int preList[] = {1, 2 , 0, 22, 0, 0, 3, 0, 33, 0, 0};
    //int preList[] = {1, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0};
    BinaryTree<int> *tree = new BinaryTree<int>(preList, 12, 0, 0);
    tree->PreOrderNonRescursion();
    tree->InOrderNonRescursion();
    tree->PostOrderNonRescursion();
    delete tree;

    return 0;
}

#endif
