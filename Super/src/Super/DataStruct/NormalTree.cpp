#include "NormalTree.h"

#include <iostream>
using namespace std;


#ifdef UNIT_TEST


int Test_NormalTree()
{

    NormalTree<int>* pTree = new NormalTree<int>(1);
    //NormalTree<int>* pTree = new NormalTree<int>;
    pTree->Insert( 1, 2 );
    pTree->Insert( 1, 3 );
    pTree->Insert( 1, 4 );
    pTree->Insert( 1, 5 );
    pTree->Insert( 1, 6 );
    pTree->Insert( 1, 7 );
    pTree->Insert( 4, 8 );
    pTree->Insert( 5, 9 );
    pTree->Insert( 5, 10 );
    pTree->Insert( 6, 11 );
    pTree->Insert( 6, 12 );
    pTree->Insert( 6, 13 );
    pTree->Insert( 10, 14 );
    pTree->Insert( 10, 15 );

    cout << "前序遍历：" << endl;
    pTree->Preorder( pTree->pRoot );
    cout << endl;

    cout << "中序遍历：" << endl;
    pTree->Inorder( pTree->pRoot );
    cout << endl;

    cout << "后序遍历：" << endl;
    pTree->postorder( pTree->pRoot );
    cout << endl;
    delete pTree;
    pTree = NULL;


    getchar(); //system("pause");不跨平台
    return 1;

}


#endif

