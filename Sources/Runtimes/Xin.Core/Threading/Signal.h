#pragma once

#include "../Xin.Core.Types.h"

namespace Xin
{
	class CORE_API FSignal
	{
	public:
		FSignal();
		~FSignal();
		FSignal(const FSignal &) = delete;
		FSignal(FSignal && That) noexcept :Handle(That.Handle)
		{
			That.Handle = nullptr;
		}

		void Signal() const;
		void Wait() const;
		voidp GetHandle() const { return Handle; }
		
	private:
		voidp Handle = nullptr;
#ifdef XIN_DEBUG
		mutable uint32 ThreadIdLocked = 0;
#endif
	};
}