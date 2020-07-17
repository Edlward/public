#ifndef _FastList_Header_
#define _FastList_Header_

#include "global.h" 

//自定义控件配置器
//一起特殊场景用来替换std::allocator



NAME_SPACE_MAIN_BEGIN


#include <list>
#include "Allocator.h"


class FastList
{
public:
    FastList(void);
    ~FastList(void);
};

NAME_SPACE_MAIN_END

#endif
