// 55805BA65540911A936E729CD119D6E1 2025-03-24 18:06:10
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ChromePanel.h"

namespace Xin
{
	class UChromePanel_Type : public TClass<UI::UChromePanel>
	{
	public:
		using UChromePanel = UI::UChromePanel;

		// --- 0 Constructors
		static inline const TConstructor<UChromePanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UChromePanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UChromePanel, FType &()> Method { u8"Type"N, &UChromePanel::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"OnConstruct"N, &UChromePanel::OnConstruct, FMetadata::None };
		};
		struct Method___void___Initialize
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"Initialize"N, &UChromePanel::Initialize, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"OnInitialize"N, &UChromePanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UChromePanel, FSize(FSize)> Method { u8"OnMeasure"N, &UChromePanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"OnArrange"N, &UChromePanel::OnArrange, FMetadata::None };
		};
		struct Method___UElement_ptr___HitElement__TVec2__float64______const
		{
			static inline const TMethod<UChromePanel, UI::UElement *(FPoint)> Method { u8"HitElement"N, &UChromePanel::HitElement, FMetadata::None };
		};
		struct Method___ECursor___QueryCursor__TVec2__float64______const
		{
			static inline const TMethod<UChromePanel, ECursor(FPoint)> Method { u8"QueryCursor"N, &UChromePanel::QueryCursor, FMetadata::None };
		};
		struct Method___EHitResult___HitTest__TVec2__float64______const
		{
			static inline const TMethod<UChromePanel, EHitResult(FPoint)> Method { u8"HitTest"N, &UChromePanel::HitTest, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChromePanel, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UChromePanel::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChromePanel, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UChromePanel::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseMove__FMouseEventArgs_lref
		{
			static inline const TMethod<UChromePanel, void(UI::FMouseEventArgs &)> Method { u8"OnMouseMove"N, &UChromePanel::OnMouseMove, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UChromePanel, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UChromePanel::OnMouseLeave, FMetadata::None };
		};
		struct Method___TReferPtr__UChrome_____FindChrome__TVec2__float64______const
		{
			static inline const TMethod<UChromePanel, UI::UChromeRef(FPoint)> Method { u8"FindChrome"N, &UChromePanel::FindChrome, FMetadata::None };
		};
		struct Method___void___ShowChromes
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"ShowChromes"N, &UChromePanel::ShowChromes, FMetadata::None };
		};
		struct Method___void___BeginDragChrome__UChrome_lref
		{
			static inline const TMethod<UChromePanel, void(UI::UChrome &)> Method { u8"BeginDragChrome"N, &UChromePanel::BeginDragChrome, FMetadata::None };
		};
		struct Method___void___EndDragChrome
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"EndDragChrome"N, &UChromePanel::EndDragChrome, FMetadata::None };
		};
		struct Method___void___BeginResizeChrome__UChrome_lref__EHitResult
		{
			static inline const TMethod<UChromePanel, void(UI::UChrome &, EHitResult)> Method { u8"BeginResizeChrome"N, &UChromePanel::BeginResizeChrome, FMetadata::None };
		};
		struct Method___void___EndResizeChrome
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"EndResizeChrome"N, &UChromePanel::EndResizeChrome, FMetadata::None };
		};
		struct Method___void___EndAdjustChrome
		{
			static inline const TMethod<UChromePanel, void()> Method { u8"EndAdjustChrome"N, &UChromePanel::EndAdjustChrome, FMetadata::None };
		};
		struct Method___void___CloseChrome__UChrome_lref
		{
			static inline const TMethod<UChromePanel, void(UI::UChrome &)> Method { u8"CloseChrome"N, &UChromePanel::CloseChrome, FMetadata::None };
		};
		struct Method___void___BringToTop__UChrome_lref
		{
			static inline const TMethod<UChromePanel, void(UI::UChrome &)> Method { u8"BringToTop"N, &UChromePanel::BringToTop, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___Initialize::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___UElement_ptr___HitElement__TVec2__float64______const::Method,
			Method___ECursor___QueryCursor__TVec2__float64______const::Method,
			Method___EHitResult___HitTest__TVec2__float64______const::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseMove__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___TReferPtr__UChrome_____FindChrome__TVec2__float64______const::Method,
			Method___void___ShowChromes::Method,
			Method___void___BeginDragChrome__UChrome_lref::Method,
			Method___void___EndDragChrome::Method,
			Method___void___BeginResizeChrome__UChrome_lref__EHitResult::Method,
			Method___void___EndResizeChrome::Method,
			Method___void___EndAdjustChrome::Method,
			Method___void___CloseChrome__UChrome_lref::Method,
			Method___void___BringToTop__UChrome_lref::Method,
		};

		// --- 2 Fields
		struct Field___AdjustingChrome
		{
			static inline const TField Field { u8"AdjustingChrome"N, &UChromePanel::AdjustingChrome, FMetadata::None };
		};
		struct Field___PreviousCursorPosition
		{
			static inline const TField Field { u8"PreviousCursorPosition"N, &UChromePanel::PreviousCursorPosition, FMetadata::None };
		};
		struct Field___PreviousChromeLocation
		{
			static inline const TField Field { u8"PreviousChromeLocation"N, &UChromePanel::PreviousChromeLocation, FMetadata::None };
		};
		struct Field___PreviousChromeSize
		{
			static inline const TField Field { u8"PreviousChromeSize"N, &UChromePanel::PreviousChromeSize, FMetadata::None };
		};
		struct Field___ResizingChromeComponent
		{
			static inline const TField Field { u8"ResizingChromeComponent"N, &UChromePanel::ResizingChromeComponent, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___AdjustingChrome::Field,
			Field___PreviousCursorPosition::Field,
			Field___PreviousChromeLocation::Field,
			Field___PreviousChromeSize::Field,
			Field___ResizingChromeComponent::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPanel>(),
		};

		// --- 5 Constructor
		UChromePanel_Type(FName Name = u8"UChromePanel"N) : TClass(Name, Classof<UI::UPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UChromePanel>
	{
		static inline UChromePanel_Type UChromePanelType { u8"UChromePanel"N };

		static FType & Type()
		{
			return UChromePanelType;
		}
	};

	FType & UI::UChromePanel::StaticType()
	{
		return TStaticType<UI::UChromePanel>().Type();
	}

	static TTypeRegister<UI::UChromePanel> UChromePanelTypeRegister { TStaticType<UI::UChromePanel>().Type() };
}

// Generated code end.
