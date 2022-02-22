// E365C791A790AD65A3A3F606916034B9 2025-03-25 15:49:21
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Control.h"

namespace Xin
{
	class UControl_Type : public TClass<UI::UControl>
	{
	public:
		using UControl = UI::UControl;

		// --- 0 Constructors
		static inline const TConstructor<UControl> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UControl, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UControl, FType &()> Method { u8"Type"N, &UControl::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UControl, void()> Method { u8"OnInitialize"N, &UControl::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UControl, void()> Method { u8"OnFinalize"N, &UControl::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UControl, FSize(FSize)> Method { u8"OnMeasure"N, &UControl::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UControl, void()> Method { u8"OnArrange"N, &UControl::OnArrange, FMetadata::None };
		};
		struct Method___void___OnResourcesChanged
		{
			static inline const TMethod<UControl, void()> Method { u8"OnResourcesChanged"N, &UControl::OnResourcesChanged, FMetadata::None };
		};
		struct Method___void___OnTemplateChanged__TReferPtr__IControlTemplate__
		{
			static inline const TMethod<UControl, void(UI::IControlTemplateRef)> Method { u8"OnTemplateChanged"N, &UControl::OnTemplateChanged, FMetadata::None };
		};
		struct Method___void___OnTemplated
		{
			static inline const TMethod<UControl, void()> Method { u8"OnTemplated"N, &UControl::OnTemplated, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnResourcesChanged::Method,
			Method___void___OnTemplateChanged__TReferPtr__IControlTemplate__::Method,
			Method___void___OnTemplated::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Template
		{
			static inline const TProxyProperty Property { u8"Template"N, &UControl::__Template, &UControl::__GetTemplate, &UControl::__SetTemplate, UControl::FPropertyMetadata::None };
		};
		struct Property___TemplateElement
		{
			static inline const TProxyProperty Property { u8"TemplateElement"N, &UControl::__TemplateElement, &UControl::__GetTemplateElement, &UControl::__SetTemplateElement, UControl::FPropertyMetadata::None };
		};
		struct Property___Foreground
		{
			struct FPropertyMetadata : UControl::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Foreground"N, &UControl::__Foreground, &UControl::__GetForeground, &UControl::__SetForegroundValue, UControl::__Foreground__, Metadata };
		};
		struct Property___Background
		{
			struct FPropertyMetadata : UControl::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Background"N, &UControl::__Background, &UControl::__GetBackground, &UControl::__SetBackgroundValue, UControl::__Background__, Metadata };
		};
		struct Property___BorderStroke
		{
			static inline const TDependencyProperty Property { u8"BorderStroke"N, &UControl::__BorderStroke, &UControl::__GetBorderStroke, &UControl::__SetBorderStrokeValue, UControl::__BorderStroke__, UControl::FPropertyMetadata::None };
		};
		struct Property___BorderThickness
		{
			static inline const TDependencyProperty Property { u8"BorderThickness"N, &UControl::__BorderThickness, &UControl::__GetBorderThickness, &UControl::__SetBorderThicknessValue, UControl::__BorderThickness__, UControl::FPropertyMetadata::None };
		};
		struct Property___Padding
		{
			static inline const TDependencyProperty Property { u8"Padding"N, &UControl::__Padding, &UControl::__GetPadding, &UControl::__SetPaddingValue, UControl::__Padding__, UControl::FPropertyMetadata::None };
		};
		struct Property___ContentHorizontalAlignment
		{
			static inline const TDependencyProperty Property { u8"ContentHorizontalAlignment"N, &UControl::__ContentHorizontalAlignment, &UControl::__GetContentHorizontalAlignment, &UControl::__SetContentHorizontalAlignmentValue, UControl::__ContentHorizontalAlignment__, UControl::FPropertyMetadata::None };
		};
		struct Property___ContentVerticalAlignment
		{
			static inline const TDependencyProperty Property { u8"ContentVerticalAlignment"N, &UControl::__ContentVerticalAlignment, &UControl::__GetContentVerticalAlignment, &UControl::__SetContentVerticalAlignmentValue, UControl::__ContentVerticalAlignment__, UControl::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Template::Property,
			Property___TemplateElement::Property,
			Property___Foreground::Property,
			Property___Background::Property,
			Property___BorderStroke::Property,
			Property___BorderThickness::Property,
			Property___Padding::Property,
			Property___ContentHorizontalAlignment::Property,
			Property___ContentVerticalAlignment::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UControl_Type(FName Name = u8"UControl"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::IControlTemplateRef> & UI::UControl::TemplateProperty() { return UControl_Type::Property___Template::Property; }

	const TProperty<UI::UElementRef> & UI::UControl::TemplateElementProperty() { return UControl_Type::Property___TemplateElement::Property; }

	const TProperty<FColor> & UI::UControl::ForegroundProperty() { return UControl_Type::Property___Foreground::Property; }

	const TProperty<IBrushRef> & UI::UControl::BackgroundProperty() { return UControl_Type::Property___Background::Property; }

	const TProperty<IBrushRef> & UI::UControl::BorderStrokeProperty() { return UControl_Type::Property___BorderStroke::Property; }

	const TProperty<FThickness> & UI::UControl::BorderThicknessProperty() { return UControl_Type::Property___BorderThickness::Property; }

	const TProperty<FThickness> & UI::UControl::PaddingProperty() { return UControl_Type::Property___Padding::Property; }

	const TProperty<UI::EElementAlignment> & UI::UControl::ContentHorizontalAlignmentProperty() { return UControl_Type::Property___ContentHorizontalAlignment::Property; }

	const TProperty<UI::EElementAlignment> & UI::UControl::ContentVerticalAlignmentProperty() { return UControl_Type::Property___ContentVerticalAlignment::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UControl>
	{
		static inline UControl_Type UControlType { u8"UControl"N };

		static FType & Type()
		{
			return UControlType;
		}
	};

	FType & UI::UControl::StaticType()
	{
		return TStaticType<UI::UControl>().Type();
	}

	static TTypeRegister<UI::UControl> UControlTypeRegister { TStaticType<UI::UControl>().Type() };
}

// Generated code end.
