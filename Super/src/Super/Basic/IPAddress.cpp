#include "IPAddress.h"
#include <iostream>
using namespace std;

NAME_SPACE_MAIN_BEGIN

IPAddress::IPAddress(int s0,int s1,int s2,int s3)
{
	//下面赋值的顺序，涉及整型数据存储结构，参考[《整型数据在内存中如何存储？》](http://blog.csdn.net/sxhelijian/article/details/51015706)
	seg[0]=s3;
	seg[1]=s2;
	seg[2]=s1;
	seg[3]=s0;
}

void IPAddress::showIP()
{
	//显示结果，是给人看的，分4段合适
	cout<<int(seg[3])<<"."<<int(seg[2])<<"."<<int(seg[1])<<"."<<int(seg[0])<<endl;
	return;
}

bool IPAddress::sameSubnet(const IPAddress &ip, const IPAddress &mark)
{
	//与子网掩码按位与，是计算机内部的操作，直接以一个整体操作更方便
	unsigned int i1, i2;
	i1=address&mark.address; //和子网掩码作逻辑与运算
	i2=ip.address&mark.address; //和子网掩码作逻辑与运算
	return (i1==i2); //与子网掩码的逻辑与运算结果相同，则属于同一子网
}

char IPAddress::whatKind()
{
	//IP地址的类型，请到百度百科中搜索《IP地址》
	if(seg[3]<128)
		return 'A';
	else if(seg[3]<192)
		return 'B';
	else if(seg[3]<224)
		return 'C';
	else if(seg[3]<240)
		return 'D';
	else
		return 'E';
}

#ifdef UNIT_TEST

int Test_IPAddress()
{
	IPAddress ip1(202,194,116,97), ip2(202,194,119,102), mark(255,255,248,0);
	cout<<"ip1: ";
	ip1.showIP();
	cout<<"ip2: ";
	ip2.showIP();
	if(ip1.sameSubnet(ip2,mark))
		cout<<"两个IP在同一子网"<<endl;
	else
		cout<<"两个IP不在同一子网"<<endl;
	cout<<"ip1属于"<<ip1.whatKind()<<"类网络"<<endl;
	return 0;
}

bool isIPAddressValid(const char* ipAddress)
{
    if(ipAddress==NULL)
    {
        return false;
    }

    int a,b,c,d;
    char end;
    //%c的作用就是：判断a[3]后面有没有字符 防止越界崩溃
    if ((sscanf(ipAddress,"%d.%d.%d.%d%c",&a,&b,&c,&d,&end)==4)
        &&(a>=0&&a<=255)
        &&(b>=0&&b<=255)
        &&(c>=0&&c<=255)
        &&(d>=0&&d<=255))
    {
        return true;
    }
    return false;
}

#endif


NAME_SPACE_MAIN_END
