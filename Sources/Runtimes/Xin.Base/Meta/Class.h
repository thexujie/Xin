#pragma once

#include "Meta.h"
#include "Type.h"
#include "Field.h"
#include "Property.h"
#include "Method.h"
#include "Enum.h"

namespace Xin
{
	struct BASE_API FClass : FType
	{
	public:
		FClass() = default;
		FClass(FName Name, FTypeid Typeid, FClass & Parent = FClass::None, const FMetadata & Metadata = FMetadata::None) : FType(Name, Typeid, Metadata), Parent(Parent) {}

	public:
		bool IsA(const FType & That) const override
		{
			if (!Valid() || !That.Valid())
				return false;

			if (operator ==(That))
				return true;

			return Parent.IsA(That);
			//if (GetPointeeType().IsA(That.GetPointeeType()))
			//	return true;

			//for (const FClass & BaseType : BaseTypes)
			//{
			//	if (BaseType.IsA(That))
			//		return true;
			//}
			//return false;
		}

	public:
		const FConstructor & FindConstructor(TView<const FType *> ParameterTypes = {}) const;

		template<typename InstanceT, typename... ArgTs>
		const TConstructor<InstanceT, ArgTs...> & FindConstructorT() const;

		const FMethod & FindMethod(FName Name) const;
		const FField & FindField(FName Name) const;
		const FProperty & FindProperty(FName Name) const;

		template<typename InstanceT, FunctionC MethodT>
		const TMethod<InstanceT, MethodT> & FindMethod(FName Name) const
		{
			const FMethod & Function = FindMethod(Name);
			if (!Function)
				return {};

			return StaticCast<TMethod<InstanceT, MethodT>>(Function);
		}

	public:
		FClass & Parent = FClass::None;

		TView<const FConstructor &> Constructors;
		TView<const FMethod &> Methods;
		TView<const FField &> Fields;
		TView<const FProperty &> Properties;

		TView<const FClass &> BaseTypes;
		mutable TList<const FClass *> DerivedTypes;

	public:
		static FClass None;
	};

	template<typename InstanceT, typename... ArgTs>
	bool TConstructor<InstanceT, ArgTs...>::IsInvocable(TView<const FType *> ParameterTypes) const
	{
		if constexpr (sizeof...(ArgTs) == 0)
			return ParameterTypes.Size == 0;
		else
		{
			static const FTypeid Typeids[sizeof...(ArgTs)] = { FTypeid::FromType<ArgTs>()... };
			if (ParameterTypes.Size != sizeof...(ArgTs))
				return false;

			for (sizet Index = 0; Index < sizeof...(ArgTs); ++Index)
			{
				if (Typeids[Index] != ParameterTypes[Index]->Typeid)
					return false;
			}
			return true;
		}
	}

	template<typename InstanceT, typename... ArgTs>
	const TConstructor<InstanceT, ArgTs...> & FClass::FindConstructorT() const
	{
		if constexpr (sizeof...(ArgTs))
		{
			const FType * ParameterTypes[sizeof...(ArgTs)] = { &Typeof<ArgTs>()... };
			const FConstructor & Constructor = FindConstructor(ParameterTypes);
			if (Constructor.Valid())
				return StaticCast<TConstructor<InstanceT, ArgTs...>>(Constructor);
			return TConstructor<InstanceT, ArgTs...>::None;
		}
		else
		{
			const FConstructor & Constructor = FindConstructor();
			if (Constructor.Valid())
				return StaticCast<TConstructor<InstanceT>>(Constructor);
			return TConstructor<InstanceT>::None;
		}
	}

	template<typename T>
	struct TClass : FClass
	{
	public:
		TClass() = default;
		TClass(FName Name, FClass & Parent = FClass::None, const FMetadata & Metadata = FMetadata::None) : FClass(Name, typeid(T), Parent, Metadata) {}

	public:
		template<FunctionC FunctionT>
		const TMethod<T, FunctionT> & FindMethodT(FName Name) const
		{
			const FMethod & Function = FClass::FindMethod(Name);
			if (!Function)
				return TMethod<T, FunctionT>::None;

			return StaticCast<TMethod<T, FunctionT>>(Function);
		}

		template<typename ValueT>
		const TInstanceProperty<T, ValueT> & FindPropertyT(FName Name) const
		{
			const FProperty & Property = FClass::FindProperty(Name);
			if (!Property)
				return TInstanceProperty<T, ValueT>::None;

			return StaticCast<TInstanceProperty<T, ValueT>>(Property);
		}
	};

	template<ClassC T>
	FClass & Classof() { return static_cast<FClass &>(Typeof<T>()); }

	template<ClassC T>
	TClass<T> & TClassof() { return static_cast<TClass<T> &>(Typeof<T>()); }
}

#if !XIN_REFLECT
#include "__Types/Basic/Strings.inl"
#include "__Types/Basic/Thickness.inl"
#include "__Types/Basic/Vec2.inl"
#include "__Types/Basic/Vec3.inl"
#include "__Types/Basic/Vec4.inl"
#endif
