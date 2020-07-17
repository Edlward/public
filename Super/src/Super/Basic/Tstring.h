#ifndef _Tstring_Header_
#define _Tstring_Header_

//#include "global.h" 
//#include <string.h>
#include <string>

std::wstring StringToWString(const std::string &str);
std::string WStringToString(const std::wstring &wstr);

inline std::wstring StringToWString(const std::string &str)
{
    std::wstring wstr(str.length(),L' ');
    std::copy(str.begin(), str.end(), wstr.begin());
    return wstr; 
}
//只拷贝低字节至string中
inline std::string WStringToString(const std::wstring &wstr)
{
    std::string str(wstr.length(), ' ');
    std::copy(wstr.begin(), wstr.end(), str.begin());
    return str; 
}



// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str);
// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str);

inline std::wstring StringToWString_utf8(const std::string &str){return utf8_to_wstring(str);}
inline std::string WStringToString_utf8(const std::wstring &wstr){return wstring_to_utf8(wstr);}

//gb2312和utf8相互转换：
//注意linux要支持zh_CN.GB18030代码才能正常运行。Ubuntu下使用cat /var/lib/locales/supported.d/local 来查看系统是否支持zh_CN.GB18030。

std::string gb2312_to_utf8(std::string const &strGb2312);
std::string utf8_to_gb2312(std::string const &strUtf8);




#if 0
//#include <string>
//class TString
//{
//    TString(const char* c)
//        :s(c)
//    {
//#if defined(UNICODE)
//        ws=StringToWString(s);
//#else
//
//#endif
//    }
//    ~TString()
//    {
//
//    }
//#if defined(UNICODE)
//    const wchar_t* c_str()
//    {
//        return ws.c_str();
//    }
//#else
//    const char* c_str()
//    {
//        return s.c_str();
//    }
//#endif
//
//private:
//    std::string s;
//#if defined(UNICODE)
//    std::wstring ws;
//#endif
//};
#endif




//宽字符支持待完善，为了跨平台，不使用windows宏
#ifdef UNICODE
typedef  std::wstring Tstring;
//typedef  strlen Tstrlen;
#define  Tstrlen wcslen
#else
typedef std::string  Tstring;
//typedef wstrlen Tstrlen;
#define  Tstrlen strlen
#endif


#ifdef UNICODE
inline std::wstring FromCstr(const wchar_t* pc)
{
    return std::wstring(pc);
}
inline std::wstring FromCstr(const char* pc)
{
    std::string s(pc);
    return StringToWString(s);
}
inline std::string TstringTostring(const Tstring& ts)
{
    return WStringToString(ts);
}
inline Tstring stringToTstring(const std::string& s)
{
    return StringToWString(s);
}

#else

inline std::string FromCstr(const char* pc)
{
    return std::string(pc);
}
inline std::string FromCstr(const wchar_t* pc)
{
    std::wstring ws(pc);
    return WStringToString(ws);
}
inline std::string TstringTostring(const Tstring& ts)
{
    return ts;
}
inline Tstring stringToTstring(const std::string& s)
{
    return s;
}
#endif


inline std::string FromTCstr(const wchar_t* pc)
{
    return TstringTostring(FromCstr(pc));
}

inline std::string FromTCstr(const char* pc)
{
    return TstringTostring(FromCstr(pc));
}










//gb2312和utf8相互转换：
//注意linux要支持zh_CN.GB18030代码才能正常运行。Ubuntu下使用cat /var/lib/locales/supported.d/local 来查看系统是否支持zh_CN.GB18030。

std::string gb2312_to_utf8(std::string const &strGb2312);
std::string utf8_to_gb2312(std::string const &strUtf8);








#endif


