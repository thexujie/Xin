// 4DB2F937F1A6E902346FD08F509C8C17 2025-03-06 15:49:46
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Resource.h"

namespace Xin
{
	class IResource_Type : public TClass<UI::IResource>
	{
	public:
		using IResource = UI::IResource;

		// --- 0 Constructors
		static inline const TConstructor<IResource> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<IResource, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IResource, FType &()> Method { u8"Type"N, &IResource::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::IUObject>(),
		};

		// --- 5 Constructor
		IResource_Type(FName Name = u8"IResource"N) : TClass(Name, Classof<UI::IUObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::IResource>
	{
		static inline IResource_Type IResourceType { u8"IResource"N };

		static FType & Type()
		{
			return IResourceType;
		}
	};

	FType & UI::IResource::StaticType()
	{
		return TStaticType<UI::IResource>().Type();
	}

	static TTypeRegister<UI::IResource> IResourceTypeRegister { TStaticType<UI::IResource>().Type() };
}

// Generated code end.
