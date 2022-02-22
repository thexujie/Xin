// D0281B30D06EC22A16AA78E817532584 2025-04-03 18:06:21
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/View.h"

namespace Xin
{
	class UView_Type : public TClass<UI::UView>
	{
	public:
		using UView = UI::UView;

		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UView, FType &()> Method { u8"Type"N, &UView::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UView, void()> Method { u8"OnConstruct"N, &UView::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UView, void()> Method { u8"OnInitialize"N, &UView::OnInitialize, FMetadata::None };
		};
		struct Method___UView_ptr___GetView____const
		{
			static inline const TMethod<UView, UI::UView *()> Method { u8"GetView"N, &UView::GetView, FMetadata::None };
		};
		struct Method___UElement_ptr___HitElement__TVec2__float64______const
		{
			static inline const TMethod<UView, UI::UElement *(FPoint)> Method { u8"HitElement"N, &UView::HitElement, FMetadata::None };
		};
		struct Method___void___UpdateCursor__ECursor__bool
		{
			static inline const TMethod<UView, void(ECursor, bool)> Method { u8"UpdateCursor"N, &UView::UpdateCursor, FMetadata::None };
		};
		struct Method___void___UpdateCapture__bool
		{
			static inline const TMethod<UView, void(bool)> Method { u8"UpdateCapture"N, &UView::UpdateCapture, FMetadata::None };
		};
		struct Method___void___SetHoveredElement__UElement_ptr__TVec2__float64__
		{
			static inline const TMethod<UView, void(UI::UElement *, FPoint)> Method { u8"SetHoveredElement"N, &UView::SetHoveredElement, FMetadata::None };
		};
		struct Method___void___SetCapturedElement__UElement_ptr
		{
			static inline const TMethod<UView, void(UI::UElement *)> Method { u8"SetCapturedElement"N, &UView::SetCapturedElement, FMetadata::None };
		};
		struct Method___void___DispatchLayout
		{
			static inline const TMethod<UView, void()> Method { u8"DispatchLayout"N, &UView::DispatchLayout, FMetadata::None };
		};
		struct Method___void___RegisterRadioButton__URadioButton_ptr
		{
			static inline const TMethod<UView, void(UI::URadioButton *)> Method { u8"RegisterRadioButton"N, &UView::RegisterRadioButton, FMetadata::None };
		};
		struct Method___void___UnregisterRadioButton__URadioButton_ptr
		{
			static inline const TMethod<UView, void(UI::URadioButton *)> Method { u8"UnregisterRadioButton"N, &UView::UnregisterRadioButton, FMetadata::None };
		};
		struct Method___void___CheckRadioButton__URadioButton_ptr
		{
			static inline const TMethod<UView, void(UI::URadioButton *)> Method { u8"CheckRadioButton"N, &UView::CheckRadioButton, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___UView_ptr___GetView____const::Method,
			Method___UElement_ptr___HitElement__TVec2__float64______const::Method,
			Method___void___UpdateCursor__ECursor__bool::Method,
			Method___void___UpdateCapture__bool::Method,
			Method___void___SetHoveredElement__UElement_ptr__TVec2__float64__::Method,
			Method___void___SetCapturedElement__UElement_ptr::Method,
			Method___void___DispatchLayout::Method,
			Method___void___RegisterRadioButton__URadioButton_ptr::Method,
			Method___void___UnregisterRadioButton__URadioButton_ptr::Method,
			Method___void___CheckRadioButton__URadioButton_ptr::Method,
		};

		// --- 2 Fields
		struct Field___HoveredElement
		{
			static inline const TField Field { u8"HoveredElement"N, &UView::HoveredElement, FMetadata::None };
		};
		struct Field___CapturedElement
		{
			static inline const TField Field { u8"CapturedElement"N, &UView::CapturedElement, FMetadata::None };
		};
		struct Field___LayoutDispatching
		{
			static inline const TField Field { u8"LayoutDispatching"N, &UView::LayoutDispatching, FMetadata::None };
		};
		struct Field___RadioButtons
		{
			static inline const TField Field { u8"RadioButtons"N, &UView::RadioButtons, FMetadata::None };
		};
		struct Field___RadioButtonGroups
		{
			static inline const TField Field { u8"RadioButtonGroups"N, &UView::RadioButtonGroups, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___HoveredElement::Field,
			Field___CapturedElement::Field,
			Field___LayoutDispatching::Field,
			Field___RadioButtons::Field,
			Field___RadioButtonGroups::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UView_Type(FName Name = u8"UView"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UView>
	{
		static inline UView_Type UViewType { u8"UView"N };

		static FType & Type()
		{
			return UViewType;
		}
	};

	FType & UI::UView::StaticType()
	{
		return TStaticType<UI::UView>().Type();
	}

	static TTypeRegister<UI::UView> UViewTypeRegister { TStaticType<UI::UView>().Type() };
}

// Generated code end.
