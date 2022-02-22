#pragma once

#include "RHITypes.h"

namespace Xin::RHI
{
	class RHI_API IRHIFence : public IRHIObject
	{
	public:
		IRHIFence() = default;

	public:
		virtual uint64 GetCompletedValue() const = 0;
		virtual void WaitFence(uint64 Value) = 0;

	public:
		uint64 GetSignalValue() const { return SignalValue; }
		uint64 SignalValue = 0;
	};
	using IRHIFenceRef = TReferPtr<IRHIFence>;

}
