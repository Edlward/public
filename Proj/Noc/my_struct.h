#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_


//#include "../Basic/global.h"

#include "mytype.h"

#pragma pack(push)
#pragma pack(1)

typedef enum
{
	KEY_NO=0,     //������
	KEY_HARD=1,   //Ӳ������
	KEY_MEET=2,   //������Կ����
} KEY_TYPE;       //���ļ�������


typedef enum
{
	UI_SEND_UPDATE_FILE=0X1000,        //���������ļ���
	UI_GET_LOG,                          //������־���Ĳ��� 
	UI_GET_MD5RES,
	UI_REBOOT,
	UI_LOG,
	UI_TMS_LIST,
	UI_TMS_INFO,
	UI_CLOSE_SOCKET,
} CMD_UItoSocket;    //UI��socket�̵߳�ͨ�����UI������socket



typedef enum
{
	SK_UPDATE_TREEWIDGET_LIST=0X2000,      //����������UI�����б�
	SK_UPDATE_PROGRESSBAR,             //���½�����

	SK_UPDATE_TEXTBROWSER,

} CMD_SockettoUI;    //UI��socket�̵߳�ͨ������

typedef enum
{
	SV_SOCKET_DISCONNECT=0X3000,    //��socket�Ͽ�

} CMD_TcpServertoUI;    //TcpServertoUI


typedef enum
{
	//Sever����Ҫ����������ǰ׺S   Client�˼�C ̫��
	M_SEND_TEST,
	PKG_HEAD=0x55,           //���ݰ�ͷ
	CMD_ZERO=0x0000,        //���cmd_Sub 
    CMD_REV=0x78563412,      //�Զ��屣����


	CMD_LOGIN_REQUEST= 0x0011,      //��¼������
	CMD_LOGIN_BACK,                  //��¼��������  
	CMD_AUTHEN_REQUEST,              //��֤������ 
	CMD_AUTHEN_BACK,                 //��֤��������
	CMD_LOST_REPORT = 0x22,
	CMD_MD5_REQUEST = 0x24,
	CMD_MD5_RES_UP_REQUEST=0x28,
    CMD_UPDATE_FILE_RECEV=0x0071,   //���ջ�Զ�����������ͱ���
    CMD_UPDATE_BACK,                 //���ջ�Զ�����������ͷ�������
    CMD_UPDATE_START,                //���ջ�Զ������������,��ʵ����������ջ���ʼ����

	CMD_UPDATE_FILE_CUSTOM=0x8071,   //�Զ�����ջ�Զ�����������ͱ���
	CMD_UPDATE_BACK_CUSTOM,                 //�Զ�����ջ�Զ�����������ͷ�������
	CMD_UPDATE_START_CUSTOM,                //�Զ�����ջ�Զ������������,��ʵ����������ջ���ʼ����

    CMD_LOG_REQUEST=0x0100,           //��־������
    CMD_LOG_BACK,                      //��־�ش�����

	CMD_HEARTBEAT_REQUEST=0x0031,
	CMD_HEARTBEAT=0x0032,
	CMD_HEARTBEAT_CONFIRM=0x0033,

    CMD_LOGFILE_REQUEST=0x0034,       //��־�ļ�������
    CMD_LOGFILE_BACK=0x0035,          //��־�ļ���������

	CMD_UI_CAST_UPLOAD=0x0ccb,			//Cast�������ϴ�
	CMD_UI_CAST_UPLOAD_ACK=0x0cc1,		//Cast�������ϴ�ȷ��
	CMD_UI_ISSUE_UPLOAD=0x0cbb,			//Issue�������ϴ�
	CMD_UI_ISSUE_UPLOAD_ACK=0x0cb1,		//Issue�������ϴ�ȷ��
	CMD_UI_UPLOAD_FIN=0x0ccc,				//Cast/Issue�������ϴ���������
	CMD_UI_REMOTE_UPDATE=0x0cca,			//����Զ�̽��ջ�
	CMD_UI_REMOTE_UPDATE_ACK=0x0cc2,		//Զ�̽��ջ����½��ȷ���
	CMD_UI_REMOTE_REBOOT=0x0cce,			////Զ������
	CMD_UI_DISCONNECT=0x0ccd,				//UI�رգ��Ͽ�����
	CMD_UI_REMOTE_LOG_REG=0x0ca1,			//UIԶ����־����
	CMD_UI_REMOTE_LOG_ACK=0x0ca2,			//UIԶ����־Ӧ��


    //test 
    CMD_TEXT_MESSAGE=0XF5F5,
} COMMOND_TYPE;



//������server�˱�����Ѿ�ע��Ľ��ջ�ID����Ӳ��ָ��
typedef struct _RegisterIDHARDKEY_
{
    uint32  ID;
    char HardKey[16];
}RegisterIDHARDKEY;



//����ͨѶЭ�鱨�Ļ����ṹ
//Ƕ���������б���ǰ��,��Ϊ����ͷ BASEprotocol
typedef struct _KH_           // HEADER ��дHD  Package Header ��дKH  pKH��pPH�ÿ�
{
	uint8   pkgHead;            //preamble;//8λ�޷������ͣ���ʼͬ���ֶΣ���Ϊ(0x55)
	uint8   flag_pwd;           //���ܱ�� 0	����δ���� 1���Ĳ���Ӳ��ָ�Ƽ��� 2	���Ĳ��û�����Կ����
	uint32  rev;                //reserved������ 
	uint16  cmd;                //�ñ��ĵ���������
	uint16  cmd_Sub;            //�ñ��ĵĸ��������� Sub_cmd
    uint32  cmd_length;         //Payload_length;//�����غ����ݳ��ȣ�(���������ݱ���+��CRC32У��ֵ)
    //uint8  Payload_data_byte;  
}KH;

//�Զ���UI���������ͱ���
typedef struct _UIUPLOADSEG_
{
	KH kh;
	uint32  SegmentNo;               //64λ�޷������ͣ���Ƭ��ţ���0��ʼ��ÿ��һ�����ݼ�1�� 
	uint32  DataLength;			//32λ�޷������ͣ�����Ƭ��������ݳ���
	//char		Data[1024*128];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�ÿ��128K
	//uint32  CRC32;  
}UIUploadSeg;

//�Զ���UI���������ͱ���ȷ��
typedef struct _UIUPLOADSEGACK_
{
	KH kh;
	uint32  SegmentNo;               //64λ�޷������ͣ���Ƭ��ţ���0��ʼ��ÿ��һ�����ݼ�1��
	uint8   status;
	//uint32  DataLength;			//32λ�޷������ͣ�����Ƭ��������ݳ���
	//char		Data[1024*128];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�ÿ��128K
	uint32  CRC32;  
}UIUploadSegAck;

typedef struct _UIREMOTEUPDATE_
{
	KH kh;
	int32  macID;		//32λ�޷������ͣ������豸id
	uint32  CRC32;  
}UIRemoteUpdate;

typedef struct _UIREMOTEUPDATE_ACK
{
	KH kh;
	int32  macID;		//32λ�޷������ͣ������豸id
	int32  sendSize;
	int32  fileSize;
	uint32  CRC32;  
}UIRemoteUpdate_Ack;

typedef struct _UIREMOTEREBOOT_
{
	KH kh;
	int32  macID;		//32λ�޷������ͣ������豸id
	uint32  CRC32;  
}UIRemoteReboot;

typedef struct _UIREMOTELOGREQ_
{
	KH kh;
	int32  macID;		//32λ�޷������ͣ������豸id
	uint32  CRC32;  
}UIRemoteLogReg;

typedef struct _UIREMOTELOG_ACK_
{
	KH kh;
	uint32  macid;               //64λ�޷������ͣ���Ƭ��ţ���0��ʼ��ÿ��һ�����ݼ�1�� 
	uint32  DataLength;			//32λ�޷������ͣ�����Ƭ��������ݳ���
	//char		Data[1024*128];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�ÿ��128K
	//uint32  CRC32;  
}UIRemoteLog_Ack;

//��½������  128λ�ֶ� 16�ֽ�
typedef struct _LOGINREQUEST_
{
	KH kh;
	uint32 Machine_ID;          //��ʾ���ջ���ID�ţ����ֶβ�����
	uint32 LoginCounter;        //���ջ�����������ĵ�¼����
	char   SoftVersion[16];     //bslbf 128λ�ֶ� ��ʾ���ջ���ǰ��������İ汾�ţ��ɳ������ж���
	char   Time_Startup[20];    //bslbf 160λ�ֶ� ��ʾ���ջ�����ʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
	char   Time_Login[20];      //bslbf 160λ�ֶ� ��ʾ���ջ���½ʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
    uint32 CRC32;                //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}LOGINREQUEST;


typedef struct _HeartBeat_1_
{
	KH kh;
	uint32 machineID;//  ���ջ��Ļ�����
	uint32 temperature;//  ���ջ��������¶�
	uint32 CPUUsage;//  CPUʹ����
	uint64 memTotal;//  �ڴ�����
	uint64 memIdle;//  �ڴ������
	uint64 networkRate;//  ��������������
	uint32 agc; //	�����ŵ����ź�ǿ�� 1-100
	uint32 snr;//  �����ŵ����ź����� 1-100
	uint32 interfaceRate;//  �ӿڽ������� Mb/s
	uint32 dataRate;//  ���ݽ������� Mb/s
	uint8  recState; //��Ŀ������������״̬
	uint64 timeStamp; //����ʱ���
	uint8  reserved[7]; //����
	uint32 filmID;//  ӰƬID ,if no film,the value is 0
	uint32 filmNameLength;//  ӰƬ���Ƴ��ȣ�if no film,the value is 0
}HeartBeat_1;

typedef struct _HeartBeat_2_ {
	uint64 filmLength;//  ӰƬ�ܴ�С
	uint64 filmSegment;//  ӰƬsegment������
	uint32 recvRound;//  �����ִ�
	uint8 taskStartTime[20];	//����Ĵ���ʱ��
	uint8 recvStartTime[20];	//���ֽ��յĿ�ʼʱ��
	uint32 filmRecvState;//  ӰƬ����״̬
	uint64 reserved2;//本结构体定义中有两处关于reserved
	uint64 recvLength;//  ӰƬ�ѽ��մ�С
	uint64 lostSegment;//  ӰƬ�Ѷ�ʧSegment������
	uint32 crc32;
}HeartBeat_2;

//��½��������
typedef struct _LOGINBACK_
{
	KH kh;
	char    ReSeverIP[16];        //�ض���ҵ�������IP 128λ�ֶΣ���ʾ�ض���ҵ���������IP��ַ����ʽΪ��xxx.xxx.xxx.xxx����
	uint32  ReSeverPort;          //�ض���ҵ�������IP �˿ں�
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
	uint32  CRC32;                 //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
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
    uint64  reserved;               //������  
	char    UpdateSerialNo[16];    //128λ�ֶΣ���������������кš�
    char    reserved2[16];          //�����ֶ�2 
    uint32  DescriptionLength;     //32λ�޷������ͣ��������������Ϣ���ȡ�
    char    Description[256];      //�ɱ䳤�ֶΣ��������������Ϣ��(�ݶ�256λ�ַ���)
    uint32  FID;                    //32λ�޷������ͣ�������������������ļ���ID�š�
    uint32  UpdateFileNameLength;  //32λ�޷������ͣ���������ļ������ȡ�
    char    UpdateFileName[256];   //�ɱ䳤�ֶΣ���������ļ�����   (�ݶ�256λ�ַ���)
    uint32  UpdateFileLength;      //32λ�޷������ͣ������Ա�ʾ4GB,��������ļ����ȡ����������ͨ�����������ͣ���ָ����������ļ����ȣ����������ͨ�������ŵ����ͣ�����ȡֵΪ0��
    //char UpdateFile[1024];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�����Ϊ���͵���������ļ��ģ����������ͨ�������ŵ����ͣ�������ڡ�
	//uint32  CRC32;               //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileINFO;
//������ɽṹ�����Ϻ�,�ڴ����ļ�����,Ȼ�����ļ�CRCУ����

//�Զ������������ͱ��� �����������䣩
typedef struct _UpdateFileCustomINFO_
{
	KH kh;
	uint64  SegmentNo;               //64λ�޷������ͣ���Ƭ��ţ���0��ʼ��ÿ��һ�����ݼ�1�� 
	char    UpdateSerialNo[16];    //128λ�ֶΣ���������������кš�
	char    reserved[16];          //�����ֶ�2 
	uint32  FID;                    //32λ�޷������ͣ�������������������ļ���ID�š�
	uint32  UpdateFileNameLength;  //32λ�޷������ͣ���������ļ������ȡ�
	char    UpdateFileName[256];   //�ɱ䳤�ֶΣ���������ļ�����  ע��Ϊ�˼��������ļ�����������Ĵ��䣬���ֶα������á���Ϊ����������ֶ���Ϊ/home/leonis/update/leonisupdate.zt����Ϊ�����ļ����ֶ���Ϊ/etc/CineCast/�����ļ���
	uint32  DataLength;			//32λ�޷������ͣ�����Ƭ��������ݳ���
	//char		Data[1024*128];       //�ɱ䳤�ֶΣ���������ļ������������ͨ�����������ͣ�ÿ��128K
	//uint32  CRC32;               //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileCustomINFO;

//���ջ�Զ�����������ͷ�������  ���ջ����յ���������ķ�����
typedef struct _UpdateFileCustomBACK_
{
	KH kh;
	uint32  UpdateCheckResult;     //32λ�޷������ͣ�����ȷ�ϡ�ֵΪ0��ʾ��������Ч��1��ʾ��������Ч��
	uint32  reserved;               //�����ֶ�
	char  OldVersion[16];          //128λ�ֶΣ������������ǰ�İ汾��Ϣ��
	char  UpdateSerialNo[16];      //128λ�ֶΣ����յ�������������кš�
	uint64  SegmentNo;			//64λ�޷������ͣ���Ƭ��ţ�����֪ͨ������ϵͳ�ĸ���Ƭ���ճɹ���ʧ�ܡ�
	char  reserved2[8];			//�����ֶ�
	uint32  CRC32;                  //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileCustomBACK;

typedef struct _UpdateCustomSTART_
{
	KH kh;
	char  UpdateSerialNo[16];          //128λ�ֶΣ��������������İ汾��Ϣ��
	char  reserved[16];
	uint32  CRC32;                   //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateCustomSTART;

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
    uint32  UpdateCheckResult;     //32λ�޷������ͣ�����ȷ�ϡ�ֵΪ0��ʾ��������Ч��1��ʾ��������Ч��
    uint32  reserved;               //�����ֶ�
    char  OldVersion[16];          //128λ�ֶΣ������������ǰ�İ汾��Ϣ��
    char  UpdateSerialNo[16];      //128λ�ֶΣ����յ�������������кš�
    char  reserved2[16];           //�����ֶ�
	uint32  CRC32;                  //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateFileBACK;


//���ջ�Զ��ά��������ϵͳ�ڽ��յ�Զ��������Я�췴�����ĺ�����ջ���������������
//������ջ���ʼ����
typedef struct _UpdateSTART_
{
	KH kh;
    char  UpdateSerialNo[16];          //128λ�ֶΣ��������������İ汾��Ϣ��
    uint32  reserved;
	uint32  CRC32;                   //rpchof ���ṹ�������ֶμ���ǰ��CRCУ��ֵ
}UpdateSTART;


//��־������,����ջ���������־
typedef struct _LOGREQUEST_
{
	KH kh;
    uint32  DateStart;   //��־�ļ���ʼ���� 32λ�޷������ͣ�ΪYYYYMMDDʮ���Ʊ�ʾ��
    uint32  DateEnd;    //��־�ļ��������� 32λ�޷������ͣ�ΪYYYYMMDDʮ���Ʊ�ʾ��
	uint32  LogType;    //��־���� 32λ�޷������ͣ�Ϊ0x74752485����ʾ��־ΪLEONISר�и�ʽ������ֵΪ��������ʽ��Ŀǰ������û��������ʽ����
	uint32  CRC32;   //CRC32 32λ�ֶΣ������ֶμ���ǰ��CRCУ��ֵ��  
}LOGREQUEST;


//��־��������,��������ͷ���������ݱ���,���ջ���������Ŀ��������
typedef struct _LOGBACKINFO_
{
	KH kh;  
    uint32  LogLengthALL;   //��־�ļ��ܳ���   uint32�����ջ���־�ļ����ܳ��ȡ�
    uint32  DateStart;      //��־�ļ���ʼ����  uint32��ΪYYYYMMDDʮ���Ʊ�ʾ��
    uint32  DateEnd;        //��־�ļ���������  uint32��ΪYYYYMMDDʮ���Ʊ�ʾ
	uint32  LogLengthCurr; //�����ϴ����ݳ��� uint32 �����ϴ�����־���ݳ��ȡ�
//	char    log[1024*10]; 	 //��־���ݱ���ɱ��ֶ�, variable	  bslbf
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
