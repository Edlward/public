#include "Tree.h"






int Test_Tree()
{
#if 0
    BinaryTree<int> binTree(-1);
    //	int init[10]={3,6,0,2,8,4,9,1,5,7};
    int init[30]={17,6,22,29,14,0,21,13,27,18,2,28,8
        ,26,3,12,20,4,9,23,15,1,11,5,19,24,16,7,10,25};
    for(int i=0;i<30;i++){
        binTree.Insert(init[i]);
    }
    //cin>>tree;
    cout<<binTree<<endl;

    cout<<binTree.GetParent(binTree.Find(20))->GetData()<<endl;
    cout<<binTree.Find(15)->GetRight()->GetData()<<endl;

    cout<<"size="<<binTree.Size()<<endl;
    cout<<"height="<<binTree.Height()<<endl;

    binTree.InOrder();
    cout<<endl<<endl;
    binTree.PreOrder();
    cout<<endl<<endl;
    binTree.PostOrder();
    cout<<endl<<endl;


    BinaryTree<int> tree2=binTree;
    cout<<tree2<<endl;

    cout<<tree2.GetParent(tree2.Find(20))->GetData()<<endl;
    cout<<tree2.Find(15)->GetRight()->GetData()<<endl;

    cout<<(binTree==tree2)<<endl;
    return 0;
#endif




#if 0
    ThreadTree<int> ThreadTree;
    ThreadInorderIterator<int> threadtree(ThreadTree);
    int init[10]={3,6,0,2,8,4,9,1,5,7};
    for(int i=0;i<10;){
        threadtree.InsertLeft(new ThreadNode<int>(init[i++]));
        threadtree.InsertRight(new ThreadNode<int>(init[i++]));
    }
    threadtree.Print();
    cout<<endl<<endl;
    threadtree.InOrder();
    return 0;
#endif



    NormalTree<int>* pTree = new NormalTree<int>(1);
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
    system("pause");
    return 1;


}

