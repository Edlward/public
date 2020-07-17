/*
 * rn_bitio.h, 8/25/2003, Yuriy A. Reznik <yreznik@real.com>
 *
 * Contains basic Bit-level IO functions, Golomb-Rice codes,
 * and the new block Gilbert-Moore codes (BGMC) for prediction residual.
 *
 * This module is submitted for inclusion in the RM0 for MPEG-4 Audio
 * Lossless coding (ALS) standard (ISO/IEC 14496-3:2001/AMD 4).
 *
 **************************************************************************
 *
 * This software module was originally developed by
 *
 * Yuriy A. Reznik (RealNetworks, Inc.)
 *
 * in the course of development of the MPEG-4 Audio standard ISO/IEC 14496-3
 * and associated amendments. This software module is an implementation of
 * a part of one or more MPEG-4 Audio Lossless Coding (ISO/IEC 14496-3:2001/
 * AMD 4) tools as specified by the MPEG-4 Audio standard.
 *
 * ISO/IEC gives users of the MPEG-4 Audio standards free license to this
 * software module or modifications thereof for use in hardware or software
 * products claiming conformance to the MPEG-4 Audio standard.
 *
 * Those intending to use this software module in hardware or software
 * products are advised that this use may infringe existing patents.
 * The original developer of this software module and his/her company,
 * the subsequent editors and their companies, and ISO/IEC have no liability
 * for use of this software module or modifications thereof in an
 * implementation.
 *
 * Copyright is not released for non MPEG-4 Audio conforming products.
 * The original developer retains full right to use the code for his/her
 * own purpose, assign or donate the code to a third party and to inhibit
 * third party from using the code for non MPEG-4 Audio conforming
 * products.
 *
 * This copyright notice must be included in all copies or derivative works.
 *
 * Copyright (c) 2003.
 *
 */

#ifndef __RN_BITIO_H__
#define __RN_BITIO_H__  1           /* prevents multiple loading            */

#include "global.h" 


//refactor by lizulin BITIO *p参数移到第一个



#ifdef __cplusplus
extern "C" {                        /* be nice to our friends in C++        */
#endif

/*
 * Bit-level IO state structure:
 */
typedef struct
{
    /* bitstream variables: */
    unsigned char *start_pbs, *pbs; /* start/current byte position          */
    unsigned int bit_offset;             /* # of leftmost bits read/written      */

    /* bgmc encoder/decoder state: */
    unsigned int low, high;           /* current code region                  */
    unsigned int bits_to_follow;    /* encoder: the number of opposite bits */
    unsigned int value;                 /* decoder: current code value          */
} BITIO;

/*
 * Function prototypes:
 */

//使用注意:BIOIO实际操作的内存尾部要至少留64bit大小，否则可能越界崩溃
BASIC_API void  bitio_init(BITIO *p,unsigned char *buffer, int write);   //要写流write设为1，读设为0
BASIC_API unsigned int  bitio_Terminate (BITIO *p);   //添加bit流起始位置到当前总字节数
BASIC_API int bitio_term(BITIO *p);

BASIC_API void bitio_print(BITIO *p);  //print BITIO status info

//generic bit-level IO functions: len<=32
BASIC_API void put_bits(BITIO *p,unsigned int var,unsigned int len);
BASIC_API unsigned int get_bits(BITIO *p,unsigned int len);  //int len


//new
BASIC_API inline void AlignBitIO(BITIO *p)    //bit位对齐到一个字节,用于读
{
    if (p->bit_offset!=0)
    {
        p->bit_offset=0;
        p->pbs++;
    }
}

BASIC_API inline void AlignBitIOWrite(BITIO *p)    //bit位对齐到一个字节,用于写,多余bit位数据清零
{
    if (p->bit_offset!=0)
    {
        //把不足1字节的多余比特位清零  add by lizulin 20200430否则可能有脏数据
        unsigned char curr=*p->pbs;
        curr>>=p->bit_offset;
        curr<<=p->bit_offset;
        *p->pbs=curr;

        p->bit_offset=0;
        p->pbs++;
    }
}


BASIC_API inline void skip_bits(BITIO *p,int N)  //忽略N个字节，直接移位跳过，不读也不写
{
    p->pbs+=N;
}



//////////
//uint64 version
BASIC_API void put_bit64(BITIO *p,uint64_t var,unsigned int len);
BASIC_API uint64_t get_bit64(BITIO *p,unsigned int len); //int len

//1bit-fast version
BASIC_API void put_bit(BITIO *p,unsigned char bit);
BASIC_API unsigned int get_bit(BITIO *p);

//<=8bit-fast version
BASIC_API void put_bit8(BITIO *p,unsigned int value,unsigned int len);
BASIC_API unsigned int get_bit8(BITIO *p,unsigned int len);

//<=24bit-fast version
BASIC_API void put_bit24(BITIO *p,unsigned int value,unsigned int len);
BASIC_API unsigned int get_bit24(BITIO *p,unsigned int len);

//<=16bit的快速版本 应该与<=24bit效率相同没有必要单独实现
BASIC_API void put_bit16(BITIO *p,unsigned int value,unsigned int len);
BASIC_API unsigned int get_bit16(BITIO *p,unsigned int len);
//////////





//generic bit-level IO functions: len<=64  add by lizulin
//64位版暂时拆分成2次32位的实现
 BASIC_API inline void put_bit64(BITIO *p,uint64_t var,unsigned int len)
{
    if (len<=32)
    {
        put_bits(p,(unsigned int)var,len);
    }
    else
    {
        put_bits(p,(unsigned int)(var>>32),len-32);
        put_bits(p,(unsigned int)var,32);
    }
}

 BASIC_API inline uint64_t get_bit64(BITIO *p,unsigned int len) //int len
{
    uint64_t hi,lo;
    if (len<=32)
    {
        lo=get_bits(p,len);
        return lo;
    }

    hi=get_bits(p,len-32);
    lo=get_bits(p,32);
    return (hi<<32)|lo;
}


//读写1bit快速版本
//1-bit insertion:
 BASIC_API inline void put_bit(BITIO *p,unsigned char bit)
{
    register unsigned int l =p->bit_offset+1;
    /* add the bit to the bitstream: */
    p->pbs[0]|=bit<<(8-l);
    p->pbs[1]=0;         /* can be avoided if we zero buffer first */

    /* update bitstream vars: */
    p->pbs+=l>>3;
    p->bit_offset=l&7;
}


//1 bit retrieval:
BASIC_API inline unsigned int get_bit(BITIO *p)
{
    // retrieve the next bit from the bitstream:
    register unsigned int l=p->bit_offset+1;
    register unsigned int bit=(unsigned int)p->pbs[0]>>(8-l);

    // update the bistream:
    p->pbs+=l>>3;
    p->bit_offset =l&7;

    return bit&1;
}


//TODO:极限性能压榨,各种快速版本实现 by lizulin 20200415
//<=8bit的快速版本
BASIC_API inline void put_bit8(BITIO *p,unsigned int value,unsigned int len)
{
    register unsigned char *pbs=p->pbs;
    register unsigned int l=p->bit_offset+len;
    register unsigned int bits=value<<(16-l);
    pbs[0]|=(unsigned char)(bits>>8);
    pbs[1]=(unsigned char)bits;
    //*(unsigned short*)pbs=bits|((unsigned short)bits<<8);    //faster //error

    /* update bitstream vars: */
    p->pbs+=l>>3;
    p->bit_offset=l&7;
}

BASIC_API inline unsigned int get_bit8(BITIO *p,unsigned int len)
{
    //if (len==0) return 0; //don't check 

    // retrieve the next bit from the bitstream:
    register unsigned char *pbs=p->pbs;
    register unsigned int l=p->bit_offset+len;
    register unsigned short bits;
    bits=((unsigned short)pbs[0]<<8)|((unsigned short)pbs[1]);
    bits<<=p->bit_offset;

    //// update the bistream:
    p->pbs+=l>>3;
    p->bit_offset =l&7;
    return (unsigned int)(bits>>(16-len));
}


//<=24bit的快速版本
BASIC_API inline void put_bit24(BITIO *p,unsigned int value,unsigned int len)
{
    register unsigned char *pbs= p->pbs;
    register unsigned int l= p->bit_offset + len;
    register unsigned int bits=(unsigned int)value<<(32-l);
    pbs[0]|=(unsigned char)(bits>>24);
    pbs[1]=(unsigned char)(bits>>16);
    pbs[2]=(unsigned char)(bits>>8);
    pbs[3]=(unsigned char)bits;
    
    //*(unsigned int*)pbs=(bits&0X000000FF)|((bits<<8)&0X0000FF00)|((bits<<16)&0X00FF0000)|((bits|pbs[0]<<24));    //faster //error


    /* update bitstream vars: */
    p->pbs+=l>>3;
    p->bit_offset=l&7;
}

BASIC_API inline unsigned int get_bit24(BITIO *p,unsigned int len)
{
    //if (len==0) return 0; //don't check 

    // retrieve the next bit from the bitstream:
    register unsigned char *pbs=p->pbs;
    register unsigned int l=p->bit_offset+len;
    register unsigned int bits;
    bits=((unsigned int)pbs[0]<<24)|((unsigned int)pbs[1]<<16)| ((unsigned int)pbs[2]<<8)|((unsigned int)pbs[3]);

    bits <<=p->bit_offset;

    // update the bistream:
    p->pbs+=l>>3;
    p->bit_offset =l&7;
    return (unsigned int) (bits>>(32-len));
}

//<=16bit的快速版本 应该与<=24bit效率相同没有必要单独实现
BASIC_API inline void put_bit16(BITIO *p,unsigned int value,unsigned int len)
{
    put_bit24(p,value,len);
}

BASIC_API inline unsigned int get_bit16(BITIO *p,unsigned int len)
{
    return get_bit24(p,len);
}


//直接读写一个字节
BASIC_API inline void put_char(BITIO *p,unsigned char chin)
{
    *p->pbs++=chin;
}

BASIC_API inline unsigned char get_char(BITIO *p)
{
    return *(p->pbs++);
}



BASIC_API inline unsigned int ReadPlex8(BITIO* pBitio)
{
    unsigned int t= get_bit8(pBitio,8);
    if (t!=0XFF)
    {
        return t;
    }

    t= get_bit16(pBitio,16);
    if (t!=0XFFFF)
    {
        return t;
    }

    //不需要 否则错误
    //t= get_bits(pBitio,24);
    //if (t!=0XFFFFFF)
    //{
    //    return t;
    //}

    t=get_bits(pBitio,32);
    return t;
}

BASIC_API inline void WritePlex8(BITIO* pBitio,unsigned int value)
{
    //by lizulin
    const unsigned int Head=0xFFFFFFFF;
    if(value<0xFF)
    {
        put_bits (pBitio,value,8);
        return;
    }
    if(value<0xFFFF)
    {
        put_bits (pBitio,Head,8);
        put_bits (pBitio,value,16);
        return;
    }

    //根据文档这句不需要？
    //if(value<0xFFFFFF)
    //{
    //	put_bits (pBitio,Head,16);
    //	put_bits (pBitio,value,24);
    //	return;
    //}

    put_bits (pBitio,Head,24);
    put_bits (pBitio,value,32);

#if 0  //old
    if(value& 0xff000000)
    {
        put_bits (pBitio,Head,24);
        if (value==0xff000000)
        {
            put_bits (pBitio,Head,8);
            put_bits (pBitio,0,8);
        }
        put_bits (value,32,pBitio);
        return;
    }
    if(value& 0x00ff0000)
    {
        put_bits (pBitio,Head,16);
        if (value==0x00ff0000)
        {
            put_bits (pBitio,Head,8);
            put_bits (pBitio,0,8);
        }
        put_bits (pBitio,value,24);
        return;
    }
    if(value&0x0000ff00)
    {
        put_bits (pBitio,Head,8);
        if (value==0x0000ff00)
        {
            put_bits (pBitio,Head,8);
            put_bits (pBitio,0,8);
        }
        put_bits (pBitio,value,16);
        return;
    }

    if (value==0xff)
    {
        put_bits (pBitio,Head,8);
        put_bits (pBitio,0,8);
        put_bits (pBitio,value,8);
        return;
    }
    put_bits (value,8,pBitio);
#endif
}

BASIC_API inline unsigned int ReadPlex4(BITIO* pBitio)
{
    unsigned int t= get_bit8(pBitio,4);
    if (t!=0XF)
    {
        return t;
    }

    t= get_bit16(pBitio,8);
    if (t!=0XFF)
    {
        return t;
    }

    //不需要 否则错误
    //t= get_bits(pBitio,12);
    //if (t!=0XFFF)
    //{
    //    return t;
    //}

    //////////////////////////////////////////////////////////////////////////
    // Follow the new specification, add 32bit number support [2/24/2018 LEONIS]
    t= get_bit16(pBitio,16);
    if(t != 0xFFFF)
    {
        return t;
    }
    t = get_bits(pBitio,32);
    //////////////////////////////////////////////////////////////////////////
    return t;
}

BASIC_API inline void WritePlex4(BITIO* pBitio,unsigned int value)
{
    const unsigned int Head=0xFFFFFFFF;
    if(value<0xF)
    {
        put_bits(pBitio,value,4);
        return;
    }
    if(value<0xFF)
    {
        put_bits (pBitio,Head,4);
        put_bits (pBitio,value,8);
        return;
    }
    //根据文档这句不需要？
    //if(value<0xFFF)
    //{
    //	put_bits (pBitio,Head,8);
    //	put_bits (pBitio,value,12);
    //	return;
    //}
    put_bits (pBitio,Head,12);
    put_bits (pBitio,value,16);
}


BASIC_API inline size_t getBytesOfElement(const BITIO& BitIOBegin,const BITIO& BitIOEnd)  
{
    return BitIOEnd.pbs-BitIOBegin.pbs;
}

BASIC_API inline size_t getBitsOfElement(const BITIO& BitIOBegin,const BITIO& BitIOEnd)
{
    size_t Bytes=BitIOEnd.pbs-BitIOBegin.pbs;
    size_t Bits=Bytes*8-BitIOBegin.bit_offset+BitIOEnd.bit_offset;
    return Bits;
}




/* Golomb-Rice codes: */
BASIC_API int rice_bits (int symbol, int s);
BASIC_API void rice_encode (BITIO *p,int symbol, int s);
BASIC_API void rice_dolby_encode (BITIO *p,int symbol, int s);
BASIC_API int rice_decode (BITIO *p,int s);
/* block-level functions: */
BASIC_API int rice_encode_block (BITIO *p,int *block, int s, int N);
BASIC_API int rice_decode_block (BITIO *p,int *block, int s, int N);
BASIC_API int rice_dolby_encode_block (BITIO *p,int *block, int s, int N);
/* new block Gilbert-Moore codes: */
BASIC_API int bgmc_encode_blocks (BITIO *p,int *blocks, int start, short *s, short *sx, int N, int sub);
BASIC_API int bgmc_decode_blocks (BITIO *p,int *blocks, int start, short *s, short *sx, int N, int sub);
BASIC_API void display_stats (void);


#ifdef __cplusplus
}
#endif







 /***************** MPEG-4 Audio Lossless Coding **************************

This software module was originally developed by

Tilman Liebchen (Technical University of Berlin)

in the course of development of the MPEG-4 Audio standard ISO/IEC 14496-3
and associated amendments. This software module is an implementation of
a part of one or more MPEG-4 Audio lossless coding tools as specified
by the MPEG-4 Audio standard. ISO/IEC gives users of the MPEG-4 Audio
standards free license to this software module or modifications
thereof for use in hardware or software products claiming conformance
to the MPEG-4 Audio standards. Those intending to use this software
module in hardware or software products are advised that this use may
infringe existing patents. The original developer of this software
module, the subsequent editors and their companies, and ISO/IEC have
no liability for use of this software module or modifications thereof
in an implementation. Copyright is not released for non MPEG-4 Audio
conforming products. The original developer retains full right to use
the code for the developer's own purpose, assign or donate the code to
a third party and to inhibit third party from using the code for non
MPEG-4 Audio conforming products. This copyright notice must be included
in all copies or derivative works.

Copyright (c) 2003.

filename : bitio.h
project  : MPEG-4 Audio Lossless Coding
author   : Tilman Liebchen (Technical University of Berlin)
date     : June 16, 2003
contents : Header file for bitio.cpp

*************************************************************************/

/*************************************************************************
 *
 * Modifications:
 *
 * 11/11/2003, Tilman Liebchen <liebchen@nue.tu-berlin.de>
 *   - added functions WriteBits() and ReadBits()
 *
 * 03/17/2004, Koichi Sugiura <ksugiura@mitaka.ntt-at.co.jp>
 *   - added BITIO_INCLUDED symbol to avoid multiple inclusion
 *
 *************************************************************************/


/*
 * rn_bitio.h/rn_bitio.c already provides generic bit-level IO routines
 * allowing much simpler and compact implementation of Golomb-Rice codes,
 * so we use this library. Function-wise it is absolutely identical to
 * Tilman's original implementation.
 */

//#include "rn_bitio.h"

/* C++ wrapper: */
class BASIC_API CBitIO
{
public:
    BITIO bio;
public:
    void InitBitWrite(unsigned char *buffer)   {bitio_init (&bio,buffer,1);}
    int EndBitWrite()                                      {return bitio_term(&bio);}
    int WriteByteAlign(unsigned char wert)    {put_bits(&bio,wert,8); return 8;}   //error?

    //add bylizulin
    void WriteAlignByte(){AlignBitIOWrite(&bio);}   
    BITIO currState(){return bio;}  //获取前后两种状态，用于计算距离差
    size_t UsedBytes(){return bio.pbs-bio.start_pbs;}
    size_t UsedBits(){return 8*(bio.pbs-bio.start_pbs)+bio.bit_offset;}


    int WriteRice(int *wert, char s, long N)     {return rice_encode_block (&bio,wert,s,N);}
    //int WriteRice(int& wert, char s, long N)     {return rice_encode_block (&bio,&wert,s,N);}

    unsigned int WriteBits(unsigned int value,unsigned int bits){put_bits (&bio,value, bits); return bits;}
    //fast version
    void WriteBit(unsigned int value){put_bit(&bio,value);}                                         //1-bit fast version 
    void WriteBit8(unsigned int value,unsigned int len){put_bit8(&bio,value,len);}      //len<=8bit fast version 
    void WriteBit16(unsigned int value,unsigned int len){put_bit16(&bio,value,len);}  //len<=16bit fast version
    void WriteBit24(unsigned int value,unsigned int len){put_bit24(&bio,value,len);}  //len<=24bit fast version

    //int版重载
    void WriteBit(int value){put_bit(&bio,value);}                                         //1-bit fast version 
    void WriteBit8(int value,unsigned int len){put_bit8(&bio,value,len);}      //len<=8bit fast version 
    void WriteBit16(int value,unsigned int len){put_bit16(&bio,value,len);}  //len<=16bit fast version
    void WriteBit24(int value,unsigned int len){put_bit24(&bio,value,len);}  //len<=24bit fast version

    int WriteDolby(int *wert, char s, long N){return rice_dolby_encode_block (&bio,wert,s,N);}

    void InitBitRead(unsigned char *buffer){bitio_init (&bio,buffer,0);}
    int EndBitRead()                                    {return bitio_term (&bio);}
    //long ReadRice(int *wert, char s, long N)        {return rice_decode_block (wert, s, N, &bio);}
    //long ReadBits(unsigned int *value, short bits)    {*value = get_bits(bits, &bio); return bits;}

    int ReadRice(int *wert, char s, int N)        {return rice_decode_block (&bio,wert,s,N);}
    
    //通用版本
    void ReadBits(unsigned int *value,unsigned int len){*value = get_bits(&bio,len);}    //len<=32bit
    void ReadBits(int *value,unsigned int len){*value = get_bits(&bio,len);}                   //len<=32bit

    //各种确定长度的快速版本 add by lizulin
    void ReadBit(unsigned int *value){*value = get_bit(&bio);}                                          //1 bit fast version 
    void ReadBit8(unsigned int *value,unsigned int len){*value = get_bit8(&bio,len);}       //len<=8bit    fast version
    void ReadBit16(unsigned int *value,unsigned int len){*value = get_bit16(&bio,len);}   //len<=16bit   fast version
    void ReadBit24(unsigned int *value,unsigned int len){*value = get_bit24(&bio,len);}   //len<=24bit   fast version
    //int
    void ReadBit(int *value){*value = get_bit(&bio);}                                          //1 bit fast version 
    void ReadBit8(int *value,unsigned int len){*value = get_bit8(&bio,len);}       //len<=8bit    fast version
    void ReadBit16(int *value,unsigned int len){*value = get_bit16(&bio,len);}   //len<=16bit   fast version
    void ReadBit24(int *value,unsigned int len){*value = get_bit24(&bio,len);}   //len<=24bit   fast version

    //引用版重载
    void ReadBits(unsigned int& value,unsigned int len){value = get_bits(&bio,len);}       //len<=32bit
    void ReadBit(unsigned int& value){value = get_bit(&bio);}                                         //1 bit fast version 
    void ReadBit8(unsigned int& value,unsigned int len){value = get_bit8(&bio,len);}       //len<=8bit    fast version
    void ReadBit16(unsigned int& value,unsigned int len){value = get_bit16(&bio,len);}   //len<=16bit   fast version
    void ReadBit24(unsigned int& value,unsigned int len){value = get_bit24(&bio,len);}   //len<=24bit   fast version

    //int 
    void ReadBits(int& value,unsigned int len){value = get_bits(&bio,len);}       //len<=32bit
    void ReadBit(int& value){value = get_bit(&bio);}                                         //1 bit fast version 
    void ReadBit8(int& value,unsigned int len){value = get_bit8(&bio,len);}       //len<=8bit    fast version
    void ReadBit16(int& value,unsigned int len){value = get_bit16(&bio,len);}   //len<=16bit   fast version
    void ReadBit24(int& value,unsigned int len){value = get_bit24(&bio,len);}   //len<=24bit   fast version

     //为了便于维护统一底层IO的实现 by lizulin
     void WritePlex8(unsigned int value)
     {
         ::WritePlex8(&bio,value);
     }
     void WritePlex8(int value)
     {
         ::WritePlex8(&bio,value);
     }
     unsigned int ReadPlex8()
     {
         return ::ReadPlex8(&bio);
     }
     void WritePlex4(unsigned int value)
     {
         ::WritePlex4(&bio,value);
     }
     void WritePlex4(int value)
     {
         ::WritePlex4(&bio,value);
     }
     unsigned int ReadPlex4()
     {
         return ::ReadPlex4(&bio);
     }



   //// add by lizulin 20171018
   // void WritePlex8(unsigned int nData)
   // {
   //     const unsigned int Head=0xFFFFFFFF;
   //     if(nData<0xFF)
   //     {
   //         WriteBits(nData,8);
   //         return;
   //     }
   //     if(nData<0xFFFF)
   //     {
   //         WriteBits(Head,8);
   //         WriteBits(nData,16);
   //         return;
   //     }

   //     ////不需要? 否则错误--待确认
   //     //if(nData<0xFFFFFF)
   //     //{
   //     //     WriteBits(Head,16);
   //     //     WriteBits(nData,24);
   //     //     return;
   //     //}
   //     WriteBits(Head,24);
   //     WriteBits(nData,32);
   // }

   // unsigned int ReadPlex8()
   // {
   //     //20171122
   //     unsigned int t;
   //     ReadBits(&t,8);
   //     if (t!=0XFF)
   //     {
   //         return t;
   //     }
   //     ReadBits(&t,16);
   //     if (t!=0XFFFF)
   //     {
   //         return t;
   //     }

   //     //ReadBits(&t, 24);
   //     //if (t!=0XFFFFFF)
   //     //{
   //     //    return t;
   //     //}

   //     ReadBits(&t,32);
   //     return t;
   // }
};



#ifdef UNIT_TEST
BASIC_API int Test_BitIO();
#endif


#endif /* __RN_BITIO_H__ */

