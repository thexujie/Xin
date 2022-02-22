#pragma once

#include "Core.Types.h"

namespace Xin
{
	class CORE_API FFileLinePeriod
	{
	public:
		FFileLinePeriod(const char * File, int Line, uintx PeriodIn = 1000, uintx CountIn = 1) : Id(File, Line), Period(PeriodIn), Count(CountIn)
		{

		}

		bool Check() const;

		TTuple<const char *, int> Id;
		/**
		 * in ms
		 */
		uintx Period;
		uintx Count = 1;
	};
}