#ifndef MsgQueueManager_H
#define MsgQueueManager_H

#include "struct_define.h"


class ShareMemMgr
{
public:	
	ShareMemMgr();
	~ShareMemMgr();
	 ShareMemOfDCAP_EAH* GetAddress(){return pSM;}

     //手动调用更新共享内存某个成员变量
     void updateWrire(void* addr,size_t len);   //addr必须在ShareMemOfDCAP_EAH范围内
     void updateRead(void* addr,size_t len);   //addr必须在ShareMemOfDCAP_EAH范围内

private:
     char* mem_start;
     int mem_fd;
     ShareMemOfDCAP_EAH* pSM;
};

ShareMemMgr* GetShareMemMgr();

#endif



