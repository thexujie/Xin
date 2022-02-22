// 45AAAB6188E6DB930C0107C8FF5EACBC 2025-03-24 18:06:48
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ToggleButton.h"

namespace Xin
{
	class UToggleButton_Type : public TClass<UI::UToggleButton>
	{
	public:
		using UToggleButton = UI::UToggleButton;

		// --- 0 Constructors
		static inline const TConstructor<UToggleButton> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UToggleButton, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UToggleButton, FType &()> Method { u8"Type"N, &UToggleButton::Type, FMetadata::None };
		};
		struct Method___TView__char8_____Describe____const
		{
			static inline const TMethod<UToggleButton, FStringV()> Method { u8"Describe"N, &UToggleButton::Describe, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UToggleButton, void()> Method { u8"OnConstruct"N, &UToggleButton::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnClick__FRoutedEventArgs_lref
		{
			static inline const TMethod<UToggleButton, void(UI::FRoutedEventArgs &)> Method { u8"OnClick"N, &UToggleButton::OnClick, FMetadata::None };
		};
		struct Method___void___OnToggle
		{
			static inline const TMethod<UToggleButton, void()> Method { u8"OnToggle"N, &UToggleButton::OnToggle, FMetadata::None };
		};
		struct Method___void___OnCheck__FRoutedEventArgs_lref
		{
			static inline const TMethod<UToggleButton, void(UI::FRoutedEventArgs &)> Method { u8"OnCheck"N, &UToggleButton::OnCheck, FMetadata::None };
		};
		struct Method___void___OnUnCheck__FRoutedEventArgs_lref
		{
			static inline const TMethod<UToggleButton, void(UI::FRoutedEventArgs &)> Method { u8"OnUnCheck"N, &UToggleButton::OnUnCheck, FMetadata::None };
		};
		struct Method___void___OnIndeterminate__FRoutedEventArgs_lref
		{
			static inline const TMethod<UToggleButton, void(UI::FRoutedEventArgs &)> Method { u8"OnIndeterminate"N, &UToggleButton::OnIndeterminate, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TView__char8_____Describe____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnClick__FRoutedEventArgs_lref::Method,
			Method___void___OnToggle::Method,
			Method___void___OnCheck__FRoutedEventArgs_lref::Method,
			Method___void___OnUnCheck__FRoutedEventArgs_lref::Method,
			Method___void___OnIndeterminate__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___IsThreeState
		{
			static inline const TDependencyProperty Property { u8"IsThreeState"N, &UToggleButton::__IsThreeState, &UToggleButton::__GetIsThreeState, &UToggleButton::__SetIsThreeStateValue, UToggleButton::__IsThreeState__, UToggleButton::FPropertyMetadata::None };
		};
		struct Property___Checked
		{
			static inline const TDependencyProperty Property { u8"Checked"N, &UToggleButton::__Checked, &UToggleButton::__GetChecked, &UToggleButton::__SetCheckedValue, UToggleButton::__Checked__, UToggleButton::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___IsThreeState::Property,
			Property___Checked::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UButtonControl>(),
		};

		// --- 5 Constructor
		UToggleButton_Type(FName Name = u8"UToggleButton"N) : TClass(Name, Classof<UI::UButtonControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::UToggleButton::IsThreeStateProperty() { return UToggleButton_Type::Property___IsThreeState::Property; }

	const TProperty<TOptional<bool>> & UI::UToggleButton::CheckedProperty() { return UToggleButton_Type::Property___Checked::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UToggleButton>
	{
		static inline UToggleButton_Type UToggleButtonType { u8"UToggleButton"N };

		static FType & Type()
		{
			return UToggleButtonType;
		}
	};

	FType & UI::UToggleButton::StaticType()
	{
		return TStaticType<UI::UToggleButton>().Type();
	}

	static TTypeRegister<UI::UToggleButton> UToggleButtonTypeRegister { TStaticType<UI::UToggleButton>().Type() };
}

// Generated code end.
