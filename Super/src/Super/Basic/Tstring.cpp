#include "Tstring.h"

#include <locale>
#include <vector>



//string与wstring转换
//方法一：MultiByteToWideChar、WideCharToMultiByte  WindowsAPI 不跨平台
//方法二：std::copy
#if 1   // 
//std::wstring StringToWString(const std::string &str)
//{
//    std::wstring wstr(str.length(),L' ');
//    std::copy(str.begin(), str.end(), wstr.begin());
//    return wstr; 
//}
////只拷贝低字节至string中
//std::string WStringToString(const std::wstring &wstr)
//{
//    std::string str(wstr.length(), ' ');
//    std::copy(wstr.begin(), wstr.end(), str.begin());
//    return str; 
//}

//std::string a = "abcdefg";
//std::wstring b(a.begin(),a.end());  //string 转换为wstring
//std::string c(b.begin(),b.end());  //wstring 转换为string
//这样转换对于所有单字节的字符集来说没问题，但是遇到多字节的字符集（如中文）
//就有问题了，因为它是一个字符一个字符转换的，而一个中文包含两个字符



#else


//string 与 wstring的相关转换：跨平台的，可在Windows下使用，也可在Linux下使用。
//方法三：使用CRT库的mbstowcs()函数和wcstombs()函数，平台无关，需设定locale。
#include <locale.h>  
//可以再改进避免多次new delete
std::string WStringToString(const std::wstring& ws)
{
    //std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";   	std::string strLocale = setlocale(LC_ALL, ""); 
    //setlocale(LC_ALL, "chs");  
    //const wchar_t* _Source = ws.c_str();  
    //size_t _Dsize = 2 * ws.size() + 1;  
    //char *_Dest = new char[_Dsize];  
    //memset(_Dest,0,_Dsize);  
    //wcstombs(_Dest,_Source,_Dsize);  
    //std::string result(_Dest,ws.size() );       //old std::string result = _Dest;   refactor by lizulin
    //delete []_Dest;  
    //setlocale(LC_ALL, curLocale.c_str());  
    //return result;

    //fast way: refactor by lizulin
    char* oldLocale = setlocale(LC_ALL,NULL); // curLocale = "C";   	std::string strLocale = setlocale(LC_ALL, ""); 
    setlocale(LC_ALL, "chs");  
    const wchar_t* _Source = ws.c_str();  
    std::string result(ws.length(),' ');  
    char *_Dest = const_cast<char*>(result.data());  //直接操作内存
    size_t _Dsize =ws.size()*sizeof(wchar_t);  
    memset(_Dest,0,_Dsize);  
    wcstombs(_Dest,_Source,_Dsize);  
    setlocale(LC_ALL,oldLocale);  
    return result;
}

std::wstring StringToWString(const std::string& s)
{
    //setlocale(LC_ALL, "chs");   //	std::string strLocale = setlocale(LC_ALL, ""); 
    //const char* _Source = s.c_str();  
    //size_t _Dsize = s.size() + 1;  
    //wchar_t *_Dest = new wchar_t[_Dsize];  
    //wmemset(_Dest, 0, _Dsize);  
    //mbstowcs(_Dest,_Source,_Dsize);  
    ////若原string有用来存放二进制数据则原写法可能错误,  refactor by lizulin old std::wstring result = _Dest;    
    //std::wstring result(_Dest,s.size());  
    //delete []_Dest;  
    //setlocale(LC_ALL, "C");   	//setlocale(LC_ALL, strLocale.c_str());
    //return result;

    //fast way: refactor by lizulin
    char* oldLocale = setlocale(LC_ALL,NULL);
    setlocale(LC_ALL,"chs");   //	std::string strLocale = setlocale(LC_ALL, ""); 
    const char* _Source = s.c_str();  
    std::wstring result(s.size(),L' ');  
    wchar_t *_Dest = const_cast<wchar_t*>(result.data());  //直接操作内存
    size_t _Dsize = s.size() ;  
    wmemset(_Dest,0, _Dsize);  
    mbstowcs(_Dest,_Source,_Dsize);  
    setlocale(LC_ALL,oldLocale);   	//setlocale(LC_ALL, strLocale.c_str());
    return result;
}
#endif







#if _MSC_VER>=1700   //VS2012支持 //gcc4.8.5 一些版本 暂不支持 #include <codecvt>


#include <codecvt> 
std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

std::string gb2312_to_utf8(std::string const &strGb2312)
{
    std::vector<wchar_t> buff(strGb2312.size());
#ifdef _MSC_VER
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    wchar_t* pwszNext = nullptr;
    const char* pszNext = nullptr;
    // mbstate_t state = {};  //old
    mbstate_t state = {0};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
        (loc).in(state,
        strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
        buff.data(), buff.data() + buff.size(), pwszNext);

    if (std::codecvt_base::ok == res)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
    }

    return "";
}

std::string utf8_to_gb2312(std::string const &strUtf8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
    std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef _MSC_VER
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    const wchar_t* pwszNext = nullptr;
    char* pszNext = nullptr;
    // mbstate_t state = {};  //old
    mbstate_t state = {0};

    std::vector<char> buff(wTemp.size() * 2);
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
        (loc).out(state,
        wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
        buff.data(), buff.data() + buff.size(), pszNext);

    if (std::codecvt_base::ok == res)
    {
        return std::string(buff.data(), pszNext);
    }
    return "";
}


#endif




//各种格式字符串编码转换,待续
#if 0
std::u16string to_utf16( std::string str ) // utf-8 to utf16
{ return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(str); }

std::string to_utf8( std::u16string str16 )
{ return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(str16); }

std::u32string to_utf32( std::string str )
{ return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(str); }

std::string to_utf8( std::u32string str32 )
{ return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(str32); }

std::wstring to_wchar_t( std::string str )
{ return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str); }

std::string to_utf8( std::wstring wstr )
{ return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr); }
 
#endif