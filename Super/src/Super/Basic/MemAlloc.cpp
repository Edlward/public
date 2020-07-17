#include "MemAlloc.h"
#include <new>
#include <map>




















#if 0
static MemTrace gMemTrace;

MemTrace& MemTrace::getInatance()
{
    return gMemTrace;
}



struct MemInfo
{
    MemInfo(size_t size=0,std::string sFileName="",int line=0):
        m_size(size),sFileName(sFileName),m_line(line)
    {
    }
    void print()
    {
        printf("size:%d file:%s line:%d\n",m_size,sFileName,m_line);
    }
    std::string sFileName;
    size_t m_size;
    int m_line;
};

class MemTracePrivate
{
public:
    MemTracePrivate()
    {

    }
    ~MemTracePrivate()
    {

    }
    std::map<void*,MemInfo> mapMem;
};
//typedef std::map<void*,MemInfo>::iterator iterator;  


MemTrace::MemTrace()
{
    pImpl=new MemTracePrivate;
}

MemTrace::~MemTrace()
{
    PrintAllMemInfo();
    delete pImpl;
}

void MemTrace::PrintAllMemInfo()
{
    if (pImpl->mapMem.size()>= 0)  
    {  
        //cout << _map.size() << " memory leaks detected" << endl;  
        typedef std::map<void*,MemInfo>::iterator iterator;
        unsigned int num=0;
        for (iterator itor =pImpl->mapMem.begin(); itor != pImpl->mapMem.end(); ++itor)  
        {  
            MemInfo& tmp=itor->second;
            printf("MemAddr[%u]: %p ",num,itor->first);
            tmp.print();

            //char const* file = itor->second.File();  
            //int line = itor->second.Line();  
            //cout << file << ", " << line << endl;  
        }  
    }  
}



void MemTrace::AddTrack(void* addr, size_t size, const char *filename, int line)
{
    pImpl->mapMem[addr]=MemInfo(size,filename,line);
}

void MemTrace::RemoveTrack(void* addr)
{
    typedef std::map<void*,MemInfo>::iterator iterator;
    iterator it=pImpl->mapMem.find(addr);
    if (it!=pImpl->mapMem.end())  
    {
       pImpl->mapMem.erase(it);  
    }
}




void * operator new(size_t size, const char *file, int line)
{
    //cout << "new size:" << size << endl;
    // cout << file << " " << line << endl;
    //void * p = malloc(size);
    void * p =::operator new(size);

    MemTrace::getInatance().AddTrack(p,size,file,line);
    return p;
}

void * operator new[](size_t size, const char *file, int line)
{
    //cout << "new size:" << size << endl;
    // cout << file << " " << line << endl;
    //void * p = malloc(size);
    void * p =::operator new[](size);

    MemTrace::getInatance().AddTrack(p,size,file,line);
    return p;
}

void operator delete(void* p)
{
    //cout << "delete " << (int)p << endl;

    MemTrace::getInatance().RemoveTrack(p);
    free(p);
}

void operator delete[](void* p)
{
    //cout << "delete [] " << (int)p << endl;

    MemTrace::getInatance().RemoveTrack(p);
    free(p);
}
#endif
