#include "PCH.h"
#include "Procedurer.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	static void CALLBACK InvokerAPCCallBack(ULONG_PTR dwParam)
	{
		FProcedurer * Procedurer = (FProcedurer *)dwParam;
		if (Procedurer)
			Procedurer->Dispatch();
	}

	FProcedurer::FProcedurer()
	{
		ThreadHandle = GetCurrentThread();
	}

	FProcedurer::~FProcedurer()
	{
		if (Dispatching)
		{
			Canceled = true;
		}
	}

	void FProcedurer::Invoke(TFunction<void()> Function)
	{
		Function();
	}

	void FProcedurer::BeginInvoke(TFunction<void()> Function)
	{
		Callbacks.Add(new FProcedurerCallback(Function));
		if (!Dispatching)
		{
			Dispatching = true;
			::QueueUserAPC(InvokerAPCCallBack, (HANDLE)ThreadHandle, (ULONG_PTR)this);
		}
	}

	void FProcedurer::Dispatch()
	{
		Dispatching = false;
		if (Canceled)
		{
			Callbacks.Clear();
			return;
		}

		TList<FProcedurerCallbackRef> TempCallbacks = Move(Callbacks);
		for (FProcedurerCallbackRef & Callback : TempCallbacks)
			Callback->Function();
	}

	FProcedurer & FProcedurer::Current()
	{
		thread_local FProcedurer CurrentProcedurer;
		return CurrentProcedurer;
	}

	struct FMainThreadDispatherInitializer
	{
		FMainThreadDispatherInitializer()
		{
			FProcedurer::MainThread();
		}
	};
	static FMainThreadDispatherInitializer MainThreadDispatherInitializer;

	FProcedurer & FProcedurer::MainThread()
	{
		static FProcedurer & MainThreadProcedurer = FProcedurer::Current();
		return MainThreadProcedurer;
	}
}
