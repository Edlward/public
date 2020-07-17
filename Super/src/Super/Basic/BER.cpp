#include "BER.h"
#include <cstdio>


//Ber±àÂë²Î¿¼×ÔAsdcp¿âKM_until.cpp  refactor add by lizulin  20190723
//------------------------------------------------------------------------------------------
// read a ber value from the buffer and compare with test value.
// Advances buffer to first character after BER value.
//

#include <stdint.h>

// Return the length in bytes of a BER encoded value
static uint32_t BER_length(const unsigned char* buf)
{
    if ( buf == 0 || (*buf & 0xf0) != 0x80 )
        return 0;

    return (*buf & 0x0f) + 1;
}


static bool read_test_BER(unsigned char **buf, int64_t test_value)
{
    if ( buf == 0 )
        return false;

    if ( ( **buf & 0x80 ) == 0 )
        return false;

    int64_t val = 0;
    uint8_t ber_size = ( **buf & 0x0f ) + 1;

    if ( ber_size > 9 )
        return false;

    for (uint8_t  i = 1; i < ber_size; i++ )
    {
        if ( (*buf)[i] > 0 )
            val |= (uint64_t)((*buf)[i]) << ( ( ( ber_size - 1 ) - i ) * 8 );
    }

    *buf += ber_size;
    return ( val == test_value );
}


//
static bool read_BER(const unsigned char* buf, uint64_t* val)
{
    uint8_t ber_size, i;

    if ( buf == 0 || val == 0 )
        return false;

    if ( ( *buf & 0x80 ) == 0 )
        return false;

    *val = 0;
    ber_size = ( *buf & 0x0f ) + 1;

    if ( ber_size > 9 )
        return false;

    for ( i = 1; i < ber_size; i++ )
    {
        if ( buf[i] > 0 )
            *val |= (uint64_t)buf[i] << ( ( ( ber_size - 1 ) - i ) * 8 );
    }
    return true;
}


static const uint64_t ber_masks[9] =
{
    uint64_t(0xffffffffffffffff), uint64_t(0xffffffffffffff00), 
    uint64_t(0xffffffffffff0000), uint64_t(0xffffffffff000000),
    uint64_t(0xffffffff00000000), uint64_t(0xffffff0000000000),
    uint64_t(0xffff000000000000), uint64_t(0xff00000000000000),0
};

//
static uint32_t get_BER_length_for_value(uint64_t val)
{
    for ( uint32_t i = 0; i < 9; i++ )
    {
        if ( ( val & ber_masks[i] ) == 0 )
            return i + 1;
    }

    //ui64Printer tmp_i(val);
    //DefaultLogSink().Error("BER integer encoding not supported for large value %s\n", tmp_i.c_str());
    return 0;
}

//
static bool write_BER(unsigned char* buf, uint64_t val, uint32_t ber_len)
{
    if ( buf == 0 )
        return false;

    if ( ber_len == 0 )
    { // calculate default length
        if ( val < 0x01000000L )
            ber_len = 4;
        else if ( val < uint64_t(0x0100000000000000) )
            ber_len = 8;
        else
            ber_len = 9;
    }
    else
    { // sanity check BER length
        if ( ber_len > 9 )
        {
            //DefaultLogSink().Error("BER integer length %u exceeds maximum size of 9\n", ber_len);
            return false;
        }

        if ( ( val & ber_masks[ber_len - 1] ) != 0 )
        {
            //ui64Printer tmp_i(val);
            //DefaultLogSink().Error("BER integer length %u too small for value %s\n", ber_len, tmp_i.c_str());
            return false;
        }
    }
    buf[0] = 0x80 + ( ber_len - 1 );
    for ( uint32_t i = ber_len - 1; i > 0; i-- )
    {
        buf[i] = (uint8_t)(val & 0xff);
        val >>= 8;
    }

    return true;
}







void Ber4Encoder(char* pb, unsigned int value)
{
#if 1
      //*(unsigned int*)pb=value;   //ÔÝ²»±àÂë
    write_BER((unsigned char*)pb,value,4);

#else
     pb[0] = 0x80 | 0x03;
     pb[1] = (value >> 16) & 0xff;
     pb[2] = (value >> 8) & 0xff;
     pb[3] = (value) & 0xff;
#endif
}

bool Ber4Decoder(const char * pb, unsigned int & value, char & derLen)
{
#if 1
     //value=*(unsigned int*)pb;  //ÔÝ²»±àÂë
    uint64_t tmp;
    bool ret=read_BER((const unsigned char*)pb,&tmp);
    value=tmp;

     return ret;
#else
    if (pb[0] & 0x80)
    {
        int count = pb[0] & 0x7f;
        if (count > 4)
        {
            printf("BER decode error\n");
            derLen = 0;
            return false;
        }

        switch (count)
        {
        case 0:
            derLen = 1;
            value = 0;
            break;
        case 1:
            derLen = 2;
            value = pb[1];
            break;
        case 2:
            derLen = 3;
            value = (pb[1] << 8) | pb[2];
            break;
        case 3:
            derLen = 4;
            value = (pb[1] << 16) | (pb[2] << 8) | pb[3];
            break;
        case 4:
            derLen = 5;
            value = (pb[1] << 24) | (pb[2] << 16) | (pb[3] << 8) | pb[4];
            break;
        }
        return true;
    }
    derLen = 0;
    value = 0;
    return false;
#endif

}


void Ber5Encoder(char* pb, unsigned int value)
{
#if 1
    write_BER((unsigned char*)pb,value,5);

#else
    pb[0] = 0x80|0x4;
    pb[1] = (value >> 24) & 0xff;
    pb[2] = (value >> 16) & 0xff;
    pb[3] = (value >> 8) & 0xff;
    pb[4] = (value) & 0xff;
#endif
}

bool Ber5Decoder(const char* pb, unsigned int& value, char& derLen)
{
#if 1
    uint64_t tmp;
    bool ret=read_BER((const unsigned char*)pb,&tmp);
    value=tmp;
    return ret;
#else
    if(pb[0] & 0x80)
    {
        int count = pb[0] & 0x7f;
        if(count > 4)
        {
            printf("BER decode error\n");
            derLen = 0;
            return false;
        }
        switch(count)
        {
        case 0:
            derLen = 1;
            value = 0;
            break;
        case 1:
            derLen = 2;
            value = pb[1];
            break;
        case 2:
            derLen = 3;
            value = (pb[1] << 8)|pb[2];
            break;
        case 3:
            derLen = 4;
            value = (pb[1]<<16)|(pb[2]<<8)|pb[3];
            break;
        case 4:
            derLen = 5;
            value = ((pb[1]<<24)&0xff000000)|((pb[2]<<16)&0x00ff0000)|((pb[3]<<8)&0x0000ff00)|(pb[4]&0xff);
            break;
        }
        return true;
    }
    derLen = 0;
    value = 0;
    return false;
#endif
}

