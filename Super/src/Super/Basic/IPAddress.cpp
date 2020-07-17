#include "IPAddress.h"
#include <iostream>
using namespace std;

NAME_SPACE_MAIN_BEGIN

IPAddress::IPAddress(int s0,int s1,int s2,int s3)
{
	//���渳ֵ��˳���漰�������ݴ洢�ṹ���ο�[�������������ڴ�����δ洢����](http://blog.csdn.net/sxhelijian/article/details/51015706)
	seg[0]=s3;
	seg[1]=s2;
	seg[2]=s1;
	seg[3]=s0;
}

void IPAddress::showIP()
{
	//��ʾ������Ǹ��˿��ģ���4�κ���
	cout<<int(seg[3])<<"."<<int(seg[2])<<"."<<int(seg[1])<<"."<<int(seg[0])<<endl;
	return;
}

bool IPAddress::sameSubnet(const IPAddress &ip, const IPAddress &mark)
{
	//���������밴λ�룬�Ǽ�����ڲ��Ĳ�����ֱ����һ���������������
	unsigned int i1, i2;
	i1=address&mark.address; //�������������߼�������
	i2=ip.address&mark.address; //�������������߼�������
	return (i1==i2); //������������߼�����������ͬ��������ͬһ����
}

char IPAddress::whatKind()
{
	//IP��ַ�����ͣ��뵽�ٶȰٿ���������IP��ַ��
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
		cout<<"����IP��ͬһ����"<<endl;
	else
		cout<<"����IP����ͬһ����"<<endl;
	cout<<"ip1����"<<ip1.whatKind()<<"������"<<endl;
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
    //%c�����þ��ǣ��ж�a[3]������û���ַ� ��ֹԽ�����
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
