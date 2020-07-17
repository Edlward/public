#include "StaticList.h"


#ifdef UNIT_TEST

int Test_StaticList()
{
        StaticList<int> TestList;
        TestList.Insert(12);
        TestList.Insert(12);
        TestList.Insert(34);

        TestList.Insert(23);
        TestList.Insert(12);

        TestList.Insert(99,4);
        TestList.Show();
        int m = 0;
        TestList.Delete(m,7);
        cout << "____________" << m << "_______________\n";
        TestList.Show();
        return 0;
}

#endif
