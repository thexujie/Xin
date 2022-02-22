#pragma once

#include "Xin.Windows.Informer.Types.h"

namespace Xin::Windows::Informer
{
	struct FCpuPerformanceCounter
	{
		uintx Hits = 0;
		uintx PercentFrequency = 0;
	};

	class INFORMER_API FCpuPerformance
	{
	public:
		FCpuPerformance();
		~FCpuPerformance();

		void Update();

	private:
		void UpdateUsage();
		void UpdateFrequency();

	public:
		bool SingleProcessorGroups = false;
		uintx NumberOfProcessorGroups = 1;
		TList<uintx> NumberOfActiveProcessors;

		uintx NumberOfProcessors = 0;
		uintx MaxMhz = 0;
		uintx CurrentMhz = 0;
		uintx ActiveFrenquencyMhz = 0;

		TList<uintx> ProcessorsMaxMhz;

	private:
		struct FPerformanceInfo
		{
			Internal::TDeltaValue<uintx> IdleTime;
			Internal::TDeltaValue<uintx> KernelTime;
			Internal::TDeltaValue<uintx> UserTime;
		};
		TList<FPerformanceInfo> PerformanceInfos;

	public:
		float32 Utilization = 0.0f;

	private:
		TList<FCpuPerformanceCounter> PerformanceCounters;
		uintx UpdateIndex = 0;
	};
}
