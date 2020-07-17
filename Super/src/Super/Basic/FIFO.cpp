#include "FIFO.h"


NAME_SPACE_MAIN_BEGIN
 
#ifdef UNIT_TEST

USING_NAME_SPACE_MAIN
int Test_FIFO()
{
    const unsigned int ArrarSize=100;
    int t[ArrarSize]={0};
    for(unsigned int n=0;n<ArrarSize;n++)
    {
        t[n]=n;
    }
    CFIFO<int,200,IsPOD> fifo(ArrarSize);
    fifo.pushDataIfPossible(t,80);
    fifo.PrintData();
    fifo.pushDataIfPossible(t+80,20);
    fifo.PrintData();

    fifo.getNullData(10);
    fifo.PrintData();

    //fifo.pushDataIfPossible(t,10);
    //fifo.PrintData();

    //fifo.pushCover(t,50);
    for (int n=0;n<10;n++)
    {
         fifo.pushCover(t[n]);
    }

    fifo.PrintData();

#if 0

    printf("While get Data\n");
    int ret=-99;
    unsigned int getNum=0;
    while (fifo.get(ret))
    {
        printf("%03d ",ret);
       getNum++;
       if (getNum%16==15)
       {
           printf("\n");
       }
    }
#endif





#if 0
    {
        int ret[ArrarSize]={0};
        
        size_t availableSize=fifo.getUsedSpace();
        size_t getSize=fifo.get(ret,availableSize);

        printf("Result Data availableSize:%u\n",(unsigned int)availableSize);
        for(unsigned int n=0;n<getSize;n++)
        {
            printf("%03d ",ret[n]);
            if (n%16==15)
            {
                printf("\n");
            }
        }
    }
#endif

    return 0;
}
#endif



NAME_SPACE_MAIN_END


