#include "MemPoolFast.h"
#include "Thread.h"   //SLEEP


NAME_SPACE_MAIN_BEGIN
 

#ifdef UNIT_TEST
#include "SuperTime.h"

USING_NAME_SPACE_MAIN

int Test_MemPoolFast()
{
#if 0
    std::vector<void*> ListMem;
    const int memChunkSize=8*1024-16;
    for (int n=0;n<1024*100;n++)
    {
        void* p=::malloc(memChunkSize);
        //::printf("p:%p =%llu\n",p,p);
        ListMem.push_back(p);
    }
    for (size_t n=0;(n+1)<ListMem.size();n++)
    {
        void* p=ListMem[n];
        void* pNext=ListMem[n+1];
        size_t offset;
            
            if(pNext>p)
            {
                 offset=(size_t)pNext-(size_t)p;
            }
            else
            {
                offset=(size_t)p-(size_t)pNext;
            }
        ::printf("p:%p pNext:%p chunkSize:%llu offset:%llu\n",p,pNext,memChunkSize,offset);
        free(p);
    }
#endif


    MemPoolFast memPool;
    const int partSize=100*1024*1024;
    char* pNewPart=(char*)::malloc(partSize);
    memPool.appendNewPartMemSpace(pNewPart,partSize);


    std::vector<void*> Vused;
    SuperTime tm("MemPool()");
    //return 0;

    void* pData;
    for (unsigned int K=0;K<10000;K++)
    {
        Vused.clear();
        unsigned sumMem=0;
        for (unsigned int n=0;n<10;n++)
        {

            unsigned int mallocSize=MyRand()%(partSize/10000);  //rand()

            //unsigned int randValue=rand();  //0-32767
            //unsigned int mallocSize=(randValue*100)%(PoolMgrSize);

            sumMem+=mallocSize;
            //printf("n:%u mallocSize:%u\n",n,mallocSize);

            tm.getBeginTime();
            for (unsigned int K=0;K<100;K++)
            {
                pData=memPool.Malloc(mallocSize);
                //memset(pData,53,mallocSize);
               printf("Index:%u mallocSize:%u p:%p\n",n*K,mallocSize,pData);


                Vused.push_back(pData);
            }
            tm.getEndPrint(1000);

            //void* pData=malloc(mallocSize);   //与系统函数对比 大约快30倍
            //Vused.push_back(pData);
            //memset(pData,53,mallocSize);
            //memPool.Free(pData);   //不能重复释放

        }

        //memPool.printStatus();

        for (size_t n=0;n<Vused.size();n++)
        {
            void* p=Vused.at(n);
            memPool.Free(p);
            //::free(p);
        }
        //memPool.FreeSpace();
    }


    return 0;
}



#endif

NAME_SPACE_MAIN_END

