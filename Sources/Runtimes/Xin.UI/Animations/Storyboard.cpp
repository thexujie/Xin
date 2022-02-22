#include "PCH.h"
#include "Storyboard.h"

//#include "Core/DateTime.h"

namespace Xin::UI
{
	FStoryboard::~FStoryboard()
	{
		if (Begun)
		{
			StopTimer();
			for (FTimelineRef & Timeline : Timelines)
				Timeline->End(Object);
		}
	}

	void FStoryboard::Play()
	{
		if (Begun)
			return;

		Activate(Object);
		Begin(Object);

		if (Duration == 0)
		{
			for (FTimelineRef & Timeline : Timelines)
				AvialiableDuration = Max(0, Timeline->Duration);
		}
		else
			AvialiableDuration = Duration;

		for (FTimelineRef & Timeline : Timelines)
		{
			Timeline->Activate(Object);
			Timeline->Begin(Object);
		}
		Timer.Interval = 1000 / 30;
		AssertExpr(Timer.Tick.Functions.Size == 0);
		Timer.Tick += { this, & FStoryboard::OnTimer };
		Timer.Start();

		FTimelineTime Time;
		Time.CurrentTime = FDateTime::SteadyMilliseconds();
		Time.Duration = 0;
		ApplyTime(Object, Time);
	}

	void FStoryboard::Stop()
	{
		if (!Begun)
			return;

		StopTimer();
		for (FTimelineRef & Timeline : Timelines)
		{
			Timeline->End(Object);
			Timeline->Inactivate(Object);
		}
		Reversing = false;
		End(Object);
		Inactivate(Object);
	}

	void FStoryboard::OnTimer()
	{
		AssertExpr(Begun);
		FTimelineTime Time {};
		Time.CurrentTime = FDateTime::SteadyMilliseconds();
		Time.Duration = Time.CurrentTime - Timer.StartTime;
		if (Time.Duration >= AvialiableDuration * 2)
		{
			if (Reversing)
			{
				Reversing = false;
				Time.Duration = 0;
				ApplyTime(Object, Time);
			}
			Stop();
		}
		else if (Time.Duration >= AvialiableDuration)
		{
			if (AutoReverse)
			{
				if (!Reversing)
					Reversing = true;

				if (Reversing)
				{
					Time.Duration = Clamp(AvialiableDuration * 2 - (Time.CurrentTime - Timer.StartTime), Start, AvialiableDuration);
					ApplyTime(Object, Time);
				}
			}
			else
			{
				Time.Duration = AvialiableDuration;
				ApplyTime(Object, Time);
				Stop();
			}
		}
		else
		{
			Time.Duration = Clamp(Time.CurrentTime - Timer.StartTime, Start, AvialiableDuration);
			ApplyTime(Object, Time);
		}
	}

	void FStoryboard::ApplyTime(IDependency & Object, const FTimelineTime & Time)
	{
		for (FTimelineRef & Timeline : Timelines)
			Timeline->ApplyTime(Object, Time);
	}

	void FStoryboard::StopTimer()
	{
		if (Timer.StartTime)
		{
			Timer.Tick -= { this, & FStoryboard::OnTimer };
			Timer.Stop();
		}
	}
}
