#include "List.h"






#include <iostream>


using namespace std;


#ifdef UNIT_TEST

int Test_List()
{
#if 1
    SeqList<int> test(15);
    int array[15]={2,5,8,1,9,9,7,6,4,3,2,9,7,7,9};
    for(int i=0;i<15;i++){
        test.Insert(array[i],0);
    }
    test.Insert(1,0);
    cout<<(test.Find(0)?"can't be found ":"Be found ")<< 0 << endl<<endl;
    test.Remove(7);
    test.Print();
    test.Remove(9);
    test.Print();
    test.Remove(0);
    test.Print();
    return 0;
#endif



#if 0
    SingleList<int> slist;
    for(int i=0;i<20;i++)
    {
        slist.Insert(i*3,i);
    }
    for(int i=0;i<5;i++)
    {
        slist.Insert(3,i*3);
    }
    cout<<"the Length of the list is "<<slist.Length()<<endl;
    slist.Print();

    slist.Remove(5);
    cout<<"the Length of the list is "<<slist.Length()<<endl;
    slist.Print();

    slist.RemoveAll(3);
    cout<<"the Length of the list is "<<slist.Length()<<endl;
    slist.Print();

    cout<<"The third element is "<<slist.Get(3)<<endl;

    cout<<*slist.Find(18,1)<<endl;

    slist.Find(100);

    slist.MakeEmpty();
    cout<<"the Length of the list is "<<slist.Length()<<endl;
    slist.Print();
#endif

#if 0

    DoublyList<int> dlist;
    for(int i=0;i<20;i++){
        dlist.Insert(i*3,i);
    }
    cout<<"the Length of the list is "<<dlist.Length()<<endl;
    dlist.Print();
    for(int i=0;i<5;i++){
        dlist.Insert(3,i*3);
    }
    cout<<"the Length of the list is "<<dlist.Length()<<endl;
    dlist.Print();
    dlist.Remove(5);
    cout<<"the Length of the list is "<<dlist.Length()<<endl;
    dlist.Print();
    cout<<dlist.FindData(54)->GetData()<<endl;
    cout<<"The third element is "<<dlist.Get(3)<<endl;
    dlist.MakeEmpty();
    cout<<"the Length of the list is "<<dlist.Length()<<endl;
    dlist.Print();
    return 0;
#endif




    CircularList<int> clist;
    for(int i=0;i<20;i++){
        clist.Insert(i*3,i);
    }
    cout<<"the Length of the list is "<<clist.Length()<<endl;
    clist.Print();
    for(int i=0;i<5;i++){
        clist.Insert(3,i*3);
    }
    cout<<"the Length of the list is "<<clist.Length()<<endl;
    clist.Print();

    clist.Remove(5);
    cout<<"the Length of the list is "<<clist.Length()<<endl;
    clist.Print();

    clist.RemoveAll(3);
    cout<<"the Length of the list is "<<clist.Length()<<endl;
    clist.Print();

    cout<<"The third element is "<<clist.Get(3)<<endl;

    clist.MakeEmpty();
    cout<<"the Length of the list is "<<clist.Length()<<endl;
    clist.Print();


    return 0;

}

#endif

