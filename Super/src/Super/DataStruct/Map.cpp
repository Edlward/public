#include "Map.h"


int Test_Map()
{
    int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
    RBTree<int, int> rt;
    for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
        rt.InsertUnique(pair<int, int>(a[i], i));
    rt.InOrder();
    if (rt.IsRBTree())
        cout << "是红黑树" << endl;
    else
        cout << "不是红黑树" << endl;

    RBTree<int, int>::Iterator it = rt.Begin();
    while (it != rt.End())
    {
        cout << (*it).first << " ";
        ++it;
    }
    cout << endl;
    return 0;
}




