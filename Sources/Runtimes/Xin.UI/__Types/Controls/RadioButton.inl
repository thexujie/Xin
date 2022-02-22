// BFB92B49E9BC1C2EDC9D2C72FA34FADE 2025-03-06 15:48:48
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/RadioButton.h"

namespace Xin
{
	class URadioButton_Type : public TClass<UI::URadioButton>
	{
	public:
		using URadioButton = UI::URadioButton;

		// --- 0 Constructors
		static inline const TConstructor<URadioButton> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<URadioButton, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<URadioButton, FType &()> Method { u8"Type"N, &URadioButton::Type, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<URadioButton, void()> Method { u8"OnInitialized"N, &URadioButton::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalized
		{
			static inline const TMethod<URadioButton, void()> Method { u8"OnFinalized"N, &URadioButton::OnFinalized, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<URadioButton, void(IPainter &)> Method { u8"OnPaint"N, &URadioButton::OnPaint, FMetadata::None };
		};
		struct Method___void___OnToggle
		{
			static inline const TMethod<URadioButton, void()> Method { u8"OnToggle"N, &URadioButton::OnToggle, FMetadata::None };
		};
		struct Method___void___OnCheck__FRoutedEventArgs_lref
		{
			static inline const TMethod<URadioButton, void(UI::FRoutedEventArgs &)> Method { u8"OnCheck"N, &URadioButton::OnCheck, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalized::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___OnToggle::Method,
			Method___void___OnCheck__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___GroupName
		{
			static inline const TDependencyProperty Property { u8"GroupName"N, &URadioButton::__GroupName, &URadioButton::__GetGroupName, &URadioButton::__SetGroupNameValue, URadioButton::__GroupName__, URadioButton::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___GroupName::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UToggleButton>(),
		};

		// --- 5 Constructor
		URadioButton_Type(FName Name = u8"URadioButton"N) : TClass(Name, Classof<UI::UToggleButton>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FName> & UI::URadioButton::GroupNameProperty() { return URadioButton_Type::Property___GroupName::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::URadioButton>
	{
		static inline URadioButton_Type URadioButtonType { u8"URadioButton"N };

		static FType & Type()
		{
			return URadioButtonType;
		}
	};

	FType & UI::URadioButton::StaticType()
	{
		return TStaticType<UI::URadioButton>().Type();
	}

	static TTypeRegister<UI::URadioButton> URadioButtonTypeRegister { TStaticType<UI::URadioButton>().Type() };
}

// Generated code end.
