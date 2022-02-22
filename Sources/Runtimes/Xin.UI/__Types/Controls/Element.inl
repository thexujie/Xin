// F70D061D76DF02CC11A5A29FA68CE790 2025-04-03 17:45:09
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Element.h"

namespace Xin
{
	class UElement_Type : public TClass<UI::UElement>
	{
	public:
		using UElement = UI::UElement;

		// --- 0 Constructors
		static inline const TConstructor<UElement> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UElement, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UElement, FType &()> Method { u8"Type"N, &UElement::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UElement, void()> Method { u8"OnConstruct"N, &UElement::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UElement, void()> Method { u8"OnInitialize"N, &UElement::OnInitialize, FMetadata::None };
		};
		struct Method___void___Finalize
		{
			static inline const TMethod<UElement, void()> Method { u8"Finalize"N, &UElement::Finalize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UElement, void()> Method { u8"OnFinalize"N, &UElement::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnPropertyChanged__FProperty_const_lref
		{
			static inline const TMethod<UElement, void(FProperty const &)> Method { u8"OnPropertyChanged"N, &UElement::OnPropertyChanged, FMetadata::None };
		};
		struct Method___void___OnChanged__FProperty_const_lref
		{
			static inline const TMethod<UElement, void(FProperty const &)> Method { u8"OnChanged"N, &UElement::OnChanged, FMetadata::None };
		};
		struct Method___IObject_ptr___GetParentObject____const
		{
			static inline const TMethod<UElement, IObject *()> Method { u8"GetParentObject"N, &UElement::GetParentObject, FMetadata::None };
		};
		struct Method___TPointerPtr__FLayoutManager_____GetLayoutManager
		{
			static inline const TMethod<UElement, TPointerPtr<UI::FLayoutManager>()> Method { u8"GetLayoutManager"N, &UElement::GetLayoutManager, FMetadata::None };
		};
		struct Method___UWindow_ptr___GetWindow____const
		{
			static inline const TMethod<UElement, UI::UWindow *()> Method { u8"GetWindow"N, &UElement::GetWindow, FMetadata::None };
		};
		struct Method___IGraphics_lref___GetGraphics____const
		{
			static inline const TMethod<UElement, IGraphics &()> Method { u8"GetGraphics"N, &UElement::GetGraphics, FMetadata::None };
		};
		struct Method___UWindow_lref___GetWindowUnsafe____const
		{
			static inline const TMethod<UElement, UI::UWindow &()> Method { u8"GetWindowUnsafe"N, &UElement::GetWindowUnsafe, FMetadata::None };
		};
		struct Method___UView_ptr___GetView____const
		{
			static inline const TMethod<UElement, UI::UView *()> Method { u8"GetView"N, &UElement::GetView, FMetadata::None };
		};
		struct Method___UView_lref___GetViewUnsafe____const
		{
			static inline const TMethod<UElement, UI::UView &()> Method { u8"GetViewUnsafe"N, &UElement::GetViewUnsafe, FMetadata::None };
		};
		struct Method___bool___IsPlaced____const
		{
			static inline const TMethod<UElement, bool()> Method { u8"IsPlaced"N, &UElement::IsPlaced, FMetadata::None };
		};
		struct Method___IDesktop_lref___GetDesktop____const
		{
			static inline const TMethod<UElement, IDesktop &()> Method { u8"GetDesktop"N, &UElement::GetDesktop, FMetadata::None };
		};
		struct Method___void___ShowContextMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void_________const_lref
		{
			static inline const TMethod<UElement, void(UI::UElement *, FVariant const &, EPopupPosition, TFunction<void()> const &)> Method { u8"ShowContextMenu"N, &UElement::ShowContextMenu, FMetadata::None };
		};
		struct Method___void___ShowToolTip__UElement_ptr__FVariant_const_lref
		{
			static inline const TMethod<UElement, void(UI::UElement *, FVariant const &)> Method { u8"ShowToolTip"N, &UElement::ShowToolTip, FMetadata::None };
		};
		struct Method___float64___GetMinWidth____const
		{
			static inline const TMethod<UElement, dimenx()> Method { u8"GetMinWidth"N, &UElement::GetMinWidth, FMetadata::None };
		};
		struct Method___void___SetMinWidth__float64
		{
			static inline const TMethod<UElement, void(dimenx)> Method { u8"SetMinWidth"N, &UElement::SetMinWidth, FMetadata::None };
		};
		struct Method___float64___GetMinHeight____const
		{
			static inline const TMethod<UElement, dimenx()> Method { u8"GetMinHeight"N, &UElement::GetMinHeight, FMetadata::None };
		};
		struct Method___void___SetMinHeight__float64
		{
			static inline const TMethod<UElement, void(dimenx)> Method { u8"SetMinHeight"N, &UElement::SetMinHeight, FMetadata::None };
		};
		struct Method___float64___GetMaxWidth____const
		{
			static inline const TMethod<UElement, dimenx()> Method { u8"GetMaxWidth"N, &UElement::GetMaxWidth, FMetadata::None };
		};
		struct Method___void___SetMaxWidth__float64
		{
			static inline const TMethod<UElement, void(dimenx)> Method { u8"SetMaxWidth"N, &UElement::SetMaxWidth, FMetadata::None };
		};
		struct Method___float64___GetMaxHeight____const
		{
			static inline const TMethod<UElement, dimenx()> Method { u8"GetMaxHeight"N, &UElement::GetMaxHeight, FMetadata::None };
		};
		struct Method___void___SetMaxHeight__float64
		{
			static inline const TMethod<UElement, void(dimenx)> Method { u8"SetMaxHeight"N, &UElement::SetMaxHeight, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____FindChild__FName____const
		{
			static inline const TMethod<UElement, UI::UElementRef(FName)> Method { u8"FindChild"N, &UElement::FindChild, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____FindChild__FType_const_lref____const
		{
			static inline const TMethod<UElement, TReferPtr<UI::UElement>(FType const &)> Method { u8"FindChild"N, &UElement::FindChild, FMetadata::None };
		};
		struct Method___bool___IsAncestorOf__UElement_const_ptr____const
		{
			static inline const TMethod<UElement, bool(UI::UElement const *)> Method { u8"IsAncestorOf"N, &UElement::IsAncestorOf, FMetadata::None };
		};
		struct Method___void___SetParent__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"SetParent"N, &UElement::SetParent, FMetadata::None };
		};
		struct Method___void___SortChildrenZ
		{
			static inline const TMethod<UElement, void()> Method { u8"SortChildrenZ"N, &UElement::SortChildrenZ, FMetadata::None };
		};
		struct Method___void___OnAddChild__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"OnAddChild"N, &UElement::OnAddChild, FMetadata::None };
		};
		struct Method___void___OnRemoveChild__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"OnRemoveChild"N, &UElement::OnRemoveChild, FMetadata::None };
		};
		struct Method___void___OnAdded__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"OnAdded"N, &UElement::OnAdded, FMetadata::None };
		};
		struct Method___void___OnRemoved__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"OnRemoved"N, &UElement::OnRemoved, FMetadata::None };
		};
		struct Method___void___OnChildMeasured__UElement_ptr
		{
			static inline const TMethod<UElement, void(UI::UElement *)> Method { u8"OnChildMeasured"N, &UElement::OnChildMeasured, FMetadata::None };
		};
		struct Method___void___OnChildDesiredSizeChanged__UElement_ptr__TVec2__float64__
		{
			static inline const TMethod<UElement, void(UI::UElement *, FSize)> Method { u8"OnChildDesiredSizeChanged"N, &UElement::OnChildDesiredSizeChanged, FMetadata::None };
		};
		struct Method___TVec2__float64_____LocalToParent__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"LocalToParent"N, &UElement::LocalToParent, FMetadata::None };
		};
		struct Method___TVec2__float64_____ParentToLocal__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"ParentToLocal"N, &UElement::ParentToLocal, FMetadata::None };
		};
		struct Method___TVec2__float64_____LocalToWindow__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"LocalToWindow"N, &UElement::LocalToWindow, FMetadata::None };
		};
		struct Method___TVec2__float64_____WindowToLocal__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"WindowToLocal"N, &UElement::WindowToLocal, FMetadata::None };
		};
		struct Method___TVec2__float64_____LocalToGlobal__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"LocalToGlobal"N, &UElement::LocalToGlobal, FMetadata::None };
		};
		struct Method___TVec2__float64_____GlobalToLocal__TVec2__float64______const
		{
			static inline const TMethod<UElement, FPoint(FPoint)> Method { u8"GlobalToLocal"N, &UElement::GlobalToLocal, FMetadata::None };
		};
		struct Method___void___InvalidateVisual
		{
			static inline const TMethod<UElement, void()> Method { u8"InvalidateVisual"N, &UElement::InvalidateVisual, FMetadata::None };
		};
		struct Method___void___InvalidateRect__TVec4__float64__
		{
			static inline const TMethod<UElement, void(FRect)> Method { u8"InvalidateRect"N, &UElement::InvalidateRect, FMetadata::None };
		};
		struct Method___UElement_ptr___HitElement__TVec2__float64______const
		{
			static inline const TMethod<UElement, UI::UElement *(FPoint)> Method { u8"HitElement"N, &UElement::HitElement, FMetadata::None };
		};
		struct Method___void___OnResourcesChanged
		{
			static inline const TMethod<UElement, void()> Method { u8"OnResourcesChanged"N, &UElement::OnResourcesChanged, FMetadata::None };
		};
		struct Method___TReferPtr__IResource_____FindResource__FName__FType_const_lref
		{
			static inline const TMethod<UElement, UI::IResourceRef(FName, FType const &)> Method { u8"FindResource"N, &UElement::FindResource, FMetadata::None };
		};
		struct Method___TReferPtr__IResource_____FindResource__FType_const_lref__FType_const_lref
		{
			static inline const TMethod<UElement, UI::IResourceRef(FType const &, FType const &)> Method { u8"FindResource"N, &UElement::FindResource, FMetadata::None };
		};
		struct Method___void___AdjustConstrainedValue__float64__float64_lref__float64__FDimen__float64
		{
			static inline const TMethod<UElement, void(dimenx, dimenx &, dimenx, UI::FDimen, dimenx)> Method { u8"AdjustConstrainedValue"N, &UElement::AdjustConstrainedValue, FMetadata::None };
		};
		struct Method___void___AdjustMeasuredValue__float64__float64_lref__float64__FDimen__float64
		{
			static inline const TMethod<UElement, void(dimenx, dimenx &, dimenx, UI::FDimen, dimenx)> Method { u8"AdjustMeasuredValue"N, &UElement::AdjustMeasuredValue, FMetadata::None };
		};
		struct Method___void___SuspendLayout
		{
			static inline const TMethod<UElement, void()> Method { u8"SuspendLayout"N, &UElement::SuspendLayout, FMetadata::None };
		};
		struct Method___void___ResumeLayout
		{
			static inline const TMethod<UElement, void()> Method { u8"ResumeLayout"N, &UElement::ResumeLayout, FMetadata::None };
		};
		struct Method___void___InvalidateMeasure
		{
			static inline const TMethod<UElement, void()> Method { u8"InvalidateMeasure"N, &UElement::InvalidateMeasure, FMetadata::None };
		};
		struct Method___TVec2__float64_____Measure__TVec2__float64__
		{
			static inline const TMethod<UElement, FSize(FSize)> Method { u8"Measure"N, &UElement::Measure, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UElement, FSize(FSize)> Method { u8"OnMeasure"N, &UElement::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnMeasured
		{
			static inline const TMethod<UElement, void()> Method { u8"OnMeasured"N, &UElement::OnMeasured, FMetadata::None };
		};
		struct Method___void___InvalidateArrange
		{
			static inline const TMethod<UElement, void()> Method { u8"InvalidateArrange"N, &UElement::InvalidateArrange, FMetadata::None };
		};
		struct Method___TVec2__float64_____Arrange__TVec4__float64__
		{
			static inline const TMethod<UElement, FSize(FRect)> Method { u8"Arrange"N, &UElement::Arrange, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UElement, void()> Method { u8"OnArrange"N, &UElement::OnArrange, FMetadata::None };
		};
		struct Method___void___OnArranged
		{
			static inline const TMethod<UElement, void()> Method { u8"OnArranged"N, &UElement::OnArranged, FMetadata::None };
		};
		struct Method___void___Paint__IPainter_lref
		{
			static inline const TMethod<UElement, void(IPainter &)> Method { u8"Paint"N, &UElement::Paint, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UElement, void(IPainter &)> Method { u8"OnPaint"N, &UElement::OnPaint, FMetadata::None };
		};
		struct Method___void___LoadStyle
		{
			static inline const TMethod<UElement, void()> Method { u8"LoadStyle"N, &UElement::LoadStyle, FMetadata::None };
		};
		struct Method___void___SetState__FVisualStateName
		{
			static inline const TMethod<UElement, void(UI::FVisualStateName)> Method { u8"SetState"N, &UElement::SetState, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UElement, void()> Method { u8"UpdateState"N, &UElement::UpdateState, FMetadata::None };
		};
		struct Method___void___PreMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"PreMouseEnter"N, &UElement::PreMouseEnter, FMetadata::None };
		};
		struct Method___void___PreMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"PreMouseLeave"N, &UElement::PreMouseLeave, FMetadata::None };
		};
		struct Method___void___PreMouseMove__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"PreMouseMove"N, &UElement::PreMouseMove, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UElement::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UElement::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnMouseHover__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"OnMouseHover"N, &UElement::OnMouseHover, FMetadata::None };
		};
		struct Method___void___OnMouseMove__FMouseEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseEventArgs &)> Method { u8"OnMouseMove"N, &UElement::OnMouseMove, FMetadata::None };
		};
		struct Method___void___PreMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"PreMouseDown"N, &UElement::PreMouseDown, FMetadata::None };
		};
		struct Method___void___PreMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"PreMouseUp"N, &UElement::PreMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UElement::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UElement::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseLeftButtonDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseLeftButtonDown"N, &UElement::OnMouseLeftButtonDown, FMetadata::None };
		};
		struct Method___void___OnMouseLeftButtonUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseLeftButtonUp"N, &UElement::OnMouseLeftButtonUp, FMetadata::None };
		};
		struct Method___void___OnMouseMiddleButtonDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseMiddleButtonDown"N, &UElement::OnMouseMiddleButtonDown, FMetadata::None };
		};
		struct Method___void___OnMouseMiddleButtonUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseMiddleButtonUp"N, &UElement::OnMouseMiddleButtonUp, FMetadata::None };
		};
		struct Method___void___OnMouseRightButtonDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseRightButtonDown"N, &UElement::OnMouseRightButtonDown, FMetadata::None };
		};
		struct Method___void___OnMouseRightButtonUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseRightButtonUp"N, &UElement::OnMouseRightButtonUp, FMetadata::None };
		};
		struct Method___void___PreMouseWhell__FMouseWhellEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseWhellEventArgs &)> Method { u8"PreMouseWhell"N, &UElement::PreMouseWhell, FMetadata::None };
		};
		struct Method___void___OnMouseWhell__FMouseWhellEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FMouseWhellEventArgs &)> Method { u8"OnMouseWhell"N, &UElement::OnMouseWhell, FMetadata::None };
		};
		struct Method___void___OnKeyDown__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyDown"N, &UElement::OnKeyDown, FMetadata::None };
		};
		struct Method___void___OnKeyUp__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyUp"N, &UElement::OnKeyUp, FMetadata::None };
		};
		struct Method___void___AcquireMouseCapture
		{
			static inline const TMethod<UElement, void()> Method { u8"AcquireMouseCapture"N, &UElement::AcquireMouseCapture, FMetadata::None };
		};
		struct Method___void___ReleaseMouseCapture
		{
			static inline const TMethod<UElement, void()> Method { u8"ReleaseMouseCapture"N, &UElement::ReleaseMouseCapture, FMetadata::None };
		};
		struct Method___void___OnSizeChanged__TVec2__FDimen___const_lref
		{
			static inline const TMethod<UElement, void(UI::FDimen2 const &)> Method { u8"OnSizeChanged"N, &UElement::OnSizeChanged, FMetadata::None };
		};
		struct Method___void___OnVisibilityChanged__EVisibility
		{
			static inline const TMethod<UElement, void(UI::EVisibility)> Method { u8"OnVisibilityChanged"N, &UElement::OnVisibilityChanged, FMetadata::None };
		};
		struct Method___void___OnShown
		{
			static inline const TMethod<UElement, void()> Method { u8"OnShown"N, &UElement::OnShown, FMetadata::None };
		};
		struct Method___void___OnHidden
		{
			static inline const TMethod<UElement, void()> Method { u8"OnHidden"N, &UElement::OnHidden, FMetadata::None };
		};
		struct Method___void___OnShownChanged__bool
		{
			static inline const TMethod<UElement, void(bool)> Method { u8"OnShownChanged"N, &UElement::OnShownChanged, FMetadata::None };
		};
		struct Method___void___OnAppeared
		{
			static inline const TMethod<UElement, void()> Method { u8"OnAppeared"N, &UElement::OnAppeared, FMetadata::None };
		};
		struct Method___void___OnDisappeared
		{
			static inline const TMethod<UElement, void()> Method { u8"OnDisappeared"N, &UElement::OnDisappeared, FMetadata::None };
		};
		struct Method___void___OnAppearedChanged__bool
		{
			static inline const TMethod<UElement, void(bool)> Method { u8"OnAppearedChanged"N, &UElement::OnAppearedChanged, FMetadata::None };
		};
		struct Method___ECursor___QueryCursor__TVec2__float64______const
		{
			static inline const TMethod<UElement, ECursor(FPoint)> Method { u8"QueryCursor"N, &UElement::QueryCursor, FMetadata::None };
		};
		struct Method___EHitResult___HitTest__TVec2__float64______const
		{
			static inline const TMethod<UElement, EHitResult(FPoint)> Method { u8"HitTest"N, &UElement::HitTest, FMetadata::None };
		};
		struct Method___void___OnMouseCapturedChanged__bool
		{
			static inline const TMethod<UElement, void(bool)> Method { u8"OnMouseCapturedChanged"N, &UElement::OnMouseCapturedChanged, FMetadata::None };
		};
		struct Method___void___OnDesiredSizeChanged__TVec2__float64__
		{
			static inline const TMethod<UElement, void(FSize)> Method { u8"OnDesiredSizeChanged"N, &UElement::OnDesiredSizeChanged, FMetadata::None };
		};
		struct Method___TVec2__float64_____GetFinalSize____const
		{
			static inline const TMethod<UElement, FSize()> Method { u8"GetFinalSize"N, &UElement::GetFinalSize, FMetadata::None };
		};
		struct Method___void___OnCommand__ICommand_lref__FRoutedEventArgs_lref
		{
			static inline const TMethod<UElement, void(UI::ICommand &, UI::FRoutedEventArgs &)> Method { u8"OnCommand"N, &UElement::OnCommand, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___Finalize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnPropertyChanged__FProperty_const_lref::Method,
			Method___void___OnChanged__FProperty_const_lref::Method,
			Method___IObject_ptr___GetParentObject____const::Method,
			Method___TPointerPtr__FLayoutManager_____GetLayoutManager::Method,
			Method___UWindow_ptr___GetWindow____const::Method,
			Method___IGraphics_lref___GetGraphics____const::Method,
			Method___UWindow_lref___GetWindowUnsafe____const::Method,
			Method___UView_ptr___GetView____const::Method,
			Method___UView_lref___GetViewUnsafe____const::Method,
			Method___bool___IsPlaced____const::Method,
			Method___IDesktop_lref___GetDesktop____const::Method,
			Method___void___ShowContextMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void_________const_lref::Method,
			Method___void___ShowToolTip__UElement_ptr__FVariant_const_lref::Method,
			Method___float64___GetMinWidth____const::Method,
			Method___void___SetMinWidth__float64::Method,
			Method___float64___GetMinHeight____const::Method,
			Method___void___SetMinHeight__float64::Method,
			Method___float64___GetMaxWidth____const::Method,
			Method___void___SetMaxWidth__float64::Method,
			Method___float64___GetMaxHeight____const::Method,
			Method___void___SetMaxHeight__float64::Method,
			Method___TReferPtr__UElement_____FindChild__FName____const::Method,
			Method___TReferPtr__UElement_____FindChild__FType_const_lref____const::Method,
			Method___bool___IsAncestorOf__UElement_const_ptr____const::Method,
			Method___void___SetParent__UElement_ptr::Method,
			Method___void___SortChildrenZ::Method,
			Method___void___OnAddChild__UElement_ptr::Method,
			Method___void___OnRemoveChild__UElement_ptr::Method,
			Method___void___OnAdded__UElement_ptr::Method,
			Method___void___OnRemoved__UElement_ptr::Method,
			Method___void___OnChildMeasured__UElement_ptr::Method,
			Method___void___OnChildDesiredSizeChanged__UElement_ptr__TVec2__float64__::Method,
			Method___TVec2__float64_____LocalToParent__TVec2__float64______const::Method,
			Method___TVec2__float64_____ParentToLocal__TVec2__float64______const::Method,
			Method___TVec2__float64_____LocalToWindow__TVec2__float64______const::Method,
			Method___TVec2__float64_____WindowToLocal__TVec2__float64______const::Method,
			Method___TVec2__float64_____LocalToGlobal__TVec2__float64______const::Method,
			Method___TVec2__float64_____GlobalToLocal__TVec2__float64______const::Method,
			Method___void___InvalidateVisual::Method,
			Method___void___InvalidateRect__TVec4__float64__::Method,
			Method___UElement_ptr___HitElement__TVec2__float64______const::Method,
			Method___void___OnResourcesChanged::Method,
			Method___TReferPtr__IResource_____FindResource__FName__FType_const_lref::Method,
			Method___TReferPtr__IResource_____FindResource__FType_const_lref__FType_const_lref::Method,
			Method___void___AdjustConstrainedValue__float64__float64_lref__float64__FDimen__float64::Method,
			Method___void___AdjustMeasuredValue__float64__float64_lref__float64__FDimen__float64::Method,
			Method___void___SuspendLayout::Method,
			Method___void___ResumeLayout::Method,
			Method___void___InvalidateMeasure::Method,
			Method___TVec2__float64_____Measure__TVec2__float64__::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnMeasured::Method,
			Method___void___InvalidateArrange::Method,
			Method___TVec2__float64_____Arrange__TVec4__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnArranged::Method,
			Method___void___Paint__IPainter_lref::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___LoadStyle::Method,
			Method___void___SetState__FVisualStateName::Method,
			Method___void___UpdateState::Method,
			Method___void___PreMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___PreMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___PreMouseMove__FMouseEventArgs_lref::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnMouseHover__FMouseEventArgs_lref::Method,
			Method___void___OnMouseMove__FMouseEventArgs_lref::Method,
			Method___void___PreMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___PreMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseLeftButtonDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseLeftButtonUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseMiddleButtonDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseMiddleButtonUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseRightButtonDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseRightButtonUp__FMouseButtonEventArgs_lref::Method,
			Method___void___PreMouseWhell__FMouseWhellEventArgs_lref::Method,
			Method___void___OnMouseWhell__FMouseWhellEventArgs_lref::Method,
			Method___void___OnKeyDown__FKeyboardEventArgs_lref::Method,
			Method___void___OnKeyUp__FKeyboardEventArgs_lref::Method,
			Method___void___AcquireMouseCapture::Method,
			Method___void___ReleaseMouseCapture::Method,
			Method___void___OnSizeChanged__TVec2__FDimen___const_lref::Method,
			Method___void___OnVisibilityChanged__EVisibility::Method,
			Method___void___OnShown::Method,
			Method___void___OnHidden::Method,
			Method___void___OnShownChanged__bool::Method,
			Method___void___OnAppeared::Method,
			Method___void___OnDisappeared::Method,
			Method___void___OnAppearedChanged__bool::Method,
			Method___ECursor___QueryCursor__TVec2__float64______const::Method,
			Method___EHitResult___HitTest__TVec2__float64______const::Method,
			Method___void___OnMouseCapturedChanged__bool::Method,
			Method___void___OnDesiredSizeChanged__TVec2__float64__::Method,
			Method___TVec2__float64_____GetFinalSize____const::Method,
			Method___void___OnCommand__ICommand_lref__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___Children
		{
			static inline const TField Field { u8"Children"N, &UElement::Children, FMetadata::None };
		};
		struct Field___Parent
		{
			static inline const TField Field { u8"Parent"N, &UElement::Parent, FMetadata::None };
		};
		struct Field___RenderDone
		{
			static inline const TField Field { u8"RenderDone"N, &UElement::RenderDone, FMetadata::None };
		};
		struct Field___MeasureDoing
		{
			static inline const TField Field { u8"MeasureDoing"N, &UElement::MeasureDoing, FMetadata::None };
		};
		struct Field___MeasureDone
		{
			static inline const TField Field { u8"MeasureDone"N, &UElement::MeasureDone, FMetadata::None };
		};
		struct Field___MeasureFinishing
		{
			static inline const TField Field { u8"MeasureFinishing"N, &UElement::MeasureFinishing, FMetadata::None };
		};
		struct Field___ArrangeDoing
		{
			static inline const TField Field { u8"ArrangeDoing"N, &UElement::ArrangeDoing, FMetadata::None };
		};
		struct Field___ArrangeDone
		{
			static inline const TField Field { u8"ArrangeDone"N, &UElement::ArrangeDone, FMetadata::None };
		};
		struct Field___ArrangeFinishing
		{
			static inline const TField Field { u8"ArrangeFinishing"N, &UElement::ArrangeFinishing, FMetadata::None };
		};
		struct Field___LayoutSuspended
		{
			static inline const TField Field { u8"LayoutSuspended"N, &UElement::LayoutSuspended, FMetadata::None };
		};
		struct Field___MeasurePendding
		{
			static inline const TField Field { u8"MeasurePendding"N, &UElement::MeasurePendding, FMetadata::None };
		};
		struct Field___ArrangePendding
		{
			static inline const TField Field { u8"ArrangePendding"N, &UElement::ArrangePendding, FMetadata::None };
		};
		struct Field___MinSize
		{
			static inline const TField Field { u8"MinSize"N, &UElement::MinSize, FMetadata::None };
		};
		struct Field___MaxSize
		{
			static inline const TField Field { u8"MaxSize"N, &UElement::MaxSize, FMetadata::None };
		};
		struct Field___ClipToBounds
		{
			static inline const TField Field { u8"ClipToBounds"N, &UElement::ClipToBounds, FMetadata::None };
		};
		struct Field___Focusable
		{
			static inline const TField Field { u8"Focusable"N, &UElement::Focusable, FMetadata::None };
		};
		struct Field___ArrangeRect
		{
			static inline const TField Field { u8"ArrangeRect"N, &UElement::ArrangeRect, FMetadata::None };
		};
		struct Field___VisibleRect
		{
			static inline const TField Field { u8"VisibleRect"N, &UElement::VisibleRect, FMetadata::None };
		};
		struct Field___CommandBindings
		{
			static inline const TField Field { u8"CommandBindings"N, &UElement::CommandBindings, FMetadata::None };
		};
		struct Field___Style
		{
			static inline const TField Field { u8"Style"N, &UElement::Style, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Children::Field,
			Field___Parent::Field,
			Field___RenderDone::Field,
			Field___MeasureDoing::Field,
			Field___MeasureDone::Field,
			Field___MeasureFinishing::Field,
			Field___ArrangeDoing::Field,
			Field___ArrangeDone::Field,
			Field___ArrangeFinishing::Field,
			Field___LayoutSuspended::Field,
			Field___MeasurePendding::Field,
			Field___ArrangePendding::Field,
			Field___MinSize::Field,
			Field___MaxSize::Field,
			Field___ClipToBounds::Field,
			Field___Focusable::Field,
			Field___ArrangeRect::Field,
			Field___VisibleRect::Field,
			Field___CommandBindings::Field,
			Field___Style::Field,
		};

		// --- 3 Properties
		struct Property___Graphics
		{
			static inline const TFunctorProperty Property { u8"Graphics"N, &UElement::GetGraphics, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___Window
		{
			static inline const TFunctorProperty Property { u8"Window"N, &UElement::GetWindowUnsafe, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___View
		{
			static inline const TFunctorProperty Property { u8"View"N, &UElement::GetViewUnsafe, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___Placed
		{
			static inline const TFunctorProperty Property { u8"Placed"N, &UElement::IsPlaced, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___Desktop
		{
			static inline const TFunctorProperty Property { u8"Desktop"N, &UElement::GetDesktop, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___ZIndex
		{
			static inline const TProxyProperty Property { u8"ZIndex"N, &UElement::__ZIndex, &UElement::__GetZIndex, &UElement::__SetZIndex, UElement::FPropertyMetadata::None };
		};
		struct Property___InnerZIndex
		{
			static inline const TProxyProperty Property { u8"InnerZIndex"N, &UElement::__InnerZIndex, &UElement::__GetInnerZIndex, &UElement::__SetInnerZIndex, UElement::FPropertyMetadata::None };
		};
		struct Property___Size
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Size"N, &UElement::__Size, &UElement::__GetSize, &UElement::__SetSizeValue, UElement::__Size__, Metadata };
		};
		struct Property___Width
		{
			static inline const TProxyProperty Property { u8"Width"N, &UElement::__Width, &UElement::__GetWidth, &UElement::__SetWidth, UElement::FPropertyMetadata::None };
		};
		struct Property___Height
		{
			static inline const TProxyProperty Property { u8"Height"N, &UElement::__Height, &UElement::__GetHeight, &UElement::__SetHeight, UElement::FPropertyMetadata::None };
		};
		struct Property___Margin
		{
			static inline const TDependencyProperty Property { u8"Margin"N, &UElement::__Margin, &UElement::__GetMargin, &UElement::__SetMarginValue, UElement::__Margin__, UElement::FPropertyMetadata::None };
		};
		struct Property___LayoutRounding
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"LayoutRounding"N, &UElement::__LayoutRounding, &UElement::__GetLayoutRounding, &UElement::__SetLayoutRoundingValue, UElement::__LayoutRounding__, Metadata };
		};
		struct Property___Visibility
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Visibility"N, &UElement::__Visibility, &UElement::__GetVisibility, &UElement::__SetVisibilityValue, UElement::__Visibility__, Metadata };
		};
		struct Property___Shown
		{
			static inline const TProxyProperty Property { u8"Shown"N, &UElement::__Shown, &UElement::__GetShown, &UElement::__SetShown, UElement::FPropertyMetadata::None };
		};
		struct Property___Appeared
		{
			static inline const TProxyProperty Property { u8"Appeared"N, &UElement::__Appeared, &UElement::__GetAppeared, &UElement::__SetAppeared, UElement::FPropertyMetadata::None };
		};
		struct Property___Enabled
		{
			static inline const TDependencyProperty Property { u8"Enabled"N, &UElement::__Enabled, &UElement::__GetEnabled, &UElement::__SetEnabledValue, UElement::__Enabled__, UElement::FPropertyMetadata::None };
		};
		struct Property___Cursor
		{
			static inline const TDependencyProperty Property { u8"Cursor"N, &UElement::__Cursor, &UElement::__GetCursor, &UElement::__SetCursorValue, UElement::__Cursor__, UElement::FPropertyMetadata::None };
		};
		struct Property___ContextMenu
		{
			static inline const TDependencyProperty Property { u8"ContextMenu"N, &UElement::__ContextMenu, &UElement::__GetContextMenu, &UElement::__SetContextMenuValue, UElement::__ContextMenu__, UElement::FPropertyMetadata::None };
		};
		struct Property___ToolTip
		{
			static inline const TDependencyProperty Property { u8"ToolTip"N, &UElement::__ToolTip, &UElement::__GetToolTip, &UElement::__SetToolTipValue, UElement::__ToolTip__, UElement::FPropertyMetadata::None };
		};
		struct Property___HitInside
		{
			static inline const TDependencyProperty Property { u8"HitInside"N, &UElement::__HitInside, &UElement::__GetHitInside, &UElement::__SetHitInsideValue, UElement::__HitInside__, UElement::FPropertyMetadata::None };
		};
		struct Property___MouseCaptured
		{
			static inline const TDependencyProperty Property { u8"MouseCaptured"N, &UElement::__MouseCaptured, &UElement::__GetMouseCaptured, &UElement::__SetMouseCapturedValue, UElement::__MouseCaptured__, UElement::FPropertyMetadata::None };
		};
		struct Property___AvailableSize
		{
			static inline const TProxyProperty Property { u8"AvailableSize"N, &UElement::__AvailableSize, &UElement::__GetAvailableSize, &UElement::__SetAvailableSize, UElement::FPropertyMetadata::None };
		};
		struct Property___DesiredSize
		{
			static inline const TProxyProperty Property { u8"DesiredSize"N, &UElement::__DesiredSize, &UElement::__GetDesiredSize, &UElement::__SetDesiredSize, UElement::FPropertyMetadata::None };
		};
		struct Property___FinalRect
		{
			static inline const TProxyProperty Property { u8"FinalRect"N, &UElement::__FinalRect, &UElement::__GetFinalRect, &UElement::__SetFinalRect, UElement::FPropertyMetadata::None };
		};
		struct Property___FinalSize
		{
			static inline const TFunctorProperty Property { u8"FinalSize"N, &UElement::GetFinalSize, nullptr, UElement::FPropertyMetadata::None };
		};
		struct Property___MinWidth
		{
			static inline const TFunctorProperty Property { u8"MinWidth"N, &UElement::GetMinWidth, &UElement::SetMinWidth, UElement::FPropertyMetadata::None };
		};
		struct Property___MinHeight
		{
			static inline const TFunctorProperty Property { u8"MinHeight"N, &UElement::GetMinHeight, &UElement::SetMinHeight, UElement::FPropertyMetadata::None };
		};
		struct Property___MaxWidth
		{
			static inline const TFunctorProperty Property { u8"MaxWidth"N, &UElement::GetMaxWidth, &UElement::SetMaxWidth, UElement::FPropertyMetadata::None };
		};
		struct Property___MaxHeight
		{
			static inline const TFunctorProperty Property { u8"MaxHeight"N, &UElement::GetMaxHeight, &UElement::SetMaxHeight, UElement::FPropertyMetadata::None };
		};
		struct Property___IsMouseOver
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"IsMouseOver"N, &UElement::__IsMouseOver, &UElement::__GetIsMouseOver, &UElement::__SetIsMouseOver, Metadata };
		};
		struct Property___IsMouseLeftButtonDown
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"IsMouseLeftButtonDown"N, &UElement::__IsMouseLeftButtonDown, &UElement::__GetIsMouseLeftButtonDown, &UElement::__SetIsMouseLeftButtonDown, Metadata };
		};
		struct Property___HorizontalAlignment
		{
			static inline const TDependencyProperty Property { u8"HorizontalAlignment"N, &UElement::__HorizontalAlignment, &UElement::__GetHorizontalAlignment, &UElement::__SetHorizontalAlignmentValue, UElement::__HorizontalAlignment__, UElement::FPropertyMetadata::None };
		};
		struct Property___VerticalAlignment
		{
			static inline const TDependencyProperty Property { u8"VerticalAlignment"N, &UElement::__VerticalAlignment, &UElement::__GetVerticalAlignment, &UElement::__SetVerticalAlignmentValue, UElement::__VerticalAlignment__, UElement::FPropertyMetadata::None };
		};
		struct Property___ScaleRatio
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ScaleRatio"N, &UElement::__ScaleRatio, &UElement::__GetScaleRatio, &UElement::__SetScaleRatioValue, UElement::__ScaleRatio__, Metadata };
		};
		struct Property___FontFamily
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"FontFamily"N, &UElement::__FontFamily, &UElement::__GetFontFamily, &UElement::__SetFontFamilyValue, UElement::__FontFamily__, Metadata };
		};
		struct Property___FontSize
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"FontSize"N, &UElement::__FontSize, &UElement::__GetFontSize, &UElement::__SetFontSizeValue, UElement::__FontSize__, Metadata };
		};
		struct Property___FontWeight
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"FontWeight"N, &UElement::__FontWeight, &UElement::__GetFontWeight, &UElement::__SetFontWeightValue, UElement::__FontWeight__, Metadata };
		};
		struct Property___FontStyle
		{
			struct FPropertyMetadata : UElement::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"FontStyle"N, &UElement::__FontStyle, &UElement::__GetFontStyle, &UElement::__SetFontStyleValue, UElement::__FontStyle__, Metadata };
		};
		struct Property___GroundingFill
		{
			static inline const TDependencyProperty Property { u8"GroundingFill"N, &UElement::__GroundingFill, &UElement::__GetGroundingFill, &UElement::__SetGroundingFillValue, UElement::__GroundingFill__, UElement::FPropertyMetadata::None };
		};
		struct Property___GroundingStroke
		{
			static inline const TDependencyProperty Property { u8"GroundingStroke"N, &UElement::__GroundingStroke, &UElement::__GetGroundingStroke, &UElement::__SetGroundingStrokeValue, UElement::__GroundingStroke__, UElement::FPropertyMetadata::None };
		};
		struct Property___GroundingStrokeThickness
		{
			static inline const TDependencyProperty Property { u8"GroundingStrokeThickness"N, &UElement::__GroundingStrokeThickness, &UElement::__GetGroundingStrokeThickness, &UElement::__SetGroundingStrokeThicknessValue, UElement::__GroundingStrokeThickness__, UElement::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Graphics::Property,
			Property___Window::Property,
			Property___View::Property,
			Property___Placed::Property,
			Property___Desktop::Property,
			Property___ZIndex::Property,
			Property___InnerZIndex::Property,
			Property___Size::Property,
			Property___Width::Property,
			Property___Height::Property,
			Property___Margin::Property,
			Property___LayoutRounding::Property,
			Property___Visibility::Property,
			Property___Shown::Property,
			Property___Appeared::Property,
			Property___Enabled::Property,
			Property___Cursor::Property,
			Property___ContextMenu::Property,
			Property___ToolTip::Property,
			Property___HitInside::Property,
			Property___MouseCaptured::Property,
			Property___AvailableSize::Property,
			Property___DesiredSize::Property,
			Property___FinalRect::Property,
			Property___FinalSize::Property,
			Property___MinWidth::Property,
			Property___MinHeight::Property,
			Property___MaxWidth::Property,
			Property___MaxHeight::Property,
			Property___IsMouseOver::Property,
			Property___IsMouseLeftButtonDown::Property,
			Property___HorizontalAlignment::Property,
			Property___VerticalAlignment::Property,
			Property___ScaleRatio::Property,
			Property___FontFamily::Property,
			Property___FontSize::Property,
			Property___FontWeight::Property,
			Property___FontStyle::Property,
			Property___GroundingFill::Property,
			Property___GroundingStroke::Property,
			Property___GroundingStrokeThickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UVisual>(),
		};

		// --- 5 Constructor
		UElement_Type(FName Name = u8"UElement"N) : TClass(Name, Classof<UI::UVisual>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<intx> & UI::UElement::ZIndexProperty() { return UElement_Type::Property___ZIndex::Property; }

	const TProperty<intx> & UI::UElement::InnerZIndexProperty() { return UElement_Type::Property___InnerZIndex::Property; }

	const TProperty<UI::FDimen2> & UI::UElement::SizeProperty() { return UElement_Type::Property___Size::Property; }

	const TProperty<UI::FDimen> & UI::UElement::WidthProperty() { return UElement_Type::Property___Width::Property; }

	const TProperty<UI::FDimen> & UI::UElement::HeightProperty() { return UElement_Type::Property___Height::Property; }

	const TProperty<FThickness> & UI::UElement::MarginProperty() { return UElement_Type::Property___Margin::Property; }

	const TProperty<bool> & UI::UElement::LayoutRoundingProperty() { return UElement_Type::Property___LayoutRounding::Property; }

	const TProperty<UI::EVisibility> & UI::UElement::VisibilityProperty() { return UElement_Type::Property___Visibility::Property; }

	const TProperty<bool> & UI::UElement::ShownProperty() { return UElement_Type::Property___Shown::Property; }

	const TProperty<bool> & UI::UElement::AppearedProperty() { return UElement_Type::Property___Appeared::Property; }

	const TProperty<bool> & UI::UElement::EnabledProperty() { return UElement_Type::Property___Enabled::Property; }

	const TProperty<ECursor> & UI::UElement::CursorProperty() { return UElement_Type::Property___Cursor::Property; }

	const TProperty<FVariant> & UI::UElement::ContextMenuProperty() { return UElement_Type::Property___ContextMenu::Property; }

	const TProperty<FVariant> & UI::UElement::ToolTipProperty() { return UElement_Type::Property___ToolTip::Property; }

	const TProperty<EHitResult> & UI::UElement::HitInsideProperty() { return UElement_Type::Property___HitInside::Property; }

	const TProperty<bool> & UI::UElement::MouseCapturedProperty() { return UElement_Type::Property___MouseCaptured::Property; }

	const TProperty<FSize> & UI::UElement::AvailableSizeProperty() { return UElement_Type::Property___AvailableSize::Property; }

	const TProperty<FSize> & UI::UElement::DesiredSizeProperty() { return UElement_Type::Property___DesiredSize::Property; }

	const TProperty<FRect> & UI::UElement::FinalRectProperty() { return UElement_Type::Property___FinalRect::Property; }

	const TProperty<bool> & UI::UElement::IsMouseOverProperty() { return UElement_Type::Property___IsMouseOver::Property; }

	const TProperty<bool> & UI::UElement::IsMouseLeftButtonDownProperty() { return UElement_Type::Property___IsMouseLeftButtonDown::Property; }

	const TProperty<UI::EElementAlignment> & UI::UElement::HorizontalAlignmentProperty() { return UElement_Type::Property___HorizontalAlignment::Property; }

	const TProperty<UI::EElementAlignment> & UI::UElement::VerticalAlignmentProperty() { return UElement_Type::Property___VerticalAlignment::Property; }

	const TProperty<float32> & UI::UElement::ScaleRatioProperty() { return UElement_Type::Property___ScaleRatio::Property; }

	const TProperty<FString> & UI::UElement::FontFamilyProperty() { return UElement_Type::Property___FontFamily::Property; }

	const TProperty<float32> & UI::UElement::FontSizeProperty() { return UElement_Type::Property___FontSize::Property; }

	const TProperty<EFontWeight> & UI::UElement::FontWeightProperty() { return UElement_Type::Property___FontWeight::Property; }

	const TProperty<EFontStyle> & UI::UElement::FontStyleProperty() { return UElement_Type::Property___FontStyle::Property; }

	const TProperty<IBrushRef> & UI::UElement::GroundingFillProperty() { return UElement_Type::Property___GroundingFill::Property; }

	const TProperty<IBrushRef> & UI::UElement::GroundingStrokeProperty() { return UElement_Type::Property___GroundingStroke::Property; }

	const TProperty<dimenx> & UI::UElement::GroundingStrokeThicknessProperty() { return UElement_Type::Property___GroundingStrokeThickness::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UElement>
	{
		static inline UElement_Type UElementType { u8"UElement"N };

		static FType & Type()
		{
			return UElementType;
		}
	};

	FType & UI::UElement::StaticType()
	{
		return TStaticType<UI::UElement>().Type();
	}

	static TTypeRegister<UI::UElement> UElementTypeRegister { TStaticType<UI::UElement>().Type() };
}

// Generated code end.
