#include "tools.h"


//#include <memory.h>
//#include <stdio.h>
#include <stdlib.h>

// 实现任意字节对齐的内存分配和释放
void *AllignedMalloc(size_t size, int aligned)
{
    // aligned is a power of 2
    //assert((aligned&(aligned - 1)) == 0);

    // 分配内存空间
    void *data = ::malloc(sizeof(void *)+aligned + size);

    // 地址对齐
    void **temp = (void **)data + 1;
    void **alignedData = (void **)(((size_t)temp + aligned - 1)&-aligned);


    // 保存原始内存地址
    alignedData[-1] = data;

    return alignedData;  // 被转换为一级指针
}



void AlignedFree(void *data)
{
    if (data)
    {
        ::free(((void **)data)[-1]);
    }
}





//from QT5.10 qmalloc.cpp
#if 0

void *qMalloc(size_t size)
{
    return ::malloc(size);
}

void qFree(void *ptr)
{
    ::free(ptr);
}

void *qRealloc(void *ptr, size_t size)
{
    return ::realloc(ptr, size);
}

void *qMallocAligned(size_t size, size_t alignment)
{
    return qReallocAligned(0, size, 0, alignment);
}

void *qReallocAligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment)
{
    // fake an aligned allocation
    void *actualptr = oldptr ? static_cast<void **>(oldptr)[-1] : 0;
    if (alignment <= sizeof(void*)) {
        // special, fast case
        void **newptr = static_cast<void **>(realloc(actualptr, newsize + sizeof(void*)));
        if (!newptr)
            return 0;
        if (newptr == actualptr) {
            // realloc succeeded without reallocating
            return oldptr;
        }

        *newptr = newptr;
        return newptr + 1;
    }

    // malloc returns pointers aligned at least at sizeof(size_t) boundaries
    // but usually more (8- or 16-byte boundaries).
    // So we overallocate by alignment-sizeof(size_t) bytes, so we're guaranteed to find a
    // somewhere within the first alignment-sizeof(size_t) that is properly aligned.

    // However, we need to store the actual pointer, so we need to allocate actually size +
    // alignment anyway.

    void *real = realloc(actualptr, newsize + alignment);
    if (!real)
        return 0;

    quintptr faked = reinterpret_cast<quintptr>(real) + alignment;
    faked &= ~(alignment - 1);
    void **faked_ptr = reinterpret_cast<void **>(faked);

    if (oldptr) {
        qptrdiff oldoffset = static_cast<char *>(oldptr) - static_cast<char *>(actualptr);
        qptrdiff newoffset = reinterpret_cast<char *>(faked_ptr) - static_cast<char *>(real);
        if (oldoffset != newoffset)
            memmove(faked_ptr, static_cast<char *>(real) + oldoffset, qMin(oldsize, newsize));
    }

    // now save the value of the real pointer at faked-sizeof(void*)
    // by construction, alignment > sizeof(void*) and is a power of 2, so
    // faked-sizeof(void*) is properly aligned for a pointer
    faked_ptr[-1] = real;

    return faked_ptr;
}

void qFreeAligned(void *ptr)
{
    if (!ptr)
        return;
    void **ptr2 = static_cast<void **>(ptr);
    free(ptr2[-1]);
}
#endif

