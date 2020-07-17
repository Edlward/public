#include "Test_Cast.h"

#include <stdio.h>        //for printf()
#include <typeinfo>  //

#ifdef UNIT_TEST

#define PRINT_OBJ_ADDR(T)  printf("Type:%s Addr:%p\n",typeid(T).name(),this);
#define PRINT_STATIC_CAST_toTYPE(T)  printf("Type:%s Cast_Addr:%p\n",typeid(T).name(),(T*)this);

class Base
{
public:
    Base()
    {
        PRINT_OBJ_ADDR(Base)
        PRINT_FUN_NAME
    }
    ~Base()
    {
        PRINT_FUN_NAME
    }
    int a;

    virtual void testFunA()
    {
        PRINT_FUN_NAME
    }
    void testFunB()
    {
        PRINT_FUN_NAME
    }
};

class Base2
{
public:
    Base2()
    {
        PRINT_OBJ_ADDR(Base2)
            PRINT_FUN_NAME
    }
    ~Base2()
    {
        PRINT_FUN_NAME
    }
    int a;

    virtual void testFunA()
    {
        PRINT_FUN_NAME
    }
    void testFunB()
    {
        PRINT_FUN_NAME
    }
};

class CTest :public Base,public Base2
{
public:
     CTest()
     {
         PRINT_FUN_NAME
         PRINT_OBJ_ADDR(CTest)
         //PRINT_STATIC_CAST_toTYPE(CTest)
     }
     ~CTest()
     {
        PRINT_FUN_NAME
     };
};


int Test_Cast()
{
    CTest c1;
    CTest* pC2=new CTest;
    //Base2 b1;


    Base2* pB1=&c1;
    printf("pBase2:%p by implicit\n",&c1);
    printf("pBase2:%p by static_cast\n",static_cast<Base2*>(&c1));
    printf("pBase2:%p by dynamic_cast\n",dynamic_cast<Base2*>(&c1));


    printf("pBase2:%p by implicit\n",pC2);
    printf("pBase2:%p by static_cast\n",static_cast<Base2*>(pC2));
    printf("pBase2:%p by dynamic_cast\n",dynamic_cast<Base2*>(pC2));

    return 0;
}

#endif

