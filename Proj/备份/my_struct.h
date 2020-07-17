#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_

//暂时把几种算法头包含在此处
//#include "My_RootHeader.h"     //调试宏定义,类型重定义等头文件

#include <string>

#if 0
typedef char  int8;
typedef short int16;
typedef int   int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

//C99的
typedef long long int64;
typedef unsigned long long uint64;


//For _WIN
//typedef unsigned __int64 uint64;
//typedef __int64          int64;

//For _LINUX
//typedef long   long    int64;
//typedef unsigned long  long  uint64;

//typedef int64_t int64;
//typedef uint64_t uint64;



//再定义两种常用的
typedef unsigned char   uchar;
typedef unsigned int    uint;
#endif


#ifndef NULL
#define NULL 0
#endif

#ifdef DEBUG
#define DPRINTF(arg,...) PRINTF(arg)
#else
#define DPRINTF(arg,...)
#endif

//#pragma pack(2)       //old #pragma pack(1)
 
//#pragma pack(pop)     //恢复对齐状态
//#pragma pack(4)       //设定为4字节对齐

#pragma pack(push,1)     //设置一个字节对齐 保存对齐状态
//#pragma pack(1)

enum FILTER_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};

typedef enum
{
	KEY_NO=0,     //不加密
	KEY_HARD=1,   //硬件加密
    KEY_MEET=2    //会晤密钥加密
} KEY_TYPE;       //报文加密类型



typedef enum
{
    UI_TEST=0x19890512,           //测试命令，重UI发出,命令socket再命令客户端
    UI_SenDataDirect,                  //UI直接发送数据
    UI_SenDatatoUI,                    //发送数据给UI


    UI_SEND_UPDATE_FILE=0X1000,          //发送升级文件包
    UI_COLLPKG_START,                    //开始集包
    UI_GET_LOG                           //请求日志报文操作
} CMD_UItoSocket;    //UI与socket线程的通信命令，UI发命令socket



typedef enum
{
    SK_UPDATE_TREEWIDGET_LIST=0X2000,  //升级操作的UI内容列表
	SK_UPDATE_PROGRESSBAR,             //更新进度条
    SK_SEND_LOG_TOUI,                  //把获取到的日志内容传送给UI显示
    SK_COLLPKG_CONTINUE,               //本线程完成后集包未完成,通知UI,命令下一个socket线程继续集包
    SK_COLLPKG_COMPLETE,               //本线程完成后集包完成,通知UI显示集包完成.
    SK_COLLPKG_DIRISNULL,              //影片目录下影片文件为空,无法完成集包操作
    SK_COLLPKG_ISDOINGFILE             //回传正在集包的文件路径到UI显示
} CMD_SockettoUI;                      //UI与socket线程的通信命令




typedef enum
{
    SV_SOCKET_NEWCOMMING=0X3000,   //有新的client连接到来
    SV_SOCKET_DISCONNECT           //有socket断开
} CMD_TcpServertoUI;               //TcpServertoUI




typedef enum
{
    SOCKET_IDLE=0X5000,   //空闲
    SOCKET_CONNECTING,    //正在连接
    SOCKET_CONNECTED,     //已经连接
    SOCKET_DISCONNECT     //有socket断开
} CMD_ClientSockettoUI;               //TcpServertoUI




typedef enum
{
	//Sever端需要处理的命令加前缀S   Client端加C 太乱
    CMD_TEST=0X999,             //自定义测试命令
    PKG_HEAD=0x55,                   //数据包头
    CMD_ZERO=0x0000,                 //填充cmd_Sub
    CMD_REV=0x78563412,              //自定义保留字
    CMD_REV_LEONIS=0x74752485,           //自定义leonis保留字


	CMD_LOGIN_REQUEST= 0x0011,       //登录请求报文
	CMD_LOGIN_BACK,                  //登录反馈报文  
	CMD_AUTHEN_REQUEST,              //认证请求报文 
	CMD_AUTHEN_BACK,                 //认证反馈报文
    CMD_UPDATE_FILE_RECEV=0x0071,    //接收机远程升级包推送报文
    CMD_UPDATE_BACK,                 //接收机远程升级包推送反馈报文
    CMD_UPDATE_START,                //接收机远程升级请求报文,其实就是命令接收机开始升级

    CMD_HEARTBEAT_REQUEST=0x0031,    //心跳信息请求报文  server请求client
    CMD_HEARTBEAT_UPLOAD,            //心跳信息上报报文  client返回给server
	CMD_HEARTBEAT_CONFIRM,           //心跳信息确认      server向client确认

    CMD_PKGLOSS_UPLOAD=0X0022,     //丢包信息回传(上报)报文
    CMD_PKGLOSS_CONFIRM=0X0023,    //丢包信息确认报文

    CMD_FILLPKG_START=0X0090,      //补包开始报文
    CMD_FILLPKG_START_CONFIRM,     //补包开始确认报文
    CMD_FILLPKG,                   //补包报文
    CMD_FILLPKG_OK,                //补包完成报文

    CMD_COLLPKG_REQUEST=0X0094,    //集包请求报文
    CMD_COLLPKG_REQUEST_CONFIRM,   //集包请求确认报文
    CMD_COLLPKG_START,             //集包开始报文
    CMD_COLLPKG_START_CONFIRM,     //集包开始确认报文
    CMD_COLLPKG,                   //集包报文
    CMD_COLLPKG_OK,                //集包完成报文,本轮

    CMD_LOG_REQUEST=0x0100,            //日志请求报文
    CMD_LOG_BACK,                      //日志回传报文

    CMD_LOGFILE_REQUEST=0x0034,       //日志文件请求报文
    CMD_LOGFILE_BACK=0x0035          //日志文件反馈报文


} COMMOND_TYPE;



typedef enum
{
   CMD_PH_HEAD=0x74752485,           //自定义leonis保留字
   CMD_BEDDATA_KEY=0x02,             //自定义leonis保留字
   CMD_OBJECTDATA_KEY=0x05           //自定义leonis保留字
}OBIA;



//OBIA通信协议结构体
typedef struct _PH_             //HEADER 简写HD  Package Header 简写KH  pKH比pPH好看
{
    uint32 Key;	       //Value is 0x74758524, it means the package is a meta-data command package.
    uint32 Length;	   //The length of total struct in bytes, including the key and length.
//Value variable Meta-data
}PH;



typedef struct _BEDDATA_
{
    uint16 Key;	    //Value is 0x2.
    uint32 Length;	//The length of total struct in bytes, including the key and length.
    uint32 CH_id;	//It’s means audio track number, such as 1, 2, 10.
    uint32 Time;    //Time code, it is the index of frame.
    uint16 Routing_destination;	//See below(routing_destination table)
}BEDDATA;


/*
Routing_destination table
VALUE	LOCATION
0	Left
1	Right
2	Center
3	LFE
4	Side Left
5	Side Right
6	Rear Left
7	Rear Right
8	Height  Left
9	Height  Right
*/
typedef struct _OBJECTDATA_
{
    uint16 Key;      //Value is 0x5.
    uint32 Length;   //The length of total struct in bytes, including the key and length.
    uint32 CH_id;	 //It’s object id number, such as 11,12 to 128.
    uint32 Time;     //Time code, it is the index of frame.
    float X;           //	Float(-1~1)	Left wall: -1 Right wall: 1
    float Y;           //	Float(-1~1)	Screen: -1 Back wall: 1
    float Z;           //	Float(-1~1)	Ceiling: 1 Floor: -1
    float Extent;      //(non-negative)	The extent(size) of the object in the room, measured as radius from the object position. 0 indicating a point source.
    float Gain;        //(non-negative)	This linear gain scales the level the audio is presented at, which is otherwise determined by the waveform level and renderer.
    float Coherence;   //(0~1)	For objects with non-zero extent, coherence affects the coherence of sound elements arriving from different directions. 1 indicating full coherence.
    float Snap;        //(non-negative)	Snap arranges for an object to jump to a speaker if one is nearby. 0 indicating no snap.
}OBJECTDATA;


//记录一个对象坐标以及其是否有效，可见
typedef struct _OBJ_
{
    bool Enable;	   //
    float X;           //	Float(-1~1)	Left wall: -1 Right wall: 1
    float Y;           //	Float(-1~1)	Screen: -1 Back wall: 1
    float Z;           //	Float(-1~1)	Ceiling: 1 Floor: -1
}OBJ;


//保存一帧内128个对象坐标，及其是否可见
typedef struct _FRAMEOBJ_
{
    OBJ ObjNum[128];
}FRAMEOBJ;














//与操纵杆JOYSTICK交互相关结构体
typedef struct _KHJOY_        //一般命令三个字节
{
    unsigned char KeyHead;
    unsigned char Key2;
    unsigned char Key3;
}KHJOY;

typedef struct _KHJOYPOS_   //传递坐标值的命令五个字节
{
    KHJOY KHjoy;
    unsigned char Key4;
    unsigned char Key5;
}KHJOYPOS;

typedef struct _POS_    //保存操纵杆回传的坐标值结构体
{
    uint32 CH_id;	 //It’s object id number, such as 11,12 to 128.
    uint32 Time;     //Time code, it is the index of frame.
    float posX;
    float posY;
    float posZ;
}POS;






//服务器server端保存的已经注册的接收机ID及其硬件指纹
typedef struct _RegisterIDHARDKEY_
{
    uint32  ID;
    char HardKey[16];
}RegisterIDHARDKEY;



//网络通讯协议报文基本结构
//嵌入以下所有报文前面,作为报文头 BASEprotocol
typedef struct _KH_             //HEADER 简写HD  Package Header 简写KH  pKH比pPH好看
{
	uint8   pkgHead;            //preamble;//8位无符号整型，起始同步字段，恒为(0x55)
	uint8   flag_pwd;           //加密标记 0	报文未加密 1报文采用硬件指纹加密 2	报文采用会唔密钥加密
	uint32  rev;                //reserved保留字 
	uint16  cmd;                //该报文的主命令字
	uint16  cmd_Sub;            //该报文的辅助命令字 Sub_cmd
    uint32  cmd_length;         //Payload_length;//报文载荷内容长度，(包含其内容本身+其CRC32校验值)
    //uint8  Payload_data_byte;  
}KH;



//登陆请求报文  128位字段 16字节
typedef struct _LOGINREQUEST_
{
	KH kh;
	uint32 Machine_ID;          //表示接收机的ID号，本字段不加密
	uint32 LoginCounter;        //接收机本次启动后的登录次数
	char   SoftVersion[16];     //bslbf 128位字段 表示接收机当前接收软件的版本号，由厂家自行定义
	char   Time_Startup[20];    //bslbf 160位字段 表示接收机启动时间 格式为YYYY-MM-DD HH:MM:SS
	char   Time_Login[20];      //bslbf 160位字段 表示接收机登陆时间 格式为YYYY-MM-DD HH:MM:SS
    uint32 CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}LOGINREQUEST;




//登陆反馈报文
typedef struct _LOGINBACK_
{
	KH kh;
    char    ReSeverIP[16];         //重定向业务服务器IP 128位字段，表示重定向业务服务器的IP地址，格式为”xxx.xxx.xxx.xxx”。
    uint32  ReSeverPort;           //重定向业务服务器IP 端口号
    uint32  Year;                  //系统当前年份,月,日，时分秒
    uint32  Month;                 //32位无符号整型，表示系统当前月份，取值范围1～12。  
    uint32  Day;                   //32位无符号整型，表示系统当前日期，取值范围1～31。
    uint32  Hour;                  //32位无符号整型，表示系统当前小时，取值范围0～24。
    uint32  Minute;                //32位无符号整型，表示系统当前分钟，取值范围0～59。
    uint32  Seconds;               //32位无符号整型，表示系统当前秒钟，取值范围0～59。
    uint32 CRC32;                  //rpchof 本结构体以上字段加密前的CRC校验值
}LOGINBACK;



//认证请求 authentication request
typedef struct _AuthenREQUEST_
{
	KH kh;
	uint32  Machine_ID;           //表示接收机的ID号。本字段不加密。
    char    SoftVersion[16];      //bslbf  128位字段 表示接收机当前接收软件的版本号，由厂家自行定义
    char    Time_Login[20];       //bslbf  160位字段 表示接收机登陆时间 格式为YYYY-MM-DD HH:MM:SS
    uint32  Model_Log;            //日志模式 表示接收机是否上报日志。此字段值为1，表示接收机上报日志；值为2，表示不上报日志。
	uint32	Reserved1;			  //Reserved
    uint32  BeatCycle;            //心跳周期heartbeat cycle 表示接收机上报心跳数据报文的周期，单位为秒。如果确认的心跳周期与接收机上报的周期不同，接收机上报周期以此为准。
    uint32  Model_Connect;        //连接方式 表示接收机与招收机远程维护控制子系统保持连接的方式。此字段值为1，表示与发送系统平台保持不间断的长期连接；值为0，表示与发送系统平台建立短连接，即接收机只当有数据传输需要时才与接收机远程维护控制子系统建立连接。
    uint32  HardDiskNum;          //32位无符号整型，表示接收机当前挂载的用于接收电影数字拷贝的硬盘数量。
    uint64  AllDiskCapacity;      //64位无符号整型，表示接收机数据硬盘的容量，单位为字节。
	uint32	Reserved2;			  //Reserved
    uint32  CRC32;                //rpchof 本结构体以上字段加密前的CRC校验值
}AuthenREQUEST;


//认证反馈 authentication feedback
typedef struct _AuthenBACK_
{
	KH kh;
	char    MeetKey[16];          //会唔密钥128位字段，接收机远程维护控制子系统，数据补包子系统与接收机后续报文的加密密钥。
    char    RandomCode[16];       //随机码128位字段，在重要报文中携带，以供接收机验证。
    uint32  Model_Log;            //日志模式32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的日志模式。
    uint32  reserved;             //保留字  
    uint32  BeatCycle;            //心跳周期 32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的心跳周期。
    uint32  Model_Connect;        //连接方式 32位无符号整型，接收机远程维护控制子系统向接收机确认其上报的连接方式。
	uint32  CRC32;                //rpchof 本结构体以上字段加密前的CRC校验值
}AuthenBACK;



//升级包推送报文 (仅包含文件信息,文件内容后续传输)  升级包Upgradepackage
typedef struct _UpdateFileINFO_
{
	KH kh;
    uint64  reserved;              //保留字
	char    UpdateSerialNo[16];    //128位字段，软件升级包的序列号。
    char    reserved2[16];         //保留字段2
    uint32  DescriptionLength;     //32位无符号整型，升级软件描述信息长度。
    char    Description[256];      //可变长字段，升级软件描述信息。(暂定256位字符串)
    uint32  FID;                   //32位无符号整型，传送升级软件与配置文件的ID号。
    uint32  UpdateFileNameLength;  //32位无符号整型，升级软件文件名长度。
    char    UpdateFileName[256];   //可变长字段，升级软件文件名。   (暂定256位字符串)
    uint32  UpdateFileLength;      //32位无符号整型，最大可以表示4GB,升级软件文件长度。如升级软件通过互联网传送，需指明升级软件文件长度；如升级软件通过卫星信道传送，该项取值为0。
    //char UpdateFile[1024];       //可变长字段，升级软件文件。如升级软件通过互联网传送，此项为传送的升级软件文件的；如升级软件通过卫星信道传送，此项不存在。
	//uint32  CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileINFO;
//传送完成结构体以上后,在传送文件本身,然后传送文件CRC校验码



/*
//自定义文件分段发送的结构体
typedef struct _SendFileInfo_
{
    char    FileName[256];          //要传送的文件名
    char    FilePath_dst[256];      //文件目的路径
    char    FilePath_rsc[256];      //文件目的路径
    uint64  FileSize;               //文件总大小,单位字节Byte
    uint32  FileCRC32;              //文件内容本身CRC32校验
    char    FileMD5[32];            //文件内容本身MD5
    uint32  FilePkgAll;             //总文件包数    一个大文件分包传送,64K每个包
    uint32  FilePkgCurrent;         //当前是第几个包
    uint32  FileEndFlag;            //当前包是否是最后一个包,是为1,否为0,
    uint32  FileEndPos;             //如果当前包最后一个包,记录结束位置,单位字节,否则忽略
    char    FileBuf[64*1024];       //文件包缓存.
}SendFileInfo;
*/



//接收机远程升级包推送反馈报文  接收机在收到升级包后的反馈。
typedef struct _UpdateFileBACK_
{
	KH kh;
    uint32  UpdateCheckResult;      //32位无符号整型，升级确认。值为0表示升级包有效，1表示升级包无效。
    uint32  reserved;               //保留字段
    char  OldVersion[16];           //128位字段，接收软件升级前的版本信息。
    char  UpdateSerialNo[16];       //128位字段，接收的软件升级包序列号。
    char  reserved2[16];            //保留字段
	uint32  CRC32;                  //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileBACK;


//接收机远程维护控制子系统在接收到远程升级包携眷反馈报文后向接收机发出的升级请求。
//命令接收机开始升级
typedef struct _UpdateSTART_
{
	KH kh;
    char  UpdateSerialNo[16];          //128位字段，接收软件升级后的版本信息。
    uint32  reserved;
    uint32  CRC32;                     //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateSTART;





//日志请求报文,向接收机请求其日志
typedef struct _LOGREQUEST_
{
	KH kh;
    uint32  DateStart;   //日志文件起始日期 32位无符号整型，为YYYYMMDD十进制表示。
    uint32  DateEnd;     //日志文件结束日期 32位无符号整型，为YYYYMMDD十进制表示。
	uint32  LogType;     //日志类型 32位无符号整型，为0x74752485，表示日志为LEONIS专有格式；其它值为科研所格式（目前科研所没定义具体格式）。
	uint32  CRC32;       //CRC32 32位字段，以上字段加密前的CRC校验值。  
}LOGREQUEST;


//日志反馈报文,仅仅报文头不包含内容本身,接收机反馈给节目管理中心
typedef struct _LOGBACKINFO_
{
	KH kh;  
    uint32  LogLengthALL;   //日志文件总长度   uint32，接收机日志文件的总长度。
    uint32  DateStart;      //日志文件起始日期  uint32，为YYYYMMDD十进制表示。
    uint32  DateEnd;        //日志文件结束日期  uint32，为YYYYMMDD十进制表示
    uint32  LogLengthCurr;  //本次上传内容长度 uint32 本次上传的日志内容长度。
//	char    log[1024*10]; 	//日志内容本身可变字段, variable	  bslbf
//	uint32  CRC32;          //          
}LOGBACKINFO;
//一般日志文件比较小,暂时定长10K,或者更多,没有用0填充 方便处理



//心跳信息请求报文             //server请求client
typedef struct _HEARTBEATREQUEST_
{
    KH kh;                   //只有报文头,无内容
}HEARTBEATREQUEST;


//心跳信息上报报文             //client发给server
typedef struct _HEARTBEATUPLOAD_
{
	KH kh; 
    char resv[168+256];      //其他状态信息，因为用不到,暂不定义,其他168字节,影片名称暂定256
}HEARTBEATUPLOAD;


//心跳信息确认报文             //server向client确认
typedef struct _HEARTBEATCONFIRM_
{
    KH kh;                   //只有报文头,无内容
}HEARTBEATCONFIRM;








//自定义升级文件路径与模式结构体, 用在UI发生到socket路径解析
typedef struct _UPDATEMODELPATH_
{
	int SendMode;       //发送类型，0表示用卫星传送,1表示用互联网传送
    char Path[256];     //要发送的文件路径,当SendMode为0时无用。
}UPDATEMODELPATH;



/*
//日志类型
 typedef enum {
	LOG_ALL = 0,
	LOG_UI,
	LOG_SYSTEM,
	LOG_ERROR,
	LOG_DVB,
	LOG_TSPARSER,
	LOG_NETCOMMU,
	LOG_SATELLITE,
	LOG_BASIC,
	LOG_CONFIG,
	LOG_TMS,
	LOG_END
}LOGTYPE;
*/

//////////////////////////////////////////////////////////////////
//数据补包相关结构体,
//【丢包信息回传报文】--用于接收机向数据补包子系统上报丢包信息。
typedef struct _PKGLOSSUPLOAD_HEAD_    //报文头
{
	KH kh; 
    uint32 FilmID;           //丢包信息的影片ID号。
	uint32 PkglossNumber;    //影片所丢失的包数量
    uint64 BytesRecv;        //该影片已接收的字节数。
    uint32 State;            //本轮接收完成后影片的接收状态，接收状态定义见下表。
	uint32 SegmentLength;    //表示数据的分割块大小。
	uint32 PkglossLength;    //丢包信息文件的长度，单位为字节。
	//variable               //可变长度，为丢包信息文件内容。
}PKGLOSSUPLOAD_HEAD;



typedef struct _PKGLOSSBACK_END_     //报文尾部
{
//	uint32  PkglossFileNameLength;    //丢包文件名长度 
//  char   PkglossFileName[256];     //variable 丢包文件名,为了方便实现定义为定长256字符串
//	uint32 CRC32;
}PKGLOSSBACK_END;

/*
影片接收状态		取值
空闲				0
数据接收			1
接收结束			2
丢包分析			3
reserved		4
丢包信息上报		5
丢包信息上报确认	6
请求MD5与密钥文件	7
MD5与密钥接收确认	8
数据校验			9
数据校验成功		10
任务完成			11
*/


//【丢包信息确认报文】--用于数据补包系统对接收到丢包信息的确认。
typedef struct _PKGLOSSCONFIRM_
{
	KH kh; 
	uint32 FilmID;     //丢包信息的影片ID号。
    uint64 rev;        //reserved保留字
	uint32 CRC32;
}PKGLOSSCONFIRM;
//rev=CMD_REV_LEONIS;//扩充命令,填充某个字段,表示搜索不到对应文件，无法完成补包



//【补包开始报文】--该报文由数据中心发出，用于发送数据包，每个数据包单独发送。
typedef struct _FILLPKGSTART_HEAD_
{
	KH kh; 
    uint32 FilmID;                   //丢包信息的影片ID号。
	uint32 SegmentLength;            //表示数据的分割块大小。
	uint32 PkglossFileNameLength;    //丢包文件名长度 
//  char   PkglossFileName[256];     //variable 丢包文件名,为了方便实现定义为定长256字符串
//	uint32 CRC32;
}FILLPKGSTART_HEAD;




//【补包开始确认报文】--该报文由数据中心发出，用于发送数据包，每个数据包单独发送。
typedef struct _FILLPKGSTARTCONFIRM_
{
	KH kh; 
    uint32 FilmID;             //丢包信息的影片ID号。
	uint32 CRC32;
}FILLPKGSTARTCONFIRM;



//【补包报文】--该报文用于发送数据包，每个数据包单独发送。
typedef struct _FILLPKG_HEAD_
{
	KH kh; 
	uint32 FilmID;             //丢包信息的影片ID号。
    uint32 SegmentNumber;      //数据分块序号。该序号从0开始。
	uint32 PkgLength;          //数据包的长度，单位为字节。
//  variable                   //为数据包内容。
//	uint32 CRC32;
}FILLPKG_HEAD;
//仅数据包信息头,接着发送数据本身和CRC



//【补包完成报文】--该报文用于标记本轮补包完成。
typedef struct _FILLPKGOK_
{
	KH kh; 
	uint32 FilmID;              //丢包信息的影片ID号。
	uint32 CRC32;
}FILLPKGOK;



//【集包请求报文】--该报文由数据中心发出，用于向设备请求数据包。
typedef struct _COLLPKGREQUEST_HEAD_
{
	KH kh; 
    uint32 FilmID;                   //丢包信息的影片ID号。
	uint32 PkglossFileNameLength;    //丢包文件名长度 
//  char PkglossFileName[256];       //variable 丢包文件名,为了方便实现定义为定长256字符串
//	uint32 ContentLength;            //集包内容长度
//  variable Content;                //为集包文件内容。集包文件格式定义见附录A。
//  uint32 CRC32;
}COLLPKGREQUEST_HEAD;



//【集包请求确认报文】该报文由设备发出，用于通知数据中心设备已准备好发送数据包，每个数据包单独发送。
typedef struct _COLLPKGREQUESTCONFIRM_
{
    KH kh;
    uint32 FilmID;            //丢包信息的影片ID号。
    uint64 rev;               //reserved保留字
    uint32 CRC32;
}COLLPKGREQUESTCONFIRM;
//rev=CMD_REV_LEONIS;//扩充命令,填充某个字段,表示搜索不到对应文件，无法完成补包


//【集包开始报文】--该报文由设备发出,用于设备对数据中心，开始补包
typedef struct _COLLPKGSTART_HEAD_
{
    KH kh;
    uint32 FilmID;                    //丢包信息的影片ID号。
    uint32 SegmentLength;            //表示数据的分割块大小。
    uint32 PkglossFileNameLength;    //丢包文件名长度
//  char   PkglossFileName[256];     //variable 丢包文件名,为了方便实现定义为定长256字符串
//	uint32 CRC32;
}COLLPKGSTART_HEAD;



//【集包开始确认报文】--该报文由数据中心发出，用于发送数据包，每个数据包单独发送。
typedef struct _COLLPKGSTARTCONFIRM_
{
    KH kh;
    uint32 FilmID;               //丢包信息的影片ID号。
    uint32 CRC32;
}COLLPKGSTARTCONFIRM;




//【集包报文】--该报文用于发送数据包，每个数据包单独发送。
typedef struct _COLLPKG_HEAD_
{
    KH kh;
    uint32 FilmID;              //丢包信息的影片ID号。
    uint32 SegmentNumber;       //数据分块序号。该序号从0开始。
    uint32 PkgLength;           //数据包的长度，单位为字节。
//  variable                    //为数据包内容。
//	uint32 CRC32;
}COLLPKG_HEAD;
//仅数据包信息头,接着发送数据本身和CRC

//【集包完成报文】--该报文用于标记本轮集包完成。
typedef struct _COLLPKGOK_
{
    KH kh;
    uint32 FilmID;              //丢包信息的影片ID号。
    uint32 CRC32;
}COLLPKGOK;






//自定义测试报文--无用
typedef struct _PAYLOADTEST_
{
    KH kh;
    char rev[256];
    uint32 CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}PAYLOADTEST;




//typedef struct _KL_
//{
//	int m_pkgHead;
//	int m_keyID;
//	int m_length;
//	int m_flag;               //用-1填充,返回0，表明数据包发送成功。
//}KL;


/*
typedef struct _KL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
	int    m_flag;           //用-1填充,返回0，表明数据包发送成功。
public:
	_KL_()
	{
		m_pkgHead = 0x7585;   //自动调用构造函数
	}
} KL;
*/


typedef struct _SL_
{
	uint16 m_sID;
	uint16 m_length;
} SL;


typedef struct tuner_conf {
	unsigned int nFreq;
	unsigned int nHiBand;
	unsigned int nSR;
	unsigned int nMis;
	unsigned int nSatNo;
	std::string strDevName;		//Such as "/dev/dvb/adapter0/frontend0"
	std::string strDelSys;		//Such as "DVB-S", "DVB-S2"
	std::string strFec;			//Such as "1/2", "2/3", "3/4", "4/5", "5/6"
	std::string strModulation;	//Such as "8PSK", "16APSK"
	std::string strRollOff;		//Such as "0.20", "0.25", "0.35"
	std::string strPolVert;		//Such as "V", "H"

	//for default
	tuner_conf()
	{
		strDevName = "/dev/dvb/adapter0/frontend0";
		strDelSys = "DVB-S2";
		strFec = "3/4";
		strModulation = "8PSK";
		strRollOff = "0.25";
		strPolVert = "V";
		nFreq = 12500000;
		nHiBand = 11300000;
		nSR = 43200000;
		nMis = -1;
		nSatNo = 1;
	};
} TUNER_CONF;

typedef struct network_conf {
	uint8 nDhcp;
	std::string strDevName;
	std::string strIp;
	std::string strNetmask;
	std::string strGateway;
} NETWORK_CONF;

typedef struct remote_conf {
	std::string strDns1;
	std::string strDns2;
	std::string strRemote;
} REMOTE_CONF;



#pragma pack(pop)
#endif

