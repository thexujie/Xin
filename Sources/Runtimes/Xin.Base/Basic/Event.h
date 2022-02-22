#pragma once

#include "Basic.Types.h"
#include "Function.h"
#include "List.h"

namespace Xin
{
	template<typename T>
	struct TDelegate;

	template<typename ReturnT, typename... ArgumentsT>
	struct TDelegate<ReturnT(ArgumentsT...)>
	{
		using FunctionT = TFunction<ReturnT(ArgumentsT...)>;

	public:
		TDelegate() = default;
		~TDelegate() = default;

		bool Binded() const { return !!Function; }
		explicit operator bool() const { return Binded(); }

		auto Excute(ArgumentsT &&... Arguments)
		{
			return Xin::Invoke(Function, Forward<ArgumentsT>(Arguments)...);
		}

		template<typename... ArgumentsTy>
		auto operator ()(ArgumentsTy &&... Arguments)
		{
			(IsConvertibleToV<ArgumentsTy, ArgumentsT> && ...);

			if (!Binded())
				return ReturnT {};

			return Xin::Invoke(Function, Forward<ArgumentsTy>(Arguments)...);
		}

		TDelegate & operator = (const FunctionT & Function)
		{
			this->Function = Function;
			return *this;
		}

		TDelegate & operator = (FunctionT && Function)
		{
			this->Function = Move(Function);
			return *this;
		}

	public:
		FunctionT Function;
	};

	struct FEventArgs
	{
	public:
		FEventArgs() = default;

	public:
		bool Handled = false;
	};

	struct FBoolEventArgs
	{
	public:
		FBoolEventArgs(bool Value) : Value(Value) {}

	public:
		bool Value = false;
	};

	using FDelegate = TDelegate<void(FEventArgs &)>;

	enum class EReply
	{
		None = 0,
		Handled,
	};

	class IEventResponser { };

	using IEventResponserPtr = IEventResponser *;


	template<typename T>
	struct TEvent
	{
		static_assert(FalseTypeV<T>, "TEvent does not accept non-function types as template arguments.");
	};

	template<typename ReturnT, typename... ParametersT>
	struct TEvent<ReturnT(ParametersT...)>
	{
	public:
		using FunctionT = TFunction<ReturnT(ParametersT...)>;

	public:
		TEvent() = default;
		~TEvent() = default;
		TEvent(const TEvent &) = delete;
		TEvent & operator =(const TEvent &) = delete;

		bool Binded() const { return !Functions.Empty(); }

		template<typename... ArgumentTs>
		auto Excute(ArgumentTs &&... Arguments)
		{
			if constexpr (IsSameV<ReturnT, void>)
			{
				for (FunctionT & Function : Functions)
				{
					Function(Forward<ArgumentTs>(Arguments)...);
				}
			}
			else
			{
				ReturnT Result;
				for (FunctionT & Function : Functions)
				{
					Result = Function(Forward<ArgumentTs>(Arguments)...);
				}
				return Result;
			}
		}

		template<typename... ArgsT>
		auto operator ()(ArgsT &&... Args)
		{
			return Excute(Forward<ArgsT>(Args)...);
		}

		template<DerivedOfC<IObject> ObjectT, typename CallableT>
		void operator +=(TPair<ObjectT *, CallableT&&> Pair)
		{
			Functions.Add(TFunction { Pair.First, Forward<CallableT>(Pair.Second) });
		}

		void operator +=(TFunction<ReturnT(ParametersT...)> Function)
		{
			AssertExpr(Function);
			Functions.Add(Function);
		}

		void operator -=(TFunction<ReturnT(ParametersT...)> Function)
		{
			AssertExpr(Function);
			Functions.Remove(Function);
		}

		bool Contains(TFunction<ReturnT(ParametersT...)> Function) const
		{
			AssertExpr(Function);
			return Functions.Contains(Function);
		}

		template<DerivedOfC<IObject> ObjectT, typename CallableT>
			requires InvocableC<DecayT<CallableT>, ObjectT, ParametersT...> && ConvertibleToC<InvokeResultT<CallableT, ObjectT, ParametersT...>, ReturnT>
		void Bind(ObjectT * Object, CallableT && Callable)
		{
			using InvokerT = TObjectInvoker<ObjectT, DecayT<CallableT>, ReturnT, ParametersT...>;
			operator +=(FunctionT { Object, Forward<CallableT>(Callable), (InvokerT *)nullptr });
		}


		template<DerivedOfC<IObject> ObjectT, typename CallableT>
			requires InvocableC<DecayT<CallableT>, ObjectT, ParametersT...> && ConvertibleToC<InvokeResultT<CallableT, ObjectT, ParametersT...>, ReturnT>
		void UnBind(ObjectT * Object, CallableT && Callable)
		{
			using InvokerT = TObjectInvoker<ObjectT, DecayT<CallableT>, ReturnT, ParametersT...>;
			operator -=(FunctionT { Object, Forward<CallableT>(Callable), (InvokerT *)nullptr });
		}

	public:
		TList<TFunction<ReturnT(ParametersT...)>> Functions;
	};

	using FEvent = TEvent<void(FEventArgs &)>;
}
