#pragma once
//#include "ST430_10.h"


//�̶�4���ֽڳ��ȵ�BER�����
void Ber4Encoder(char* pb, unsigned int value);
bool Ber4Decoder(const char* pb, unsigned int& value, char& derLen);

//�̶�5���ֽڳ��ȵ�BER�����
void Ber5Encoder(char* pb, unsigned int value);
bool Ber5Decoder(const char* pb, unsigned int& value, char& derLen);

