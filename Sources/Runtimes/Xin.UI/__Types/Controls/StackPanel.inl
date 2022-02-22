// C08260FF77010FAD2572EE59B32300D5 2025-03-24 18:06:44
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/StackPanel.h"

namespace Xin
{
	class UStackPanel_Type : public TClass<UI::UStackPanel>
	{
	public:
		using UStackPanel = UI::UStackPanel;

		// --- 0 Constructors
		static inline const TConstructor<UStackPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UStackPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UStackPanel, FType &()> Method { u8"Type"N, &UStackPanel::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UStackPanel, void()> Method { u8"OnInitialize"N, &UStackPanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UStackPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UStackPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UStackPanel, void()> Method { u8"OnArrange"N, &UStackPanel::OnArrange, FMetadata::None };
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
			static inline const TDependencyProperty Property { u8"Orientation"N, &UStackPanel::__Orientation, &UStackPanel::__GetOrientation, &UStackPanel::__SetOrientationValue, UStackPanel::__Orientation__, UStackPanel::FPropertyMetadata::None };
		};
		struct Property___ScrollOffset
		{
			struct FPropertyMetadata : UStackPanel::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"ScrollOffset"N, &UStackPanel::__ScrollOffset, &UStackPanel::__GetScrollOffset, &UStackPanel::__SetScrollOffset, Metadata };
		};
		struct Property___ElementAlignment
		{
			struct FPropertyMetadata : UStackPanel::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ElementAlignment"N, &UStackPanel::__ElementAlignment, &UStackPanel::__GetElementAlignment, &UStackPanel::__SetElementAlignmentValue, UStackPanel::__ElementAlignment__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Orientation::Property,
			Property___ScrollOffset::Property,
			Property___ElementAlignment::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPanel>(),
		};

		// --- 5 Constructor
		UStackPanel_Type(FName Name = u8"UStackPanel"N) : TClass(Name, Classof<UI::UPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EOrientation> & UI::UStackPanel::OrientationProperty() { return UStackPanel_Type::Property___Orientation::Property; }

	const TProperty<FVec2> & UI::UStackPanel::ScrollOffsetProperty() { return UStackPanel_Type::Property___ScrollOffset::Property; }

	const TProperty<UI::EElementAlignment> & UI::UStackPanel::ElementAlignmentProperty() { return UStackPanel_Type::Property___ElementAlignment::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UStackPanel>
	{
		static inline UStackPanel_Type UStackPanelType { u8"UStackPanel"N };

		static FType & Type()
		{
			return UStackPanelType;
		}
	};

	FType & UI::UStackPanel::StaticType()
	{
		return TStaticType<UI::UStackPanel>().Type();
	}

	static TTypeRegister<UI::UStackPanel> UStackPanelTypeRegister { TStaticType<UI::UStackPanel>().Type() };
}

// Generated code end.
