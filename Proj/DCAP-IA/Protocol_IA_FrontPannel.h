#ifndef  Protocol_DCAP_IA_FrontPannel_H
#define  Protocol_DCAP_IA_FrontPannel_H


//DCAP-IA-Core与前面板串口通讯协议

//说明:为了简化单片机程序的逻辑,前面版单片机内部程序为无状态协议,只负责把变化的动作发送给DCAP-IA-Core。
//具体的值由DCAP-IA-Core计算并返回通知单片机,并控制UI界面刷新。

#include <stdint.h>

enum DCAP_IA_SP_CMD_KEY
{
    SPCMD_UNKNOW,
    SPCMD_GET_MUTE_STATE,     //获取静音状态    前面版-->IA--预留不实现
    SPCMD_SET_MUTE_STATE,     //设置静音反转    前面版-->IA
    SPCMD_MUTE_STATE_RESP,   //静音状态返回     IA->前面版

    SPCMD_GET_DIM_STATE,       //获取衰减状态  前面版-->IA--预留不实现
    SPCMD_SET_DIM_STATE,       //设置衰减反转   前面版-->IA  协议预留,不实现
    SPCMD_DIM_STATE_RESP,     //衰减状态返回   IA->前面版   协议预留,不实现

    SPCMD_SELECT_VOLUME_DEVICE,           //选择设置音量的设备      前面版-->IA
    SPCMD_SELECT_VOLUME_DEVICE_RESP,  //选择结果返回                IA-->前面版

	SPCMD_SET_VOLUME_ADJUST_STATE,      //璁剧疆闊抽噺璋冩暣鐘舵€? 鍓嶉潰鐗?->IA
	SPCMD_SET_VOLUME_ADJUST_STATE_RESP, //璁剧疆鐘舵€佽繑鍥?     IA-->鍓嶉潰鐗

    SPCMD_GET_VOLUME,                    //获取当前音量  前面版-->IA--预留不实现
    SPCMD_SET_VOLUME_OFFSET,        //设置音量偏移  前面版-->IA
    SPCMD_VOLUME_RESP,                   //当前音量返回  IA-->前面版

    SPCMD_GET_CHANNEL_STATUS,      //请求各个声道音量状态        前面版-->IA
    SPCMD_CHANNEL_STATUS_RESP,    //各个声道音量状态变化通知  IA-->前面版
};

#pragma pack(push,1)     //一字节对齐 


enum 
{
    SP_HEAD=0xFECD,
};

//通信报文头
typedef struct 
{
    uint16_t head;           //恒为SP_HEAD
    uint16_t key;              //命令类型 为DCAP_IA_SP_CMD_KEY枚举值
    uint32_t FrameNum;  //预留,命令序号,
    uint32_t len;              //报文内容长度,不包含此报文头
}SHead;


//关于检验值,所有报文结尾都会填充int32型的检验值。
//设总报文长度为len（包含报文头),int32型号的checkSum放在len-4处。
//命令结构本身不再包含校验值。
//为了避免函数名冲突加IASP_前缀 IA serial port protocal

void IASP_initHead(SHead* pH)
{
    pH->head=SP_HEAD;
    pH->key=SPCMD_UNKNOW;
    pH->len=sizeof(int32_t);   //至少预留存放校验和的
}

//自定义简单校验码的获取方法
inline int32_t IASP_getCheckSum(const char* buf,uint32_t len)
{
    if (buf==0||len==0)
    {
        return 0;
    }
    int32_t res=0;
    for (uint32_t n=0;n<len;n++)
    {
        res+=buf[n];
    }
    return res;
}

inline void IASP_FillCheckSum(SHead* pH)   //发送前调用,自动在报文后添加校验值
{
    if (pH->len==0)
    {
        pH->len=sizeof(int32_t);
    }
    int32_t* posCheckSum=(int32_t*)((char*)pH+sizeof(SHead)+pH->len-sizeof(int32_t));
    *posCheckSum=IASP_getCheckSum((char*)pH,sizeof(SHead)+pH->len-sizeof(int32_t));
}

inline int IASP_doCheck(SHead* pH)        //接收后调用,校验失败抛弃此报文,返回0表示失败,1表示校验成功
{
    if (pH->len==0)
    {
        return 0;
    }
    int32_t* posCheckSum=(int*)((char*)pH+sizeof(SHead)+pH->len)-sizeof(int32_t);
    int32_t newCheckSum=IASP_getCheckSum((char*)pH,(sizeof(SHead)+pH->len-sizeof(int32_t)));
    if (newCheckSum==(*posCheckSum))
    {
        return 1;
    }
    return 0;
}


//静音反转设置命令,每次按下静音钮发送,无历史状态记忆
typedef struct 
{
    unsigned char MuteReverse;   //多余暂留
}DcapIAMuteReverseSet;

//静音状态返回
typedef struct 
{
    unsigned char MuteState;   //1表示静音,0表示非静音
}DcapIAMuteState;


//衰减反转设置命令,每次按下衰减钮发送在0dB与20dB之间切换--协议预留暂不实现
typedef struct 
{
    unsigned char DimReverse;   //多余暂留
}DcapIADimReverseSet;

//衰减状态返回
typedef struct 
{
    unsigned char DimState;   //1表示-20dB,0表示0dB
}DcapIADimState;


//音量设备选择命令,每次点击音量旋钮发送此命令,切换到下一个要调节的设备
typedef struct 
{
    char NextDevice;   //多余暂留
}DcapIAVolumeDeviceSet;

//选择音量设备后返回
typedef struct 
{
    char numDevice;   //当前设备号 0总音量,1是Mic1音量,2是Mic2音量
}DcapIAVolumeDeviceState;

//璁剧疆闊抽噺璋冩暣鐘舵€
typedef struct 
{
	char adjustState;   //褰撳墠闊抽噺璋冩暣鐘舵€侊紝1锛氳皟鏁寸姸鎬?0锛氶潪璋冩暣鐘舵€
}DcapIAVolumeAdjustState;

//音量旋钮设置命令
typedef struct 
{
    int32_t VolumeOffset;          //音量偏移值,正负表示增加或减少。左旋减小，右旋增大
}DcapIAVolumeSet;

//当前音量值返回
typedef struct 
{
    float currVolume;             //当前音量值0-10
}DcapIAVolumeState;


//请求各声道音量状态值
typedef struct 
{
    unsigned char  Reserved;   //保留
}DcapIAChannelStatusRequest;


//各声道音量状态通知命令—不请求也会定时通知
typedef struct 
{
    unsigned char hasSound[32];    //声道的音量0-255,暂时只区分有声无声0/1。为进一步扩展预留,根据音量值控制LED闪烁频率
}DcapIAChannelStatusResp;

#pragma pack(pop) //恢复原来的字节对齐状态

#endif

