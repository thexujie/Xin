// A08142B63409F258E3F5C4BDACB65077 2025-03-06 15:48:15
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Button.h"

namespace Xin
{
	class UButton_Type : public TClass<UI::UButton>
	{
	public:
		using UButton = UI::UButton;

		// --- 0 Constructors
		static inline const TConstructor<UButton> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UButton, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UButton, FType &()> Method { u8"Type"N, &UButton::Type, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UButton, void(IPainter &)> Method { u8"OnPaint"N, &UButton::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UButtonControl>(),
		};

		// --- 5 Constructor
		UButton_Type(FName Name = u8"UButton"N) : TClass(Name, Classof<UI::UButtonControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UButton>
	{
		static inline UButton_Type UButtonType { u8"UButton"N };

		static FType & Type()
		{
			return UButtonType;
		}
	};

	FType & UI::UButton::StaticType()
	{
		return TStaticType<UI::UButton>().Type();
	}

	static TTypeRegister<UI::UButton> UButtonTypeRegister { TStaticType<UI::UButton>().Type() };
}

// Generated code end.
