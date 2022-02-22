// EEF11F01FBD9D26173DC9320729D3D4A 2025-03-24 18:06:26
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/OverlapPanel.h"

namespace Xin
{
	class UOverlapPanel_Type : public TClass<UI::UOverlapPanel>
	{
	public:
		using UOverlapPanel = UI::UOverlapPanel;

		// --- 0 Constructors
		static inline const TConstructor<UOverlapPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UOverlapPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UOverlapPanel, FType &()> Method { u8"Type"N, &UOverlapPanel::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UOverlapPanel, void()> Method { u8"OnInitialize"N, &UOverlapPanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UOverlapPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UOverlapPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UOverlapPanel, void()> Method { u8"OnArrange"N, &UOverlapPanel::OnArrange, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___ElementHorizontalAlignment
		{
			static inline const TDependencyProperty Property { u8"ElementHorizontalAlignment"N, &UOverlapPanel::__ElementHorizontalAlignment, &UOverlapPanel::__GetElementHorizontalAlignment, &UOverlapPanel::__SetElementHorizontalAlignmentValue, UOverlapPanel::__ElementHorizontalAlignment__, UOverlapPanel::FPropertyMetadata::None };
		};
		struct Property___ElementVerticalAlignment
		{
			static inline const TDependencyProperty Property { u8"ElementVerticalAlignment"N, &UOverlapPanel::__ElementVerticalAlignment, &UOverlapPanel::__GetElementVerticalAlignment, &UOverlapPanel::__SetElementVerticalAlignmentValue, UOverlapPanel::__ElementVerticalAlignment__, UOverlapPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ElementHorizontalAlignment::Property,
			Property___ElementVerticalAlignment::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPanel>(),
		};

		// --- 5 Constructor
		UOverlapPanel_Type(FName Name = u8"UOverlapPanel"N) : TClass(Name, Classof<UI::UPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EElementAlignment> & UI::UOverlapPanel::ElementHorizontalAlignmentProperty() { return UOverlapPanel_Type::Property___ElementHorizontalAlignment::Property; }

	const TProperty<UI::EElementAlignment> & UI::UOverlapPanel::ElementVerticalAlignmentProperty() { return UOverlapPanel_Type::Property___ElementVerticalAlignment::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UOverlapPanel>
	{
		static inline UOverlapPanel_Type UOverlapPanelType { u8"UOverlapPanel"N };

		static FType & Type()
		{
			return UOverlapPanelType;
		}
	};

	FType & UI::UOverlapPanel::StaticType()
	{
		return TStaticType<UI::UOverlapPanel>().Type();
	}

	static TTypeRegister<UI::UOverlapPanel> UOverlapPanelTypeRegister { TStaticType<UI::UOverlapPanel>().Type() };
}

// Generated code end.
