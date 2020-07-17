#pragma once

#ifndef READWRITELOCKER_H
#define READWRITELOCKER_H
//读写锁的几种实现,成熟后移动到thread.h


#include "ThreadSync.h"

#ifdef  _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//////////////////////////////////////////////////////////////////////////
// 多证书链读写锁类
#ifdef  _WIN32

typedef struct _RWLock
{
	HANDLE	hMutex;    
	HANDLE	hDataLock;
	int		nReaderCount;
} RWLock;

#endif

class  CRWLock
{
public:
	CRWLock();
	~CRWLock();

	BOOL  AcquireReadLock(void);    //请求读操作锁,请求不成功则阻塞
	BOOL  ReleaseReadLock(void);    //释放读操作锁

	BOOL  AcquireWriteLock(void);   //请求写操作锁,请求不成功则阻塞
	BOOL  ReleaseWriteLock(void);   //释放写操作锁

private:
//	BOOL  CRWLock::MyWaitForSingleObject(HANDLE hObject);
//	BOOL  CRWLock::InitRWLock();
//	BOOL  CRWLock::DestroyRWLock();
    BOOL  MyWaitForSingleObject(HANDLE hObject);
    BOOL  InitRWLock();
    BOOL  DestroyRWLock();
private:

#ifdef  WIN32
	RWLock	m_RWLock;   //读写锁
#else
	pthread_rwlock_t m_RWLock;
#endif

};

















class MRWMutex
{
public:
	MRWMutex();
	~MRWMutex();

	void LockRead();
	void UnLockRead();
	void LockWrite();
	void UnLockWrite();

	////// Accessor functions
	int GetWaitingWriters();
	int GetWaitingReaders();
	int GetLockCount();

#ifdef WIN32
private:
	int WaitingWriters();
	int IncrementWriters();
	int DecrementWriters();

	int WaitingReaders();
	int IncrementReaders();
	int DecrementReaders();

private:
	int m_nLockCount;

	int m_nWaitingWriters;
	int m_nWaitingReaders;

	//MSemaphore m_semReader;
	//MSemaphore m_semWriter;
	//MMutex m_mtxLockCount;
	////HANDLE m_hReaderEvent;
	////HANDLE m_hWriterEvent;
	////HANDLE m_hLockCountMutex;	
	MySemaphore m_semReader;
	MySemaphore m_semWriter;
	MyMutex m_mtxLockCount;


private:
	MRWMutex(const MRWMutex&);
	const MRWMutex& operator=(const MRWMutex&);

#else

private:
	pthread_rwlock_t fRWMutex;
#endif
};


#endif

