// 2A4E05E3AF9AE5E6DB9B81D71E59D115 2025-03-06 15:49:13
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Visual.h"

namespace Xin
{
	class UVisual_Type : public TClass<UI::UVisual>
	{
	public:
		using UVisual = UI::UVisual;

		// --- 0 Constructors
		static inline const TConstructor<UVisual> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UVisual, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UVisual, FType &()> Method { u8"Type"N, &UVisual::Type, FMetadata::None };
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
			Classof<UI::UObject>(),
		};

		// --- 5 Constructor
		UVisual_Type(FName Name = u8"UVisual"N) : TClass(Name, Classof<UI::UObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UVisual>
	{
		static inline UVisual_Type UVisualType { u8"UVisual"N };

		static FType & Type()
		{
			return UVisualType;
		}
	};

	FType & UI::UVisual::StaticType()
	{
		return TStaticType<UI::UVisual>().Type();
	}

	static TTypeRegister<UI::UVisual> UVisualTypeRegister { TStaticType<UI::UVisual>().Type() };
}

// Generated code end.
