
#ifndef PROTOCOLSTATUS_H
#define PROTOCOLSTATUS_H

#pragma pack(push,1)     //设置一个字节对齐  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dcap-Core通过UDP发送设备音量状态给外部使用的通信协议结构体
typedef struct _DecStatus_{
	unsigned short int  AudioType;               //0:无有效数据 //1:基于对象的中国多维声//2:基于声道的中国多维声
	                                                              //3:标准5.1   //7.1?//4:atmos//5:dts//6:holosound
	unsigned short int SpkNum;                   //扬声器总数,  add by lizulin 
	unsigned short int AudioLevel[256];       //分解到多个点的电平信号，用于影厅演示点灯用，注1                                                                                                                 //后面根据需要扩展……
	_DecStatus_()
	{
		AudioType=1;
		SpkNum=0;
		memset(&AudioLevel,0,sizeof(AudioLevel)*sizeof(unsigned short int));
	}
}DecStatus;

typedef struct _DecStatusPkg_{
	char Cmd[10];                              //MyStatus”
	char IDCode[20];                          //设备标识字   “DACP-OBIA”  IdentifyingCode IDcode
	unsigned char StartByte;              //恒为0xca
	unsigned char CmdType;              //恒为0x3
	unsigned short ParaStaAdd;         //恒为6000
	unsigned short ParaNum;            //sizeof( TObjDecStatus)
	DecStatus Status;
	//unsigned char Para[sizeof(TObjDecStatus)]; //TObjDecStatus实体，目前来看，1个就够了（可点256个灯）
	unsigned short ChkSum;              //不关心
	_DecStatusPkg_()
	{
		 char tmpCmd[sizeof(Cmd)]="MyStatus";
	     for (int i=0;i<sizeof(Cmd);i++)
	     {
			 Cmd[i]=tmpCmd[i];       //不用函数初始化结构内字符数组的方法
	     }
		 char tmpIdentifyingCode[sizeof(IDCode)]="DACP-OBIA";
		 for (int i=0;i<sizeof(IDCode);i++)
		 {
			 IDCode[i]=tmpIdentifyingCode[i];       //不用函数初始化数组的方法
		 }
		StartByte=0xca;                                         //恒为0xca
		CmdType=0x03;                                          //恒为0x3
		ParaStaAdd=6000;                                     //恒为6000
		ParaNum=sizeof(DecStatus);            //sizeof( TObjDecStatus)
	}
}DecStatusPkg;   //UDP封装包
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)       //恢复原来对齐状态

#endif



