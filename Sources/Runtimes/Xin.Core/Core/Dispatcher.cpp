#include "PCH.h"
#include "Dispatcher.h"

#include "Debug.h"
#include "DispatcherContext.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	//static void CALLBACK InvokerAPCCallBack(ULONG_PTR dwParam)
	//{
	//	FDispatcher * Dispatcher = (FDispatcher *)dwParam;
	//	if (Dispatcher)
	//		Dispatcher->Dispatch();
	//}

	FDispatcher::FDispatcher()
	{
		ThreadHandle = GetCurrentThread();
		ThreadId = GetCurrentThreadId();
		AssertExpr(ThreadId != 0);
		//FConsole::WriteLine(u8"Initialize Dispatcher on thread {}. this={}"V, ThreadId, voidp(this));
	}

	FDispatcher::~FDispatcher()
	{
		//FConsole::WriteLine(u8"Finalize Dispatcher on thread {}. this={}"V, ThreadId, voidp(this));

		if (Dispatching)
		{
			Canceled = true;
		}

		ThreadId = NullIndex;
		ThreadHandle = nullptr;
	}

	void FDispatcher::Invoke(TFunction<void()> Function)
	{
		Function();
	}

	void FDispatcher::BeginInvoke(TFunction<void()> Function)
	{
		Callbacks.Add(new FDispatcherCallback(Function));
		if (!Dispatching)
		{
			Dispatching = true;
			IDispatcherContext::Instance().DispatchInvoke();
			//::QueueUserAPC(InvokerAPCCallBack, (HANDLE)ThreadHandle, (ULONG_PTR)this);
		}
	}

	void FDispatcher::Dispatch()
	{
		Dispatching = false;
		if (Canceled)
		{
			Callbacks.Clear();
			return;
		}

		TList<FDispatchInstanceRef> TempCallbacks = Move(Callbacks);
		for (FDispatchInstanceRef & Callback : TempCallbacks)
			Callback->Function();
	}

	FDispatcher & FDispatcher::Current()
	{
		thread_local FDispatcher CurrentDispatcher;
		return CurrentDispatcher;
	}

	struct FMainThreadDispatherInitializer
	{
		FMainThreadDispatherInitializer()
		{
			FDispatcher::MainThread();
		}
	};
	static FMainThreadDispatherInitializer MainThreadDispatherInitializer;

	FDispatcher & FDispatcher::MainThread()
	{
		static FDispatcher & MainThreadDispatcher = FDispatcher::Current();
		return MainThreadDispatcher;
	}
}
