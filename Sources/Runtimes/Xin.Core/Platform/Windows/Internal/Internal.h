#pragma once

#include "FileVersion.h"
#include "Path.h"
#include "Process.h"
#include "Winversion.h"

namespace Xin::Windows::Internal
{
	template<typename T>
	struct TDeltaValue
	{
		T Value = 0;
		T Delta = 0;

		TDeltaValue() = default;
		TDeltaValue(const TDeltaValue &) = default;
		~TDeltaValue() = default;

		void Update(T NewValue)
		{
			Delta = NewValue - Value;
			Value = NewValue, Delta;
		}

		TDeltaValue & operator=(uint64 NewValue)
		{
			Update(NewValue);
			return *this;
		}
	};
}