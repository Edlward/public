#ifndef DEFINITION_H_
#define DEFINITION_H_

#if defined(_WIN32)
    #if defined(BASIC_API_EXPORT)
        #define BASIC_API __declspec(dllexport)
    #elif defined(BASIC_API_IMPORT)
        #define BASIC_API __declspec(dllimport)
    #else
        #define BASIC_API 
    #endif
#else
    #define BASIC_API 
#endif


//定义各状态名称的头文件  20159812 王领      --refactor by lizulin 2017-0915
//0 - 100: OBIA类可用取值范围 负数：错误代码

//////////////////////////////////////////////////////////////////////////
// 声卡声道定义、音轨定义、OBJECT定义
//////////////////////////////////////////////////////////////////////////
#define IN_TRACK_MAX	(128)
#define OUT_TRACK_MAX	(128)
#define TRACK_MAX		IN_TRACK_MAX
#define RING_BUFFER_NUM		(8)                                           //音频环形缓冲区个数
#define RING_MEM_NUM		((RING_BUFFER_NUM*2)+2)     //*2以指针形式使用时为了避免环形覆盖,理论上*2+1



#ifdef _WIN32
#define PREFIX  ""
#else
#define PREFIX  "/media/card/"
#endif

#define LHC_FILE_PATH "./conf.xml"                  //影厅配置文件默认保存路径
#define MACRO_FILE_PATH "./macro.xml"          //待整理


//#define BAND61  //均衡算法是否使用61频段否则为31


//跨进程的音频环形缓冲区个数用在AES和Dcap或 Madi与Mpu 
//一般情况一个先打开缓冲区立即被填满，另一个程序再取，可能会造成固有延时,要有条件清空
#define RING_BUFFER_PROCESS_NUM		(8)     
#define RING_MEM_PROCESS_NUM		    ((RING_BUFFER_PROCESS_NUM*2)+2) 

// *************一些常量的定义************** //
//#define BEDNUM               19        //IAbitstream v02
//#define BEDNUM                 (11)	       //IAbitstream v3
//#define NORMALCHANNEL  (1)


typedef enum 
{
    CSRate48K=0x0,
    CSRate96K
}SAMPLERATECODE;

typedef enum 
{
    SRate48K=48000,
    SRate96K=96000,
}SAMPLERATE;

typedef enum 
{
    CFRate24=0x0,  //C means Code
    CFRate25,
    CFRate30,
    CFRate48,
    CFRate50,
    CFRate60,
    CFRate96,
    CFRate100,
    CFRate120
}FRAMERATECODE;

typedef enum 
{
    FRate24=24,  //C means Code
    FRate25=25,
    FRate30=30,
    FRate48=48,
    FRate50=50,
    FRate60=60,
    FRate96=96,
    FRate100=100,
    FRate120=120
}FRAMERATE;


//默认帧率与采样率
const SAMPLERATE SRateDefault=SRate48K;
const FRAMERATE FRateDefault=FRate24;


//采样率值有效范围判断
inline bool isValidSampleRate(unsigned int SampleRate)
{
    if (SampleRate== SRate48K)
    {
        return true;
    }
    if (SampleRate== SRate96K)
    {
        return true;
    }
    return false;
}


//帧率值有效范围判断
static const unsigned int ArraryFrameRate[]={24U,25U,30U,48U,50U,60U,96U,100U,120U};
inline bool isValidFrameRate(unsigned int FrameRate)
{
    const unsigned int FRArrayNum=sizeof(ArraryFrameRate)/sizeof(ArraryFrameRate[0]);
    for (unsigned int n=0;n<FRArrayNum;n++)
    {
        if (FrameRate==ArraryFrameRate[n])
        {
            return true;
        }
    }
    return false;
}



// 系统初始状态
enum
{
    SYSTEMSIG_BEGIN=0X01,
    SYSTEMSIG_UNKNOW,       //             1
    SYSTEMSIG_RUN,               // 系统状态，正常运行（默认）
    SYSTEMSIG_SHUTDOWN,   // 系统状态，关机。
    //SYSTEMSIG_RESTART,      // 系统状态，重启
    SYSTEMSIG_END
};



//主要在这几种模式切换
enum WORKMODE
{
    MODE_BEGIN=0X0,         //在MODE_BEGIN到MODE_END之间的模式才是有效的 
    MODE_IDLE,                    //空闲模式，什么功能也没有，主要用在模式切换，以下模式之间不能切换必须回到IDLE模式
    MODE_MONITOR,           //监听模式，Mpu的监听，或者Dcap的正常播放
    MODE_PLAYBACK,           //Mpu,DCAP的播放列表的回放模式
    MODE_EQSET,                  //Mpu/Dcap的均衡调节模式

    MODE_StreamRecord,      //Mpu的录制模式
    MODE_StreamPlay,          //Mpu的录制后播放


    //待统一废弃
    //MODE_OBIA,               //模式，OBIA播放--废弃
    //MODE_NORMAL,        //模式，非OBIA播放--废弃
    //MODE_VBAPSET,        //模式，VBAP设置--废弃
    MODE_UNKNOW,
    MODE_END
};


enum STATUS
{
    STATUS_BEGIN=0X0,         //BEGIN/END之间的才是有效的 
    //STATUS_UNKNOW,            //未知,未确认状态
    STATUS_PLAY,                   // 状态，播放
    STATUS_PAUSE,                // 状态，暂停
    STATUS_STOP,                  // 状态，停止
    STATUS_END,
};

//废除意义不大的错误码
enum ERROR_CODE
{
    NOERR=0X00,
    ERR_BEGIN=-10000,
    ERR_SYSTEMSIG,
    ERR_MODE,
    ERR_STATUS,
    ERR_CHANGEMODE,
    ERR_SAMPLERATE,
    ERR_FRAMERATE,
    ERR_PAINIT,
    ERR_WRONGDEVICE,
    ERR_GAIN,
    ERR_CHANNEL,
    ERR_WAVEFORMID,
};

//#define IMPLAY               -1000 // 即时播放时间标记

// *************Routing Destinations************** //
enum
{
    FL=0X00,
    FR,
    FC,
    LFE,
    SL,
    SR,
    RL,
    RR,
    RC,
    HFL,
    HFR,
    HFC,
    HSL,
    HSR,
    HRL,
    HRR,
    HRC,
    TC
};

//阵列分配排序，记得要与Core的结构同步
// ****************** Bed Destinations ************* //
//下面用于从ATMOS/PROTOOLS/MPU传过来的BED声道定义
enum 
{
	B_L=0X00,     //Left Channel               //左声道
	B_R,              //Rigth Channel             //右声道
	B_C,              //Center Channel           //中置声道
	B_LFE,           //Sub Channel               //低音声道
	B_LSS,           //Left Side Surround      //左环绕声道
	B_RSS,          //Right Side Surround    //右环绕声道
	B_LRS,          //Left Rear Surround      //左后环绕声道     //7.1
	B_RRS,          //Right Rear Surround    //右后环绕声道    //7.1
	B_LTS,          //Left Top Surround        //左顶环绕声道    //9.1
	B_RTS,          //Right Top Surround     //右顶环绕声道    //9.1
};
//下面用于从DCS服务器传过来的16声道定义
#ifndef ISDCF
enum
{
    S_L=0X00,     //Left Channel               //左声道
    S_R,              //Rigth Channel             //右声道
    S_C,              //Center Channel           //中置声道
    S_LFE,           //Sub Channel               //低音声道
    S_LSS,           //Left Side Surround      //左环绕声道
    S_RSS,          //Right Side Surround    //右环绕声道
    S_LRS,          //Left Rear Surround      //左后环绕声道     //7.1
    S_RRS,          //Right Rear Surround    //右后环绕声道    //7.1
    S_LTS,          //Left Top Surround        //左顶环绕声道    //9.1
    S_RTS,          //Right Top Surround     //右顶环绕声道    //9.1
    S_Total_Channel,  //add by lizulin
    S_FSK=0X13//FSK Signal 13    FSK声道固定为13
};
#else
enum
{
	S_L=0X00,     //Left Channel               //左声道			0
	S_R,              //Rigth Channel             //右声道		1
	S_C,              //Center Channel           //中置声道		2
	S_LFE,           //Sub Channel               //低音声道		3
	S_LSS,           //Left Side Surround      //左环绕声道		4
	S_RSS,          //Right Side Surround    //右环绕声道		5
	S_HI,			//Hearing Impaired							6
	S_VI,			//visible Impaired/Narration				7
	S_LTS,          //Left Top Surround        //左顶环绕声道	8    //9.1
	S_RTS,          //Right Top Surround     //右顶环绕声道		9    //9.1
	S_LRS,          //Left Rear Surround      //左后环绕声道	10    //7.1
	S_RRS,          //Right Rear Surround    //右后环绕声道		11   //7.1
	S_DBP,			//D-BOX Primary,							12
	S_FSK=0X13,		//FSK Signal 13    FSK声道固定为13			13
	S_Total_Channel,  //add by lizulin
};
#endif

//BASIC_API const char* getBedName(unsigned int nBed);

/*
const char* getBedName(unsigned int nBed)
{
    if (nBed>=S_Total_Channel)
    {
        return "";
    }
    switch (nBed)
    {
    case S_L: return "L";                  //Left Channel               //左声道
    case S_R: return "R";                 //Rigth Channel             //右声道
    case S_C: return "C";                 //Center Channel           //中置声道
    case S_LFE: return "LFE";           //Sub Channel               //低音声道
    case S_LSS: return "LSS";           //Left Side Surround      //左环绕声道
    case S_RSS: return "RSS";          //Right Side Surround    //右环绕声道
    case S_LRS: return "LRS";          //Left Rear Surround      //左后环绕声道     //7.1
    case S_RRS: return "RRS";          //Right Rear Surround    //右后环绕声道    //7.1
    case S_LTS: return "LTS";          //Left Top Surround        //左顶环绕声道    //9.1
    case S_RTS: return "RTS";          //Right Top Surround     //右顶环绕声道    //9.1
    }
    return "";
}
*/


// ****************** Channel ID ******************* // 
enum 
{
    C_L=0X0,//Left screen speaker
    C_LC	,//Left center screen speaker
    C_C,//Center screen speaker
    C_RC	,//Right center screen speaker
    C_R,//Right screen speaker
    C_LS,//Left surround speaker(5.1)
    C_LSS,//Left side surround speaker(7.1)
    C_LRS,//Left Rear surround speaker(7.1)
    C_RRS,//Right rear surround speaker(7.1)
    C_RS,//Right surround speaker(5.1)
    C_RSS,//Right side surround speaker(7.1)
    C_LTS,//Left top surround speaker(9.1)
    C_RTS,//Right top surround speaker(9.1)
    C_LFE,//LFE speaker
    C_LW,//Left wide = left side front speaker
    C_RW,//Right wide = right side front speaker
    C_LTF,//Left top front speaker
    C_RTF,//Right top front speaker
    C_LH	,//Left screen height speaker(above L)
    C_RH,//Right screen height speaker(above R)
    C_CH,//Center screen height speaker(above C)
    C_LSH,//Left surround height speaker(above Lw)
    C_RSH,//Right surround height speaker(above Rw)

    //add by lizulin 
    C_LSSH,   //Left Side Surround Height
    C_RSSH,  //Right Side Surround Height
    C_LRSH,  //Left Rear Surround Height
    C_RRSH,  //Right Rear Surround Height
    C_CRS,	//Center Rear Surround
    C_CRSH, //Center Rear Surround Height
    C_CTS,	  //Top Surround

    C_Total_Channel,


    C_VOG,//Voice of God, Center top surround speaker
    C_W,//B-format omni-directional
    C_X,//B-format front-back pressure gradient
    C_Y,//B-format left-right pressure gradient
    C_Z,//B-format up-down pressure gradient

    C_SUB,
};


// ******************** Zones ********************** //
enum
{
    ZFLC=0X00,
    ZFC,
    ZFRC,
    ZFHLC,
    ZFHC,
    ZFHRC,
    ZBLC,
    ZBC,
    ZBRC,
    ZBHLC,
    ZBHC,
    ZBHRC,
    ZL,
    ZLH,
    ZR,
    ZRH,
    ZCLC,
    ZCC,
    ZCRC
};
//#define CC  5




struct ErrorCode
{
    enum ECode{
        Err_No=0,
        //Err_Begin=-10000,   //错误返回小于0
        Err_Mode,
        Err_State,
        Err_Total
    };
    const char* sName[Err_Total];
    //方式1
    ErrorCode(){
                sName[Err_Mode]="Err Mode";
                sName[Err_State]="Err State";
    }
    const char* getCodetoText(ECode eCode)
    {
        return sName[eCode];
    }

    //方式2
    static const char* ErrorCodetoString(ECode eCode)
    {
        const char *result;
        switch (eCode)
        {
            case  Err_Mode:   result="Err Mode";break;
            case  Err_State:    result="Err State";break;
            default:   result="unknow Error";break;
        }
        return result;
    }
};



//const char* NameUsers="users";
//const char* Nameadmin="admin";

//Dcap-UI登陆返回取得的权限
enum loginPrivileges
{
    loginPri_Unknow=-2,        //没有任何权限,初始状态
    loginPri_failed=-1,            //登陆直接返回失败
    loginPri_admin=0,            //管理者
    loginPri_users,                  //普通用户

    //精简权限
    //loginPri_technician,    
    //loginPri_manager,      
    //loginPri_operator       
};



//原版本 待删除
#if 0
//以下函数没有被用到，且不是跨平台的暂时屏蔽 review by lizulin 

#if 0    
string BASIC_API toUTF8(const wchar_t* buffer, int len);
string BASIC_API ToUTF8(const std::wstring& str);
wstring BASIC_API StringToWString(string s);
bool BASIC_API ReadUtf8ToString(const string &filename, string &content);
#endif
#endif

#endif  // DEFINITION_H_
