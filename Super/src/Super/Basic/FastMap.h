#ifndef _FastMap_Header_
#define _FastMap_Header_

#include "global.h" 

//自定义控件配置器
//一起特殊场景用来替换std::allocator



NAME_SPACE_MAIN_BEGIN


#include <map>
#include "Allocator.h"


class FastMap
{
public:
    FastMap(void);
    ~FastMap(void);
};

NAME_SPACE_MAIN_END

#endif
