// 65C5A6FB995B40454298585C326338F0 2025-03-06 15:48:50
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/RepeatButton.h"

namespace Xin
{
	class URepeatButton_Type : public TClass<UI::URepeatButton>
	{
	public:
		using URepeatButton = UI::URepeatButton;

		// --- 0 Constructors
		static inline const TConstructor<URepeatButton> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<URepeatButton, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<URepeatButton, FType &()> Method { u8"Type"N, &URepeatButton::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<URepeatButton, void()> Method { u8"OnConstruct"N, &URepeatButton::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<URepeatButton, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &URepeatButton::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<URepeatButton, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &URepeatButton::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<URepeatButton, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &URepeatButton::OnMouseLeave, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Delay
		{
			static inline const TDependencyProperty Property { u8"Delay"N, &URepeatButton::__Delay, &URepeatButton::__GetDelay, &URepeatButton::__SetDelayValue, URepeatButton::__Delay__, URepeatButton::FPropertyMetadata::None };
		};
		struct Property___Interval
		{
			static inline const TDependencyProperty Property { u8"Interval"N, &URepeatButton::__Interval, &URepeatButton::__GetInterval, &URepeatButton::__SetIntervalValue, URepeatButton::__Interval__, URepeatButton::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Delay::Property,
			Property___Interval::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UButtonControl>(),
		};

		// --- 5 Constructor
		URepeatButton_Type(FName Name = u8"URepeatButton"N) : TClass(Name, Classof<UI::UButtonControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uint32> & UI::URepeatButton::DelayProperty() { return URepeatButton_Type::Property___Delay::Property; }

	const TProperty<uint32> & UI::URepeatButton::IntervalProperty() { return URepeatButton_Type::Property___Interval::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::URepeatButton>
	{
		static inline URepeatButton_Type URepeatButtonType { u8"URepeatButton"N };

		static FType & Type()
		{
			return URepeatButtonType;
		}
	};

	FType & UI::URepeatButton::StaticType()
	{
		return TStaticType<UI::URepeatButton>().Type();
	}

	static TTypeRegister<UI::URepeatButton> URepeatButtonTypeRegister { TStaticType<UI::URepeatButton>().Type() };
}

// Generated code end.
