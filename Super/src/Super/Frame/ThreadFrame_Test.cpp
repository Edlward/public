#include "ThreadFrame_Test.h"

#include "../Basic/global.h"
#include "ParallelFrame.h"
#include "SequentialFrame.h"

//�����̼߳��ٿ�ܲ��ԡ�





 int Test_ThreadPoolSerial();


 //ʹ�÷���:
 //���� ��Ƶ�ṹcardinput;
 //��Ҫ˳�����ProcessStepA,ProcessStepB,ProcessStepC,ProcessStepD���㷨����
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

     //ģ��ĳ���㷨��ʼ����˳�����A,B,C,D�����㷨
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
                 if(data[ch][j]!=theoryRes)   //��Ԥ�ڽ��
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
//ThreadPoolSerialV2<cardinput*> threadPool;   //ʵ��˼·2

//�㷨����û��֡˳��Ҫ��ļ��ٷ���
ThreadPoolParallel<cardinput*> threadPool;   //ʵ��˼·2


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


     //��֯�㷨����
     UserSerialTaskMgr usk;
     usk.addStep(FunObjWrap(StepA::ProcessStepA,&a,0,0));
     usk.addStep(FunObjWrap(StepB::ProcessStepB,&b,0,0));
     usk.addStep(FunObjWrap(StepC::ProcessStepC,&c,0,0));
     usk.addStep(FunObjWrap(StepD::ProcessStepD,&d,0,0));

     //���ø����м����̳߳�
     threadPool.setSerialTaskMgr(usk);

     //��ʼ�߳�
     threadPool.Start();


     ////Ͷ�����ݸ��̴߳���
     //for (int n=0;n<100;n++)
     //{
     //    threadPool.addData(&card[n]);
     //}

     //Ͷ�������߳�
     std::thread threadPush(threadPush);

     //��ȡ�������߳�
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


