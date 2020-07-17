#ifndef MsgQueueManager_H
#define MsgQueueManager_H

#include "struct_define.h"


class ShareMemMgr
{
public:	
	ShareMemMgr();
	~ShareMemMgr();
	 ShareMemOfDCAP_EAH* GetAddress(){return pSM;}

     //�ֶ����ø��¹����ڴ�ĳ����Ա����
     void updateWrire(void* addr,size_t len);   //addr������ShareMemOfDCAP_EAH��Χ��
     void updateRead(void* addr,size_t len);   //addr������ShareMemOfDCAP_EAH��Χ��

private:
     char* mem_start;
     int mem_fd;
     ShareMemOfDCAP_EAH* pSM;
};

ShareMemMgr* GetShareMemMgr();

#endif



