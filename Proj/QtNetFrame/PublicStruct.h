#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//Core��UI���õĽṹ�壬�꿪�ص�
#include <string>
#include <vector>

typedef struct _FilePacket_
{
    char fileName[1024];
    unsigned int fileSize;
    unsigned int TotalPackages;
    unsigned int CurrNum;
    unsigned int len;
    char data[100*1024];

}FilePacket;

#endif
