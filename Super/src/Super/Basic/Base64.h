#ifndef _Base64_Header_
#define _Base64_Header_

#include "global.h" 
#include <string>
#include <vector>

NAME_SPACE_MAIN_BEGIN

enum Base64Option
{
    Encode_STD=0,               //��׼Base64������ĸ��
    Encode_URL=1,              //URL��,��+���͡�/���ֱ�ĳ��ˡ�-���͡�_������url���Ѻ� Base64UrlEncoding
    KeepTrailingEquals=0,    //(Ĭ��)���ֱ�������ĩβ�����Ⱥţ����ݵĴ�С����4�ı�����
    OmitTrailingEquals=2,     //ʡ�Ա�������ĩβ�ĵȺ�
    //add new 
    Encode_AutoWrap=4,    //���봫�� ���Ʊ���ʱÿ76���ַ��Զ�׷�ӻ��з���win="\r\n",����ƽ̨Ϊ"\n"
};

class Base64Encrypt
{
public:
    Base64Encrypt(int option=Encode_STD|KeepTrailingEquals|Encode_AutoWrap)
        :m_option(option)
    {
        initMember();
    }
    Base64Encrypt(const void *input, size_t length,int option=Encode_STD|KeepTrailingEquals|Encode_AutoWrap)
        :m_option(option)
        ,groupLen(0)
    {
        initMember();
        Update(input,length);
    }
    void initMember();
    void Update(const void *input,size_t length);
    void Final();   //Final �����ֶ�����
    const unsigned char *CipherText()
    {
        if (!bdoFinal)
        {
             Final();
             bdoFinal=true;
        }
        return Buf.data();
    }
    std::string GetString()
    {
        //const char *pstr = (const char *)CipherText();
        //size_t length = GetSize();
        //return std::string(pstr,length);
        if (!bdoFinal)
        {
            Final();
            bdoFinal=true;
        }
        return std::string((const char *)Buf.data(),Buf.size());
    }
    void Reset()
    {
        Buf.clear();
        groupLen=0;
        for(unsigned int k = 0; k < sizeof(group)/sizeof(group[0]);++k)
        {
            group[k] = 0;
        }
    }
    size_t GetSize()
    {
        //CipherText();
        if (!bdoFinal)
        {
            Final();
            bdoFinal=true;
        }
        return Buf.size();
    }
private:
    void Encode();
private:
    int m_option;
    bool bdoFinal;   //�Ƿ��Ѿ�����������
    std::vector<unsigned char> Buf;
    unsigned char group[3];
    unsigned int groupLen;
    const unsigned char* Base64EncodeMap;
private:
    Base64Encrypt(const Base64Encrypt &){};
    Base64Encrypt& operator=(const Base64Encrypt&){};
};


class Base64Decrypt
{
public:
    Base64Decrypt(int option=Encode_STD|KeepTrailingEquals)
        :m_option(option)
    {
        initMember();
    }
    Base64Decrypt(const void *input, size_t length,int option=Encode_STD|KeepTrailingEquals)
        :m_option(option)
    {
        initMember();
        Update(input,length);
    }
    void initMember();
    void Update(const void *input, size_t length);
    void Final()   //Final �����ֶ�����
    {
        if (groupLen)
        {
            throw std::invalid_argument("ciphertext's length must be a multiple of 4");
        }
    }
    bool CheckFinal()
    {
        return groupLen==0;
    }
    std::vector<unsigned char> Data()
    {
        return Buf;
    }
    const std::vector<unsigned char>& const_Data() const
    {
        return Buf;
    }
    const unsigned char *PlainText()
    {
        Final();
        return Buf.data();
    }
    void Reset()
    {
        Buf.clear();
        groupLen = 0;
        for (unsigned int k = 0; k < sizeof(group)/sizeof(group[0]);++k)
        {
            group[k] = 0;
        }
    }
    size_t GetSize()
    {
        //PlainText();
        return Buf.size();
    }
private:
    void Decode();
private:
    int m_option;
    std::vector<unsigned char> Buf;
    unsigned char group[4];
    unsigned int groupLen;
    const unsigned char* Base64DecodeMap;
private:
    Base64Decrypt(const Base64Decrypt &){};
    Base64Decrypt& operator=(const Base64Decrypt &){};
};




//new by lizulin 
std::string Base64Encode(const char* buf,size_t len,int option=Encode_STD|KeepTrailingEquals|Encode_AutoWrap);
bool Base64Decode(const std::string& sSrc,char* out,size_t& outLen);




std::string encode_base64(const  std::string sourceData, int options = Encode_STD);
std::string decode_base64(const  std::string sourceData, int options = Encode_STD);

#ifdef UNIT_TEST
void Test_Base64();
#endif

NAME_SPACE_MAIN_END

#endif

