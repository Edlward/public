#include "Test_Basic.h"

#include "SuperTime.h"
#include "NetStream.h"
#include "BasicOperation.h"

#include  <mutex>
#include  <thread>

#include "Int24.h"

USING_NAME_SPACE_MAIN

//基础类型之间加减乘除运算测试结论;
//数组运算编译器会开SIMD指令,
//各种运算差距不大,int64比int32慢一倍,float反而比int32快,double比float

//单个数据运算:

//debug模式:


//两种数据类型之间加减乘除运算时间打印
template<class TA,class TB=TA> 
class DataTest
{
public:
    DataTest()
    {
        doTest();
    }
   ~DataTest(){}
private:
public:
     template<class T>
     void FillRandData(T* Array,size_t N)
     {
         for (int n=0;n<N;n++)
         {
              Array[n]=::rand();
         }
     }
     template<class T>
     void FillRandDataNotZero(T* Array,size_t N)  //除法运算不能被除数不能等于0,否则崩溃
     {
         T tmp;
         for (int n=0;n<N;n++)
         {
             tmp=(T)::rand();
             if (tmp==0)
             {
                 tmp=1.0;
             }

             Array[n]=tmp;
         }
     }
     void doTest()
    {
        std::string sTName("TestType:");
        sTName+=typeid(TA).name();
        sTName+="&";
        sTName+=typeid(TB).name();
        SuperTime tm(sTName.c_str());

        TA varA;
        TB varB;
        decltype(varA+varB) varR=0;
        //long long varR=0;

        const int ArrayNum=500;
        TA arryA[ArrayNum];
        TB arryB[ArrayNum];
        //TA arrayR[ArrayNum];
        decltype(arryA[0]+arryB[0]) arrayR[ArrayNum];

        const size_t TmNum=100;
        const size_t LoopNum=1*10*1000*1000;


        tm.reSet();
        for (size_t T=0;T<TmNum;T++)
        {
            varA=(TA)::rand();
            varB=(TB)::rand();
            FillRandData(arryA,ArrayNum);
            FillRandData(arryB,ArrayNum);
            tm.getBeginTime();
            for (size_t K=0;K<LoopNum;K++)
            {
                if ((K>>1)==0)   //避免被优化加一下条件
                {
                    continue;
                }

             varR+=(varA+varB);


                //for (int n=0;n<ArrayNum;n++)
                //{
                //    arrayR[n]=arryA[n]+arryB[n];
                //}
            }
            tm.getEndCompute();//tm.getEndPrint();
        }

        double retAdd=tm.getAvrgTime();
        //printf("加法测试完成\n");


        tm.reSet();
        for (size_t T=0;T<TmNum;T++)
        {
            varA=(TA)::rand();
            varB=(TB)::rand();
            FillRandData(arryA,ArrayNum);
            FillRandData(arryB,ArrayNum);
            tm.getBeginTime();
            for (size_t  K=0;K<LoopNum;K++)
            {
                if ((K>>1)==0)   //避免被优化加一下条件
                {
                    continue;
                }

                varR+=(varA-varB);

                //for (int n=0;n<ArrayNum;n++)
                //{
                //    arrayR[n]=arryA[n]-arryB[n];
                //}
            }
            tm.getEndCompute();//tm.getEndPrint();
        }
        double retSub=tm.getAvrgTime();
        //printf("减法测试完成\n");

        tm.reSet();
        for (size_t T=0;T<TmNum;T++)
        {
            varA=(TA)::rand();
            varB=(TB)::rand();
            FillRandData(arryA,ArrayNum);
            FillRandData(arryB,ArrayNum);
            tm.getBeginTime();
            for (size_t K=0;K<LoopNum;K++)
            {
                if ((K>>1)==0)   //避免被优化加一下条件
                {
                    continue;
                }


                varR+=(varA*varB);

                //for (int n=0;n<ArrayNum;n++)
                //{
                //    arrayR[n]=arryA[n]*arryB[n];
                //}
            }
            tm.getEndCompute();//tm.getEndPrint();
        }
        double retMul=tm.getAvrgTime();
        //printf("乘法测试完成\n");


        tm.reSet();
        for (size_t T=0;T<TmNum;T++)
        {
            varA=(TA)::rand();
            //varB=1;//::abs(::rand());

            //int tmp=::rand();   //除数不能为零,此写法有误,即使int 不为0,向下截断为char 也可能为0.
            //if (tmp<=0)
            //{
            //    tmp=2.0;
            //}
            //varB=tmp;

            if (varB==0)
            {
                varB=1.0;
            }

            FillRandDataNotZero(arryA,ArrayNum);
            FillRandDataNotZero(arryB,ArrayNum);
            tm.getBeginTime();
            for (size_t K=0;K<LoopNum;K++)
            {
                if ((K>>1)==0)   //避免被优化加一下条件
                {
                    continue;
                }


                varR+=(varA/varB);

                //for (int n=0;n<ArrayNum;n++)
                //{
                //     arrayR[n]=arryA[n]/arryB[n];
                //}
            }
            tm.getEndCompute();//tm.getEndPrint();
        }
        double retDiv=tm.getAvrgTime();
        //printf("除法测试完成\n");

        printf("Ret:%d ",(int)varR);   //避免被优化,打印结果

        ::printf("运行帧率:加法:%8.3f 减法:%8.3f 乘法:%8.3f 除法:%8.3f 类型:%s\n",
            1.0/retAdd,1.0/retSub,1.0/retMul,1.0/retDiv,sTName.c_str());
    }
};

//测试基础类型加减乘除效率差异
void TestDataType()
{
    {
        DataTest<char,char> tmp;
    }
    {
        DataTest<unsigned char,unsigned char> tmp;
    }
    {
        DataTest<short,short> tmp;
    }
    {
        DataTest<int,int> tmp;
    }
    {
        DataTest<unsigned int,unsigned int> tmp;
    }
    {
        DataTest<int64_t,int64_t> tmp;
    }
    {
        DataTest<uint64_t,uint64_t> tmp;
    }
    {
        DataTest<int,unsigned int> tmp;
    }
    {
        DataTest<int64_t,uint64_t> tmp;
    }
    {
        DataTest<float,float> tmp;
    }
    {
        DataTest<double,double> tmp;
    }

    {
        DataTest<int,char> tmp;
    }

    {
        DataTest<int,float> tmp;
    }
    {
        DataTest<int,double> tmp;
    }
    {
        DataTest<double,float> tmp;
    }
    {
        DataTest<unsigned int,float> tmp;
    }
    {
        DataTest<uint64_t,float> tmp;
    }
    {
        DataTest<uint64_t,double> tmp;
    }
    {
        DataTest<Int24,Int24> tmp;
    }
    {
        DataTest<Int24,int> tmp;
    }
    {
        DataTest<Int24,float> tmp;
    }
    {
        DataTest<Int24,double> tmp;
    }
}


//global.h简单数学函数性能测试
void Test_basic2()
{
       SuperTime tm("Test_Global");
       //unsigned int nCurr=0;
       int nCurr=0;
       for (int T=0;T<10000;T++)
       {
           tm.getBeginTime();
           for (int n=0;n<10000000;n++)
           {
               //nCurr=getNextCycleIndex(nCurr,8);
               //NextCycleIndex(nCurr,8);
               //nCurr=getPrevCycleIndex(nCurr,8);
               //nCurr=getCycleIndex(nCurr,1,100);
               //nCurr=getCycleNumber(nCurr,1,-20,30);
               //printf("result:%d\n",nCurr);   //避免被优化,打印结果
           }
           tm.getEndPrint(100);
       }
        printf("result:%u\n",nCurr);   //避免被优化,打印结果
}



#ifdef UNIT_TEST

int Test_publicBasic()
{
    TestDataType();
    //Test_Global();

    //一秒钟2000万次加锁解锁
#if 0
    std::mutex mutex;
    int i=0;
    for (int k=0;k<10000;++k)
    {
        tm.getBeginTime();
        for (int n=0;n<10000;n++)
        {
            mutex.lock();
            i++;
            mutex.unlock();
        }
         tm.getEndPrint(100);
    }
    printf("i:%d\n");
#endif

   // Test_NetStream();


    //Test_MyBasicOperation();

    return 0;
}

#endif

