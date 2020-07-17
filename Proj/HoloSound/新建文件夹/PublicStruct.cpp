#include "PublicStruct.h"

static SpkChMap gSpkChMap;
SpkChMap* GetSpkChMap()
{
    return &gSpkChMap;
}

bool isArrayByName(const char* srcName)
{
    //根据新的设计bed的前4个L,R,C,LFE 称为main主扬声器，后六个称为array 阵列扬声器
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

unsigned int ObjMetaDataTmp::getSpeedLevel() //根据子帧首位坐标求运动速度等级，决定是否进一步填充平滑子帧
{
    float offsetX=SubPos[NumSubBlocks-1].x-SubPos[0].x;
    float offsetY=SubPos[NumSubBlocks-1].y-SubPos[0].y;
    float offsetZ=SubPos[NumSubBlocks-1].z-SubPos[0].z;
    //float offsetSize=SubPos[NumSubBlocks].size-SubPos[0].size;
    //float distance=sqrt(offsetX*offsetX+offsetY*offsetY+offsetZ*offsetZ);// 为了效率 不需要精确求移动距离
    float distance=fabs(offsetX)+fabs(offsetY)+fabs(offsetZ);
    float speed=distance/NumSubBlocks;  //理论应该除以NumSubBlocks-1

    unsigned int SpeedLevel=0;

    //分成4等 分别用 8/16/40/80个子坐标
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
    unsigned int SpeedLevel=0;   //3暂时用恒定值

    //根据文档定义NumPanSubBlock只有8/4/2个子坐标，但是为了应对快速运动对象，根据运动速度动态决定中间过渡子坐标
    //在不同帧率24/25/30/48/50/60/96/100/120下被除进
    //对应每帧长2000 1920 1600 1000 960 800 500 480 400   共同公约数是1 8 20  //16 80
    //当每帧采样点为(2000 1920 1600)时  8 16 40 80 
    //当每帧采样点为(1000 960 800)   时   4  8  20 40 
    //当每帧采样点为(500 480 400)     时   2  4  10 20      
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
    //unsigned int devideblock=NumSubSmoothBlocks-NumSubSmoothBlocks/NumSubBlocks;   //如8格划分成16格，每一格间距最准确求法
    unsigned int devideblock=NumSubSmoothBlocks-1;   //如8格划分成16格，每一格间距最准确求法

    //理论上应该在各个原子坐标之间做平滑，但是没有必要那么精确，直接对首尾做平滑
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
    //unsigned int devideblock=NumSubSmoothBlocks-1;   //如8格划分成16格，每一格间距最准确求法
    ////理论上应该在各个原子坐标之间做平滑，但是没有必要那么精确，直接对首尾做平滑
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
    unsigned int devideblock=NumSubSmoothBlocks-1;   //如8格划分成16格，每一格间距最准确求法
    //理论上应该在各个原子坐标之间做平滑，但是没有必要那么精确，直接对首尾做平滑
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
