#pragma once

namespace Xin
{
	enum class MemoryOrder
	{
		Relaxed = 0,
		Consume,
		Acquire,
		Release,
		AcquireRelease,
		SequenceConst,
	};

	template<typename T, uintx = sizeof(T)> struct TAtomicBase;

	template<typename T>
	struct TAtomicBase<T, 1>
	{
		using InterT = char;

		TAtomicBase() = default;
		TAtomicBase(const T & ValueIn) : Value(ValueIn) {}

		T Load(MemoryOrder Order = MemoryOrder::SequenceConst) const noexcept { return Value; }
		void Store(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			_InterlockedExchange8(AddressAs<char, T>(Value), ValueAs<char, T>(Desired));
		}

		T FetchStore(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchange8(AddressAs<char, T>(Value), ValueAs<char, T>(Desired));
			return ValueAs<T>(Result);
		}

		T FetchAdd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd8(AddressAs<char, T>(Value), ValueAs<char, T>(Arg));
			return ValueAs<T>(Result);
		}

		template<bool = IsSignedV<T>>
		T FetchSub(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd8(AddressAs<char, T>(Value), ValueAs<char, T>(-Arg));
			return ValueAs<T>(Result);
		}

		T FetchAnd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedAnd8(AddressAs<char, T>(Value), ValueAs<char, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchOr(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedOr8(AddressAs<char, T>(Value), ValueAs<char, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchXor(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedXor8(AddressAs<char, T>(Value), ValueAs<char, T>(Arg));
			return ValueAs<T>(Result);
		}

		T Value;
	};

	template<typename T>
	struct TAtomicBase<T, 2>
	{
		using InterT = short;

		TAtomicBase() = default;
		TAtomicBase(const T & ValueIn) : Value(ValueIn) {}

		T Load(MemoryOrder Order = MemoryOrder::SequenceConst) const noexcept { return Value; }
		void Store(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			_InterlockedExchange16(AddressAs<short, T>(Value), ValueAs<short, T>(Desired));
		}

		T FetchStore(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchange16(AddressAs<short, T>(Value), ValueAs<short, T>(Desired));
			return ValueAs<T>(Result);
		}

		template<bool = IsSignedV<T>>
		T FetchSub(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd16(AddressAs<short, T>(Value), ValueAs<short, T>(-Arg));
			return ValueAs<T>(Result);
		}

		T FetchAnd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedAnd16(AddressAs<short, T>(Value), ValueAs<short, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchOr(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedOr16(AddressAs<short, T>(Value), ValueAs<short, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchXor(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedXor16(AddressAs<short, T>(Value), ValueAs<short, T>(Arg));
			return ValueAs<T>(Result);
		}

		T Value;
	};

	template<typename T>
	struct TAtomicBase<T, 4>
	{
		using InterT = long;

		TAtomicBase() = default;
		TAtomicBase(const T & ValueIn) : Value(ValueIn) {}

		T Load(MemoryOrder Order = MemoryOrder::SequenceConst) const noexcept { return Value; }
		void Store(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			_InterlockedExchange(AddressAs<long, T>(Value), ValueAs<long, T>(Desired));
		}

		T FetchStore(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchange(AddressAs<long, T>(Value), ValueAs<long, T>(Desired));
			return ValueAs<T>(Result);
		}
		
		T FetchAdd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd(AddressAs<long, T>(Value), ValueAs<long, T>(Arg));
			return ValueAs<T>(Result);
		}

		template<bool = IsSignedV<T>>
		T FetchSub(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd(AddressAs<long, T>(Value), ValueAs<long, T>(-Arg));
			return ValueAs<T>(Result);
		}

		T FetchAnd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedAnd(AddressAs<long, T>(Value), ValueAs<long, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchOr(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedOr(AddressAs<long, T>(Value), ValueAs<long, T>(Arg));
			return ValueAs<T>(Result);
		}

		T FetchXor(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedXor(AddressAs<long, T>(Value), ValueAs<long, T>(Arg));
			return ValueAs<T>(Result);
		}

		T CompareExchange(T Comparand, T Exchange) noexcept
		{
			InterT Result = _InterlockedCompareExchange(AddressAs<long, T>(Value), ValueAs<long, T>(Exchange), ValueAs<long, T>(Comparand));
			return ValueAs<T>(Result);
		}

		T Value;
	};

	template<typename T>
	struct TAtomicBase<T, 8>
	{
		using InterT = long long;

		TAtomicBase() = default;
		TAtomicBase(const T & ValueIn) : Value(ValueIn) {}

		T Load(MemoryOrder Order = MemoryOrder::SequenceConst) const noexcept { return Value; }
		void Store(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			_InterlockedExchange64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Desired));
		}

		T FetchStore(T Desired, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchange64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Desired));
			return static_cast<T>(Result);
		}

		T FetchAdd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Arg));
			return static_cast<T>(Result);
		}

		template<bool = IsSignedV<T>>
		T FetchSub(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedExchangeAdd64(AddressAs<long long, T>(Value), ValueAs<long long, T>(-Arg));
			return static_cast<T>(Result);
		}

		T FetchAnd(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedAnd64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Arg));
			return static_cast<T>(Result);
		}

		T FetchOr(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedOr64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Arg));
			return static_cast<T>(Result);
		}

		T FetchXor(T Arg, MemoryOrder Order = MemoryOrder::SequenceConst) noexcept
		{
			InterT Result = _InterlockedXor64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Arg));
			return static_cast<T>(Result);
		}

		T CompareExchange(T Comparand, T Exchange) noexcept
		{
			InterT Result = _InterlockedCompareExchange64(AddressAs<long long, T>(Value), ValueAs<long long, T>(Exchange), ValueAs<long long, T>(Comparand));
			return ValueAs<T>(Result);
		}

		T Value;
	};
	
	template<typename T>
	class TAtomic : public TAtomicBase<T>
	{
	public:
		TAtomic() = default;
		TAtomic(const T & ValueIn) : TAtomicBase<T>(ValueIn) {}

		template<bool = IsIntegerV<T>>
		T operator++() noexcept { return TAtomicBase<T>::FetchAdd(1) + 1; }

		template<bool = IsIntegerV<T>>
		T operator++(int) noexcept { return TAtomicBase<T>::FetchAdd(1); }

		template<bool = IsIntegerV<T> && IsSignedV<T>>
		T operator--() noexcept { return TAtomicBase<T>::FetchSub(1) - 1; }

		template<bool = IsIntegerV<T> && IsSignedV<T>>
		T operator--(int) noexcept { return TAtomicBase<T>::FetchSub(1); }

		operator T() const { return TAtomicBase<T>::Load(MemoryOrder::SequenceConst); }
	};

	using FAtomicBool = TAtomic<bool>;
	using FAtomicInt32 = TAtomic<int32>;
	using FAtomicUInt32 = TAtomic<uint32>;
	using FAtomicInt64 = TAtomic<int64_t>;
	using FAtomicUInt64 = TAtomic<uint64>;
	using FAtomicFloat32 = TAtomic<float32>;
	using FAtomicFloat64 = TAtomic<float64>;

	template class BASE_API TAtomic<bool>;
	template class BASE_API TAtomic<int32>;
	template class BASE_API TAtomic<uint32>;
	template class BASE_API TAtomic<int64_t>;
	template class BASE_API TAtomic<uint64>;
	template class BASE_API TAtomic<float32>;
	template class BASE_API TAtomic<float64>;

	namespace Atomics
	{
		template<IntegerC T>
		forceinline static T IncFetch(T & Value)
		{
			if constexpr (sizeof(T) == 4)
			{
				long ModifiedValue = _InterlockedIncrement(AddressAs<long, T>(Value));
				return ValueAs<T, long>(ModifiedValue);
			}
			else if constexpr (sizeof(T) == 8)
			{
				__int64 ModifiedValue = _InterlockedIncrement64(AddressAs<__int64, T>(Value));
				return ValueAs<T, __int64>(ModifiedValue);
			}
			else
			{
				static_assert(false, "unsupported type T.");
				return Value;
			}
		}

		template<IntegerC T>
		forceinline static T DecFetch(T & Value)
		{
			if constexpr (sizeof(T) == 4)
			{
				long ModifiedValue = _InterlockedDecrement(AddressAs<long, T>(Value));
				return ValueAs<T, long>(ModifiedValue);
			}
			else if constexpr (sizeof(T) == 8)
			{
				__int64 ModifiedValue = _InterlockedDecrement64(AddressAs<__int64, T>(Value));
				return ValueAs<T, __int64>(ModifiedValue);
			}
			else
			{
				static_assert(false, "unsupported type T.");
				return Value;
			}
		}

		template<IntegerC T>
		forceinline static T FetchExchange(T & Value, T NewValue)
		{
			if constexpr (sizeof(T) == 4)
			{
				long ModifiedValue = _InterlockedExchange(AddressAs<long, T>(Value), NewValue);
				return ValueAs<T, long>(ModifiedValue);
			}
			else if constexpr (sizeof(T) == 8)
			{
				__int64 ModifiedValue = _InterlockedExchange64(AddressAs<__int64, T>(Value), NewValue);
				return ValueAs<T, __int64>(ModifiedValue);
			}
			else
			{
				static_assert(false, "unsupported type T.");
				return Value;
			}
		}

		template<IntegerC T>
		forceinline static bool CompareExchange(T & Value, T & Expected, T Desired)
		{
			if constexpr (sizeof(T) == 4)
			{
				T PreviousValue = ValueAs<T, long>(_InterlockedCompareExchange(AddressAs<long, T>(Value), Desired, Expected));
				if (PreviousValue == Expected)
					return true;
				Expected = PreviousValue;
				return false;
			}
			else if constexpr (sizeof(T) == 8)
			{
				T PreviousValue = ValueAs<T, __int64>(_InterlockedCompareExchange64(AddressAs<__int64, T>(Value), Desired, Expected));
				if (PreviousValue == Expected)
					return true;
				Expected = PreviousValue;
				return false;
			}
			else
			{
				static_assert(false, "unsupported type T.");
				return Value;
			}
		}

		template<IntegerC T>
		forceinline static T ExchangeAdd(T & Value, T Addend)
		{
			if constexpr (sizeof(T) == 4)
			{
				long PreviousValue = _InterlockedExchangeAdd(AddressAs<long, T>(Value), Addend);
				return ValueAs<T, long>(PreviousValue);
			}
			else if constexpr (sizeof(T) == 8)
			{
				__int64 PreviousValue = _InterlockedExchangeAdd64(AddressAs<__int64, T>(Value), Addend);
				return ValueAs<T, __int64>(PreviousValue);
			}
			else
			{
				static_assert(false, "unsupported type T.");
				return Value;
			}
		}
	}
}
