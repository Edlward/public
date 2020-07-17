#include "Queue.h"


 

#ifdef UNIT_TEST

int Test_Queue()
{
#if 0
    SeqQueue<int> seqQueue(10);
    int init[10]={1,6,9,0,2,5,8,3,7,4};
    for(int i=0;i<5;i++){
        seqQueue.Append(init[i]);
    }
    seqQueue.Print();

    cout<<seqQueue.Delete()<<endl;
    seqQueue.Print();

    for(int i=5;i<10;i++){
        seqQueue.Append(init[i]);
    }
    seqQueue.Print();

    cout<<seqQueue.Get()<<endl;

    seqQueue.MakeEmpty();
    seqQueue.Print();

    seqQueue.Append(1);
    seqQueue.Print();
    return 0;
#endif



#if 0
    LinkQueue<int> linkQueue;
    int init[10]={1,3,6,8,9,2,0,5,4,7};
    for(int i=0;i<10;i++){
        linkQueue.Append(init[i]);
    }
    linkQueue.Print();
    linkQueue.Delete();
    linkQueue.Print();
    cout<<linkQueue.GetFront()<<endl;
    linkQueue.Print();
    linkQueue.MakeEmpty();
    linkQueue.Print();
    linkQueue.Delete();
    return 0;
#endif

    PriorityQueue<int,Compare<int> > priorityQueue;
    int init[10]={1,9,3,5,0,8,2,4,6,7};
    for(int i=0;i<10;i++){
        priorityQueue.Append(init[i]);
    }
    priorityQueue.Print();
    priorityQueue.Delete();
    priorityQueue.Print();
    getchar(); //system("pause");²»¿çÆ½Ì¨
    system("cls");
    PriorityQueue<SpecialData,SpecialCmp> spe_queue;
    int init2[5][2]={{34,2},{64,1},{18,3},{24,2},{55,4}};
    SpecialData data[5];
    for(int i=0;i<5;i++){
        data[i].m_npir=init2[i][1];
        data[i].m_ntenor=init2[i][0];
    }
    for(int i=0;i<5;i++){
        spe_queue.Append(data[i]);
    }
    spe_queue.Print();

    cout<<spe_queue.GetFront()<<endl<<endl;
    spe_queue.Delete();
    spe_queue.Print();
    return 0;

}


#endif

