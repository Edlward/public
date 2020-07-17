#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//Core与UI共用的结构体，宏开关等
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
