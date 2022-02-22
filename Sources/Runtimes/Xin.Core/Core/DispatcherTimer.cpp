#include "PCH.h"
#include "DispatcherTimer.h"
#include "DispatcherContext.h"

namespace Xin
{
	FDispatcherTimer::FDispatcherTimer(uint32 Interal) : __Interval(Interal)
	{
		
	}

	FDispatcherTimer::~FDispatcherTimer()
	{
		if (StartTime > 0)
		{
			IDispatcherContext::Instance().StopTimer(this);
			StartTime = 0;
		}
	}

	uint32 FDispatcherTimer::Start()
	{
		//FConsole::WriteLineF(u8"FDispatcherTimer::Start {}"V, (void *)this);
		if (StartTime)
			Stop();
		return StartTime = IDispatcherContext::Instance().StartTimer(this);
	}

	uint32 FDispatcherTimer::Stop()
	{
		//FConsole::WriteLineF(u8"FDispatcherTimer::End {}"V, (void *)this);
		StartTime = 0;
		return IDispatcherContext::Instance().StopTimer(this);
	}

	void FDispatcherTimer::OnIntervalChanged(uint32 ValueBefore)
	{
	}

	void FDispatcherTimer::OnTimer()
	{
		Tick();
	}
}
