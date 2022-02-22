#include "PCH.h"
#include "Mutex.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	FMutex::FMutex()
	{
		Handle = new CRITICAL_SECTION();
		InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)Handle, 200);
	}

	FMutex::~FMutex()
	{
		if (Handle)
			delete (CRITICAL_SECTION *)Handle;
	}

	void FMutex::Lock() const
	{
		EnterCriticalSection((CRITICAL_SECTION *)Handle);

#ifdef XIN_DEBUG
		Atomics::FetchExchange(ThreadIdLocked, FSystem::ThreadId());
#endif
	}

	bool FMutex::TryLock() const
	{
		if (TryEnterCriticalSection((CRITICAL_SECTION *)Handle))
		{
#ifdef XIN_DEBUG
			Atomics::FetchExchange(ThreadIdLocked, FSystem::ThreadId());
#endif
			return true;
		}
		else
			return false;
	}

	void FMutex::Unlock() const
	{
		LeaveCriticalSection((CRITICAL_SECTION *)Handle);
#ifdef XIN_DEBUG
		Atomics::FetchExchange(ThreadIdLocked, 0u);
#endif
	}

	FSharedMutex::FSharedMutex()
	{
		static_assert(sizeof(RTL_SRWLOCK) == sizeof(Handle));
		//InitializeSRWLock(InterpretCast<RTL_SRWLOCK>(&Handle));
		InterpretCastRef<RTL_SRWLOCK>(&Handle).Ptr = SRWLOCK_INIT;
	}

	FSharedMutex::~FSharedMutex()
	{
	}

	void FSharedMutex::LockShared() const
	{
		AcquireSRWLockShared((RTL_SRWLOCK *)(&Handle));
	}

	bool FSharedMutex::TryLockShared() const
	{
		return TryAcquireSRWLockShared((RTL_SRWLOCK *)(&Handle));
	}

	void FSharedMutex::Lock() const
	{
		AcquireSRWLockExclusive((RTL_SRWLOCK *)(&Handle));
	}

	bool FSharedMutex::TryLock() const
	{
		return !!TryAcquireSRWLockExclusive((RTL_SRWLOCK *)(&Handle));
	}

	void FSharedMutex::Unlock() const
	{
		ReleaseSRWLockShared((RTL_SRWLOCK *)(&Handle));
	}
}