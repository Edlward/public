#include "ReadWriteLocker.h"


#ifndef FALSE 
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

//Íê³É¶ÁÐ´Ëø³õÊ¼»¯
CRWLock::CRWLock()
{

	InitRWLock();

}

//
CRWLock::~CRWLock()
{
	DestroyRWLock();
}

BOOL  CRWLock::MyWaitForSingleObject(HANDLE hObject)
{
#ifdef WIN32

	DWORD result;

	result = WaitForSingleObject(hObject, INFINITE);

	return (result == WAIT_OBJECT_0);

#endif
}

BOOL  CRWLock::InitRWLock()
{
#ifdef WIN32

	m_RWLock.nReaderCount = 0;
	m_RWLock.hDataLock = CreateSemaphore(NULL, 1, 1, NULL);
	if (m_RWLock.hDataLock == NULL)
	{
		return FALSE;
	}

	m_RWLock.hMutex = CreateMutex(NULL, FALSE, NULL);
	if (m_RWLock.hMutex == NULL)
	{
		CloseHandle(m_RWLock.hDataLock);
		return FALSE;
	}

	return TRUE;

#else

	pthread_rwlock_init(&m_RWLock, NULL);

	return TRUE;
#endif

}

BOOL  CRWLock::DestroyRWLock()
{
#ifdef WIN32

	DWORD result = WaitForSingleObject(m_RWLock.hDataLock, INFINITE);
	if (result != WAIT_OBJECT_0)
	{
		return FALSE;
	}

	CloseHandle(m_RWLock.hMutex);
	CloseHandle(m_RWLock.hDataLock);
	return TRUE;

#else

	pthread_rwlock_destroy(&m_RWLock);

	return TRUE;

#endif
}

BOOL CRWLock::AcquireReadLock()
{
	BOOL result = TRUE;

#ifdef WIN32

	if (MyWaitForSingleObject(m_RWLock.hMutex) == FALSE)
	{
		return FALSE;
	}

	m_RWLock.nReaderCount ++;
	if(m_RWLock.nReaderCount == 1)
	{
		result = MyWaitForSingleObject(m_RWLock.hDataLock);
	}

	ReleaseMutex(m_RWLock.hMutex);

	return result;

#else
	pthread_rwlock_rdlock(&m_RWLock);

	return result;
#endif
}


BOOL CRWLock::ReleaseReadLock()
{
	int result = TRUE;

#ifdef WIN32

	LONG lPrevCount;

	if (MyWaitForSingleObject(m_RWLock.hMutex) == FALSE)
	{
		return FALSE;
	}

	--m_RWLock.nReaderCount;

	if (m_RWLock.nReaderCount == 0)
	{
		result = ReleaseSemaphore(m_RWLock.hDataLock, 1, &lPrevCount);
	}

	ReleaseMutex(m_RWLock.hMutex);

	return result;
#else

	return pthread_rwlock_unlock(&m_RWLock);	

#endif
}

BOOL CRWLock::AcquireWriteLock()
{

#ifdef WIN32

	return MyWaitForSingleObject(m_RWLock.hDataLock);

#else

	return pthread_rwlock_wrlock(&m_RWLock);

#endif
}


BOOL CRWLock::ReleaseWriteLock()
{
#ifdef WIN32

	int result = TRUE;
	LONG lPrevCount;

	result = ReleaseSemaphore(m_RWLock.hDataLock, 1, &lPrevCount);
	if (lPrevCount != 0)
	{
		return FALSE;
	}

	return result;

#else
	return  pthread_rwlock_unlock(&m_RWLock);
#endif
}




















#ifdef _WIN32

// MRWMutex - constructs a read-write lock object and events
///////////////////////////////////////////////////////////////////////

MRWMutex::MRWMutex() : m_nLockCount(0), m_nWaitingReaders(0), m_nWaitingWriters(0)
{
}
MRWMutex::~MRWMutex()
{
	// Make sure mutex is unlocked when object is destroyed
	m_mtxLockCount.UnLock();
}

void MRWMutex::LockRead()
{
	WaitingReaders();
	if (m_nLockCount < 0)
	{
		// a reader thread must wait for the read event.. if a writer is holding lock
		//m_semReader.Wait();  //ÔÝÊ±ÆÁ±Î20181017
	}
	IncrementReaders();
}

void MRWMutex::UnLockRead()
{
	DecrementReaders();
	if (!m_nWaitingReaders && !m_nLockCount)
	{
		m_semWriter.Signal();
	}
}

void MRWMutex::LockWrite()
{
	WaitingWriters();
	while (m_nLockCount)
	{
		//m_semWriter.Wait(); //ÔÝÊ±ÆÁ±Î20181017
	}
	IncrementWriters();
}

void MRWMutex::UnLockWrite()
{
	DecrementWriters();
	// Readers are preferred in the following section....
	if (m_nWaitingWriters && !m_nWaitingReaders)
	{
		// when no readers are waiting... signal writers, if any
		m_semWriter.Signal();
	}
	else
	{
		while (m_nWaitingReaders > 0)
		{
			m_semReader.Signal();
		}
	}
}

int MRWMutex::WaitingReaders()
{
	m_mtxLockCount.Lock();
	// One more thread is waiting on read event
	m_nWaitingReaders++;
	m_mtxLockCount.UnLock();
	return true;
}

int MRWMutex::IncrementReaders()
{
	// InterlockedIncrement() could also be used, instead of this mutex
	m_mtxLockCount.Lock();
	// One more thread has obtained a read lock and is no longer waiting
	if (m_nLockCount >= 0)
	{
		m_nLockCount++;
		m_nWaitingReaders--;
	}
	m_mtxLockCount.UnLock();
	return true;
}

int MRWMutex::DecrementReaders()
{
	// InterlockedDecrement() could also be used, instead of this mutex
	m_mtxLockCount.Lock();
	if (m_nLockCount > 0)
	{
		// One less thread holding read lock
		m_nLockCount--;
	}
	m_mtxLockCount.UnLock();
	return true;
}

int MRWMutex::WaitingWriters()
{
	m_mtxLockCount.Lock();
	m_nWaitingWriters++;
	m_mtxLockCount.UnLock();
	return true;
}

int MRWMutex::IncrementWriters()
{
	m_mtxLockCount.Lock();
	// Only one thread can obtain a write lock .. so lock count goes underground
	// InterlockedDecrement() could also be used
	if (m_nLockCount == 0)
	{
		m_nLockCount = -1;
		m_nWaitingWriters--;
	}
	m_mtxLockCount.UnLock();
	return true;
}

int MRWMutex::DecrementWriters()
{
	m_mtxLockCount.Lock();
	if (m_nLockCount == -1)
	{
		m_nLockCount = 0;
	}
	m_mtxLockCount.UnLock();
	return true;
}


int MRWMutex::GetLockCount()
{
	int nCount = 0;
	m_mtxLockCount.Lock();
	nCount = m_nLockCount;
	m_mtxLockCount.UnLock();
	return nCount;

}



int MRWMutex::GetWaitingReaders()
{
	int nCount = 0;
    m_mtxLockCount.Lock();
	nCount = m_nWaitingReaders;
	m_mtxLockCount.UnLock();
	return nCount;
}

int MRWMutex::GetWaitingWriters()
{
	int nCount = 0;
	m_mtxLockCount.Lock();
	nCount = m_nWaitingWriters;
	m_mtxLockCount.UnLock();
	return nCount;
}





#else

static pthread_rwlockattr_t  *sRWMutexAttr=NULL;
static void RWMutexAttrInit();
static pthread_once_t sRWMutexAttrInit = PTHREAD_ONCE_INIT;

void RWMutexAttrInit()
{
	sRWMutexAttr = (pthread_rwlockattr_t*)malloc(sizeof(pthread_rwlockattr_t));
	memset(sRWMutexAttr, 0, sizeof(pthread_rwlockattr_t));
	pthread_rwlockattr_init(sRWMutexAttr);
}

MRWMutex::MRWMutex()
{
	(void)pthread_once(&sRWMutexAttrInit, RWMutexAttrInit);
	(void)pthread_rwlock_init(&fRWMutex, sRWMutexAttr);
}


MRWMutex::~MRWMutex() 
{
	pthread_rwlock_destroy(&fRWMutex);
}



void MRWMutex::LockRead()
{
	pthread_rwlock_rdlock(&fRWMutex);
}



void MRWMutex::LockWrite()
{
	pthread_rwlock_wrlock(&fRWMutex);
}



void MRWMutex::UnLockRead()
{
	pthread_rwlock_unlock(&fRWMutex);
}



void MRWMutex::UnLockWrite()
{
	pthread_rwlock_unlock(&fRWMutex);
}



int MRWMutex::GetWaitingWriters()
{
	return 0;
}

int MRWMutex::GetWaitingReaders()
{
	return 0;
}

int MRWMutex::GetLockCount()
{
	return 0;
}


#endif


