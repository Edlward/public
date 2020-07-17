#include "TestTmp2.h"

#include <stdio.h>
#include <stdint.h>

//#include "Super/Basic/global.h"



static uint64_t getNextCycle(uint64_t nCurr,uint64_t nMax)
{
    return (nCurr+1)%nMax;
}

//上一个回环计数器  在(Max-1)-0之间循环
static unsigned int getPrevCycle(unsigned int nCurr,unsigned int nMax)
{
    //return (nCurr-1)%nMax;  //error 
    return (nCurr+nMax-1)%nMax;  //ok

    //if(nCurr>nMax)  //正确使用不会出现
    //{
    //    return (nCurr-1)%nMax;
    //}
    //if (nCurr>=1)  //170xhz
    //{
    //    return nCurr-1;
    //}
    //return nMax-1;
}

 
//下N个回环计数,无符号 在0-(Max-1)之间回环
static unsigned int getNextNCycle(unsigned int nCurr,unsigned int nOffset,unsigned int nMax)
{
    return (nCurr+nOffset)%nMax;
}

 
//上N个回环计数,无符号  在(Max-1)-0之间回环
static unsigned int getPrevNCycle(unsigned int nCurr,unsigned int offset,unsigned int nMax)
{
    //return (nCurr-offset)%nMax;  //error
    offset%=nMax;
    return (nMax+nCurr-offset)%nMax;  //error

    if(nCurr>nMax)  //正确使用不会出现
    {
        return (nCurr-offset)%nMax;
    }
    if (offset<=nCurr)
    {
        return nCurr-offset;
    }
    return nCurr+nMax-offset;
}

//未完待续
static int getCycleNumber(int nCurr,int nOffset,int nMin,int nMax)
{
    int rMax=nMax-nMin;
    nOffset%=rMax;
    nCurr-=nMin;
    nCurr%=rMax;
    return nMin+((rMax+nCurr+nOffset)%(rMax));
}


void test_cycle()
{
    for (unsigned int n=0;n<1;n++)
    {
        unsigned int tmp=0;
        for (int offset=-10;offset<10;offset++)
        {
            tmp=getPrevCycle(tmp,5);
            printf("curr:%u prev1 is:%u\n",n,tmp);
        }

        for (int offset=-10;offset<10;offset++)
        {
            unsigned int index=getPrevNCycle(n,offset,5);
            printf("curr:%u prev:%d is:%u\n",n,offset,index);
        }
        for (int offset=-10;offset<10;offset++)
        {
            unsigned int index=getNextNCycle(n,offset,5);
            printf("curr:%u next:%d is:%u\n",n,offset,index);
        }
        //for (int offset=-10;offset<10;offset++)
        //{
        //    unsigned int index=getCycleNumber(n,offset,5);
        //    printf("curr:%u cyc:%d is:%u\n",n,offset,index);
        //}

        for (int offset=-10;offset<10;offset++)
        {
             int index=getCycleNumber(n,offset,2,9);
            printf("curr:%d cyc:%d is:%d\n",n,offset,index);
        }
    }
}






int Test_Tmp2()
{
    test_cycle();
    return 0;
}