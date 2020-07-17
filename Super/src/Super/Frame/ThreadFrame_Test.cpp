#include "ThreadFrame_Test.h"

#include "../Basic/global.h"
#include "ParallelFrame.h"
#include "SequentialFrame.h"

//各种线程加速框架测试。





 int Test_ThreadPoolSerial();


 //使用方法:
 //设有 音频结构cardinput;
 //需要顺序进行ProcessStepA,ProcessStepB,ProcessStepC,ProcessStepD等算法步骤
 struct cardinput
 {
     enum MyEnum
     {
         NumChs=2,
         Samples=200
     };
     int data[NumChs][Samples];
     int frameNum;
     long long checkSum;
     cardinput()
     {
         initData();
         FillCheck();
     }
     void FillRandData()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]=MyRand();////rand();
             }
         }
     }
     long long getCheckSum()
     {
         long long Sum=0;
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 Sum+=data[ch][j];
             }
         }
         return Sum;
     }
     void FillCheck()
     {
         checkSum=getCheckSum();
     }
     void doCheck()
     {
         long long currCheck=getCheckSum();
         if (currCheck!=this->checkSum)
         {
             printf("data has bad\n");
         }
     }

     //模拟某种算法初始化后顺序进行A,B,C,D几种算法
     void initData()
     {
         frameNum=0;
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]=j;
             }
         }
     }
     void doProcessA()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]+=10;
             }
         }
     }
     void doProcessB()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]+=20;
             }
         }
     }
     void doProcessC()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]+=30;
             }
         }
     }
     void doProcessD()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 data[ch][j]+=40;
             }
         }
     }
     void doCheckResult()
     {
         for (int ch=0;ch<NumChs;ch++)
         {
             for (int j=0;j<Samples;j++)
             {
                 int theoryRes=j+10+20+30+40;
                 if(data[ch][j]!=theoryRes)   //非预期结果
                 {                  
                       printf("Unexpected results data!\n");
                       return;
                 }
             }
         }
     }
 };

 class StepA
 {
 public:
     static void ProcessStepA(void* parent,void* pdata,int* ctrl)
     {
         cardinput* pc=*(cardinput**)pdata;

         TracePrint(" pc:%p frameNum:%u\n",pc,pc->frameNum);

         pc->doCheck();
         //pc->FillRandData();
         pc->doProcessA();
         pc->FillCheck();
     }
 };
 class StepB
 {
 public:
     static void ProcessStepB(void* parent,void* pdata,int* ctrl)
     {
         cardinput* pc=*(cardinput**)pdata;
         TracePrint(" pc:%p frameNum:%u\n",pc,pc->frameNum);

         pc->doCheck();
         //pc->FillRandData();
         pc->doProcessB();
         pc->FillCheck();
     }
 };

 class StepC
 {
 public:
     static void ProcessStepC(void* parent,void* pdata,int* ctrl)
     {
         cardinput* pc=*(cardinput**)pdata;
         TracePrint(" pc:%p frameNum:%u\n",pc,pc->frameNum);

         pc->doCheck();
         //pc->FillRandData();
         pc->doProcessC();
         pc->FillCheck();
     }
 };
 class StepD
 {
 public:
     static void ProcessStepD(void* parent,void* pdata,int* ctrl)
     {
         cardinput* pc=*(cardinput**)pdata;
         TracePrint(" pc:%p frameNum:%u\n",pc,pc->frameNum);

         pc->doCheck();
         //pc->FillRandData();
         pc->doProcessD();
         pc->FillCheck();
     }
 };

//ThreadPoolSerial<cardinput*> threadPool;
//ThreadPoolSerialV2<cardinput*> threadPool;   //实现思路2

//算法步骤没有帧顺序要求的加速方法
ThreadPoolParallel<cardinput*> threadPool;   //实现思路2


 cardinput card[100];
 static unsigned int index=0;
 static unsigned int gindex=0;

 void threadPush()
 {
     while (true)
     {
         cardinput* pin=&card[index++%100];
         pin->initData();
         pin->frameNum=gindex++;
         pin->FillCheck();
         bool retPush=threadPool.pushData(pin);

         //printf("Push cardNum:%u retPush:%d\n",pin->frameNum,retPush);
         uint32_t tm=(uint32_t)rand()%10;
         std::this_thread::sleep_for(std::chrono::microseconds(tm));  //100
     }
 }
 void threadGet()
 {
     while (true)
     {
         cardinput* pcard;
         bool retGet=threadPool.getResData(pcard);
         if (!retGet)
         {
             //std::this_thread::sleep_for(std::chrono::microseconds(100));
             continue;
         }
         pcard->doCheck();
         pcard->doCheckResult();

         //printf("Get cardNum:%u retGet:%d\n",pcard->frameNum,retGet);
     }
 }


 int Test_ThreadPoolSerial()
 {
     StepA a;
     StepB b;
     StepC c;
     StepD d;


     //组织算法步骤
     UserSerialTaskMgr usk;
     usk.addStep(FunObjWrap(StepA::ProcessStepA,&a,0,0));
     usk.addStep(FunObjWrap(StepB::ProcessStepB,&b,0,0));
     usk.addStep(FunObjWrap(StepC::ProcessStepC,&c,0,0));
     usk.addStep(FunObjWrap(StepD::ProcessStepD,&d,0,0));

     //设置给串行加速线程池
     threadPool.setSerialTaskMgr(usk);

     //开始线程
     threadPool.Start();


     ////投递数据给线程处理
     //for (int n=0;n<100;n++)
     //{
     //    threadPool.addData(&card[n]);
     //}

     //投递数据线程
     std::thread threadPush(threadPush);

     //获取处理结果线程
     std::thread threadGet(threadGet);
     threadPush.join();
     threadGet.join();

     threadPool.waitFinish();
     return 0;
 }



#ifdef UNIT_TEST
int Test_ThreadFrame()
 {
     Test_ThreadPoolSerial();

     return  0;
 }
#endif


