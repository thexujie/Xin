#pragma once

#include "Basic.Types.h"
#include "Tuple.h"

namespace Xin
{
	template<typename CallableT, typename... ArgumentTs>
		requires InvocableC<DecayT<CallableT>, ArgumentTs...>
	constexpr auto Invoke(CallableT && Callable, ArgumentTs &&... Arguments)
	{
		return std::invoke(Forward<CallableT>(Callable), Forward<ArgumentTs>(Arguments)...);
	}

	class ICallableStorge
	{
	public:
		virtual ~ICallableStorge() = default;
		virtual ICallableStorge * Clone() = 0;
	};

	template<typename CallableT, typename... CallableParametersT>
	class TCallableStorge : public ICallableStorge
	{
	public:
		TCallableStorge(CallableT && Callable, CallableParametersT &&... CallableParameters) : Callable(Callable), CallableParameters(Forward<CallableParametersT>(CallableParameters)...) {}
		TCallableStorge(const TCallableStorge & That) : Callable(That.Callable), CallableParameters(That.CallableParameters) {}
		ICallableStorge * Clone() override { return new TCallableStorge(*this); }

	public:
		CallableT Callable;
		TTuple<DecayT<CallableParametersT>...> CallableParameters;
	};

	template<typename CallableT, typename ReturnT, typename... ParameterTs>
	class TCallableCaller
	{
	public:
		TCallableCaller() = default;

		template<uintx ... Indices, typename... ArgsT>
		static auto CallThis(IntegerSequence<uintx, Indices...>, CallableT && Callable, TTuple<ArgsT...> & Args, ParameterTs &&... Parameters)
		{
			if constexpr (IsVoidV<ReturnT>)
				Invoke(Callable, (Get<Indices>(Args))..., Forward<ParameterTs>(Parameters)...);
			else
				return Invoke(Callable, (Get<Indices>(Args))..., Forward<ParameterTs>(Parameters)...);
		}

		template<typename... ArgsT>
		static auto Call(const ICallableStorge * This, ParameterTs &&... Parameters)
		{
			TCallableStorge<CallableT, ArgsT...> * CallableStorge = const_cast<TCallableStorge<CallableT, ArgsT...> *>(static_cast<const TCallableStorge<CallableT, ArgsT...> *>(This));
			if constexpr (IsVoidV<ReturnT>)
				CallThis(IndexSequenceFor<ArgsT...> {}, Forward<CallableT>(CallableStorge->Callable), CallableStorge->CallableParameters, Forward<ParameterTs>(Parameters)...);
			else
				return CallThis(IndexSequenceFor<ArgsT...> {}, Forward<CallableT>(CallableStorge->Callable), CallableStorge->CallableParameters, Forward<ParameterTs>(Parameters)...);
		}
	};

	template<typename T>
	struct TCallable
	{
		static_assert(FalseTypeV<T>, "TCallable does not accept non-function types as template arguments.");
	};

	template<typename ReturnT, typename... ArgumentTs>
	struct TCallable<ReturnT(ArgumentTs...)>
	{
	public:
		TCallable() = default;
		TCallable(NullptrT) {}
		auto operator <=>(const TCallable &) const noexcept = default;

		TCallable(const TCallable & That)
		{
			if (That.Storage)
			{
				Storage = That.Storage->Clone();
				Caller = That.Caller;
			}
		}

		TCallable(TCallable && That) noexcept
		{
			Storage = That.Storage;
			Caller = That.Caller;
			That.Storage = nullptr;
			That.Caller = nullptr;
		}

		template<typename CallableT, typename... CallableParametersT>
			requires (!IsSameV<DecayT<CallableT>, TCallable<ReturnT(ArgumentTs...)>>)
		TCallable(CallableT && Callable, CallableParametersT &&... CallableParameters)
			: Storage(new TCallableStorge<CallableT, CallableParametersT...>(Forward<CallableT>(Callable), Forward<CallableParametersT>(CallableParameters)...))
			, Caller(TCallableCaller<CallableT, ReturnT, ArgumentTs...>::template Call<CallableParametersT...>)
		{
			using CallableStorgeT = TCallableStorge<CallableT, CallableParametersT...>;
			using CallableCallerT = TCallableCaller<CallableT, ReturnT, ArgumentTs...>;
		}

		~TCallable()
		{
			if (Storage)
			{
				delete Storage;
				Storage = nullptr;
			}
		}

		ReturnT operator ()(ArgumentTs... Arguments) const
		{
			return Caller(Storage, Forward<ArgumentTs>(Arguments)...);
		}

		TCallable & operator =(const TCallable & That) noexcept
		{
			if (Storage)
			{
				delete Storage;
				Storage = nullptr;
			}
			Storage = That.Storage->Clone();
			Caller = That.Caller;
			return *this;
		}

		TCallable & operator =(TCallable && That) noexcept
		{
			if (Storage)
			{
				delete Storage;
				Storage = nullptr;
			}

			Storage = That.Storage;
			Caller = That.Caller;
			That.Storage = nullptr;
			That.Caller = nullptr;
			return *this;
		}

		operator bool() const { return !!Storage; }

	public:
		ICallableStorge * Storage = nullptr;
		ReturnT (* Caller)(const ICallableStorge *, ArgumentTs &&...) = nullptr;
	};


	template<int Index>
	struct TPlacer
	{
		TPlacer() = default;
		TPlacer(const TPlacer &) = default;
		TPlacer(TPlacer &&) noexcept = default;
	};

	inline constexpr TPlacer<1> Placer1 {};
	inline constexpr TPlacer<2> Placer2 {};
	inline constexpr TPlacer<3> Placer3 {};
	inline constexpr TPlacer<4> Placer4 {};
	inline constexpr TPlacer<5> Placer5 {};
	inline constexpr TPlacer<6> Placer6 {};
	inline constexpr TPlacer<7> Placer7 {};
	inline constexpr TPlacer<8> Placer8 {};
	inline constexpr TPlacer<9> Placer9 {};
	inline constexpr TPlacer<10> Placer10 {};
	inline constexpr TPlacer<11> Placer11 {};
	inline constexpr TPlacer<12> Placer12 {};
	inline constexpr TPlacer<13> Placer13 {};
	inline constexpr TPlacer<14> Placer14 {};
	inline constexpr TPlacer<15> Placer15 {};
	inline constexpr TPlacer<16> Placer16 {};
	inline constexpr TPlacer<17> Placer17 {};
	inline constexpr TPlacer<18> Placer18 {};
	inline constexpr TPlacer<19> Placer19 {};
	inline constexpr TPlacer<20> Placer20 {};

	template<typename T>
	struct IsPlaceholder : IntegerConstant<int, 0> {};

	template<int Index>
	struct IsPlaceholder<TPlacer<Index>> : IntegerConstant<int, Index> { };

	template<typename T>
	struct IsPlaceholder<const T> : IsPlaceholder<T>::Type { };

	template<typename T>
	struct IsPlaceholder<volatile T> : IsPlaceholder<T>::Type { };

	template<typename T>
	struct IsPlaceholder<const volatile T> : IsPlaceholder<T>::Type { };

	template<typename T>
	constexpr int IsPlaceholderV = IsPlaceholder<T>::Value;

	template<typename ParameterT, typename... ArgumentTs>
	struct TParameterExtractor
	{
		template<typename ArgumentT>
		auto Extract(ArgumentT && Parameter, ArgumentTs &&...)
		{
			return Parameter;
		}
	};

	template<int Index, typename... ArgumentTs>
	struct TParameterExtractor<const TPlacer<Index>&, ArgumentTs...>
	{
		template<typename ReturnT = PackIndexingT<Index - 1, ArgumentTs...>>
		auto Extract(const TPlacer<Index> &, ArgumentTs &&... Arguments) -> ReturnT &&
		{
			static_assert(Index > 0, "Placer index must be greater than 0");
			return Forward<PackIndexingT<Index - 1, ArgumentTs...>>(PackIndexingArgument<Index - 1, ArgumentTs...>(Forward<ArgumentTs>(Arguments)...));
		}
	};


	template<uintx ... Indices, typename CallableT, typename ParametersTupleT, typename... ArgumentTs>
	auto InvokeCallable(IntegerSequence<uintx, Indices...>, CallableT && Callable, ParametersTupleT & Parameters, ArgumentTs &&... Arguments)
	{
		return Invoke(Callable, TParameterExtractor<TupleElementT<Indices, ParametersTupleT>, ArgumentTs...>().Extract(Get<Indices>(Parameters), Forward<ArgumentTs>(Arguments)...)...);
	}

	template<typename CallableT, typename... ParameterTs>
	struct TBinder
	{
		CallableT Callable;
		TTuple<ParameterTs...> Parameters;

		TBinder(CallableT && Callable, ParameterTs &&... Parameters) : Callable(Forward<CallableT>(Callable)), Parameters(Forward<ParameterTs>(Parameters)...) { }

		//auto operator ()(ParameterTs ... Arguments) const
		//{
		//	return InvokeCallable(IndexSequenceFor<ParameterTs...> { }, Callable, Parameters, Forward<ParameterTs>(Arguments)...);
		//}

		//auto operator ()(ParameterTs ... Arguments)
		//{
		//	return InvokeCallable(IndexSequenceFor<ParameterTs...> { }, Callable, Parameters, Forward<ParameterTs>(Arguments)...);
		//}

		template<typename... ArgumentTs>
		auto operator ()(ArgumentTs &&... Arguments) const
		{
			return InvokeCallable(IndexSequenceFor<ParameterTs...> {}, Callable, Parameters, Forward<ArgumentTs>(Arguments)...);
		}

		template<typename... ArgumentTs>
		auto operator ()(ArgumentTs &&... Arguments)
		{
			return InvokeCallable(IndexSequenceFor<ParameterTs...> {}, Callable, Parameters, Forward<ArgumentTs>(Arguments)...);
		}
	};

	template<typename CallableT, typename... ParameterTs>
	auto Bind(CallableT && Callable, ParameterTs &&... Parameters)
	{
		return TBinder<CallableT, ParameterTs...>(Forward<CallableT>(Callable), Forward<ParameterTs>(Parameters)...);
	}
}
