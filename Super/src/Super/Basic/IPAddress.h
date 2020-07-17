#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "global.h"

NAME_SPACE_MAIN_BEGIN

//IP��ַתΨһ��int��ǵĿ��ٷ������ù�����ʵ��
class IPAddress
{
private:
	union
	{
		unsigned char seg[4]; //IP��ַ��4�ֽڣ����߿����޷����ַ�������ָ4����
		unsigned int address; //���߿���һ������
	};
public:
	IPAddress(int=0,int=0,int=0,int=0);
	void showIP();
	bool sameSubnet(const IPAddress &ip, const IPAddress &mark);
	char whatKind();
};


//�ж�IP��ַ�Ƿ�Ϸ�
bool isIPAddressValid(const char* ipAddress);



#ifdef UNIT_TEST
int Test_IPAddress();
#endif


NAME_SPACE_MAIN_END
#endif
