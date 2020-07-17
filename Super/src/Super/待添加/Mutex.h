#ifndef __LYNX_PLATFORM_LIB_MUTEX_H__
#define __LYNX_PLATFORM_LIB_MUTEX_H__


#include "Condition.h"

//#include "Lock.h"  //ÄÚÈÝÈçÏÂ
namespace Lynx
{
	template <typename MutexTraits, typename LockTraits>
	class BaseLock
	{
	public:    
		BaseLock(MutexTraits& mtx): mMutex(mtx)
		{
			LockTraits::lock(mMutex);
		}

		~BaseLock()
		{
			LockTraits::unlock(mMutex);
		}

		inline void lock()
		{
			LockTraits::lock(mMutex);
		}

		inline void unlock()
		{
			LockTraits::unlock(mMutex);
		}
	protected:
		MutexTraits& mMutex;
	};
} // namespace Lynx




namespace Lynx 
{
    class Mutex
    {
    protected:
#ifndef _WIN32
        mutable  pthread_mutex_t mId;
#else
        void* mId; //CRITICAL_SECTION
#endif
    private:
        //  no value sematics, therefore private and not implemented.
        Mutex (const Mutex&);
        Mutex& operator= (const Mutex&);
    public:
        Mutex();
        virtual ~Mutex();

        void lock();
        void unlock();
#ifndef _WIN32
        pthread_mutex_t* getId()
        {
            return &mId;
        }
#endif
    };

    class  SpinMutex
    {
    public:
        SpinMutex();
        virtual ~SpinMutex();

        void lock();
        void unlock();

    private:
#ifdef _WIN32
        void* mId; // CRITICAL_SECTION
#else
        mutable pthread_mutex_t mId;
#endif
    };
   
    class  ReadWriteMutex
    {
    public:
        ReadWriteMutex();
        virtual ~ReadWriteMutex();
         
        void unlock();
        void readLock();
        void writeLock();
#if 0
        unsigned int readerCount() const;
        unsigned int pendingWriterCount() const;
#endif
    private:
#ifdef _WIN32
        Mutex mMutex;
        Condition mReadCondition;
        Condition mPendingWriteCondition;
        unsigned int mReaderCount;
        bool mWriterHasLock;
        unsigned int mPendingWriterCount;
#else
        pthread_rwlock_t mId;
#endif
    };

	class  LockTraits
    {
    public:
        inline static void lock(Mutex& mtx)
        {
            mtx.lock();
        }
    
        inline static void unlock(Mutex& mtx)
        {
            mtx.unlock();
        }
    };

    class  SpinLockTraits
    {
    public:
        inline static void lock(SpinMutex& mtx)
        {
            mtx.lock();
        }

        inline static void unlock(SpinMutex& mtx)
        {
            mtx.unlock();
        }
    };

    class  ReadLockTraits
    {
    public:
        inline static void lock(ReadWriteMutex& mtx)
        {
            mtx.readLock();
        }
    
        inline static void unlock(ReadWriteMutex& mtx)
        {
            mtx.unlock();
        }
    };
   
    class  WriteLockTraits
    {
    public:
        inline static void lock(ReadWriteMutex& mtx)
        {
            mtx.writeLock();
        }
    
        inline static void unlock(ReadWriteMutex& mtx)
        {
            mtx.unlock();
        }
    };

    typedef BaseLock<Mutex, LockTraits> lock;
    typedef BaseLock<SpinMutex, SpinLockTraits> SpinLock;
    typedef BaseLock<ReadWriteMutex, ReadLockTraits> readLock;
    typedef BaseLock<ReadWriteMutex, WriteLockTraits> writeLock;
   
} // namespace Lynx 

#endif // __LYNX_PLATFORM_LIB_MUTEX_H__


