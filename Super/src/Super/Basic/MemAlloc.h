#ifndef MemAlloc_H_
#define MemAlloc_H_



class MyAlloc
{

};









//#include <new>
#include <stdio.h>
#if 0

class MemTracePrivate;
class MemTrace
{
public:
    MemTrace();
    ~MemTrace();
    static MemTrace& getInatance();
    void AddTrack(void* addr, size_t size, const char *filename, int line);
    void RemoveTrack(void* addr);
    void PrintAllMemInfo();
private:
    MemTracePrivate* pImpl;
};

void * operator new(size_t size,const char* file,const size_t line);
void * operator new[](size_t size,const char* file,const size_t line);

void operator delete (void* pointer,const char* file,const size_t line);
void operator delete[](void* pointer,const char* file,const size_t line);

//#define MC_NEW new(__FILE__, __LINE__)
//#define new MC_NEW
////#define new new(__FILE__, __LINE__)
//#define malloc(s) ((void*)new unsigned char[s])
//#define free(p)   (delete [] (char*)(p));


#endif



#endif

