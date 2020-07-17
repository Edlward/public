#ifndef _FastVector_Header_
#define _FastVector_Header_


#include "global.h" 

//自定义控件配置器
//一起特殊场景用来替换std::allocator



NAME_SPACE_MAIN_BEGIN


#include <vector>

class FastVector
{
public:
    FastVector(void);
    ~FastVector(void);
};

NAME_SPACE_MAIN_END

#endif

