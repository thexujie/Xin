#pragma once

#include "RHITypes.h"
#include "RHIAdapter.h"

namespace Xin::RHI
{
	enum class ECreateDeviceFlag
	{
		None = 0,
	};

	class RHI_API IRHIFactory : public IRHIObject
	{
	public:
		IRHIFactory() = default;
		virtual TList<IRHIAdapterRef> GetAdapters() const = 0;
	};
	using IRHIFactoryRef = TReferPtr<IRHIFactory>;
}
