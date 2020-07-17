 #ifndef TEMP_H
#define TEMP_H

//���ڵ����еĺ�������

#include "global.h"


class BaseRef
{
    private:
        int RefCount;
    protected:
        virtual ~ BaseRef ()
        {
            //if (!--RefCount)  
             //   delete this; 
        };
    public:
         BaseRef ()
        {
            RefCount = 1;
        }
        void IncRef ()
        {
            RefCount++;  
        }
        void DecRef ()
        {
            if (!--RefCount)  
                delete this;  
        }
        int GetRefCount ()
        {
            return RefCount;  
        }
};
 
 



//int Test_Temp()
//{
//	return 0;
//}


#endif
