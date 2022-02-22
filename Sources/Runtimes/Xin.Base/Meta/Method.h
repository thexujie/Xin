#pragma once

#include "Meta.h"

namespace Xin
{
	struct BASE_API FInvocable : FMeta
	{
	public:
		FInvocable() = default;
		FInvocable(FName Name, const FMetadata & Metadata) : FMeta(Name, Metadata) {}

	public:
		static FInvocable None;
	};

	struct BASE_API FMethod : FInvocable
	{
	public:
		FMethod() = default;
		FMethod(FName Name, const FMetadata & Metadata) : FInvocable(Name, Metadata) {}

		virtual FVariant Invoke(FVariant Instance, TView<FVariant> Arguments) const { return nullptr; }

	public:
		static FMethod None;
	};

	struct BASE_API FConstructor : FInvocable
	{
	public:
		FConstructor() = default;
		FConstructor(FName Name, const FMetadata & Metadata) : FInvocable(Name, Metadata) {}

		virtual bool IsInvocable(TView<const FType *> ParameterTypes) const { return false; }
		virtual FVariant Invoke(TView<FVariant> Arguments) const { return nullptr; }

	public:
		static FConstructor None;
	};

	template<typename InstanceT, typename... ArgTs>
	struct TConstructor : FConstructor
	{
	public:
		template<typename... ArgsT> requires IsConstructibleV<InstanceT, ArgsT...>
		static InstanceT * ConstructInstance(ArgsT &&... Args)
		{
			if constexpr (IsDerivedOfV<InstanceT, IObject>)
				return MakeRefer<InstanceT, ArgsT...>(Forward<ArgsT>(Args)...);
			else if constexpr (!IsDerivedOfV<InstanceT, IObject>)
				return new InstanceT(Args...);
			else
				return nullptr;
		}
		TConstructor() = default;
		TConstructor(FName Name, const FMetadata & Metadata = FMetadata::None) : FConstructor(Name, Metadata) {}

		InstanceT * Invoke(ArgTs &&... Args) const
		{
			if (!Valid())
				throw EError::InvalidOperation;

			return ConstructInstance(Args...);
		}

		FVariant Invoke(TView<FVariant> Arguments) const override
		{
			if (Arguments.Size < sizeof...(ArgTs))
				return nullptr;

			if constexpr (sizeof...(ArgTs) == 0)
				return ConstructInstance();
			else
			{
				sizet ParameterIndex = sizeof...(ArgTs) - 1;
				FVariant Instance { ConstructInstance(Arguments[ParameterIndex--].Get<ArgTs>()...) };
				return Instance;
			}
		}

		bool IsInvocable(TView<const FType *> ParameterTypes) const override;

	public:
		static TConstructor None;
	};

	template<typename InstanceT, typename... ArgTs>
	TConstructor<InstanceT, ArgTs...> TConstructor<InstanceT, ArgTs...>::None;

	template<typename InstanceT, typename FunctorT>
	struct TMethod : FMethod {};

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	struct TMethod<InstanceT, ReturnT(ParameterTs...)> : FMethod
	{
	public:
		using MethodType = ReturnT(InstanceT:: *)(ParameterTs...);
		using ConstMethodType = ReturnT(InstanceT:: *)(ParameterTs...) const;
		using ConstNoexceptMethodType = ReturnT(InstanceT:: *)(ParameterTs...) const noexcept;

	public:
		TMethod() = default;
		TMethod(FName Name, MethodType Method, const FMetadata & Metadata) : FMethod(Name, Metadata), Method(Method) {}
		TMethod(FName Name, ConstMethodType ConstMethod, const FMetadata & Metadata) : FMethod(Name, Metadata), ConstMethod(ConstMethod) {}
		TMethod(FName Name, ConstNoexceptMethodType ConstNoexceptMethod, const FMetadata & Metadata) : FMethod(Name, Metadata), ConstNoexceptMethod(ConstNoexceptMethod) {}

		ReturnT Invoke(InstanceT & Instance, ParameterTs &&... Arguments) const
		{
			if (Method == nullptr)
				return ReturnT { };
			return (Instance.*Method)(Forward<ParameterTs>(Arguments)...);
		}

		FVariant Invoke(FVariant Instance, TView<FVariant> Arguments) const override
		{
			if (Arguments.Size < sizeof...(ParameterTs))
				return nullptr;

			if (Method == nullptr)
				return nullptr;

			sizet ParameterIndex = sizeof...(ParameterTs) - 1;
			InstanceT & This = Instance.Get<InstanceT &>();
			if constexpr (IsVoidV<ReturnT>)
			{
				(This.*Method)(Forward<ParameterTs>(Arguments[ParameterIndex--].Get<ParameterTs>())...);
				return nullptr;
			}
			else
				return (This.*Method)(Forward<ParameterTs>(Arguments[ParameterIndex--].Get<ParameterTs>())...);
		}

	public:
		union
		{
			MethodType Method = nullptr;
			ConstMethodType ConstMethod;
			ConstNoexceptMethodType ConstNoexceptMethod;
		};

	public:
		static TMethod None;
	};

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	TMethod<InstanceT, ReturnT(ParameterTs...)> TMethod<InstanceT, ReturnT(ParameterTs...)>::None;
}
