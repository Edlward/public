#include "RingBuffer.h"
#include "RingBufferProcess.h"

USING_NAME_SPACE_MAIN

#include "Thread.h"
#include "ThreadSync.h"
#include "SuperTime.h"

struct CardFrame
{
    uint64_t timeIn;
    uint64_t timeOut;
	unsigned int FrameNum;
};

RingBuffer<CardFrame> ringBf(8);
//RingBufferProcess<CardFrame> ringBf(2,"test");


unsigned int Fr=0;
Mutex m_mtx;
unsigned int GetNextNum()
{
    LockerGuard<Mutex> lock(m_mtx);
    return Fr++;
}


void ProducerTask(void* p)
{
    int nThread=(long)p;

    printf("环形缓冲区总容量大小:%u\n",ringBf.getCapacity());

    printf("生产者线程:%d\n",nThread);
	unsigned int num=0;
	while(1)
	{   
        CardFrame card;
		card.FrameNum=GetNextNum();//num++;
        card.timeIn=SuperTime::getHighResolutionTicks();//::time(NULL);
		//ringBf.pushOnlySingle(card,true);//
		//ringBf.pushMayMultiple(card,true);//

        while (1)
        {
            bool ret=ringBf.pushMayMultiple(card,false);//
            if (ret) break;
        }

        //printf("生产者线程%02d生产:%u  ringSize:%u\n",nThread,card.FrameNum,ringBf.getUsedSpace());
		unsigned int sleepNs=rand()%5;
		Thread::sleep_for(sleepNs);
	}
}


void ConsumerTask(void* p)
{
    int nThread=(long)p;

    printf("消费者线程:%d\n",nThread);

    unsigned int lastGetNum=0;
	while(1)
	{ 
        CardFrame card;  
		//bool ret=ringBf.getOnlySingle(card,false);
        bool ret=ringBf.getMayMultiple(card,false);
        if (!ret)
        {
            continue;
        }

        //获取顺序和打印顺序不一定一致，用获取时间更能精确说明问题,只要获取时间不乱序。即正确
        
        //在RingBuffer内部打上时间才百分之八严格，暂时在外面
        card.timeOut=SuperTime::getHighResolutionTicks();//::time(NULL);


        printf("                                  消费者线程%02d消费:%u timeIn:%lld timeOut:%lld ringSize:%u\n"
            ,nThread,card.FrameNum,card.timeIn,card.timeOut,ringBf.getUsedSpace());


        if (lastGetNum>card.FrameNum)
        {
            printf("消费者可能乱序:Last:%u Curr:%d\n",lastGetNum,card.FrameNum);
            lastGetNum=card.FrameNum;
            Thread::sleep_for(10000);
        }
        //lastGetNum=card.FrameNum;
        unsigned int sleepNs=rand()%10;
        //Thread::sleep_for(sleepNs);
	}
}











#ifdef UNIT_TEST

int Test_ringbuffer()
{
    


	//生产者线程
    const unsigned int ThreadNumProduce=5;
    Thread::Handle handleProducer[ThreadNumProduce];
	for(unsigned int n=0;n<ThreadNumProduce;n++)
	{
       handleProducer[n]=Thread::startThread((void*)ProducerTask,(void*)n); 
	}

	
	//消费者线程
    const unsigned int ThreadNumConsumer=2;
    Thread::Handle handleConsumer[ThreadNumConsumer];
    for(unsigned int n=0;n<ThreadNumConsumer;n++)
    {
        handleConsumer[n]=Thread::startThread((void*)ConsumerTask,(void*)n); 
    }


    for(unsigned int n=0;n<ThreadNumProduce;n++)
    {
           Thread::waitFinish(handleProducer[n]);   
    }

    for(unsigned int n=0;n<ThreadNumConsumer;n++)
    {
           Thread::waitFinish(handleConsumer[n]);   
    }

#if 0
	
        std::cout<<"Test_ringbufferSimple()"<<std::endl;
    {

        std::cout<<"Test_ringbufferSimple()"<<std::endl;

        ringbufferSimple ring(sizeof(double),1000);
        for (int i=0;i<50;i++)
        {
            double v=i;
            ring.push(&v,false);
            std::cout<<"put v= "<<v<<std::endl;
        }
        for (int i=0;i<50;i++)
        {
            double v;
            ring.get(&v,false);
            std::cout<<"get v= "<<v<<std::endl;
        }
    }


    {
        std::cout<<"ringbufferProcessSimple()"<<std::endl;

        ringbufferProcessSimple ring(sizeof(double),1000,"name");
        for (int i=0;i<50;i++)
        {
            double v=i;
            ring.push(&v,false);
            std::cout<<"put v= "<<v<<std::endl;
        }


        {   
            //两个实例，模拟跨进程
            ringbufferProcessSimple ring(sizeof(double),1000,"name");
            for (int i=0;i<50;i++)
            {
                double v;
                ring.get(&v,false);
                std::cout<<"get v= "<<v<<std::endl;
            }
        }

    }
#endif

    return 0;
}


#endif

