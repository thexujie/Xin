#include "PCH.h"
#include "Signal.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	FSignal::FSignal()
	{
		Handle = CreateEventExW(NULL, NULL, 0, SYNCHRONIZE | EVENT_MODIFY_STATE);

	}
	FSignal::~FSignal()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = NULL;
		}
	}

	void FSignal::Signal() const
	{
		SetEvent(Handle);
	}

	void FSignal::Wait() const
	{
		WaitForSingleObject(Handle, INFINITE);
	}
}