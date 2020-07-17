#ifndef _CRC_Header__
#define _CRC_Header__
//include crc32 crc16 crc64 

#include "global.h" 
#include<stdio.h>
#include<stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    //TO TEST
    unsigned short CRC16_CCITT(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_CCITT_FALSE(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_XMODEM(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_X25(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_MODBUS(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_IBM(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_MAXIM(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_USB(const unsigned char *puchMsg, unsigned int usDataLen);
    unsigned short CRC16_XModemByTable(const void *pData, int ilength);
    unsigned short crc16(const unsigned char *buf,unsigned short length);


    uint32_t Get_CRC32_fromBuffer(uint32_t crcinit, uint8_t * bs, uint32_t bssize);//获得CRC
    uint32_t GetCRC32_fromFile(FILE *fd);//获得文件CRC


     uint64_t crc64(const char *buf,size_t len);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif



