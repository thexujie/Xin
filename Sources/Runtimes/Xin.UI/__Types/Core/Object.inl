// 29803639A5C7FEEF647C2B2AD32233DD 2025-03-24 17:06:04
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Object.h"

namespace Xin
{
	class UObject_Type : public TClass<UI::UObject>
	{
	public:
		using UObject = UI::UObject;

		// --- 0 Constructors
		static inline const TConstructor<UObject> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UObject, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UObject, FType &()> Method { u8"Type"N, &UObject::Type, FMetadata::None };
		};
		struct Method___void___Construct
		{
			static inline const TMethod<UObject, void()> Method { u8"Construct"N, &UObject::Construct, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UObject, void()> Method { u8"OnConstruct"N, &UObject::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnConstructed
		{
			static inline const TMethod<UObject, void()> Method { u8"OnConstructed"N, &UObject::OnConstructed, FMetadata::None };
		};
		struct Method___void___Initialize
		{
			static inline const TMethod<UObject, void()> Method { u8"Initialize"N, &UObject::Initialize, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UObject, void()> Method { u8"OnInitialize"N, &UObject::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UObject, void()> Method { u8"OnInitialized"N, &UObject::OnInitialized, FMetadata::None };
		};
		struct Method___void___Finalize
		{
			static inline const TMethod<UObject, void()> Method { u8"Finalize"N, &UObject::Finalize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UObject, void()> Method { u8"OnFinalize"N, &UObject::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnFinalized
		{
			static inline const TMethod<UObject, void()> Method { u8"OnFinalized"N, &UObject::OnFinalized, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___Construct::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnConstructed::Method,
			Method___void___Initialize::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___void___Finalize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnFinalized::Method,
		};

		// --- 2 Fields
		struct Field___Constructing
		{
			static inline const TField Field { u8"Constructing"N, &UObject::Constructing, FMetadata::None };
		};
		struct Field___Constructed
		{
			static inline const TField Field { u8"Constructed"N, &UObject::Constructed, FMetadata::None };
		};
		struct Field___Initializing
		{
			static inline const TField Field { u8"Initializing"N, &UObject::Initializing, FMetadata::None };
		};
		struct Field___Finalizing
		{
			static inline const TField Field { u8"Finalizing"N, &UObject::Finalizing, FMetadata::None };
		};
		struct Field___Initialized
		{
			static inline const TField Field { u8"Initialized"N, &UObject::Initialized, FMetadata::None };
		};
		struct Field___Bindings
		{
			static inline const TField Field { u8"Bindings"N, &UObject::Bindings, FMetadata::None };
		};
		struct Field___Triggers
		{
			static inline const TField Field { u8"Triggers"N, &UObject::Triggers, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Constructing::Field,
			Field___Constructed::Field,
			Field___Initializing::Field,
			Field___Finalizing::Field,
			Field___Initialized::Field,
			Field___Bindings::Field,
			Field___Triggers::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IDependency>(),
		};

		// --- 5 Constructor
		UObject_Type(FName Name = u8"UObject"N) : TClass(Name, Classof<IDependency>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UObject>
	{
		static inline UObject_Type UObjectType { u8"UObject"N };

		static FType & Type()
		{
			return UObjectType;
		}
	};

	FType & UI::UObject::StaticType()
	{
		return TStaticType<UI::UObject>().Type();
	}

	static TTypeRegister<UI::UObject> UObjectTypeRegister { TStaticType<UI::UObject>().Type() };
}

// Generated code end.
