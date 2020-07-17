
#ifndef PROTOCOLSTATUS_H
#define PROTOCOLSTATUS_H

#pragma pack(push,1)     //����һ���ֽڶ���  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dcap-Coreͨ��UDP�����豸����״̬���ⲿʹ�õ�ͨ��Э��ṹ��
typedef struct _DecStatus_{
	unsigned short int  AudioType;               //0:����Ч���� //1:���ڶ�����й���ά��//2:�����������й���ά��
	                                                              //3:��׼5.1   //7.1?//4:atmos//5:dts//6:holosound
	unsigned short int SpkNum;                   //����������,  add by lizulin 
	unsigned short int AudioLevel[256];       //�ֽ⵽�����ĵ�ƽ�źţ�����Ӱ����ʾ����ã�ע1                                                                                                                 //���������Ҫ��չ����
	_DecStatus_()
	{
		AudioType=1;
		SpkNum=0;
		memset(&AudioLevel,0,sizeof(AudioLevel)*sizeof(unsigned short int));
	}
}DecStatus;

typedef struct _DecStatusPkg_{
	char Cmd[10];                              //MyStatus��
	char IDCode[20];                          //�豸��ʶ��   ��DACP-OBIA��  IdentifyingCode IDcode
	unsigned char StartByte;              //��Ϊ0xca
	unsigned char CmdType;              //��Ϊ0x3
	unsigned short ParaStaAdd;         //��Ϊ6000
	unsigned short ParaNum;            //sizeof( TObjDecStatus)
	DecStatus Status;
	//unsigned char Para[sizeof(TObjDecStatus)]; //TObjDecStatusʵ�壬Ŀǰ������1���͹��ˣ��ɵ�256���ƣ�
	unsigned short ChkSum;              //������
	_DecStatusPkg_()
	{
		 char tmpCmd[sizeof(Cmd)]="MyStatus";
	     for (int i=0;i<sizeof(Cmd);i++)
	     {
			 Cmd[i]=tmpCmd[i];       //���ú�����ʼ���ṹ���ַ�����ķ���
	     }
		 char tmpIdentifyingCode[sizeof(IDCode)]="DACP-OBIA";
		 for (int i=0;i<sizeof(IDCode);i++)
		 {
			 IDCode[i]=tmpIdentifyingCode[i];       //���ú�����ʼ������ķ���
		 }
		StartByte=0xca;                                         //��Ϊ0xca
		CmdType=0x03;                                          //��Ϊ0x3
		ParaStaAdd=6000;                                     //��Ϊ6000
		ParaNum=sizeof(DecStatus);            //sizeof( TObjDecStatus)
	}
}DecStatusPkg;   //UDP��װ��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)       //�ָ�ԭ������״̬

#endif



