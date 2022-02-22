// 071C2B6047E875FF882ED0E6674F5285 2024-12-25 11:51:06
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Name.h"

namespace Xin
{
	class FName_Type : public TClass<FName>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FName> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FName, FName const &> Constructor___FName_const_lref { u8"!Constructor___FName_const_lref"N, FMetadata::None };
		static inline const TConstructor<FName, char8 const *> Constructor___char8_const_ptr { u8"!Constructor___char8_const_ptr"N, FMetadata::None };
		static inline const TConstructor<FName, FStringV> Constructor___TView__char8__ { u8"!Constructor___TView__char8__"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName_const_lref,
			Constructor___char8_const_ptr,
			Constructor___TView__char8__,
		};

		// --- 1 Methods
		struct Method___TView__char8_____Resolve____const
		{
			static inline const TMethod<FName, FStringV()> Method { u8"Resolve"N, &FName::Resolve, FMetadata::None };
		};
		struct Method___bool___Valid____const
		{
			static inline const TMethod<FName, bool()> Method { u8"Valid"N, &FName::Valid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___TView__char8_____Resolve____const::Method,
			Method___bool___Valid____const::Method,
		};

		// --- 2 Fields
		struct Field___Nameid
		{
			static inline const TField Field { u8"Nameid"N, &FName::Nameid, FMetadata::None };
		};
		struct Field___NameEntry
		{
			static inline const TField Field { u8"NameEntry"N, &FName::NameEntry, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Nameid::Field,
			Field___NameEntry::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FName_Type(FName Name = u8"FName"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FName>
	{
		static FType & Type()
		{
			static FName_Type FNameType { u8"FName"N };
			return FNameType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FName>()
	{
		return TStaticType<FName>().Type();
	}

	static TTypeRegister<FName> FNameTypeRegister { TStaticType<FName>().Type() };
}

// Generated code end.
