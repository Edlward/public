#include "ByteOrder.h"

//�ο���qt


//��������Ԥ����ָ����Ԥ������ȷ���ֽ���Ĵ�С��ģʽ��

//ͨ�������֧��,����ȷ�ϴ���




#if 0
���ߣ�WSDAB
���ӣ�https://www.zhihu.com/question/377057346/answer/1071668573
��Դ��֪��
����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������

// �������� SDL2 2.0.10 �� SDL_endian.h �ļ�
#define CPU_LIL_ENDIAN 1234
#define CPU_BIG_ENDIAN 4321

#ifdef __linux__
#   include <endian.h>
#   define CPU_BYTEORDER  __BYTE_ORDER
#else /* __linux__ */
#   if defined(__hppa__) || \
        defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
        (defined(__MIPS__) && defined(__MISPEB__)) || \
        defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
        defined(__sparc__)
#       define CPU_BYTEORDER   CPU_BIG_ENDIAN
#   else
#       define CPU_BYTEORDER   CPU_LIL_ENDIAN
#   endif
#endif /* __linux__ */
#endif





//����ʵ��  ͨ��constexpr ���볣��ȷ�� C++11    ��֧��
//
//#ifdef _WIN32
//
//#if _MSC_VER <= 1900   //VS2015֮ǰ
//#define constexpr 
//#endif
//
//#else
//
//#define constexpr 
//
//#endif
//


 

//#include <cstdint>
#include <iostream>
 
enum hl_endianness
{
    HL_LITTLE_ENDIAN   = 0x03020100,
    HL_BIG_ENDIAN      = 0x00010203,
    HL_PDP_ENDIAN      = 0x01000302,
    HL_UNKNOWN_ENDIAN  = 0xFFFFFFFF
};

//constexpr 
static const unsigned int endianValues[4] = {0, 1, 2, 3};


//constexpr

 hl_endianness getEndianOrder() 
 {
    return
        (0x00 == endianValues[0])           // If Little Endian Byte Order,
            ? HL_LITTLE_ENDIAN              // return 0 for little endian.
            : (0x03 == endianValues[0])     // Else if Big Endian Byte Order,
                ? HL_BIG_ENDIAN             // return 1 for big endian.
                : (0x02 == endianValues[0]) // Else if PDP Endian Byte Order,
                    ? HL_PDP_ENDIAN         // return 2 for pdp endian.
                    : HL_UNKNOWN_ENDIAN;    // Else return -1 for wtf endian.
}

#define HL_ENDIANNESS getEndianOrder()




/*
 * Test program
 */
int test() 
{

    //#if defined _WIN32 || defined _WIN64
    //    static_assert(
    //        HL_ENDIANNESS == HL_LITTLE_ENDIAN,
    //        "Aren't Windows programs Little-Endian?"
    //    );
    //#endif


    //constexpr 
        hl_endianness endianness = HL_ENDIANNESS;

    std::cout << "This machine is: ";

    switch (endianness) {
        case HL_LITTLE_ENDIAN:
            std::cout << "LITTLE";
            break;
        case HL_BIG_ENDIAN:
            std::cout << "BIG";
            break;
        case HL_PDP_ENDIAN:
            std::cout << "PDP";
            break;
        case HL_UNKNOWN_ENDIAN:
        default:
            std::cout << "UNKNOWN";
    }

    std::cout << " endian" << std::endl;
}

