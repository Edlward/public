 #ifndef ByteArray_H
#define ByteArray_H

#include "global.h" 

NAME_SPACE_MAIN_BEGIN

//������ʽ����(дʱ����) ʵ������QByteArray���ࡣ
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
//resize()�������¶����С
//
//    data()���洢�����ݶ��󣬷���һ��ָ��
//
//    at()��[]Ч��һ��������ȥ��i+1���ֽ�
//
//    Left(int len) right(int len) mid(int  pos, int len)���ǴӸ��ַ�������ȡ���Һʹ�pos��ʼ��len���ֽڵĳ�Ա����������QByteArray����
//
//    Prepend() append() �ֱ��Ǵ�ǰ��ͺ�������ַ���



/*
4��һ�ֳ�ʼ��QByteArray��ʽ�Ǹ����Ĺ��캯������һ��const char* ���ɡ���ʱ��QByteArrayִ���������deep copy����ֵ���ͣ����������Ч�ʿ��ǲ���ִ�������ʹ��QByteArray::fromRawData(const char * data, int siz)/* ���ص� QByteArray ���󽫻������ָ������� 
*/
