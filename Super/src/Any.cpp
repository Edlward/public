#include "Any.h"



#ifdef UNIT_TEST


#include <string>
int Test_Any()
{


#if 0
    Any n;
    std::cout << n.IsNull() << std::endl;
    std::string str = "Hello";
    //赋值操作，其实分两步进行
    //先调用any的移动构造函数把str转换为any类型
    //再调用any的赋值构造函数完成真正的赋值
    n = str;
    try
    {
        std::cout << n.Cast<int>() << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << '\n';
    }
    std::cout << std::endl;
    std::cout << "-----------------------------" << std::endl;
    //直接初始化只调用移动构造函数,比采用赋值构造少了很多步骤，所以直接初始化效率更高
    Any n1(1);
    std::cout << n1.Is<int>() << '\n';
    std::cout << std::endl;
    std::cout << "-----------------------------" << std::endl;
    Any n2(std::move(n1));
    std::cout << n2.Cast<int>() << '\n';
    std::cout << std::endl;
    system("pause");
#endif


    return 0;
}

#endif
