#include "SmartPointer.h"
#include "smart_ptr.h"

#include <iostream>


#include <memory>
class TestClass
{
public:
	TestClass()
	{
		NumRef++;
		std::cout<<"TestClass()"<<" SumObject="<<NumRef<<std::endl;
	}
	~TestClass()
	{
		NumRef--;
		std::cout<<"~TestClass()"<<" SumObject="<<NumRef<<std::endl;
	}
	static int NumRef;
};
int TestClass::NumRef=0;



//#include <memory>


#ifdef UNIT_TEST

void Test_SmartPtr()
{
	//CMyAutoPtr<Test> p=new Test[10];  //error
    //CMyAutoPtr<TestClass> p=new TestClass;

    {
        super::auto_ptr<TestClass> p1(new TestClass);
        super::shared_ptr<TestClass> p2(new TestClass);
        super::unique_ptr<TestClass> p3(new TestClass);
    }



    //super::shared_ptr<std::string*> p(new std::string);
    //super::unique_ptr<std::string*> p2(new std::string);
    //std::weak_ptr<std::string> p(new std::string);


    super::shared_ptr<int> p1;
    //super::shared_ptr<int> p2 (nullptr);
    super::shared_ptr<int> p3 (new int);
    super::shared_ptr<int> p4 (new int, std::default_delete<int>());
    super::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
    super::shared_ptr<int> p6 (p5);
    super::shared_ptr<int> p7 (std::move(p6));
    //super::shared_ptr<int> p8 (std::unique_ptr<int>(new int));


    super::unique_ptr<int> up1(new int());    //okay,直接初始化  
    //super::unique_ptr<int> up2 = new int();   //error! 构造函数是explicit  
    //super::unique_ptr<int> up3(up1);          //error! 不允许拷贝  
    super::unique_ptr<int> pInt(new int(5));
    super::unique_ptr<int> pInt2 = std::move(pInt);    // 转移所有权
    //cout << *pInt << endl; // 出错，pInt为空
    cout << *pInt2 << endl;
    super::unique_ptr<int> pInt3(std::move(pInt2));

}

#endif

