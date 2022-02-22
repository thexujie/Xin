#pragma once

#include "Lock.h"
#include "Mutex.h"

namespace Xin
{
	class CORE_API FConditionalVariable
	{
	public:
		FConditionalVariable();
		FConditionalVariable(const FConditionalVariable &) = delete;
		FConditionalVariable(FConditionalVariable && That) noexcept : Handle(That.Handle) { That.Handle = nullptr; }
		~FConditionalVariable();

		void NotifyOne();
		void NotifyAll();
		void Wait(TUniqueLock<FMutex> & Lock);
		void Wait(TUniqueLock<FMutex> & Lock, TFunction<bool()> Condition);

	private:
		voidp Handle = nullptr;
#ifdef XIN_DEBUG
		mutable uint32 ThreadIdLocked = 0;
#endif
	};
}
