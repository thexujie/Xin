#pragma once

#include "Core.Types.h"
#include "Dependency.h"

namespace Xin
{
	class CORE_API [[Meta]] FDispatcherTimer : public IObject
	{
	public:
		FDispatcherTimer() = default;
		FDispatcherTimer(uint32 Interal);
		~FDispatcherTimer();

	public:
		uint32 Start();
		uint32 Stop();
		bool IsTicking() const { return !!StartTime; }
		PropertyR(bool, IsTicking) Ticking;

	public:
		void OnIntervalChanged(uint32 ValueBefore);

	public:
		void OnTimer();

	public:
		ProxyPropertyRW(uint32, Interval) = 1000;

	public:
		uint32 StartTime = 0;

	public:
		TEvent<void()> Tick;
	};
	using FDispatcherTimerRef = TReferPtr<FDispatcherTimer>;
}
