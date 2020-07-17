#ifndef PROTOCOL_H
#define PROTOCOL_H

#pragma pack(push,1)     //设置一个字节对齐  

typedef struct _KL_
{
	unsigned short head;         //报文头  0x7585  0x7587?;
	unsigned short key;           //报文类型  
	unsigned int len;               //报文长度  不包括此头  ID为16bits。 为了传送大文件，改int类型
} KL;
#pragma pack(pop)       //恢复原来对齐状态
 

typedef enum
{   
    KL_HEAD=0x7585,             //数据报文头
    ZERO=0x0000                 //填充cmd_Sub
}ETYPE_TEST; 


//客户端类型枚举，MPU用来区分客户端类型   Protool插件，Monitor,MPU UI
typedef struct _ClientType_
{
	int ClientType;
}ClientType;

enum EMClientType
{
	CLIENT_UNKNOW=0x10,    //初始化时的未知状态
	CLIENT_PANNER,     //protool插件
	CLIENT_MONITOR,   //monitor
	CLIENT_MPUUI,        //mpu ui
	CLIENT_DCAPUI,       //mpu ui
	CLIENT_DESIGNER    //待添加
};


enum PROTOCOL 
{
	//协议汇总
	//定义新报文的时候，可以暂不指定key的值, 只需要确认枚举名，  在对接时候同步此enum PROTOCOL结构  即可
	//待项目稳定时候再填充协议key值,

	CMD_TEXT_MESSAGE,
	//CMD_SEND_MESSAGE_RESP,

	//MPU与MPUUI,MONITOR,PANNER共用的通讯协议
	CMD_SET_CLIENTTYPE=0X1000,          //用在MPU的Server区分Client类型
	CMD_SET_CLIENTTYPE_RESP,

	//CMD_START表示各个子窗口命令的起始倿
	CMD_UI_BEGIN=0X9000,
	CMD_UI_END,
};

#endif







