#pragma once
//#include "ST430_10.h"


//固定4个字节长度的BER编解码
void Ber4Encoder(char* pb, unsigned int value);
bool Ber4Decoder(const char* pb, unsigned int& value, char& derLen);

//固定5个字节长度的BER编解码
void Ber5Encoder(char* pb, unsigned int value);
bool Ber5Decoder(const char* pb, unsigned int& value, char& derLen);

