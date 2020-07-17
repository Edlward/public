#include "BinarySearchTree.h"

 

 
#include <iostream>
using namespace std;


#ifdef UNIT_TEST

int Test_BinarySearch()
{
    static int arr[]= {1,5,4,3,2,6};
   #define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )
    int i, ilen;
    BSTree<int>* tree=new BSTree<int>();

    cout << "== 依次添加: ";
    ilen = TBL_SIZE(arr);
    for(i=0; i<ilen; i++) 
    {
        cout << arr[i] <<" ";
        tree->insert(arr[i]);
    }

    cout << "\n== 前序遍历: ";
    tree->preOrder();

    cout << "\n== 中序遍历: ";
    tree->inOrder();

    cout << "\n== 后序遍历: ";
    tree->postOrder();
    cout << endl;

    cout << "== 最小值: " << tree->findMin() << endl;
    cout << "== 最大值: " << tree->findMax() << endl;
    cout << "== 树的详细信息: " << endl;
    tree->print();

    cout << "\n== 删除根节点: " << arr[3];
    tree->remove(arr[3]);

    cout << "\n== 中序遍历: ";
    tree->inOrder();
    cout << endl;

    // 销毁二叉树
    tree->destroy();

    return 0;
}

#endif

