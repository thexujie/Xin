// 9B16A6DC54512F0968EE712089C045F3 2024-12-25 11:51:16
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Text.h"

namespace Xin
{
	class FText_Type : public TClass<FText>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FText> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FText, FText const &> Constructor___FText_const_lref { u8"!Constructor___FText_const_lref"N, FMetadata::None };
		static inline const TConstructor<FText, FStringV> Constructor___TView__char8__ { u8"!Constructor___TView__char8__"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FText_const_lref,
			Constructor___TView__char8__,
		};

		// --- 1 Methods
		struct Method___TView__char8_____Resolve____const
		{
			static inline const TMethod<FText, FStringV()> Method { u8"Resolve"N, &FText::Resolve, FMetadata::None };
		};
		struct Method___bool___Valid____const
		{
			static inline const TMethod<FText, bool()> Method { u8"Valid"N, &FText::Valid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___TView__char8_____Resolve____const::Method,
			Method___bool___Valid____const::Method,
		};

		// --- 2 Fields
		struct Field___Nameid
		{
			static inline const TField Field { u8"Nameid"N, &FText::Nameid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Nameid::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FText_Type(FName Name = u8"FText"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FText>
	{
		static FType & Type()
		{
			static FText_Type FTextType { u8"FText"N };
			return FTextType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FText>()
	{
		return TStaticType<FText>().Type();
	}

	static TTypeRegister<FText> FTextTypeRegister { TStaticType<FText>().Type() };
}

// Generated code end.
