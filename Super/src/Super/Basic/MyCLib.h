#ifndef MYCLIB_H
#define MYCLIB_H

//#include<stdio.h>


void* MyMemcpy(void *pDst,const void *pSrc,size_t len);
void* MyMemset(void *s,char c,size_t num);
size_t MyStrlent(const char * str);

#ifdef UNIT_TEST
int Test_MyClib();
#endif

#endif



