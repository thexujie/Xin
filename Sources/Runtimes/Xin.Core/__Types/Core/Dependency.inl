// 56B184E1C7019F7FEA567B261ED248A4 2025-03-10 18:02:43
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Dependency.h"

namespace Xin
{
	class IDependency_Type : public TClass<IDependency>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<IDependency> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<IDependency, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IDependency, FType &()> Method { u8"Type"N, &IDependency::Type, FMetadata::None };
		};
		struct Method___void___OnPropertyChanging__FProperty_const_lref
		{
			static inline const TMethod<IDependency, void(FProperty const &)> Method { u8"OnPropertyChanging"N, &IDependency::OnPropertyChanging, FMetadata::None };
		};
		struct Method___void___OnPropertyChanged__FProperty_const_lref
		{
			static inline const TMethod<IDependency, void(FProperty const &)> Method { u8"OnPropertyChanged"N, &IDependency::OnPropertyChanged, FMetadata::None };
		};
		struct Method___void___OnChanged__FProperty_const_lref
		{
			static inline const TMethod<IDependency, void(FProperty const &)> Method { u8"OnChanged"N, &IDependency::OnChanged, FMetadata::None };
		};
		struct Method___void___ResetValues__EValueLayer
		{
			static inline const TMethod<IDependency, void(EValueLayer)> Method { u8"ResetValues"N, &IDependency::ResetValues, FMetadata::None };
		};
		struct Method___IPropertyValue_lref___FindPropertyValue__FProperty_const_lref____const
		{
			static inline const TMethod<IDependency, IPropertyValue &(FProperty const &)> Method { u8"FindPropertyValue"N, &IDependency::FindPropertyValue, FMetadata::None };
		};
		struct Method___IPropertyValue_lref___FetchPropertyValue__FProperty_const_lref____const
		{
			static inline const TMethod<IDependency, IPropertyValue &(FProperty const &)> Method { u8"FetchPropertyValue"N, &IDependency::FetchPropertyValue, FMetadata::None };
		};
		struct Method___FVariant___GetPropertyValue__FProperty_const_lref____const
		{
			static inline const TMethod<IDependency, FVariant(FProperty const &)> Method { u8"GetPropertyValue"N, &IDependency::GetPropertyValue, FMetadata::None };
		};
		struct Method___void___SetPropertyValue__FProperty_const_lref__FVariant_const_lref
		{
			static inline const TMethod<IDependency, void(FProperty const &, FVariant const &)> Method { u8"SetPropertyValue"N, &IDependency::SetPropertyValue, FMetadata::None };
		};
		struct Method___void___ResetValue__FProperty_const_lref__EValueLayer
		{
			static inline const TMethod<IDependency, void(FProperty const &, EValueLayer)> Method { u8"ResetValue"N, &IDependency::ResetValue, FMetadata::None };
		};
		struct Method___FMetadata_const_lref___FetchMetadata__FProperty_const_lref
		{
			static inline const TMethod<IDependency, FMetadata const &(FProperty const &)> Method { u8"FetchMetadata"N, &IDependency::FetchMetadata, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnPropertyChanging__FProperty_const_lref::Method,
			Method___void___OnPropertyChanged__FProperty_const_lref::Method,
			Method___void___OnChanged__FProperty_const_lref::Method,
			Method___void___ResetValues__EValueLayer::Method,
			Method___IPropertyValue_lref___FindPropertyValue__FProperty_const_lref____const::Method,
			Method___IPropertyValue_lref___FetchPropertyValue__FProperty_const_lref____const::Method,
			Method___FVariant___GetPropertyValue__FProperty_const_lref____const::Method,
			Method___void___SetPropertyValue__FProperty_const_lref__FVariant_const_lref::Method,
			Method___void___ResetValue__FProperty_const_lref__EValueLayer::Method,
			Method___FMetadata_const_lref___FetchMetadata__FProperty_const_lref::Method,
		};

		// --- 2 Fields
		struct Field___PropertyValues
		{
			static inline const TField Field { u8"PropertyValues"N, &IDependency::PropertyValues, FMetadata::None };
		};
		struct Field___OverridenMetadatas
		{
			static inline const TField Field { u8"OverridenMetadatas"N, &IDependency::OverridenMetadatas, FMetadata::None };
		};
		struct Field___Bindings
		{
			static inline const TField Field { u8"Bindings"N, &IDependency::Bindings, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___PropertyValues::Field,
			Field___OverridenMetadatas::Field,
			Field___Bindings::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IObject>(),
		};

		// --- 5 Constructor
		IDependency_Type(FName Name = u8"IDependency"N) : TClass(Name, Classof<IObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<IDependency>
	{
		static inline IDependency_Type IDependencyType { u8"IDependency"N };

		static FType & Type()
		{
			return IDependencyType;
		}
	};

	FType & IDependency::StaticType()
	{
		return TStaticType<IDependency>().Type();
	}

	static TTypeRegister<IDependency> IDependencyTypeRegister { TStaticType<IDependency>().Type() };
}

// Generated code end.
