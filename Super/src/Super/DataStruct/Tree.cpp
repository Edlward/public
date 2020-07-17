#include "Tree.h"





#ifdef UNIT_TEST
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


    return 0;
}

#endif



