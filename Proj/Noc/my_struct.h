#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_


//#include "../Basic/global.h"

#include "mytype.h"

#pragma pack(push)
#pragma pack(1)

typedef enum
{
	KEY_NO=0,     //不加密
	KEY_HARD=1,   //硬件加密
	KEY_MEET=2,   //会晤密钥加密
} KEY_TYPE;       //报文加密类型


typedef enum
{
	UI_SEND_UPDATE_FILE=0X1000,        //发送升级文件包
	UI_GET_LOG,                          //请求日志报文操作 
	UI_GET_MD5RES,
	UI_REBOOT,
	UI_LOG,
	UI_TMS_LIST,
	UI_TMS_INFO,
	UI_CLOSE_SOCKET,
} CMD_UItoSocket;    //UI与socket线程的通信命令，UI发命令socket



typedef enum
{
	SK_UPDATE_TREEWIDGET_LIST=0X2000,      //升级操作的UI内容列表
	SK_UPDATE_PROGRESSBAR,             //更新进度条

	SK_UPDATE_TEXTBROWSER,

} CMD_SockettoUI;    //UI与socket线程的通信命令

typedef enum
{
	SV_SOCKET_DISCONNECT=0X3000,    //有socket断开

} CMD_TcpServertoUI;    //TcpServertoUI


typedef enum
{
	//Sever端需要处理的命令加前缀S   Client端加C 太乱
	M_SEND_TEST,
	PKG_HEAD=0x55,           //数据包头
	CMD_ZERO=0x0000,        //填充cmd_Sub 
    CMD_REV=0x78563412,      //自定义保留字


	CMD_LOGIN_REQUEST= 0x0011,      //登录请求报文
	CMD_LOGIN_BACK,                  //登录反馈报文  
	CMD_AUTHEN_REQUEST,              //认证请求报文 
	CMD_AUTHEN_BACK,                 //认证反馈报文
	CMD_LOST_REPORT = 0x22,
	CMD_MD5_REQUEST = 0x24,
	CMD_MD5_RES_UP_REQUEST=0x28,
    CMD_UPDATE_FILE_RECEV=0x0071,   //接收机远程升级包推送报文
    CMD_UPDATE_BACK,                 //接收机远程升级包推送反馈报文
    CMD_UPDATE_START,                //接收机远程升级请求报文,其实就是命令接收机开始升级

	CMD_UPDATE_FILE_CUSTOM=0x8071,   //自定义接收机远程升级包推送报文
	CMD_UPDATE_BACK_CUSTOM,                 //自定义接收机远程升级包推送反馈报文
	CMD_UPDATE_START_CUSTOM,                //自定义接收机远程升级请求报文,其实就是命令接收机开始升级

    CMD_LOG_REQUEST=0x0100,           //日志请求报文
    CMD_LOG_BACK,                      //日志回传报文

	CMD_HEARTBEAT_REQUEST=0x0031,
	CMD_HEARTBEAT=0x0032,
	CMD_HEARTBEAT_CONFIRM=0x0033,

    CMD_LOGFILE_REQUEST=0x0034,       //日志文件请求报文
    CMD_LOGFILE_BACK=0x0035,          //日志文件反馈报文

	CMD_UI_CAST_UPLOAD=0x0ccb,			//Cast升级包上传
	CMD_UI_CAST_UPLOAD_ACK=0x0cc1,		//Cast升级包上传确认
	CMD_UI_ISSUE_UPLOAD=0x0cbb,			//Issue升级包上传
	CMD_UI_ISSUE_UPLOAD_ACK=0x0cb1,		//Issue升级包上传确认
	CMD_UI_UPLOAD_FIN=0x0ccc,				//Cast/Issue升级包上传结束报文
	CMD_UI_REMOTE_UPDATE=0x0cca,			//更新远程接收机
	CMD_UI_REMOTE_UPDATE_ACK=0x0cc2,		//远程接收机更新进度反馈
	CMD_UI_REMOTE_REBOOT=0x0cce,			////远程重启
	CMD_UI_DISCONNECT=0x0ccd,				//UI关闭，断开连接
	CMD_UI_REMOTE_LOG_REG=0x0ca1,			//UI远程日志请求
	CMD_UI_REMOTE_LOG_ACK=0x0ca2,			//UI远程日志应答


    //test 
    CMD_TEXT_MESSAGE=0XF5F5,
} COMMOND_TYPE;



//服务器server端保存的已经注册的接收机ID及其硬件指纹
typedef struct _RegisterIDHARDKEY_
{
    uint32  ID;
    char HardKey[16];
}RegisterIDHARDKEY;



//网络通讯协议报文基本结构
//嵌入以下所有报文前面,作为报文头 BASEprotocol
typedef struct _KH_           // HEADER 简写HD  Package Header 简写KH  pKH比pPH好看
{
	uint8   pkgHead;            //preamble;//8位无符号整型，起始同步字段，恒为(0x55)
	uint8   flag_pwd;           //加密标记 0	报文未加密 1报文采用硬件指纹加密 2	报文采用会唔密钥加密
	uint32  rev;                //reserved保留字 
	uint16  cmd;                //该报文的主命令字
	uint16  cmd_Sub;            //该报文的辅助命令字 Sub_cmd
    uint32  cmd_length;         //Payload_length;//报文载荷内容长度，(包含其内容本身+其CRC32校验值)
    //uint8  Payload_data_byte;  
}KH;

//自定义UI升级包推送报文
typedef struct _UIUPLOADSEG_
{
	KH kh;
	uint32  SegmentNo;               //64位无符号整型，分片编号，从0开始，每传一个包递加1。 
	uint32  DataLength;			//32位无符号整型，本分片传输的数据长度
	//char		Data[1024*128];       //可变长字段，升级软件文件。如升级软件通过互联网传送，每次128K
	//uint32  CRC32;  
}UIUploadSeg;

//自定义UI升级包推送报文确认
typedef struct _UIUPLOADSEGACK_
{
	KH kh;
	uint32  SegmentNo;               //64位无符号整型，分片编号，从0开始，每传一个包递加1。
	uint8   status;
	//uint32  DataLength;			//32位无符号整型，本分片传输的数据长度
	//char		Data[1024*128];       //可变长字段，升级软件文件。如升级软件通过互联网传送，每次128K
	uint32  CRC32;  
}UIUploadSegAck;

typedef struct _UIREMOTEUPDATE_
{
	KH kh;
	int32  macID;		//32位无符号整型，升级设备id
	uint32  CRC32;  
}UIRemoteUpdate;

typedef struct _UIREMOTEUPDATE_ACK
{
	KH kh;
	int32  macID;		//32位无符号整型，升级设备id
	int32  sendSize;
	int32  fileSize;
	uint32  CRC32;  
}UIRemoteUpdate_Ack;

typedef struct _UIREMOTEREBOOT_
{
	KH kh;
	int32  macID;		//32位无符号整型，升级设备id
	uint32  CRC32;  
}UIRemoteReboot;

typedef struct _UIREMOTELOGREQ_
{
	KH kh;
	int32  macID;		//32位无符号整型，升级设备id
	uint32  CRC32;  
}UIRemoteLogReg;

typedef struct _UIREMOTELOG_ACK_
{
	KH kh;
	uint32  macid;               //64位无符号整型，分片编号，从0开始，每传一个包递加1。 
	uint32  DataLength;			//32位无符号整型，本分片传输的数据长度
	//char		Data[1024*128];       //可变长字段，升级软件文件。如升级软件通过互联网传送，每次128K
	//uint32  CRC32;  
}UIRemoteLog_Ack;

//登陆请求报文  128位字段 16字节
typedef struct _LOGINREQUEST_
{
	KH kh;
	uint32 Machine_ID;          //表示接收机的ID号，本字段不加密
	uint32 LoginCounter;        //接收机本次启动后的登录次数
	char   SoftVersion[16];     //bslbf 128位字段 表示接收机当前接收软件的版本号，由厂家自行定义
	char   Time_Startup[20];    //bslbf 160位字段 表示接收机启动时间 格式为YYYY-MM-DD HH:MM:SS
	char   Time_Login[20];      //bslbf 160位字段 表示接收机登陆时间 格式为YYYY-MM-DD HH:MM:SS
    uint32 CRC32;                //rpchof 本结构体以上字段加密前的CRC校验值
}LOGINREQUEST;


typedef struct _HeartBeat_1_
{
	KH kh;
	uint32 machineID;//  接收机的机器号
	uint32 temperature;//  接收机的运行温度
	uint32 CPUUsage;//  CPU使用率
	uint64 memTotal;//  内存总数
	uint64 memIdle;//  内存空闲数
	uint64 networkRate;//  互联网数据速率
	uint32 agc; //	卫星信道的信号强度 1-100
	uint32 snr;//  卫星信道的信号质量 1-100
	uint32 interfaceRate;//  接口接收速率 Mb/s
	uint32 dataRate;//  数据接收速率 Mb/s
	uint8  recState; //节目管理中心连接状态
	uint64 timeStamp; //开机时间戳
	uint8  reserved[7]; //保留
	uint32 filmID;//  影片ID ,if no film,the value is 0
	uint32 filmNameLength;//  影片名称长度，if no film,the value is 0
}HeartBeat_1;

typedef struct _HeartBeat_2_ {
	uint64 filmLength;//  影片总大小
	uint64 filmSegment;//  影片segment包数量
	uint32 recvRound;//  接收轮次
	uint8 taskStartTime[20];	//任务的创建时间
	uint8 recvStartTime[20];	//本轮接收的开始时间
	uint32 filmRecvState;//  影片接收状态
	uint64 reserved2;//鏈粨鏋勪綋瀹氫箟涓湁涓ゅ鍏充簬reserved
	uint64 recvLength;//  影片已接收大小
	uint64 lostSegment;//  影片已丢失Segment包数量
	uint32 crc32;
}HeartBeat_2;

//登陆反馈报文
typedef struct _LOGINBACK_
{
	KH kh;
	char    ReSeverIP[16];        //重定向业务服务器IP 128位字段，表示重定向业务服务器的IP地址，格式为”xxx.xxx.xxx.xxx”。
	uint32  ReSeverPort;          //重定向业务服务器IP 端口号
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
	uint32  CRC32;                 //rpchof 本结构体以上字段加密前的CRC校验值
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
    uint64  reserved;               //保留字  
	char    UpdateSerialNo[16];    //128位字段，软件升级包的序列号。
    char    reserved2[16];          //保留字段2 
    uint32  DescriptionLength;     //32位无符号整型，升级软件描述信息长度。
    char    Description[256];      //可变长字段，升级软件描述信息。(暂定256位字符串)
    uint32  FID;                    //32位无符号整型，传送升级软件与配置文件的ID号。
    uint32  UpdateFileNameLength;  //32位无符号整型，升级软件文件名长度。
    char    UpdateFileName[256];   //可变长字段，升级软件文件名。   (暂定256位字符串)
    uint32  UpdateFileLength;      //32位无符号整型，最大可以表示4GB,升级软件文件长度。如升级软件通过互联网传送，需指明升级软件文件长度；如升级软件通过卫星信道传送，该项取值为0。
    //char UpdateFile[1024];       //可变长字段，升级软件文件。如升级软件通过互联网传送，此项为传送的升级软件文件的；如升级软件通过卫星信道传送，此项不存在。
	//uint32  CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileINFO;
//传送完成结构体以上后,在传送文件本身,然后传送文件CRC校验码

//自定义升级包推送报文 （互联网传输）
typedef struct _UpdateFileCustomINFO_
{
	KH kh;
	uint64  SegmentNo;               //64位无符号整型，分片编号，从0开始，每传一个包递加1。 
	char    UpdateSerialNo[16];    //128位字段，软件升级包的序列号。
	char    reserved[16];          //保留字段2 
	uint32  FID;                    //32位无符号整型，传送升级软件与配置文件的ID号。
	uint32  UpdateFileNameLength;  //32位无符号整型，升级软件文件名长度。
	char    UpdateFileName[256];   //可变长字段，升级软件文件名。  注：为了兼容配置文件与升级软件的传输，此字段必须设置。如为升级软件，字段名为/home/leonis/update/leonisupdate.zt。如为配置文件，字段名为/etc/CineCast/配置文件名
	uint32  DataLength;			//32位无符号整型，本分片传输的数据长度
	//char		Data[1024*128];       //可变长字段，升级软件文件。如升级软件通过互联网传送，每次128K
	//uint32  CRC32;               //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileCustomINFO;

//接收机远程升级包推送反馈报文  接收机在收到升级包后的反馈。
typedef struct _UpdateFileCustomBACK_
{
	KH kh;
	uint32  UpdateCheckResult;     //32位无符号整型，升级确认。值为0表示升级包有效，1表示升级包无效。
	uint32  reserved;               //保留字段
	char  OldVersion[16];          //128位字段，接收软件升级前的版本信息。
	char  UpdateSerialNo[16];      //128位字段，接收的软件升级包序列号。
	uint64  SegmentNo;			//64位无符号整型，分片编号，用于通知控制子系统哪个分片接收成功或失败。
	char  reserved2[8];			//保留字段
	uint32  CRC32;                  //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileCustomBACK;

typedef struct _UpdateCustomSTART_
{
	KH kh;
	char  UpdateSerialNo[16];          //128位字段，接收软件升级后的版本信息。
	char  reserved[16];
	uint32  CRC32;                   //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateCustomSTART;

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
    uint32  UpdateCheckResult;     //32位无符号整型，升级确认。值为0表示升级包有效，1表示升级包无效。
    uint32  reserved;               //保留字段
    char  OldVersion[16];          //128位字段，接收软件升级前的版本信息。
    char  UpdateSerialNo[16];      //128位字段，接收的软件升级包序列号。
    char  reserved2[16];           //保留字段
	uint32  CRC32;                  //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateFileBACK;


//接收机远程维护控制子系统在接收到远程升级包携眷反馈报文后向接收机发出的升级请求。
//命令接收机开始升级
typedef struct _UpdateSTART_
{
	KH kh;
    char  UpdateSerialNo[16];          //128位字段，接收软件升级后的版本信息。
    uint32  reserved;
	uint32  CRC32;                   //rpchof 本结构体以上字段加密前的CRC校验值
}UpdateSTART;


//日志请求报文,向接收机请求其日志
typedef struct _LOGREQUEST_
{
	KH kh;
    uint32  DateStart;   //日志文件起始日期 32位无符号整型，为YYYYMMDD十进制表示。
    uint32  DateEnd;    //日志文件结束日期 32位无符号整型，为YYYYMMDD十进制表示。
	uint32  LogType;    //日志类型 32位无符号整型，为0x74752485，表示日志为LEONIS专有格式；其它值为科研所格式（目前科研所没定义具体格式）。
	uint32  CRC32;   //CRC32 32位字段，以上字段加密前的CRC校验值。  
}LOGREQUEST;


//日志反馈报文,仅仅报文头不包含内容本身,接收机反馈给节目管理中心
typedef struct _LOGBACKINFO_
{
	KH kh;  
    uint32  LogLengthALL;   //日志文件总长度   uint32，接收机日志文件的总长度。
    uint32  DateStart;      //日志文件起始日期  uint32，为YYYYMMDD十进制表示。
    uint32  DateEnd;        //日志文件结束日期  uint32，为YYYYMMDD十进制表示
	uint32  LogLengthCurr; //本次上传内容长度 uint32 本次上传的日志内容长度。
//	char    log[1024*10]; 	 //日志内容本身可变字段, variable	  bslbf
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

//
//typedef struct _SL_
//{
//	uint16 m_sID;
//	uint16 m_length;
//} SL;
//
////KLV
//typedef enum
//{
//	RECEIVE_STATUS = 0,
//	RECEIVE_FILM_NAME,
//	RECEIVE_FILM_UUID,
//	RECEIVE_FILM_ISSUE_DATE,
//	RECEIVE_FILM_ISSUER,
//	RECEIVE_FILM_CREATOR,
//
//	TUNER_CONFIG = 0x20,
//	TUNER_DEV_NAME,
//	TUNER_DEL_SYS,
//	TUNER_FEC,
//	TUNER_MOD,
//	TUNER_ROLL_OFF,
//	TUNER_POL,
//
//	NET_CONFIG = 0x40,
//	NET_DEV_NAME,
//	NET_DHCP,
//	NET_IP,
//	NET_NETMASK,
//	NET_GATEWAY,
//	
//	REMOTE_CONFIG = 0x60,
//	REMOTE_DNS,
//	REMOTE_SERVER,
//
//} ReceiveKey;
//
//typedef struct receive_info
//{
//	uint64 nFileLength;
//	uint64 nReceiveLength;
//	uint64 nTotalSegment;
//	uint64 nReceiveSegment;
//	uint64 nCrcErrorSegment;
//	uint64 nLostSegment; //Reserved
//	uint32 nFileID;
//	uint16 nReceiveStatus;
//	std::string strFilmName;
//	std::string strUuid;
//	std::string strIssueDate;
//	std::string strIssuer;
//	std::string strCreator;
//} RECEIVE_INFO;
//
//typedef struct tuner_conf {
//	unsigned int nFreq;
//	unsigned int nHiBand;
//	unsigned int nSR;
//	unsigned int nMis;
//	unsigned int nSatNo;
//	std::string strDevName;		//Such as "/dev/dvb/adapter0/frontend0"
//	std::string strDelSys;		//Such as "DVB-S", "DVB-S2"
//	std::string strFec;			//Such as "1/2", "2/3", "3/4", "4/5", "5/6"
//	std::string strModulation;	//Such as "8PSK", "16APSK"
//	std::string strRollOff;		//Such as "0.20", "0.25", "0.35"
//	std::string strPolVert;		//Such as "V", "H"
//
//	//for default
//	tuner_conf()
//	{
//		strDevName = "/dev/dvb/adapter0/frontend0";
//		strDelSys = "DVB-S2";
//		strFec = "3/4";
//		strModulation = "8PSK";
//		strRollOff = "0.25";
//		strPolVert = "V";
//		nFreq = 12500000;
//		nHiBand = 11300000;
//		nSR = 43200000;
//		nMis = -1;
//		nSatNo = 1;
//	};
//} TUNER_CONF;
//
//typedef struct network_conf {
//	uint8 nDhcp;
//	std::string strDevName;
//	std::string strIp;
//	std::string strNetmask;
//	std::string strGateway;
//} NETWORK_CONF;
//
//typedef struct remote_conf {
//	std::string strDns1;
//	std::string strDns2;
//	std::string strRemote;
//} REMOTE_CONF;
//

#pragma pack(pop)

#endif
