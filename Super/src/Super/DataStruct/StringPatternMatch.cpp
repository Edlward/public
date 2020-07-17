#include "StringPatternMatch.h"


/*
Brute-Force�㷨���BF�㷨��������(Brute Force)�㷨������ͨ��ģʽƥ���㷨��BF�㷨��˼����ǽ�Ŀ�괮S�ĵ�һ���ַ���ģʽ��T�ĵ�һ���ַ�����ƥ�䣬����ȣ�������Ƚ�S�ĵڶ����ַ��� T�ĵڶ����ַ���������ȣ���Ƚ�S�ĵڶ����ַ���T�ĵ�һ���ַ������αȽ���ȥ��ֱ���ó�����ƥ������
*/

#include<string>
int StringPatternMatchBy_BruteForce(std::string sSrc, std::string sPattern) 
{
    unsigned int i = 0;
    unsigned int j = 0;
    int pos = -1;  
    while (i < sSrc.length()&&j < sPattern.length())   // ѭ���Ƚϵ�����
    {
        if (sSrc[i] == sPattern[j])  // ������ ������Ƚ�
        {
            i++;
            j++;
        }
        else 
        { // ���������ָ�������ġ��ƶ�ָ�롱i���ݣ���ָ��ģʽ���ġ��ƶ�ָ�롱jָ���ʼλ��0
            i = i - (j - 1); // �����ġ��ƶ�ָ�롱iӦ�û��ݣ�i-1������λ
            j = 0;
        }
    }
    // ���j>=T.length() ��˵��ƥ��ɹ�  ��ô�޸�pos
    if (j >= sPattern.length()) 
    {
        pos = i - sPattern.length();
        return pos;
    }
    else   // ����ֱ��-1
    {
        return -1;
    }
}

