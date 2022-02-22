#include "PCH.h"
#include "ConditionalVariable.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	FConditionalVariable::FConditionalVariable()
	{
		InitializeConditionVariable((CONDITION_VARIABLE *)&Handle);
	}
	
	FConditionalVariable::~FConditionalVariable()
	{
		
	}

	void FConditionalVariable::NotifyOne()
	{
		WakeConditionVariable((CONDITION_VARIABLE *)&Handle);
	}
	
	void FConditionalVariable::NotifyAll()
	{
		WakeAllConditionVariable((CONDITION_VARIABLE *)&Handle);
	}

	void FConditionalVariable::Wait(TUniqueLock<FMutex> & Lock)
	{
		SleepConditionVariableCS((CONDITION_VARIABLE *)&Handle, (CRITICAL_SECTION *)Lock.Mutex.GetHandle(), INFINITE);
	}

	void FConditionalVariable::Wait(TUniqueLock<FMutex> & Lock, TFunction<bool()> Condition)
	{
		while(!Condition())
		{
			SleepConditionVariableCS((CONDITION_VARIABLE *)&Handle, (CRITICAL_SECTION *)Lock.Mutex.GetHandle(), INFINITE);
		}
	}
}