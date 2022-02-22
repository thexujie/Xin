#pragma once

#include "Core.Types.h"

namespace Xin
{
	class CORE_API FThread
	{
	public:
		FThread() = default;
		FThread(const FThread & That) = delete;
		FThread(FThread && That) noexcept : ThreadId(That.ThreadId), Handle(That.Handle)
		{
			That.ThreadId = 0;
			That.Handle = nullptr;
		}
		FThread & operator = (const FThread &) = delete;
		FThread & operator = (FThread && That) noexcept;
		
		~FThread()
		{
			if (ThreadId)
			{
				Join();
				ThreadId = 0;
			}
		}

		explicit FThread(uint32 ThreadId) : ThreadId(ThreadId) {}
		explicit FThread(TFunction<void()> Function);
		
		template<typename CallableT, typename... ArgsT, typename = EnableIfT<!IsSameV<RemoveCVRefT<CallableT>, FThread>>>
		explicit FThread(CallableT && Callable, ArgsT &&... Args)
		{
			struct SThreadCallableArgs
			{
				SThreadCallableArgs(CallableT && CallableIn, ArgsT &&... Args) : Callable(CallableIn), Tuple(Args...)
				{
					
				}
				CallableT Callable;
				TTuple<ArgsT...> Tuple;
			};

			auto FuncEntry = [](void * Args)->uint32
			{
				SThreadCallableArgs * FunctionArgs = (SThreadCallableArgs *)Args;
				Apply<CallableT, TTuple<ArgsT...>>(Forward<CallableT>(FunctionArgs->Callable), Forward<TTuple<ArgsT...>>(FunctionArgs->Tuple));
				delete FunctionArgs;
				return 0;
			};

			SThreadCallableArgs * FunctionArgs = new SThreadCallableArgs(Forward<CallableT>(Callable), Forward<ArgsT>(Args)...);
			Handle = (voidp)_beginthreadex(nullptr, 0, FuncEntry, FunctionArgs, 0, &ThreadId);
		}
		bool Joinable() const { return !!Handle; }
		void Join();

	protected:
		uint32 ThreadId = 0;
		voidp Handle = nullptr;

	public:
		static void Sleep(uint32 TimeInMS);
		static void YieldCurrent();
		static uint32 CurrentThreadId();
	};
}