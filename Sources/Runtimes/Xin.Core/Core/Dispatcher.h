#pragma once

#include "Core.Types.h"

namespace Xin
{
	class CORE_API FDispatcher : public IObject
	{
	public:
		FDispatcher();
		~FDispatcher();

	public:
		void Invoke(TFunction<void()> Function);
		void BeginInvoke(TFunction<void()> Function);

	public:
		void Dispatch();

	public:
		class CORE_API FDispatcherCallback : public ISharedRefer
		{
		public:
			FDispatcherCallback(TFunction<void()> Function) : Function(Function) {}

			TFunction<void()> Function;
		};
		using FDispatchInstanceRef = TReferPtr<FDispatcherCallback>;
		TList<FDispatchInstanceRef> Callbacks;

	public:
		uint32 ThreadId = NullIndex;
		bool Dispatching = false;
		bool Canceled = false;
		voidp ThreadHandle = nullptr;

	public:
		static FDispatcher & Current();
		static FDispatcher & MainThread();
	};
}
