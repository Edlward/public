#include <math.h>
#include "PublicStruct.h"
#include "definition.h"

#include "Super/Basic/BasicOperation.h"



SpkChMap* GetSpkChMap()
{
    //单例对象,放函数外更高效,但无法保证使用时已构造完成(main函数前被调用),//若单例之间依赖复杂为保证构造顺序就放函数内,并为构造时线程安全C++11之前要加锁 C++11之后由编译器保证 by lizulin
     static SpkChMap gSpkChMap;
    return &gSpkChMap;
}



bool isArrayByName(const char* srcName)
{
     static const std::string sBedName[10]={"L","R","C","LFE","LSS","RSS", "LRS","RRS","LTS","RTS"};   

    //根据新的设计bed的前4个L,R,C,LFE 称为main主扬声器，后六个称为array 阵列扬声器
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





unsigned int ObjMetaDataTmp::getSpeedLevel() const //根据子帧首位坐标求运动速度等级，决定是否进一步填充平滑子帧
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

unsigned int ObjMetaDataTmp::getSmoothSubBlocks(unsigned int FrameRate) const
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

void ObjMetaDataTmp::checkRange() const
{
    if (id>=128||id<10)
    {
        printf("Objmeta data out of range:%d\n",id);
    }

    for (unsigned int n=0;n<NumSubBlocks;n++)
    {
        const PositionInfo& pos=SubPos[n];
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
        pos.LimitRange();
    }
}
 

size_t NeedBytesToStream(const KDMInfo& kInfo)
{
    return NeedBytesToStream(kInfo.mStr);
}
size_t StreamPut(char*& pos,size_t& AccLen,const KDMInfo& kInfo)
{
    return StreamPut(pos,AccLen,kInfo.mStr);
}

size_t StreamGet(char*& pos,size_t& AccLen,KDMInfo& kInfo)
{
    return StreamGet(pos,AccLen,kInfo.mStr);
}


size_t NeedBytesToStream(const AtomsData& ad)
{
    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(ad.strAtomsMxf);
    nBytes+=::NeedBytesToStream(ad.MxfCheckSum);
    nBytes+=::NeedBytesToStream(ad.strAtomsUuid);

    nBytes+=::NeedBytesToStream(ad.strUuid1);
    nBytes+=::NeedBytesToStream(ad.strUuid2);
    nBytes+=::NeedBytesToStream(ad.strHash);
    nBytes+=::NeedBytesToStream(ad.strKeyId);

    nBytes+=::NeedBytesToStream(ad.strKeyFilePath);
    nBytes+=::NeedBytesToStream(ad.strKeyNotValidBefore);
    nBytes+=::NeedBytesToStream(ad.strKeyNotValidAfter);

    nBytes+=::NeedBytesToStream(ad.binAssetID);
    nBytes+=::NeedBytesToStream(ad.binAssetHash);

    nBytes+=::NeedBytesToStream(ad.bFindAesKey);
    nBytes+=::NeedBytesToStream(ad.binAesKey);

    nBytes+=::NeedBytesToStream(ad.TotalFrames);
    nBytes+=::NeedBytesToStream(ad.EntryPoint);
    nBytes+=::NeedBytesToStream(ad.nDuration);
    nBytes+=::NeedBytesToStream(ad.FrameRate);
 

    nBytes+=::NeedBytesToStream(ad.strContentName);
    return nBytes;
 
}


size_t StreamPut(char*& pos,size_t& AccLen,const AtomsData& ad)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,ad.strAtomsMxf);
    StreamPut(pos,AccLen,ad.MxfCheckSum);
    StreamPut(pos,AccLen,ad.strAtomsUuid);
    StreamPut(pos,AccLen,ad.strUuid1);
    StreamPut(pos,AccLen,ad.strUuid2);
    StreamPut(pos,AccLen,ad.strKeyId);
    StreamPut(pos,AccLen,ad.strHash);

    StreamPut(pos,AccLen,ad.strKeyFilePath);
    StreamPut(pos,AccLen,ad.strKeyNotValidBefore);
    StreamPut(pos,AccLen,ad.strKeyNotValidAfter);

    StreamPut(pos,AccLen,ad.binAssetID);
    StreamPut(pos,AccLen,ad.binAssetHash);

    StreamPut(pos,AccLen,ad.bFindAesKey);
    StreamPut(pos,AccLen,ad.binAesKey);

    StreamPut(pos,AccLen,ad.TotalFrames);
    StreamPut(pos,AccLen,ad.EntryPoint);
    StreamPut(pos,AccLen,ad.nDuration);
    StreamPut(pos,AccLen,ad.FrameRate);
    StreamPut(pos,AccLen,ad.strContentName);
    return pos-pBegin;
}

size_t StreamGet(char*& pos,size_t& AccLen,AtomsData& ad)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,ad.strAtomsMxf);
    StreamGet(pos,AccLen,ad.MxfCheckSum);
    StreamGet(pos,AccLen,ad.strAtomsUuid);
    StreamGet(pos,AccLen,ad.strUuid1);
    StreamGet(pos,AccLen,ad.strUuid2);
    StreamGet(pos,AccLen,ad.strKeyId);
    StreamGet(pos,AccLen,ad.strHash);

    StreamGet(pos,AccLen,ad.strKeyFilePath);
    StreamGet(pos,AccLen,ad.strKeyNotValidBefore);
    StreamGet(pos,AccLen,ad.strKeyNotValidAfter);

    StreamGet(pos,AccLen,ad.binAssetID);
    StreamGet(pos,AccLen,ad.binAssetHash);

    StreamGet(pos,AccLen,ad.bFindAesKey);
    StreamGet(pos,AccLen,ad.binAesKey);

    StreamGet(pos,AccLen,ad.TotalFrames);
    StreamGet(pos,AccLen,ad.EntryPoint);
    StreamGet(pos,AccLen,ad.nDuration);
    StreamGet(pos,AccLen,ad.FrameRate);

    StreamGet(pos,AccLen,ad.strContentName);
    return pos-pBegin;
}

size_t StreamPut(char*& pos,size_t& AccLen,const DcpInfo& di)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,di.mStr,DcpInfo::C_TOTAL);
    unsigned int reelSize=di.ListSubReelInfo.size();
    StreamPut(pos,AccLen,reelSize);
    for (size_t n=0;n<di.ListSubReelInfo.size();n++)
    {
        StreamPut(pos,AccLen,di.ListSubReelInfo.at(n));
    }
    return pos-pBegin;
}


size_t StreamGet(char*& pos,size_t& AccLen,DcpInfo& di)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,di.mStr,DcpInfo::C_TOTAL);
    unsigned int reelSize=0;
    StreamGet(pos,AccLen,reelSize);
    di.ListSubReelInfo.clear();
    for (size_t n=0;n<reelSize;n++)
    {
        AtomsData ad;
        StreamGet(pos,AccLen,ad);
        di.ListSubReelInfo.push_back(ad);
    }
    return pos-pBegin;
}






void AtomsData::print() const
{
    printf("AtomsData Info:------------------------------------>Begin()\n");
    printf("strAtmosMxf:%s\n",strAtomsMxf.c_str());
    printf("MxfCheckSum:%llu\n",MxfCheckSum);
    printf("strAtomsUuid:%s\n",strAtomsUuid.c_str());
    printf("strUuid1:%s\n",strUuid1.c_str());
    printf("strUuid2:%s\n",strUuid2.c_str());
    printf("strKeyId:%s\n",strKeyId.c_str());
    printf("strHash:%s\n",strHash.c_str());
    printf("KeyFilePath:%s\n",strKeyFilePath.c_str());
    printf("KeyValidDate From %s to %s\n",strKeyNotValidBefore.c_str(),strKeyNotValidAfter.c_str());
    printf("AssetIDbin:");
    print_hex(binAssetID,sizeof(binAssetID));
    printf("AssetHashbin:");
    print_hex(binAssetHash,sizeof(binAssetHash));

    printf("bFindAesKey:%d\n",bFindAesKey);
    printf("AesKeybin:");
    print_hex(binAesKey,sizeof(binAesKey));
    printf("TotalFrames:%u Frames\n",TotalFrames);      //以帧数为单位的时间
    printf("EntryPoint:%u \n",EntryPoint);            //
    printf("Duration:%u Frames\n",nDuration);   //以帧数为单位的时间
    printf("FrameRate:%u\n",FrameRate);           //帧率
    printf("ContentName:%s\n", strContentName.c_str());
    printf("AtomsData Info:------------------------------------>>End\n\n");
}

size_t NeedBytesToStream(const DcpInfo& di)
{
    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(di.mStr);    //字符串数组有重载特化,可以直接调用
    nBytes+=sizeof(unsigned int);               //注意:可能不一致
    for (size_t n=0;n<di.ListSubReelInfo.size();n++)
    {
        nBytes+=NeedBytesToStream(di.ListSubReelInfo.at(n));
    }
    return nBytes;
}

void DcpInfo::print() const
{
    std::string sDetail=toDetail();
    printf("DcpInfo:\n%s\n",sDetail.c_str());
    printf("SubReelAtmosData:\n");
    for (unsigned int n=0;n<ListSubReelInfo.size();n++)
    {
        printf("SubReel Index:%u\n",n);
        ListSubReelInfo.at(n).print();
    }
}


size_t NeedBytesToStream(const MediaFileInfo& mi)
{
    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(mi.isSuccess);
    nBytes+=::NeedBytesToStream(mi.FileType);
    nBytes+=::NeedBytesToStream(mi.SampleRate);
    nBytes+=::NeedBytesToStream(mi.FrameRate);
    nBytes+=::NeedBytesToStream(mi.TotalFrame);
    nBytes+=::NeedBytesToStream(mi.NeedKDM);
    nBytes+=::NeedBytesToStream(mi.mxfbinAesKey);
    nBytes+=::NeedBytesToStream(mi.sName);
    nBytes+=::NeedBytesToStream(mi.sDriver);
    nBytes+=::NeedBytesToStream(mi.sProjectName);
    nBytes+=::NeedBytesToStream(mi.sDir);
    nBytes+=::NeedBytesToStream(mi.sFullPath);
    return nBytes;
}

size_t StreamPut(char*& pos,size_t& AccLen,const MediaFileInfo& mediaInfo)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,mediaInfo.isSuccess);
    StreamPut(pos,AccLen,mediaInfo.FileType);
    StreamPut(pos,AccLen,mediaInfo.SampleRate);
    StreamPut(pos,AccLen,mediaInfo.FrameRate);
    StreamPut(pos,AccLen,mediaInfo.TotalFrame);
    StreamPut(pos,AccLen,mediaInfo.NeedKDM);
    StreamPut(pos,AccLen,mediaInfo.mxfbinAesKey);

    StreamPut(pos,AccLen,mediaInfo.sName);
    StreamPut(pos,AccLen,mediaInfo.sDriver);
    StreamPut(pos,AccLen,mediaInfo.sProjectName);
    StreamPut(pos,AccLen,mediaInfo.sDir);
    StreamPut(pos,AccLen,mediaInfo.sFullPath);
    return pos-pBegin;
}



size_t StreamGet(char*& pos,size_t& AccLen,MediaFileInfo& mediaInfo)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,mediaInfo.isSuccess);
    StreamGet(pos,AccLen,mediaInfo.FileType);
    StreamGet(pos,AccLen,mediaInfo.SampleRate);
    StreamGet(pos,AccLen,mediaInfo.FrameRate);
    StreamGet(pos,AccLen,mediaInfo.TotalFrame);
    StreamGet(pos,AccLen,mediaInfo.NeedKDM);
    StreamGet(pos,AccLen,mediaInfo.mxfbinAesKey);
    StreamGet(pos,AccLen,mediaInfo.sName);
    StreamGet(pos,AccLen,mediaInfo.sDriver);
    StreamGet(pos,AccLen,mediaInfo.sProjectName);
    StreamGet(pos,AccLen,mediaInfo.sDir);
    StreamGet(pos,AccLen,mediaInfo.sFullPath);
    return pos-pBegin;
}


size_t NeedBytesToStream(const ProgramInfo& pi)
{
    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(pi.sCurrentPlayingPromgramName);
    nBytes+=::NeedBytesToStream(pi.sCurrKeyFilePath);
    nBytes+=::NeedBytesToStream(pi.sCurrKeyNotValidBefore);
    nBytes+=::NeedBytesToStream(pi.sCurrKeyNotValidAfter);
    return nBytes;
}



size_t StreamPut(char*& pos,size_t& AccLen,const ProgramInfo& Info)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,Info.sCurrentPlayingPromgramName);
    StreamPut(pos,AccLen,Info.sCurrKeyFilePath);
    StreamPut(pos,AccLen,Info.sCurrKeyNotValidBefore);
    StreamPut(pos,AccLen,Info.sCurrKeyNotValidAfter);
    return pos-pBegin;
}


size_t StreamGet(char*& pos,size_t& AccLen,ProgramInfo& Info)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,Info.sCurrentPlayingPromgramName);
    StreamGet(pos,AccLen,Info.sCurrKeyFilePath);
    StreamGet(pos,AccLen,Info.sCurrKeyNotValidBefore);
    StreamGet(pos,AccLen,Info.sCurrKeyNotValidAfter);
    return pos-pBegin;
}


size_t NeedBytesToStream(const PM_INFO& Info)
{
        size_t nBytes=0;
        nBytes+=NeedBytesToStream(Info.pmFullPath);
        nBytes+=NeedBytesToStream(Info.seekPos);
        nBytes+=NeedBytesToStream(Info.reelInfo);
        return nBytes;
}



size_t StreamPut(char*& pos,size_t& AccLen,const PM_INFO& Info)
{
        char* pBegin=pos;
        StreamPut(pos,AccLen,Info.pmFullPath);
        StreamPut(pos,AccLen,Info.seekPos);
        StreamPut(pos,AccLen,Info.reelInfo);
         return pos-pBegin;
}




size_t StreamGet(char*& pos,size_t& AccLen,PM_INFO& Info)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,Info.pmFullPath);
    StreamGet(pos,AccLen,Info.seekPos);
    StreamGet(pos,AccLen,Info.reelInfo);
    return pos-pBegin;
}


size_t NeedBytesToStream(const PM_SPLIT_POLICY& Info)
{
    return NeedBytesToStream(Info.pmInfo);
}

size_t NeedBytesToStream(const MainSpeakerCrossoverCoef& mccc)
{
    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(mccc.EnableCrossover);   
    nBytes+=::NeedBytesToStream(mccc.co_speaker); 
    return nBytes;
}

size_t StreamPut(char*& pos,size_t& AccLen,const MainSpeakerCrossoverCoef& mccc)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,mccc.EnableCrossover);
    StreamPut(pos,AccLen,mccc.co_speaker);
    return pos-pBegin;
}

size_t StreamGet(char*& pos,size_t& AccLen,MainSpeakerCrossoverCoef& mccc)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,mccc.EnableCrossover);
    StreamGet(pos,AccLen,mccc.co_speaker);
    return pos-pBegin;
}



size_t StreamPut(char*& pos,size_t& AccLen,const PM_SPLIT_POLICY& Info)
{
    return StreamPut(pos,AccLen,Info.pmInfo);
}


size_t StreamGet(char*& buf,size_t& AccLen,PM_SPLIT_POLICY& Info)
{
    return StreamGet(buf,AccLen,Info.pmInfo);
}




EQCoeffAllMgr* GetEQCoeffAllMgr()
{
    static EQCoeffAllMgr gEQCoeffAllMgr;
    return &gEQCoeffAllMgr;
}

EQCoeffAllMgr::EQCoeffAllMgr()
{

}

EQCoeffAllMgr::~EQCoeffAllMgr()
{

}
