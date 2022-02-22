#pragma once

#include "Core.Types.h"

namespace Xin
{
	class CORE_API FProcedurer : public IObject
	{
	public:
		FProcedurer();
		~FProcedurer();

	public:
		void Invoke(TFunction<void()> Function);
		void BeginInvoke(TFunction<void()> Function);

	public:
		void Dispatch();

	public:
		class CORE_API FProcedurerCallback : public ISharedRefer
		{
		public:
			FProcedurerCallback(TFunction<void()> Function) : Function(Function) {}

			TFunction<void()> Function;
		};
		using FProcedurerCallbackRef = TReferPtr<FProcedurerCallback>;
		TList<FProcedurerCallbackRef> Callbacks;

	private:
		bool Dispatching = false;
		bool Canceled = false;
		voidp ThreadHandle = nullptr;

	public:
		static FProcedurer & Current();
		static FProcedurer & MainThread();
	};
}
