// FD7FB0FC1230007CCC729F318C3A39D2 2025-03-24 18:06:37
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ScrollBar.h"

namespace Xin
{
	class UScrollBar_Type : public TClass<UI::UScrollBar>
	{
	public:
		using UScrollBar = UI::UScrollBar;

		// --- 0 Constructors
		static inline const TConstructor<UScrollBar> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UScrollBar, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UScrollBar, FType &()> Method { u8"Type"N, &UScrollBar::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UScrollBar, void()> Method { u8"OnConstruct"N, &UScrollBar::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UScrollBar, void()> Method { u8"OnInitialize"N, &UScrollBar::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UScrollBar, FSize(FSize)> Method { u8"OnMeasure"N, &UScrollBar::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UScrollBar, void()> Method { u8"OnArrange"N, &UScrollBar::OnArrange, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UScrollBar, void(IPainter &)> Method { u8"OnPaint"N, &UScrollBar::OnPaint, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UScrollBar::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UScrollBar::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UScrollBar::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UScrollBar::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnLineUpCommand__FRoutedEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FRoutedEventArgs &)> Method { u8"OnLineUpCommand"N, &UScrollBar::OnLineUpCommand, FMetadata::None };
		};
		struct Method___void___OnLineDownCommand__FRoutedEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FRoutedEventArgs &)> Method { u8"OnLineDownCommand"N, &UScrollBar::OnLineDownCommand, FMetadata::None };
		};
		struct Method___void___OnPageUpCommand__FRoutedEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FRoutedEventArgs &)> Method { u8"OnPageUpCommand"N, &UScrollBar::OnPageUpCommand, FMetadata::None };
		};
		struct Method___void___OnPageDownCommand__FRoutedEventArgs_lref
		{
			static inline const TMethod<UScrollBar, void(UI::FRoutedEventArgs &)> Method { u8"OnPageDownCommand"N, &UScrollBar::OnPageDownCommand, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnLineUpCommand__FRoutedEventArgs_lref::Method,
			Method___void___OnLineDownCommand__FRoutedEventArgs_lref::Method,
			Method___void___OnPageUpCommand__FRoutedEventArgs_lref::Method,
			Method___void___OnPageDownCommand__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___LineUp
		{
			static inline const TField Field { u8"LineUp"N, &UScrollBar::LineUp, FMetadata::None };
		};
		struct Field___LineDown
		{
			static inline const TField Field { u8"LineDown"N, &UScrollBar::LineDown, FMetadata::None };
		};
		struct Field___PageUp
		{
			static inline const TField Field { u8"PageUp"N, &UScrollBar::PageUp, FMetadata::None };
		};
		struct Field___PageDown
		{
			static inline const TField Field { u8"PageDown"N, &UScrollBar::PageDown, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___LineUp::Field,
			Field___LineDown::Field,
			Field___PageUp::Field,
			Field___PageDown::Field,
		};

		// --- 3 Properties
		struct Property___Orientation
		{
			static inline const TDependencyProperty Property { u8"Orientation"N, &UScrollBar::__Orientation, &UScrollBar::__GetOrientation, &UScrollBar::__SetOrientationValue, UScrollBar::__Orientation__, UScrollBar::FPropertyMetadata::None };
		};
		struct Property___ViewportSize
		{
			static inline const TDependencyProperty Property { u8"ViewportSize"N, &UScrollBar::__ViewportSize, &UScrollBar::__GetViewportSize, &UScrollBar::__SetViewportSizeValue, UScrollBar::__ViewportSize__, UScrollBar::FPropertyMetadata::None };
		};
		struct Property___Maximum
		{
			static inline const TDependencyProperty Property { u8"Maximum"N, &UScrollBar::__Maximum, &UScrollBar::__GetMaximum, &UScrollBar::__SetMaximumValue, UScrollBar::__Maximum__, UScrollBar::FPropertyMetadata::None };
		};
		struct Property___Minimum
		{
			static inline const TDependencyProperty Property { u8"Minimum"N, &UScrollBar::__Minimum, &UScrollBar::__GetMinimum, &UScrollBar::__SetMinimumValue, UScrollBar::__Minimum__, UScrollBar::FPropertyMetadata::None };
		};
		struct Property___Value
		{
			static inline const TDependencyProperty Property { u8"Value"N, &UScrollBar::__Value, &UScrollBar::__GetValue, &UScrollBar::__SetValueValue, UScrollBar::__Value__, UScrollBar::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Orientation::Property,
			Property___ViewportSize::Property,
			Property___Maximum::Property,
			Property___Minimum::Property,
			Property___Value::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UScrollBar_Type(FName Name = u8"UScrollBar"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EOrientation> & UI::UScrollBar::OrientationProperty() { return UScrollBar_Type::Property___Orientation::Property; }

	const TProperty<dimenx> & UI::UScrollBar::ViewportSizeProperty() { return UScrollBar_Type::Property___ViewportSize::Property; }

	const TProperty<dimenx> & UI::UScrollBar::MaximumProperty() { return UScrollBar_Type::Property___Maximum::Property; }

	const TProperty<dimenx> & UI::UScrollBar::MinimumProperty() { return UScrollBar_Type::Property___Minimum::Property; }

	const TProperty<dimenx> & UI::UScrollBar::ValueProperty() { return UScrollBar_Type::Property___Value::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UScrollBar>
	{
		static inline UScrollBar_Type UScrollBarType { u8"UScrollBar"N };

		static FType & Type()
		{
			return UScrollBarType;
		}
	};

	FType & UI::UScrollBar::StaticType()
	{
		return TStaticType<UI::UScrollBar>().Type();
	}

	static TTypeRegister<UI::UScrollBar> UScrollBarTypeRegister { TStaticType<UI::UScrollBar>().Type() };
}

// Generated code end.
