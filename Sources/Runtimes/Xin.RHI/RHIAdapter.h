#pragma once

#include "RHITypes.h"

namespace Xin::RHI
{
	class RHI_API IRHIAdapter: public IRHIObject
	{
	public:
		IRHIAdapter() = default;
		virtual uint64 GetMemoryUsage() const = 0;

	public:
		uint64 VendorId = 0;
		uint64 DeviceId = 0;
	};

	using IRHIAdapterRef = TReferPtr<IRHIAdapter>;
}
