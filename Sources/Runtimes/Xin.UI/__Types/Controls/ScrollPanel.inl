// E79FB6AB91BE582C411F096E6F731AC1 2025-03-24 18:06:39
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ScrollPanel.h"

namespace Xin
{
	class UScrollPanel_Type : public TClass<UI::UScrollPanel>
	{
	public:
		using UScrollPanel = UI::UScrollPanel;

		// --- 0 Constructors
		static inline const TConstructor<UScrollPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UScrollPanel, FType &()> Method { u8"Type"N, &UScrollPanel::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"OnInitialize"N, &UScrollPanel::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"OnInitialized"N, &UScrollPanel::OnInitialized, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UScrollPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UScrollPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"OnArrange"N, &UScrollPanel::OnArrange, FMetadata::None };
		};
		struct Method___void___OnArranged
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"OnArranged"N, &UScrollPanel::OnArranged, FMetadata::None };
		};
		struct Method___void___UpdateScrollInfo
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"UpdateScrollInfo"N, &UScrollPanel::UpdateScrollInfo, FMetadata::None };
		};
		struct Method___void___UpdateScrollBars
		{
			static inline const TMethod<UScrollPanel, void()> Method { u8"UpdateScrollBars"N, &UScrollPanel::UpdateScrollBars, FMetadata::None };
		};
		struct Method___bool___IsHorizontalScrollBarVisible____const
		{
			static inline const TMethod<UScrollPanel, bool()> Method { u8"IsHorizontalScrollBarVisible"N, &UScrollPanel::IsHorizontalScrollBarVisible, FMetadata::None };
		};
		struct Method___bool___IsVerticalScrollBarVisible____const
		{
			static inline const TMethod<UScrollPanel, bool()> Method { u8"IsVerticalScrollBarVisible"N, &UScrollPanel::IsVerticalScrollBarVisible, FMetadata::None };
		};
		struct Method___void___OnChildChanged__TReferPtr__UElement__
		{
			static inline const TMethod<UScrollPanel, void(UI::UElementRef)> Method { u8"OnChildChanged"N, &UScrollPanel::OnChildChanged, FMetadata::None };
		};
		struct Method___void___OnScrollOffsetChanged__TVec2__float64__
		{
			static inline const TMethod<UScrollPanel, void(FVec2)> Method { u8"OnScrollOffsetChanged"N, &UScrollPanel::OnScrollOffsetChanged, FMetadata::None };
		};
		struct Method___void___OnHorizontalScrollBarChanged__TReferPtr__UScrollBar__
		{
			static inline const TMethod<UScrollPanel, void(UI::UScrollBarRef)> Method { u8"OnHorizontalScrollBarChanged"N, &UScrollPanel::OnHorizontalScrollBarChanged, FMetadata::None };
		};
		struct Method___void___OnVerticalScrollBarChanged__TReferPtr__UScrollBar__
		{
			static inline const TMethod<UScrollPanel, void(UI::UScrollBarRef)> Method { u8"OnVerticalScrollBarChanged"N, &UScrollPanel::OnVerticalScrollBarChanged, FMetadata::None };
		};
		struct Method___void___OnConnerElementChanged__TReferPtr__UElement__
		{
			static inline const TMethod<UScrollPanel, void(UI::UElementRef)> Method { u8"OnConnerElementChanged"N, &UScrollPanel::OnConnerElementChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnArranged::Method,
			Method___void___UpdateScrollInfo::Method,
			Method___void___UpdateScrollBars::Method,
			Method___bool___IsHorizontalScrollBarVisible____const::Method,
			Method___bool___IsVerticalScrollBarVisible____const::Method,
			Method___void___OnChildChanged__TReferPtr__UElement__::Method,
			Method___void___OnScrollOffsetChanged__TVec2__float64__::Method,
			Method___void___OnHorizontalScrollBarChanged__TReferPtr__UScrollBar__::Method,
			Method___void___OnVerticalScrollBarChanged__TReferPtr__UScrollBar__::Method,
			Method___void___OnConnerElementChanged__TReferPtr__UElement__::Method,
		};

		// --- 2 Fields
		struct Field___ScrollInfo
		{
			static inline const TField Field { u8"ScrollInfo"N, &UScrollPanel::ScrollInfo, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ScrollInfo::Field,
		};

		// --- 3 Properties
		struct Property___Child
		{
			static inline const TProxyProperty Property { u8"Child"N, &UScrollPanel::__Child, &UScrollPanel::__GetChild, &UScrollPanel::__SetChild, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___ViewportSize
		{
			static inline const TDependencyProperty Property { u8"ViewportSize"N, &UScrollPanel::__ViewportSize, &UScrollPanel::__GetViewportSize, &UScrollPanel::__SetViewportSizeValue, UScrollPanel::__ViewportSize__, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___ScrollExtent
		{
			static inline const TDependencyProperty Property { u8"ScrollExtent"N, &UScrollPanel::__ScrollExtent, &UScrollPanel::__GetScrollExtent, &UScrollPanel::__SetScrollExtentValue, UScrollPanel::__ScrollExtent__, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___ScrollOffset
		{
			static inline const TDependencyProperty Property { u8"ScrollOffset"N, &UScrollPanel::__ScrollOffset, &UScrollPanel::__GetScrollOffset, &UScrollPanel::__SetScrollOffsetValue, UScrollPanel::__ScrollOffset__, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___HorizontalScrollBarVisibility
		{
			static inline const TProxyProperty Property { u8"HorizontalScrollBarVisibility"N, &UScrollPanel::__HorizontalScrollBarVisibility, &UScrollPanel::__GetHorizontalScrollBarVisibility, &UScrollPanel::__SetHorizontalScrollBarVisibility, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___VerticalScrollBarVisibility
		{
			static inline const TProxyProperty Property { u8"VerticalScrollBarVisibility"N, &UScrollPanel::__VerticalScrollBarVisibility, &UScrollPanel::__GetVerticalScrollBarVisibility, &UScrollPanel::__SetVerticalScrollBarVisibility, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___HorizontalScrollBar
		{
			static inline const TProxyProperty Property { u8"HorizontalScrollBar"N, &UScrollPanel::__HorizontalScrollBar, &UScrollPanel::__GetHorizontalScrollBar, &UScrollPanel::__SetHorizontalScrollBar, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___VerticalScrollBar
		{
			static inline const TProxyProperty Property { u8"VerticalScrollBar"N, &UScrollPanel::__VerticalScrollBar, &UScrollPanel::__GetVerticalScrollBar, &UScrollPanel::__SetVerticalScrollBar, UScrollPanel::FPropertyMetadata::None };
		};
		struct Property___ConnerElement
		{
			static inline const TProxyProperty Property { u8"ConnerElement"N, &UScrollPanel::__ConnerElement, &UScrollPanel::__GetConnerElement, &UScrollPanel::__SetConnerElement, UScrollPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Child::Property,
			Property___ViewportSize::Property,
			Property___ScrollExtent::Property,
			Property___ScrollOffset::Property,
			Property___HorizontalScrollBarVisibility::Property,
			Property___VerticalScrollBarVisibility::Property,
			Property___HorizontalScrollBar::Property,
			Property___VerticalScrollBar::Property,
			Property___ConnerElement::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UScrollPanel_Type(FName Name = u8"UScrollPanel"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::UElementRef> & UI::UScrollPanel::ChildProperty() { return UScrollPanel_Type::Property___Child::Property; }

	const TProperty<FSize> & UI::UScrollPanel::ViewportSizeProperty() { return UScrollPanel_Type::Property___ViewportSize::Property; }

	const TProperty<FVec2> & UI::UScrollPanel::ScrollExtentProperty() { return UScrollPanel_Type::Property___ScrollExtent::Property; }

	const TProperty<FVec2> & UI::UScrollPanel::ScrollOffsetProperty() { return UScrollPanel_Type::Property___ScrollOffset::Property; }

	const TProperty<TOptional<UI::EVisibility>> & UI::UScrollPanel::HorizontalScrollBarVisibilityProperty() { return UScrollPanel_Type::Property___HorizontalScrollBarVisibility::Property; }

	const TProperty<TOptional<UI::EVisibility>> & UI::UScrollPanel::VerticalScrollBarVisibilityProperty() { return UScrollPanel_Type::Property___VerticalScrollBarVisibility::Property; }

	const TProperty<UI::UScrollBarRef> & UI::UScrollPanel::HorizontalScrollBarProperty() { return UScrollPanel_Type::Property___HorizontalScrollBar::Property; }

	const TProperty<UI::UScrollBarRef> & UI::UScrollPanel::VerticalScrollBarProperty() { return UScrollPanel_Type::Property___VerticalScrollBar::Property; }

	const TProperty<UI::UElementRef> & UI::UScrollPanel::ConnerElementProperty() { return UScrollPanel_Type::Property___ConnerElement::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UScrollPanel>
	{
		static inline UScrollPanel_Type UScrollPanelType { u8"UScrollPanel"N };

		static FType & Type()
		{
			return UScrollPanelType;
		}
	};

	FType & UI::UScrollPanel::StaticType()
	{
		return TStaticType<UI::UScrollPanel>().Type();
	}

	static TTypeRegister<UI::UScrollPanel> UScrollPanelTypeRegister { TStaticType<UI::UScrollPanel>().Type() };
}

// Generated code end.
