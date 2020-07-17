#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_

//��ʱ�Ѽ����㷨ͷ�����ڴ˴�
//#include "My_RootHeader.h"     //���Ժ궨��,�����ض����ͷ�ļ�

#include <string>

#if 0
typedef char  int8;
typedef short int16;
typedef int   int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

//C99��
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



//�ٶ������ֳ��õ�
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
 
//#pragma pack(pop)     //�ָ�����״̬
//#pragma pack(4)       //�趨Ϊ4�ֽڶ���

#pragma pack(push,1)     //����һ���ֽڶ��� �������״̬
//#pragma pack(1)

enum FILTER_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};

typedef enum
{
	KEY_NO=0,     //������
	KEY_HARD=1,   //Ӳ������
    KEY_MEET=2    //������Կ����
} KEY_TYPE;       //���ļ�������



typedef enum
{
    UI_TEST=0x19890512,           //���������UI����,����socket������ͻ���
    UI_SenDataDirect,                  //UIֱ�ӷ�������
    UI_SenDatatoUI,                    //�������ݸ�UI


    UI_SEND_UPDATE_FILE=0X1000,          //���������ļ���
    UI_COLLPKG_START,                    //��ʼ����
    UI_GET_LOG                           //������־���Ĳ���
} CMD_UItoSocket;    //UI��socket�̵߳�ͨ�����UI������socket



typedef enum
{
    SK_UPDATE_TREEWIDGET_LIST=0X2000,  //����������UI�����б�
	SK_UPDATE_PROGRESSBAR,             //���½�����
    SK_SEND_LOG_TOUI,                  //�ѻ�ȡ������־���ݴ��͸�UI��ʾ
    SK_COLLPKG_CONTINUE,               //���߳���ɺ󼯰�δ���,֪ͨUI,������һ��socket�̼߳�������
    SK_COLLPKG_COMPLETE,               //���߳���ɺ󼯰����,֪ͨUI��ʾ�������.
    SK_COLLPKG_DIRISNULL,              //ӰƬĿ¼��ӰƬ�ļ�Ϊ��,�޷���ɼ�������
    SK_COLLPKG_ISDOINGFILE             //�ش����ڼ������ļ�·����UI��ʾ
} CMD_SockettoUI;                      //UI��socket�̵߳�ͨ������




typedef enum
{
    SV_SOCKET_NEWCOMMING=0X3000,   //���µ�client���ӵ���
    SV_SOCKET_DISCONNECT           //��socket�Ͽ�
} CMD_TcpServertoUI;               //TcpServertoUI




typedef enum
{
    SOCKET_IDLE=0X5000,   //����
    SOCKET_CONNECTING,    //��������
    SOCKET_CONNECTED,     //�Ѿ�����
    SOCKET_DISCONNECT     //��socket�Ͽ�
} CMD_ClientSockettoUI;               //TcpServertoUI




typedef enum
{
	//Sever����Ҫ����������ǰ׺S   Client�˼�C ̫��
    CMD_TEST=0X999,             //�Զ����������
    PKG_HEAD=0x55,                   //���ݰ�ͷ
    CMD_ZERO=0x0000,                 //���cmd_Sub
    CMD_REV=0x78563412,              //�Զ��屣����
    CMD_REV_LEONIS=0x74752485,           //�Զ���leonis������


	CMD_LOGIN_REQUEST= 0x0011,       //��¼������
	CMD_LOGIN_BACK,                  //��¼��������  
	CMD_AUTHEN_REQUEST,              //��֤������ 
	CMD_AUTHEN_BACK,                 //��֤��������
    CMD_UPDATE_FILE_RECEV=0x0071,    //���ջ�Զ�����������ͱ���
    CMD_UPDATE_BACK,                 //���ջ�Զ�����������ͷ�������
    CMD_UPDATE_START,                //���ջ�Զ������������,��ʵ����������ջ���ʼ����

    CMD_HEARTBEAT_REQUEST=0x0031,    //������Ϣ������  server����client
    CMD_HEARTBEAT_UPLOAD,            //������Ϣ�ϱ�����  client���ظ�server
	CMD_HEARTBEAT_CONFIRM,           //������Ϣȷ��      server��clientȷ��

    CMD_PKGLOSS_UPLOAD=0X0022,     //������Ϣ�ش�(�ϱ�)����
    CMD_PKGLOSS_CONFIRM=0X0023,    //������Ϣȷ�ϱ���

    CMD_FILLPKG_START=0X0090,      //������ʼ����
    CMD_FILLPKG_START_CONFIRM,     //������ʼȷ�ϱ���
    CMD_FILLPKG,                   //��������
    CMD_FILLPKG_OK,                //������ɱ���

    CMD_COLLPKG_REQUEST=0X0094,    //����������
    CMD_COLLPKG_REQUEST_CONFIRM,   //��������ȷ�ϱ���
    CMD_COLLPKG_START,             //������ʼ����
    CMD_COLLPKG_START_CONFIRM,     //������ʼȷ�ϱ���
    CMD_COLLPKG,                   //��������
    CMD_COLLPKG_OK,                //������ɱ���,����

    CMD_LOG_REQUEST=0x0100,            //��־������
    CMD_LOG_BACK,                      //��־�ش�����

    CMD_LOGFILE_REQUEST=0x0034,       //��־�ļ�������
    CMD_LOGFILE_BACK=0x0035          //��־�ļ���������


} COMMOND_TYPE;



typedef enum
{
   CMD_PH_HEAD=0x74752485,           //�Զ���leonis������
   CMD_BEDDATA_KEY=0x02,             //�Զ���leonis������
   CMD_OBJECTDATA_KEY=0x05           //�Զ���leonis������
}OBIA;



//OBIAͨ��Э��ṹ��
typedef struct _PH_             //HEADER ��дHD  Package Header ��дKH  pKH��pPH�ÿ�
{
    uint32 Key;	       //Value is 0x74758524, it means the package is a meta-data command package.
    uint32 Length;	   //The length of total struct in bytes, including the key and length.
//Value variable Meta-data
}PH;



typedef struct _BEDDATA_
{
    uint16 Key;	    //Value is 0x2.
    uint32 Length;	//The length of total struct in bytes, including the key and length.
    uint32 CH_id;	//It��s means audio track number, such as 1, 2, 10.
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
    uint32 CH_id;	 //It��s object id number, such as 11,12 to 128.
    uint32 Time;     //Time code, it is the index of frame.
    float X;           //	Float(-1~1)	Left wall: -1 Right wall: 1
    float Y;           //	Float(-1~1)	Screen: -1 Back wall: 1
    float Z;           //	Float(-1~1)	Ceiling: 1 Floor: -1
    float Extent;      //(non-negative)	The extent(size) of the object in the room, measured as radius from the object position. 0 indicating a point source.
    float Gain;        //(non-negative)	This linear gain scales the level the audio is presented at, which is otherwise determined by the waveform level and renderer.
    float Coherence;   //(0~1)	For objects with non-zero extent, coherence affects the coherence of sound elements arriving from different directions. 1 indicating full coherence.
    float Snap;        //(non-negative)	Snap arranges for an object to jump to a speaker if one is nearby. 0 indicating no snap.
}OBJECTDATA;


//��¼һ�����������Լ����Ƿ���Ч���ɼ�
typedef struct _OBJ_
{
    bool Enable;	   //
    float X;           //	Float(-1~1)	Left wall: -1 Right wall: 1
    float Y;           //	Float(-1~1)	Screen: -1 Back wall: 1
    float Z;           //	Float(-1~1)	Ceiling: 1 Floor: -1
}OBJ;


//����һ֡��128���������꣬�����Ƿ�ɼ�
typedef struct _FRAMEOBJ_
{
    OBJ ObjNum[128];
}FRAMEOBJ;














//����ݸ�JOYSTICK������ؽṹ��
typedef struct _KHJOY_        //һ�����������ֽ�
{
    unsigned char KeyHead;
    unsigned char Key2;
    unsigned char Key3;
}KHJOY;

typedef struct _KHJOYPOS_   //��������ֵ����������ֽ�
{
    KHJOY KHjoy;
    unsigned char Key4;
    unsigned char Key5;
}KHJOYPOS;

typedef struct _POS_    //������ݸ˻ش�������ֵ�ṹ��
{
    uint32 CH_id;	 //It��s object id number, such as 11,12 to 128.
    uint32 Time;     //Time code, it is the index of frame.
    float posX;
    float posY;
    float posZ;
}POS;






//������server�˱�����Ѿ�ע��Ľ��ջ�ID����Ӳ��ָ��
typedef struct _RegisterIDHARDKEY_
{
    uint32  ID;
    char HardKey[16];
}RegisterIDHARDKEY;



//����ͨѶЭ�鱨�Ļ����ṹ
//Ƕ���������б���ǰ��,��Ϊ����ͷ BASEprotocol
typedef struct _KH_             //HEADER ��дHD  Package Header ��дKH  pKH��pPH�ÿ�
{
	uint8   pkgHead;            //preamble;//8λ�޷������ͣ���ʼͬ���ֶΣ���Ϊ(0x55)
	uint8   flag_pwd;           //���ܱ�� 0	����δ���� 1���Ĳ���Ӳ��ָ�Ƽ��� 2	���Ĳ��û�����Կ����
	uint32  rev;                //reserved������ 
	uint16  cmd;                //�ñ��ĵ���������
	uint16  cmd_Sub;            //�ñ��ĵĸ��������� Sub_cmd
    uint32  cmd_length;         //Payload_length;//�����غ����ݳ��ȣ�(���������ݱ���+��CRC32У��ֵ)
    //uint8  Payload_data_byte;  
}KH;



//��½������  128λ�ֶ� 16�ֽ�
typedef struct _LOGINREQUEST_
{
	KH kh;
	uint32 Machine_ID;          //��ʾ���ջ���ID�ţ����ֶβ�����
	uint32 LoginCounter;        //���ջ�����������ĵ�¼����
	char   SoftVersion[16];     //bslbf 128λ�ֶ� ��ʾ���ջ���ǰ��������İ汾�ţ��ɳ������ж���
	char   Time_Startup[20];    //bslbf 160λ�ֶ� ��ʾ���ջ�����ʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
	char   Time_Login[20];      //bslbf 160λ�ֶ� ��ʾ���ջ���½ʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
    uint32 CRC32;               //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}LOGINREQUEST;




//��½��������
typedef struct _LOGINBACK_
{
	KH kh;
    char    ReSeverIP[16];         //�ض���ҵ�������IP 128λ�ֶΣ���ʾ�ض���ҵ���������IP��ַ����ʽΪ��xxx.xxx.xxx.xxx����
    uint32  ReSeverPort;           //�ض���ҵ�������IP �˿ں�
    uint32  Year;                  //ϵͳ��ǰ���,��,�գ�ʱ����
    uint32  Month;                 //32λ�޷������ͣ���ʾϵͳ��ǰ�·ݣ�ȡֵ��Χ1��12��  
    uint32  Day;                   //32λ�޷������ͣ���ʾϵͳ��ǰ���ڣ�ȡֵ��Χ1��31��
    uint32  Hour;                  //32λ�޷������ͣ���ʾϵͳ��ǰСʱ��ȡֵ��Χ0��24��
    uint32  Minute;                //32λ�޷������ͣ���ʾϵͳ��ǰ���ӣ�ȡֵ��Χ0��59��
    uint32  Seconds;               //32λ�޷������ͣ���ʾϵͳ��ǰ���ӣ�ȡֵ��Χ0��59��
    uint32 CRC32;                  //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}LOGINBACK;



//��֤���� authentication request
typedef struct _AuthenREQUEST_
{
	KH kh;
	uint32  Machine_ID;           //��ʾ���ջ���ID�š����ֶβ����ܡ�
    char    SoftVersion[16];      //bslbf  128λ�ֶ� ��ʾ���ջ���ǰ��������İ汾�ţ��ɳ������ж���
    char    Time_Login[20];       //bslbf  160λ�ֶ� ��ʾ���ջ���½ʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
    uint32  Model_Log;            //��־ģʽ ��ʾ���ջ��Ƿ��ϱ���־�����ֶ�ֵΪ1����ʾ���ջ��ϱ���־��ֵΪ2����ʾ���ϱ���־��
	uint32	Reserved1;			  //Reserved
    uint32  BeatCycle;            //��������heartbeat cycle ��ʾ���ջ��ϱ��������ݱ��ĵ����ڣ���λΪ�롣���ȷ�ϵ�������������ջ��ϱ������ڲ�ͬ�����ջ��ϱ������Դ�Ϊ׼��
    uint32  Model_Connect;        //���ӷ�ʽ ��ʾ���ջ������ջ�Զ��ά��������ϵͳ�������ӵķ�ʽ�����ֶ�ֵΪ1����ʾ�뷢��ϵͳƽ̨���ֲ���ϵĳ������ӣ�ֵΪ0����ʾ�뷢��ϵͳƽ̨���������ӣ������ջ�ֻ�������ݴ�����Ҫʱ������ջ�Զ��ά��������ϵͳ�������ӡ�
    uint32  HardDiskNum;          //32λ�޷������ͣ���ʾ���ջ���ǰ���ص����ڽ��յ�Ӱ���ֿ�����Ӳ��������
    uint64  AllDiskCapacity;      //64λ�޷������ͣ���ʾ���ջ�����Ӳ�̵���������λΪ�ֽڡ�
	uint32	Reserved2;			  //Reserved
    uint32  CRC32;                //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}AuthenREQUEST;


//��֤���� authentication feedback
typedef struct _AuthenBACK_
{
	KH kh;
	char    MeetKey[16];          //������Կ128λ�ֶΣ����ջ�Զ��ά��������ϵͳ�����ݲ�����ϵͳ����ջ��������ĵļ�����Կ��
    char    RandomCode[16];       //�����128λ�ֶΣ�����Ҫ������Я�����Թ����ջ���֤��
    uint32  Model_Log;            //��־ģʽ32λ�޷������ͣ����ջ�Զ��ά��������ϵͳ����ջ�ȷ�����ϱ�����־ģʽ��
    uint32  reserved;             //������  
    uint32  BeatCycle;            //�������� 32λ�޷������ͣ����ջ�Զ��ά��������ϵͳ����ջ�ȷ�����ϱ����������ڡ�
    uint32  Model_Connect;        //���ӷ�ʽ 32λ�޷������ͣ����ջ�Զ��ά��������ϵͳ����ջ�ȷ�����ϱ������ӷ�ʽ��
	uint32  CRC32;                //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}AuthenBACK;



//���������ͱ��� (�������ļ���Ϣ,�ļ����ݺ�������)  ������Upgradepackage
typedef struct _UpdateFileINFO_
{
	KH kh;
    uint64  reserved;              //������
	char    UpdateSerialNo[16];    //128λ�ֶΣ���������������кš�
    char    reserved2[16];         //�����ֶ�2
    uint32  DescriptionLength;     //32λ�޷������ͣ��������������Ϣ���ȡ�
    char    Description[256];      //�ɱ䳤�ֶΣ��������������Ϣ��(�ݶ�256λ�ַ���)
    uint32  FID;                   //32λ�޷������ͣ�������������������ļ���ID�š�
    uint32  UpdateFileNameLength;  //32λ�޷������ͣ���������ļ������ȡ�
    char    UpdateFileName[256];   //�ɱ䳤�ֶΣ���������ļ�����   (�ݶ�256λ�ַ���)
    uint32  UpdateFileLength;      //32λ�޷������ͣ������Ա�ʾ4GB,��������ļ����ȡ����������ͨ�����������ͣ���ָ����������ļ����ȣ����������ͨ�������ŵ����ͣ�����ȡֵΪ0��
    //char UpdateFile[1024];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�����Ϊ���͵���������ļ��ģ����������ͨ�������ŵ����ͣ�������ڡ�
	//uint32  CRC32;               //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileINFO;
//������ɽṹ�����Ϻ�,�ڴ����ļ�����,Ȼ�����ļ�CRCУ����



/*
//�Զ����ļ��ֶη��͵Ľṹ��
typedef struct _SendFileInfo_
{
    char    FileName[256];          //Ҫ���͵��ļ���
    char    FilePath_dst[256];      //�ļ�Ŀ��·��
    char    FilePath_rsc[256];      //�ļ�Ŀ��·��
    uint64  FileSize;               //�ļ��ܴ�С,��λ�ֽ�Byte
    uint32  FileCRC32;              //�ļ����ݱ���CRC32У��
    char    FileMD5[32];            //�ļ����ݱ���MD5
    uint32  FilePkgAll;             //���ļ�����    һ�����ļ��ְ�����,64Kÿ����
    uint32  FilePkgCurrent;         //��ǰ�ǵڼ�����
    uint32  FileEndFlag;            //��ǰ���Ƿ������һ����,��Ϊ1,��Ϊ0,
    uint32  FileEndPos;             //�����ǰ�����һ����,��¼����λ��,��λ�ֽ�,�������
    char    FileBuf[64*1024];       //�ļ�������.
}SendFileInfo;
*/



//���ջ�Զ�����������ͷ�������  ���ջ����յ���������ķ�����
typedef struct _UpdateFileBACK_
{
	KH kh;
    uint32  UpdateCheckResult;      //32λ�޷������ͣ�����ȷ�ϡ�ֵΪ0��ʾ��������Ч��1��ʾ��������Ч��
    uint32  reserved;               //�����ֶ�
    char  OldVersion[16];           //128λ�ֶΣ������������ǰ�İ汾��Ϣ��
    char  UpdateSerialNo[16];       //128λ�ֶΣ����յ�������������кš�
    char  reserved2[16];            //�����ֶ�
	uint32  CRC32;                  //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileBACK;


//���ջ�Զ��ά��������ϵͳ�ڽ��յ�Զ��������Я�췴�����ĺ�����ջ���������������
//������ջ���ʼ����
typedef struct _UpdateSTART_
{
	KH kh;
    char  UpdateSerialNo[16];          //128λ�ֶΣ��������������İ汾��Ϣ��
    uint32  reserved;
    uint32  CRC32;                     //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateSTART;





//��־������,����ջ���������־
typedef struct _LOGREQUEST_
{
	KH kh;
    uint32  DateStart;   //��־�ļ���ʼ���� 32λ�޷������ͣ�ΪYYYYMMDDʮ���Ʊ�ʾ��
    uint32  DateEnd;     //��־�ļ��������� 32λ�޷������ͣ�ΪYYYYMMDDʮ���Ʊ�ʾ��
	uint32  LogType;     //��־���� 32λ�޷������ͣ�Ϊ0x74752485����ʾ��־ΪLEONISר�и�ʽ������ֵΪ��������ʽ��Ŀǰ������û��������ʽ����
	uint32  CRC32;       //CRC32 32λ�ֶΣ������ֶμ���ǰ��CRCУ��ֵ��  
}LOGREQUEST;


//��־��������,��������ͷ���������ݱ���,���ջ���������Ŀ��������
typedef struct _LOGBACKINFO_
{
	KH kh;  
    uint32  LogLengthALL;   //��־�ļ��ܳ���   uint32�����ջ���־�ļ����ܳ��ȡ�
    uint32  DateStart;      //��־�ļ���ʼ����  uint32��ΪYYYYMMDDʮ���Ʊ�ʾ��
    uint32  DateEnd;        //��־�ļ���������  uint32��ΪYYYYMMDDʮ���Ʊ�ʾ
    uint32  LogLengthCurr;  //�����ϴ����ݳ��� uint32 �����ϴ�����־���ݳ��ȡ�
//	char    log[1024*10]; 	//��־���ݱ���ɱ��ֶ�, variable	  bslbf
//	uint32  CRC32;          //          
}LOGBACKINFO;
//һ����־�ļ��Ƚ�С,��ʱ����10K,���߸���,û����0��� ���㴦��



//������Ϣ������             //server����client
typedef struct _HEARTBEATREQUEST_
{
    KH kh;                   //ֻ�б���ͷ,������
}HEARTBEATREQUEST;


//������Ϣ�ϱ�����             //client����server
typedef struct _HEARTBEATUPLOAD_
{
	KH kh; 
    char resv[168+256];      //����״̬��Ϣ����Ϊ�ò���,�ݲ�����,����168�ֽ�,ӰƬ�����ݶ�256
}HEARTBEATUPLOAD;


//������Ϣȷ�ϱ���             //server��clientȷ��
typedef struct _HEARTBEATCONFIRM_
{
    KH kh;                   //ֻ�б���ͷ,������
}HEARTBEATCONFIRM;








//�Զ��������ļ�·����ģʽ�ṹ��, ����UI������socket·������
typedef struct _UPDATEMODELPATH_
{
	int SendMode;       //�������ͣ�0��ʾ�����Ǵ���,1��ʾ�û���������
    char Path[256];     //Ҫ���͵��ļ�·��,��SendModeΪ0ʱ���á�
}UPDATEMODELPATH;



/*
//��־����
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
//���ݲ�����ؽṹ��,
//��������Ϣ�ش����ġ�--���ڽ��ջ������ݲ�����ϵͳ�ϱ�������Ϣ��
typedef struct _PKGLOSSUPLOAD_HEAD_    //����ͷ
{
	KH kh; 
    uint32 FilmID;           //������Ϣ��ӰƬID�š�
	uint32 PkglossNumber;    //ӰƬ����ʧ�İ�����
    uint64 BytesRecv;        //��ӰƬ�ѽ��յ��ֽ�����
    uint32 State;            //���ֽ�����ɺ�ӰƬ�Ľ���״̬������״̬������±�
	uint32 SegmentLength;    //��ʾ���ݵķָ���С��
	uint32 PkglossLength;    //������Ϣ�ļ��ĳ��ȣ���λΪ�ֽڡ�
	//variable               //�ɱ䳤�ȣ�Ϊ������Ϣ�ļ����ݡ�
}PKGLOSSUPLOAD_HEAD;



typedef struct _PKGLOSSBACK_END_     //����β��
{
//	uint32  PkglossFileNameLength;    //�����ļ������� 
//  char   PkglossFileName[256];     //variable �����ļ���,Ϊ�˷���ʵ�ֶ���Ϊ����256�ַ���
//	uint32 CRC32;
}PKGLOSSBACK_END;

/*
ӰƬ����״̬		ȡֵ
����				0
���ݽ���			1
���ս���			2
��������			3
reserved		4
������Ϣ�ϱ�		5
������Ϣ�ϱ�ȷ��	6
����MD5����Կ�ļ�	7
MD5����Կ����ȷ��	8
����У��			9
����У��ɹ�		10
�������			11
*/


//��������Ϣȷ�ϱ��ġ�--�������ݲ���ϵͳ�Խ��յ�������Ϣ��ȷ�ϡ�
typedef struct _PKGLOSSCONFIRM_
{
	KH kh; 
	uint32 FilmID;     //������Ϣ��ӰƬID�š�
    uint64 rev;        //reserved������
	uint32 CRC32;
}PKGLOSSCONFIRM;
//rev=CMD_REV_LEONIS;//��������,���ĳ���ֶ�,��ʾ����������Ӧ�ļ����޷���ɲ���



//��������ʼ���ġ�--�ñ������������ķ��������ڷ������ݰ���ÿ�����ݰ��������͡�
typedef struct _FILLPKGSTART_HEAD_
{
	KH kh; 
    uint32 FilmID;                   //������Ϣ��ӰƬID�š�
	uint32 SegmentLength;            //��ʾ���ݵķָ���С��
	uint32 PkglossFileNameLength;    //�����ļ������� 
//  char   PkglossFileName[256];     //variable �����ļ���,Ϊ�˷���ʵ�ֶ���Ϊ����256�ַ���
//	uint32 CRC32;
}FILLPKGSTART_HEAD;




//��������ʼȷ�ϱ��ġ�--�ñ������������ķ��������ڷ������ݰ���ÿ�����ݰ��������͡�
typedef struct _FILLPKGSTARTCONFIRM_
{
	KH kh; 
    uint32 FilmID;             //������Ϣ��ӰƬID�š�
	uint32 CRC32;
}FILLPKGSTARTCONFIRM;



//���������ġ�--�ñ������ڷ������ݰ���ÿ�����ݰ��������͡�
typedef struct _FILLPKG_HEAD_
{
	KH kh; 
	uint32 FilmID;             //������Ϣ��ӰƬID�š�
    uint32 SegmentNumber;      //���ݷֿ���š�����Ŵ�0��ʼ��
	uint32 PkgLength;          //���ݰ��ĳ��ȣ���λΪ�ֽڡ�
//  variable                   //Ϊ���ݰ����ݡ�
//	uint32 CRC32;
}FILLPKG_HEAD;
//�����ݰ���Ϣͷ,���ŷ������ݱ����CRC



//��������ɱ��ġ�--�ñ������ڱ�Ǳ��ֲ�����ɡ�
typedef struct _FILLPKGOK_
{
	KH kh; 
	uint32 FilmID;              //������Ϣ��ӰƬID�š�
	uint32 CRC32;
}FILLPKGOK;



//�����������ġ�--�ñ������������ķ������������豸�������ݰ���
typedef struct _COLLPKGREQUEST_HEAD_
{
	KH kh; 
    uint32 FilmID;                   //������Ϣ��ӰƬID�š�
	uint32 PkglossFileNameLength;    //�����ļ������� 
//  char PkglossFileName[256];       //variable �����ļ���,Ϊ�˷���ʵ�ֶ���Ϊ����256�ַ���
//	uint32 ContentLength;            //�������ݳ���
//  variable Content;                //Ϊ�����ļ����ݡ������ļ���ʽ�������¼A��
//  uint32 CRC32;
}COLLPKGREQUEST_HEAD;



//����������ȷ�ϱ��ġ��ñ������豸����������֪ͨ���������豸��׼���÷������ݰ���ÿ�����ݰ��������͡�
typedef struct _COLLPKGREQUESTCONFIRM_
{
    KH kh;
    uint32 FilmID;            //������Ϣ��ӰƬID�š�
    uint64 rev;               //reserved������
    uint32 CRC32;
}COLLPKGREQUESTCONFIRM;
//rev=CMD_REV_LEONIS;//��������,���ĳ���ֶ�,��ʾ����������Ӧ�ļ����޷���ɲ���


//��������ʼ���ġ�--�ñ������豸����,�����豸���������ģ���ʼ����
typedef struct _COLLPKGSTART_HEAD_
{
    KH kh;
    uint32 FilmID;                    //������Ϣ��ӰƬID�š�
    uint32 SegmentLength;            //��ʾ���ݵķָ���С��
    uint32 PkglossFileNameLength;    //�����ļ�������
//  char   PkglossFileName[256];     //variable �����ļ���,Ϊ�˷���ʵ�ֶ���Ϊ����256�ַ���
//	uint32 CRC32;
}COLLPKGSTART_HEAD;



//��������ʼȷ�ϱ��ġ�--�ñ������������ķ��������ڷ������ݰ���ÿ�����ݰ��������͡�
typedef struct _COLLPKGSTARTCONFIRM_
{
    KH kh;
    uint32 FilmID;               //������Ϣ��ӰƬID�š�
    uint32 CRC32;
}COLLPKGSTARTCONFIRM;




//���������ġ�--�ñ������ڷ������ݰ���ÿ�����ݰ��������͡�
typedef struct _COLLPKG_HEAD_
{
    KH kh;
    uint32 FilmID;              //������Ϣ��ӰƬID�š�
    uint32 SegmentNumber;       //���ݷֿ���š�����Ŵ�0��ʼ��
    uint32 PkgLength;           //���ݰ��ĳ��ȣ���λΪ�ֽڡ�
//  variable                    //Ϊ���ݰ����ݡ�
//	uint32 CRC32;
}COLLPKG_HEAD;
//�����ݰ���Ϣͷ,���ŷ������ݱ����CRC

//��������ɱ��ġ�--�ñ������ڱ�Ǳ��ּ�����ɡ�
typedef struct _COLLPKGOK_
{
    KH kh;
    uint32 FilmID;              //������Ϣ��ӰƬID�š�
    uint32 CRC32;
}COLLPKGOK;






//�Զ�����Ա���--����
typedef struct _PAYLOADTEST_
{
    KH kh;
    char rev[256];
    uint32 CRC32;               //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}PAYLOADTEST;




//typedef struct _KL_
//{
//	int m_pkgHead;
//	int m_keyID;
//	int m_length;
//	int m_flag;               //��-1���,����0���������ݰ����ͳɹ���
//}KL;


/*
typedef struct _KL_
{
	uint16 m_pkgHead;
	uint16 m_keyID;
	uint16 m_length;
	int    m_flag;           //��-1���,����0���������ݰ����ͳɹ���
public:
	_KL_()
	{
		m_pkgHead = 0x7585;   //�Զ����ù��캯��
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

