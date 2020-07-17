 #ifndef ByteArray_H
#define ByteArray_H

#include "global.h" 

NAME_SPACE_MAIN_BEGIN

//基于隐式共享(写时复制) 实现类似QByteArray的类。
class ByteArray
{
public:
	ByteArray(){};
	virtual ~ByteArray(){};
protected:

};


NAME_SPACE_MAIN_END

#endif
//
//resize()可以重新定义大小
//
//    data()所存储的数据对象，返回一个指针
//
//    at()与[]效果一样，都是去第i+1的字节
//
//    Left(int len) right(int len) mid(int  pos, int len)都是从该字符串中提取左、右和从pos开始的len个字节的成员函数，返回QByteArray类型
//
//    Prepend() append() 分别是从前面和后面添加字符串



/*
4）一种初始化QByteArray方式是给它的构造函数传入一个const char* 即可。此时，QByteArray执行了深拷贝（deep copy、如值类型），如果出于效率考虑不想执行深拷贝，使用QByteArray::fromRawData(const char * data, int siz)/* 返回的 QByteArray 对象将会和数据指针相关联 
*/
