#pragma once

#include "Basic.Types.h"
#include <coroutine>

namespace Xin
{
	template<typename ElementT>
	struct IEnumerable
	{
		struct FPromise
		{
			FPromise() noexcept { }
			~FPromise() noexcept { }

			const ElementT * CurrentValue = nullptr;
			std::exception_ptr StdException = nullptr;

			IEnumerable get_return_object() noexcept { return IEnumerable { *this }; }
			std::suspend_always initial_suspend() noexcept { return {}; }

			// We promise that CurrentValue is usable even if the coroutine is finished,
			// and the handle (this) will be destroy in ~TEnumerable(), will not be destroy automaticly.
			std::suspend_always final_suspend() noexcept { return {}; }

			std::suspend_always yield_value(const ElementT & Value) noexcept
			{
				CurrentValue = AddressOf(Value);
				return {};
			}

			void return_void() noexcept {}

			void unhandled_exception() noexcept
			{
				StdException = std::current_exception();
			}

			void rethrow_if_exception()
			{
				if (StdException)
				{
					std::rethrow_exception(StdException);
				}
			}


			//static void * operator  new(uintx _Size)
			//static void operator  delete(void * Pointer, uintx _Size) noexcept 
		};

		using promise_type = FPromise;

		struct Iterator
		{
			using ValueType = ElementT;
			using ReferenceType = const ElementT&;
			using PointerType = const ElementT *;
			std::coroutine_handle<FPromise> * CoroutineHandle = nullptr;

			Iterator() noexcept = default;
			Iterator(std::coroutine_handle<FPromise> * CoroutineHandle) noexcept : CoroutineHandle(CoroutineHandle) {}

			Iterator & operator ++() noexcept
			{
				CoroutineHandle->resume();
				if (CoroutineHandle->done())
				{
					CoroutineHandle->promise().rethrow_if_exception();
					CoroutineHandle = nullptr;
				}

				return *this;
			}

			void operator ++(int) noexcept { ++*this; }
			bool operator ==(Iterator const & Another) const noexcept { return CoroutineHandle == Another.CoroutineHandle; }

			ReferenceType operator *() const noexcept { return *(CoroutineHandle->promise().CurrentValue); }
			PointerType operator ->() const noexcept { return CoroutineHandle->promise().CurrentValue; }
		};

		Iterator begin() noexcept
		{
			if (CoroutineHandle)
			{
				CoroutineHandle.resume();
				if (CoroutineHandle.done())
				{
					CoroutineHandle.promise().rethrow_if_exception();
					return { nullptr };
				}
			}

			return { &CoroutineHandle };
		}

		Iterator end() noexcept { return { nullptr }; }


		IEnumerable() noexcept = default;
		IEnumerable(FPromise & Promise) : CoroutineHandle(std::coroutine_handle<FPromise>::from_promise(Promise)) {}
		IEnumerable(const IEnumerable &) = delete;
		IEnumerable & operator =(const IEnumerable &) = delete;
		IEnumerable(IEnumerable && Another) noexcept : CoroutineHandle(Another.CoroutineHandle) { Another.CoroutineHandle = nullptr; }

		~IEnumerable() noexcept
		{
			if (CoroutineHandle)
			{
				CoroutineHandle.destroy();
			}
		}

		IEnumerable & operator =(IEnumerable && Another) noexcept
		{
			if (this != AddressOf(Another))
			{
				CoroutineHandle = Another.CoroutineHandle;
				Another.CoroutineHandle = nullptr;
			}
			return *this;
		}

	private:
		std::coroutine_handle<FPromise> CoroutineHandle = nullptr;
	};
}
