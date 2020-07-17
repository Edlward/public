#ifndef _FastString_Header_
#define _FastString_Header_

#include "global.h" 

//自定义控件配置器
//一起特殊场景用来替换std::allocator



NAME_SPACE_MAIN_BEGIN


#include <string>
#include "Allocator.h"

//typedef basic_string<char, char_traits<char>, allocator<char> >
//    string;
//typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
//    wstring;

 

class FastString
{
public:
    FastString(void);
    ~FastString(void);
};

NAME_SPACE_MAIN_END

#endif
