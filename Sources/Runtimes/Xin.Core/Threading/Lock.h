#pragma once

#include "../Xin.Core.Types.h"

namespace Xin
{
	class TDefaultLock { public: TDefaultLock() = default; };


	class TTryToLock { public: TTryToLock() = default; };


	template<typename MutexT>
	class TUniqueLock
	{
	public:
		TUniqueLock(MutexT & MutexIn) : Mutex(MutexIn)
		{
			Mutex.Lock();
			OwnsLock = true;
		}

		TUniqueLock(MutexT & MutexIn, TTryToLock) : Mutex(MutexIn)
		{
			OwnsLock = Mutex.TryLock();
		}

		TUniqueLock(const TUniqueLock &) = delete;
		TUniqueLock(TUniqueLock && That) = delete;

		~TUniqueLock()
		{
			if (OwnsLock)
				Mutex.Unlock();
		}

	public:
		MutexT & Mutex;
		bool OwnsLock = false;
	};


	template<typename MutexT>
	class TSharedLock
	{
	public:
		TSharedLock(MutexT & Mutex) : Mutex(Mutex)
		{
			Mutex.LockShared();
			OwnsLock = true;
		}

		TSharedLock(MutexT & Mutex, TTryToLock) : Mutex(Mutex)
		{
			OwnsLock = Mutex.TryLockShared();
		}

		TSharedLock(const TSharedLock &) = delete;
		TSharedLock(TSharedLock &&) = delete;

		~TSharedLock()
		{
			if (OwnsLock)
				Mutex.Unlock();
		}

	public:
		MutexT & Mutex;
		bool OwnsLock = false;
	};
}
