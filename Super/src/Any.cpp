#include "Any.h"



#ifdef UNIT_TEST


#include <string>
int Test_Any()
{


#if 0
    Any n;
    std::cout << n.IsNull() << std::endl;
    std::string str = "Hello";
    //��ֵ��������ʵ����������
    //�ȵ���any���ƶ����캯����strת��Ϊany����
    //�ٵ���any�ĸ�ֵ���캯����������ĸ�ֵ
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
    //ֱ�ӳ�ʼ��ֻ�����ƶ����캯��,�Ȳ��ø�ֵ�������˺ܶಽ�裬����ֱ�ӳ�ʼ��Ч�ʸ���
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
