#include "Allocator.h"

#include "MemoryPool.h"

//MemoryPool mempool;

void* myAllocator_malloc(size_t nSize)
{
    return  GetMemPool().Malloc(nSize);
}
void myAllocator_free(void* p)
{
    return GetMemPool().Free(p);
}

