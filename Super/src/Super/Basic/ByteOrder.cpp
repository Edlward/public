#include "ByteOrder.h"

//参考自qt


//怎样利用预处理指令在预编译期确定字节序的大小端模式？

//通过编译宏支持,测试确认待续




#if 0
作者：WSDAB
链接：https://www.zhihu.com/question/377057346/answer/1071668573
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

// 代码来自 SDL2 2.0.10 的 SDL_endian.h 文件
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





//其他实现  通过constexpr 编译常量确定 C++11    才支持
//
//#ifdef _WIN32
//
//#if _MSC_VER <= 1900   //VS2015之前
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

