#include "PCH.h"
#include "Thread.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	FThread::FThread(TFunction<void()> Function)
	{
		struct FThreadContext
		{
			TFunction<void()> Function;
		};
		
		auto FuncEntry = [](void * Context)->uint32
		{
			FThreadContext * ThreadContext = (FThreadContext *)Context;
			ThreadContext->Function();
			delete ThreadContext;
			//_endthreadex(0);
			return 0;
		};

		FThreadContext * ThreadContext = new FThreadContext();
		ThreadContext->Function = Function;
		Handle = (voidp)_beginthreadex(nullptr, 0, FuncEntry, ThreadContext, 0, &ThreadId);
	}
	
	void FThread::Join()
	{
		if (!ThreadId)
			return;

		[[maybe_unused]] DWORD Wait = ::WaitForSingleObject(Handle, INFINITE);
		ThreadId = 0;
	}

	FThread & FThread::operator =(FThread && That) noexcept
	{
		ThreadId = That.ThreadId;
		Handle = That.Handle;
		That.ThreadId = 0;
		That.Handle = nullptr;
		return *this;
	}

	void FThread::Sleep(uint32 TimeInMS)
	{
		::Sleep(TimeInMS);
	}

	void FThread::YieldCurrent()
	{
		::Sleep(0);
	}

	uint32 FThread::CurrentThreadId()
	{
		return ::GetCurrentThreadId();
	}
}