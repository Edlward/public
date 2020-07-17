#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "global.h"

NAME_SPACE_MAIN_BEGIN

//IP地址转唯一的int标记的快速方法，用共用体实现
class IPAddress
{
private:
	union
	{
		unsigned char seg[4]; //IP地址共4字节，或者看成无符号字符数组所指4部分
		unsigned int address; //或者看成一个整体
	};
public:
	IPAddress(int=0,int=0,int=0,int=0);
	void showIP();
	bool sameSubnet(const IPAddress &ip, const IPAddress &mark);
	char whatKind();
};


//判断IP地址是否合法
bool isIPAddressValid(const char* ipAddress);



#ifdef UNIT_TEST
int Test_IPAddress();
#endif


NAME_SPACE_MAIN_END
#endif
