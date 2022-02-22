#pragma once

#include "../Xin.Core.Types.h"

namespace Xin
{
	class CORE_API FMutex
	{
	public:
		FMutex();
		~FMutex();
		FMutex(const FMutex &) = delete;
		FMutex(FMutex && That) noexcept :Handle(That.Handle)
		{
			That.Handle = nullptr;
		}

		void Lock() const;
		bool TryLock() const;
		void Unlock() const;
		voidp GetHandle() const { return Handle; }

	private:
		voidp Handle = nullptr;
#ifdef XIN_DEBUG
		mutable uint32 ThreadIdLocked = 0;
#endif
	};

	class CORE_API FSharedMutex
	{
	public:
		FSharedMutex();
		~FSharedMutex();
		FSharedMutex(const FSharedMutex &) = delete;
		FSharedMutex(FSharedMutex && That) noexcept :Handle(That.Handle)
		{
			That.Handle = nullptr;
		}

		void LockShared() const;
		bool TryLockShared() const;

		void Lock() const;
		bool TryLock() const;
		void Unlock() const;
		voidp GetHandle() const { return Handle; }

	private:
		voidp Handle = nullptr;
	};
}