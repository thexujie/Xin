#pragma once

#include "Core.Types.h"

namespace Xin
{
	class FDispatcherTimer;

	class CORE_API IDispatcherContext : public IObject
	{
	public:
		static IDispatcherContext & Instance();

	public:
		virtual void DispatchInvoke() = 0;

		virtual uint32 StartTimer(FDispatcherTimer * DispatcherTimer) = 0;
		virtual uint32 StopTimer(FDispatcherTimer * DispatcherTimer) = 0;
	};
}
