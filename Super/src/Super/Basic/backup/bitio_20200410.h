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
    unsigned int bit_offset;        /* # of leftmost bits read/written      */

    /* bgmc encoder/decoder state: */
    unsigned int low, high;         /* current code region                  */
    unsigned int bits_to_follow;    /* encoder: the number of opposite bits */
    unsigned int value;             /* decoder: current code value          */

} BITIO;

/*
 * Function prototypes:
 */
BASIC_API void  bitio_init (unsigned char *buffer, int write, BITIO *p);   //要写流write设为1，读设为0

BASIC_API unsigned int  bitio_Terminate (BITIO *p);   //添加bit流起始位置到当前总字节数

BASIC_API long bitio_term (BITIO *p);

/* generic bit-level IO functions: */
BASIC_API void put_bits (unsigned int var,unsigned int len, BITIO *p);
BASIC_API unsigned int get_bits (unsigned int len, BITIO *p);  //int len

//直接读写一个字节
BASIC_API inline void put_char(unsigned char chin, BITIO *p)
{
    *p->pbs=chin;
     p->pbs++;
}

BASIC_API inline unsigned char get_char(BITIO *p)
{
    unsigned char cget=*p->pbs;
    p->pbs++;
    return cget;
}



//new
BASIC_API inline void AlignBitIO(BITIO *p)    //bit位对齐到一个字节
{
    if (p->bit_offset!=0)
    {
        p->bit_offset=0;
        p->pbs++;
    }
}

BASIC_API inline void ignore_bits(BITIO *p,int N)  //忽略N个字节，直接移位跳过，不读也不写
{
    p->pbs+=N;
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






#if 1   //以下代码无用，暂时屏蔽


/* Golomb-Rice codes: */
BASIC_API int rice_bits (int symbol, int s);
BASIC_API void rice_encode (int symbol, int s, BITIO *p);
BASIC_API void rice_dolby_encode (int symbol, int s, BITIO *p);
BASIC_API int rice_decode (int s, BITIO *p);
/* block-level functions: */
BASIC_API int rice_encode_block (int *block, int s, int N, BITIO *p);
BASIC_API int rice_decode_block (int *block, int s, int N, BITIO *p);
BASIC_API int rice_dolby_encode_block (int *block, int s, int N, BITIO *p);
/* new block Gilbert-Moore codes: */
BASIC_API int bgmc_encode_blocks (int *blocks, int start, short *s, short *sx, int N, int sub, BITIO *p);
BASIC_API int bgmc_decode_blocks (int *blocks, int start, short *s, short *sx, int N, int sub, BITIO *p);

BASIC_API void display_stats (void);
#endif



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

//#ifndef	BITIO_INCLUDED
//#define	BITIO_INCLUDED

#if defined(__cplusplus)


#if 0
class BASIC_API CBitIO
{
protected:
	unsigned char *pb, mask;
	long total;
public:
	void InitBitWrite(unsigned char *buffer);
	long EndBitWrite();
	long WriteByteAlign(unsigned char wert);
	long WriteBits(unsigned int value, short bits);
	long WriteRice(int *wert, char s, long N);

	void InitBitRead(unsigned char *buffer);
	long EndBitRead();
	long ReadBits(unsigned int *value, short bits);
	long ReadRice(long *wert, char s, long N);
};

#else

/*
 * rn_bitio.h/rn_bitio.c already provides generic bit-level IO routines
 * allowing much simpler and compact implementation of Golomb-Rice codes,
 * so we use this library. Function-wise it is absolutely identical to
 * Tilman's original implementation.
 */

//#include "rn_bitio.h"

/* C++ wrapper: */
struct BASIC_API CBitIO
{
    BITIO bio;

    void InitBitWrite(unsigned char *buffer)		{bitio_init (buffer, 1, &bio);}
    long EndBitWrite()								{return bitio_term (&bio);}
    long WriteByteAlign(unsigned char wert)			{put_bits (wert, 8, &bio); return 8;}
    long WriteRice(int *wert, char s, long N)		{return rice_encode_block (wert, s, N, &bio);}
	long WriteBits(unsigned int value,unsigned int bits)	{put_bits (value, bits, &bio); return bits;}
	long WriteDolby(int *wert, char s, long N)		{return rice_dolby_encode_block (wert, s, N, &bio);}

    void InitBitRead(unsigned char *buffer)			{bitio_init (buffer, 0, &bio);}
    long EndBitRead()								{return bitio_term (&bio);}
    //long ReadRice(int *wert, char s, long N)		{return rice_decode_block (wert, s, N, &bio);}
	//long ReadBits(unsigned int *value, short bits)	{*value = get_bits(bits, &bio); return bits;}

    long ReadRice(int *wert, char s, int N)		{return rice_decode_block (wert, s, N, &bio);}
    //old short 反而更慢  返回值没有必要
    void ReadBits(unsigned int *value,unsigned int bits)	{*value = get_bits(bits, &bio);}   


   // add by lizulin 20171018
	void WritePlex8(unsigned int nData)
	{
		//by lizulin 20171019
		const unsigned int Head=0xFFFFFFFF;
		if(nData<0xFF)
		{
			//put_bits (value,8,pBitio);
		    WriteBits(nData,8);
			return;
		}
		if(nData<0xFFFF)
		{
			//put_bits (Head,8,pBitio);
			//put_bits (value,16,pBitio);
			WriteBits(Head,8);
			WriteBits(nData,16);
			return;
		}
        
        //不需要 否则错误
		//if(nData<0xFFFFFF)
		//{
		//	//put_bits (Head,16,pBitio);
		//	//put_bits (value,24,pBitio);
		//	WriteBits(Head,16);
		//	WriteBits(nData,24);
		//	return;
		//}

		//put_bits (Head,24,pBitio);
		//put_bits (value,32,pBitio);
		WriteBits(Head,24);
		WriteBits(nData,32);

#if 0
		if(nData & 0xff000000)
		{
			m_wBitIO.WriteBits(0xffffff, 24);
			m_wBitIO.WriteBits(nData, 32);
			return;
		}
		if(nData & 0xff0000)
		{
			m_wBitIO.WriteBits(0xffff, 16);
			m_wBitIO.WriteBits(nData, 24);
			return;
		}
		if(nData & 0xff00)
		{
			m_wBitIO.WriteBits(0xff, 8);
			m_wBitIO.WriteBits(nData, 16);
			return;
		}
		m_wBitIO.WriteBits(nData, 8);
		return;
#endif
	}

	unsigned int ReadPlex8()
	{
        //20171122
        unsigned int t;
        ReadBits(&t, 8);
        if (t!=0XFF)
        {
            return t;
        }
        ReadBits(&t, 16);
        if (t!=0XFFFF)
        {
            return t;
        }

        //ReadBits(&t, 24);
        //if (t!=0XFFFFFF)
        //{
        //    return t;
        //}

        ReadBits(&t, 32);
        return t;



        //unsigned int t, tt;

        //ReadBits(&t, 8);
        //if(t != 0xff)
        //    return t;
        //ReadBits(&t, 8);
        //if(t != 0xff)
        //{
        //    ReadBits(&tt, 8);
        //    t = (t << 8) | tt;
        //    return t;
        //}
        //ReadBits(&t, 8);
        //if (t != 0xff)
        //{
        //    ReadBits(&tt, 16);
        //    t = (t << 16) | tt;
        //    return t;
        //}
        //ReadBits(&t, 32);
        //return t;

    }
};

#endif

//#endif	// BITIO_INCLUDED
#endif


#endif /* __RN_BITIO_H__ */

