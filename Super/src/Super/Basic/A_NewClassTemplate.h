#ifndef _A_NewClassTemplate_Header_
#define _A_NewClassTemplate_Header_

#include "global.h" 

//CN ������������ģ��
//Template for adding new classes

NAME_SPACE_MAIN_BEGIN

class A_NewClassTemplate
{
public:
    A_NewClassTemplate(void);
    ~A_NewClassTemplate(void);
private:
};

NAME_SPACE_MAIN_END


#ifdef UNIT_TEST
BASIC_API int Test_A_NewClassTemplate();
#endif

#endif


