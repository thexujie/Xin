// 10C44B194C4F3FE379FE283BB493AB34 2025-03-24 18:06:56
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/WrapPanel.h"

namespace Xin
{
	class UWrapPanel_Type : public TClass<UI::UWrapPanel>
	{
	public:
		using UWrapPanel = UI::UWrapPanel;

		// --- 0 Constructors
		static inline const TConstructor<UWrapPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UWrapPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UWrapPanel, FType &()> Method { u8"Type"N, &UWrapPanel::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UWrapPanel, void()> Method { u8"OnInitialize"N, &UWrapPanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UWrapPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UWrapPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UWrapPanel, void()> Method { u8"OnArrange"N, &UWrapPanel::OnArrange, FMetadata::None };
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
		struct Property___Orientation
		{
			static inline const TDependencyProperty Property { u8"Orientation"N, &UWrapPanel::__Orientation, &UWrapPanel::__GetOrientation, &UWrapPanel::__SetOrientationValue, UWrapPanel::__Orientation__, UWrapPanel::FPropertyMetadata::None };
		};
		struct Property___ElementAlignment
		{
			struct FPropertyMetadata : UWrapPanel::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ElementAlignment"N, &UWrapPanel::__ElementAlignment, &UWrapPanel::__GetElementAlignment, &UWrapPanel::__SetElementAlignmentValue, UWrapPanel::__ElementAlignment__, Metadata };
		};
		struct Property___ItemSize
		{
			static inline const TDependencyProperty Property { u8"ItemSize"N, &UWrapPanel::__ItemSize, &UWrapPanel::__GetItemSize, &UWrapPanel::__SetItemSizeValue, UWrapPanel::__ItemSize__, UWrapPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Orientation::Property,
			Property___ElementAlignment::Property,
			Property___ItemSize::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPanel>(),
		};

		// --- 5 Constructor
		UWrapPanel_Type(FName Name = u8"UWrapPanel"N) : TClass(Name, Classof<UI::UPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EOrientation> & UI::UWrapPanel::OrientationProperty() { return UWrapPanel_Type::Property___Orientation::Property; }

	const TProperty<UI::EElementAlignment> & UI::UWrapPanel::ElementAlignmentProperty() { return UWrapPanel_Type::Property___ElementAlignment::Property; }

	const TProperty<UI::FDimen2> & UI::UWrapPanel::ItemSizeProperty() { return UWrapPanel_Type::Property___ItemSize::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UWrapPanel>
	{
		static inline UWrapPanel_Type UWrapPanelType { u8"UWrapPanel"N };

		static FType & Type()
		{
			return UWrapPanelType;
		}
	};

	FType & UI::UWrapPanel::StaticType()
	{
		return TStaticType<UI::UWrapPanel>().Type();
	}

	static TTypeRegister<UI::UWrapPanel> UWrapPanelTypeRegister { TStaticType<UI::UWrapPanel>().Type() };
}

// Generated code end.
