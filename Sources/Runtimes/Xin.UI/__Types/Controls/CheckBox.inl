// 1850DE82943FFA1B38803ACFB0AD342B 2025-03-06 15:48:19
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/CheckBox.h"

namespace Xin
{
	class UCheckBox_Type : public TClass<UI::UCheckBox>
	{
	public:
		using UCheckBox = UI::UCheckBox;

		// --- 0 Constructors
		static inline const TConstructor<UCheckBox> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UCheckBox, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UCheckBox, FType &()> Method { u8"Type"N, &UCheckBox::Type, FMetadata::None };
		};
		struct Method___void___OnToggle
		{
			static inline const TMethod<UCheckBox, void()> Method { u8"OnToggle"N, &UCheckBox::OnToggle, FMetadata::None };
		};
		struct Method___void___OnCheck__FRoutedEventArgs_lref
		{
			static inline const TMethod<UCheckBox, void(UI::FRoutedEventArgs &)> Method { u8"OnCheck"N, &UCheckBox::OnCheck, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnToggle::Method,
			Method___void___OnCheck__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___GroupName
		{
			static inline const TDependencyProperty Property { u8"GroupName"N, &UCheckBox::__GroupName, &UCheckBox::__GetGroupName, &UCheckBox::__SetGroupNameValue, UCheckBox::__GroupName__, UCheckBox::FPropertyMetadata::None };
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
		UCheckBox_Type(FName Name = u8"UCheckBox"N) : TClass(Name, Classof<UI::UToggleButton>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FName> & UI::UCheckBox::GroupNameProperty() { return UCheckBox_Type::Property___GroupName::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UCheckBox>
	{
		static inline UCheckBox_Type UCheckBoxType { u8"UCheckBox"N };

		static FType & Type()
		{
			return UCheckBoxType;
		}
	};

	FType & UI::UCheckBox::StaticType()
	{
		return TStaticType<UI::UCheckBox>().Type();
	}

	static TTypeRegister<UI::UCheckBox> UCheckBoxTypeRegister { TStaticType<UI::UCheckBox>().Type() };
}

// Generated code end.
