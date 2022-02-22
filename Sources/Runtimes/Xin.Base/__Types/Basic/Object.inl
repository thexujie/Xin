// E209E4E38D74324342EAC61701BF2911 2024-12-25 11:51:07
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Object.h"

namespace Xin
{
	class IObject_Type : public TClass<IObject>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<IObject> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<IObject, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IObject, FType &()> Method { u8"Type"N, &IObject::Type, FMetadata::None };
		};
		struct Method___uintx___AddRef
		{
			static inline const TMethod<IObject, uintx()> Method { u8"AddRef"N, &IObject::AddRef, FMetadata::None };
		};
		struct Method___uintx___Release
		{
			static inline const TMethod<IObject, uintx()> Method { u8"Release"N, &IObject::Release, FMetadata::None };
		};
		struct Method___void___Dispose
		{
			static inline const TMethod<IObject, void()> Method { u8"Dispose"N, &IObject::Dispose, FMetadata::None };
		};
		struct Method___IObject_ptr___GetParentObject____const
		{
			static inline const TMethod<IObject, IObject *()> Method { u8"GetParentObject"N, &IObject::GetParentObject, FMetadata::None };
		};
		struct Method___IObject_ptr___GetChildObject__FName____const
		{
			static inline const TMethod<IObject, IObject *(FName)> Method { u8"GetChildObject"N, &IObject::GetChildObject, FMetadata::None };
		};
		struct Method___void___SetName__FName
		{
			static inline const TMethod<IObject, void(FName)> Method { u8"SetName"N, &IObject::SetName, FMetadata::None };
		};
		struct Method___TView__char8_____ToString__TView__char8______const
		{
			static inline const TMethod<IObject, FStringV(FStringV)> Method { u8"ToString"N, &IObject::ToString, FMetadata::None };
		};
		struct Method___TView__char8_____Describe____const
		{
			static inline const TMethod<IObject, FStringV()> Method { u8"Describe"N, &IObject::Describe, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___uintx___AddRef::Method,
			Method___uintx___Release::Method,
			Method___void___Dispose::Method,
			Method___IObject_ptr___GetParentObject____const::Method,
			Method___IObject_ptr___GetChildObject__FName____const::Method,
			Method___void___SetName__FName::Method,
			Method___TView__char8_____ToString__TView__char8______const::Method,
			Method___TView__char8_____Describe____const::Method,
		};

		// --- 2 Fields
		struct Field___Name
		{
			static inline const TField Field { u8"Name"N, &IObject::Name, FMetadata::None };
		};
		struct Field___ObjectEntry
		{
			static inline const TField Field { u8"ObjectEntry"N, &IObject::ObjectEntry, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Name::Field,
			Field___ObjectEntry::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IRefer>(),
		};

		// --- 5 Constructor
		IObject_Type(FName Name = u8"IObject"N) : TClass(Name, Classof<IRefer>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<IObject>
	{
		static FType & Type()
		{
			static IObject_Type IObjectType { u8"IObject"N };
			return IObjectType;
		}
	};

	FType & IObject::StaticType()
	{
		return TStaticType<IObject>().Type();
	}

	static TTypeRegister<IObject> IObjectTypeRegister { TStaticType<IObject>().Type() };
}

// Generated code end.
