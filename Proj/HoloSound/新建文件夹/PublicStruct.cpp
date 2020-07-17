#include "PublicStruct.h"

static SpkChMap gSpkChMap;
SpkChMap* GetSpkChMap()
{
    return &gSpkChMap;
}

bool isArrayByName(const char* srcName)
{
    //�����µ����bed��ǰ4��L,R,C,LFE ��Ϊmain������������������Ϊarray ����������
    std::string sBedName[10]={"L","R","C","LFE","LSS","RSS", "LRS","RRS","LTS","RTS"};
    bool bArray=false;
    for (int n=4;n<10;n++)
    {
        if (sBedName[n]==srcName)
        {
            bArray=true;
            break;
        }
    }
    return  bArray;
}

unsigned int ObjMetaDataTmp::getSpeedLevel() //������֡��λ�������˶��ٶȵȼ��������Ƿ��һ�����ƽ����֡
{
    float offsetX=SubPos[NumSubBlocks-1].x-SubPos[0].x;
    float offsetY=SubPos[NumSubBlocks-1].y-SubPos[0].y;
    float offsetZ=SubPos[NumSubBlocks-1].z-SubPos[0].z;
    //float offsetSize=SubPos[NumSubBlocks].size-SubPos[0].size;
    //float distance=sqrt(offsetX*offsetX+offsetY*offsetY+offsetZ*offsetZ);// Ϊ��Ч�� ����Ҫ��ȷ���ƶ�����
    float distance=fabs(offsetX)+fabs(offsetY)+fabs(offsetZ);
    float speed=distance/NumSubBlocks;  //����Ӧ�ó���NumSubBlocks-1

    unsigned int SpeedLevel=0;

    //�ֳ�4�� �ֱ��� 8/16/40/80��������
    if (speed<0.00005)         //0.0001
    {
        SpeedLevel=0;
    }else if (speed<0.0001) //0.0004
    {
        SpeedLevel=1;
    }else if (speed<0.0002)  //0.002
    {
        SpeedLevel=2;
    }
    else
    {
        SpeedLevel=3;
    }
    //printf("speed=%f Level=%d\n",speed,SpeedLevel);
    return  SpeedLevel;
}

unsigned int ObjMetaDataTmp::getSmoothSubBlocks(unsigned int FrameRate)
{
    //unsigned int SpeedLevel=getSpeedLevel();
    unsigned int SpeedLevel=0;   //3��ʱ�ú㶨ֵ

    //�����ĵ�����NumPanSubBlockֻ��8/4/2�������꣬����Ϊ��Ӧ�Կ����˶����󣬸����˶��ٶȶ�̬�����м����������
    //�ڲ�ͬ֡��24/25/30/48/50/60/96/100/120�±�����
    //��Ӧÿ֡��2000 1920 1600 1000 960 800 500 480 400   ��ͬ��Լ����1 8 20  //16 80
    //��ÿ֡������Ϊ(2000 1920 1600)ʱ  8 16 40 80 
    //��ÿ֡������Ϊ(1000 960 800)   ʱ   4  8  20 40 
    //��ÿ֡������Ϊ(500 480 400)     ʱ   2  4  10 20      
    const unsigned int SubBlockTable[3][4]={{8,16,40,80},
    {4,8,20,40},
    {2,4,10,20}};
    unsigned int IndexFRate=0;
    switch (FrameRate)
    {
    case 24:
    case 25:
    case 30:IndexFRate=0;break;
    case 48:
    case 50:
    case 60:IndexFRate=1;break;
    case 96:
    case 100:
    case 120:IndexFRate=2;break;
    }

    return SubBlockTable[IndexFRate][SpeedLevel];
}

void ObjMetaDataTmp::FillSmoothBlocks(PositionInfo SmoothSubPos[80],unsigned int NumSubSmoothBlocks)
{
#if 1
    PositionInfo Offet;
    //unsigned int devideblock=NumSubSmoothBlocks-NumSubSmoothBlocks/NumSubBlocks;   //��8�񻮷ֳ�16��ÿһ������׼ȷ��
    unsigned int devideblock=NumSubSmoothBlocks-1;   //��8�񻮷ֳ�16��ÿһ������׼ȷ��

    //������Ӧ���ڸ���ԭ������֮����ƽ��������û�б�Ҫ��ô��ȷ��ֱ�Ӷ���β��ƽ��
    Offet.x=(this->SubPos[NumSubBlocks-1].x-this->SubPos[0].x)/devideblock;
    Offet.y=(this->SubPos[NumSubBlocks-1].y-this->SubPos[0].y)/devideblock;
    Offet.z=(this->SubPos[NumSubBlocks-1].z-this->SubPos[0].z)/devideblock;

    if (fabs(Offet.x)>0.1||fabs(Offet.y)>0.1||fabs(Offet.z)>0.1)
    {
        //printf("SubBlock Offset");
        //Offet.print();
    }


    SmoothSubPos[0]=this->SubPos[0];
    SmoothSubPos[NumSubSmoothBlocks-1]=this->SubPos[this->NumSubBlocks-1];
    PositionInfo tmp=this->SubPos[0];
    for (unsigned int N=1;N<NumSubSmoothBlocks-1;++N)
    {
        tmp.x+=Offet.x;
        tmp.y+=Offet.y;
        tmp.z+=Offet.z;
        SmoothSubPos[N]=tmp;
    }



    return;
#else
    //PositionInfo Offet;
    //unsigned int devideblock=NumSubSmoothBlocks-1;   //��8�񻮷ֳ�16��ÿһ������׼ȷ��
    ////������Ӧ���ڸ���ԭ������֮����ƽ��������û�б�Ҫ��ô��ȷ��ֱ�Ӷ���β��ƽ��
    //Offet.x=(this->SubPos[NumSubBlocks-1].x-this->SubPos[0].x)/devideblock;
    //Offet.y=(this->SubPos[NumSubBlocks-1].y-this->SubPos[0].y)/devideblock;
    //Offet.z=(this->SubPos[NumSubBlocks-1].z-this->SubPos[0].z)/devideblock;
    //SmoothSubPos[0]=this->SubPos[0];
    //SmoothSubPos[NumSubSmoothBlocks-1]=this->SubPos[NumSubBlocks-1];

    ////SmoothSubPos[NumSubSmoothBlocks-1]=this->SubPos[this->NumSubBlocks-1];
    //PositionInfo tmp=this->SubPos[0];
    //for (unsigned int N=1;N<NumSubSmoothBlocks-1;++N)
    //{
    //    tmp.x+=Offet.x;
    //    tmp.y+=Offet.y;
    //    tmp.z+=Offet.z;
    //    SmoothSubPos[N]=tmp;
    //}
    unsigned int NSub=0;
    unsigned int Increase=NumSubSmoothBlocks/NumSubBlocks;
    for (unsigned int N=0;N<NumSubSmoothBlocks;++N)
    {
        SmoothSubPos[N]=this->SubPos[N/Increase];
    }
    return;


    PositionInfo Offet;
    unsigned int devideblock=NumSubSmoothBlocks-1;   //��8�񻮷ֳ�16��ÿһ������׼ȷ��
    //������Ӧ���ڸ���ԭ������֮����ƽ��������û�б�Ҫ��ô��ȷ��ֱ�Ӷ���β��ƽ��
    Offet.x=(this->SubPos[NumSubBlocks-1].x-this->SubPos[0].x)/devideblock;
    Offet.y=(this->SubPos[NumSubBlocks-1].y-this->SubPos[0].y)/devideblock;
    Offet.z=(this->SubPos[NumSubBlocks-1].z-this->SubPos[0].z)/devideblock;
    SmoothSubPos[0]=this->SubPos[0];
    SmoothSubPos[NumSubSmoothBlocks-1]=this->SubPos[NumSubBlocks-1];

    //SmoothSubPos[NumSubSmoothBlocks-1]=this->SubPos[this->NumSubBlocks-1];
    PositionInfo tmp=this->SubPos[0];
    for (unsigned int N=1;N<NumSubSmoothBlocks-1;++N)
    {
        tmp.x+=Offet.x;
        tmp.y+=Offet.y;
        tmp.z+=Offet.z;
        SmoothSubPos[N]=tmp;
    }


#endif
}

void ObjMetaDataTmp::checkRange()
{
    if (id>=128||id<10)
    {
        printf("Objmeta data out of range:%d\n",id);
    }

    for (unsigned int n=0;n<NumSubBlocks;n++)
    {
        PositionInfo& pos=SubPos[n];
        if(pos.x>1.0f||pos.x<0.0f||pos.y>1.0f||pos.y<0.0f||pos.z>1.0f||pos.z<0.0f)
        {
            printf("id:%d Pos: Out of range: x=%f y=%f z=%f\n",id,pos.x,pos.y,pos.z);
        }
    }
}

void ObjMetaDataTmp::limitRange()
{
    for (unsigned int n=0;n<NumSubBlocks;n++)
    {
        PositionInfo& pos=SubPos[n];
        pos.LimitPosRange();
    }
}
