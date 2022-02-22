// B9203C403F719D7478AE2C49CBE0ED33 2025-04-03 18:06:22
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Window.h"

namespace Xin
{
	class UWindow_Type : public TClass<UI::UWindow>
	{
	public:
		using UWindow = UI::UWindow;

		// --- 0 Constructors
		static inline const TConstructor<UWindow, FText, UI::UWindow *> Constructor___FText__UWindow_ptr { u8"!Constructor___FText__UWindow_ptr"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FText__UWindow_ptr,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UWindow, FType &()> Method { u8"Type"N, &UWindow::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnConstruct"N, &UWindow::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnInitialize"N, &UWindow::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnFinalize"N, &UWindow::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnMeasured
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnMeasured"N, &UWindow::OnMeasured, FMetadata::None };
		};
		struct Method___TReferPtr__IResource_____FindResource__FName__FType_const_lref
		{
			static inline const TMethod<UWindow, UI::IResourceRef(FName, FType const &)> Method { u8"FindResource"N, &UWindow::FindResource, FMetadata::None };
		};
		struct Method___TReferPtr__IResource_____FindResource__FType_const_lref__FType_const_lref
		{
			static inline const TMethod<UWindow, UI::IResourceRef(FType const &, FType const &)> Method { u8"FindResource"N, &UWindow::FindResource, FMetadata::None };
		};
		struct Method___void___OnKeyDown__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UWindow, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyDown"N, &UWindow::OnKeyDown, FMetadata::None };
		};
		struct Method___IGraphics_lref___GetGraphics____const
		{
			static inline const TMethod<UWindow, IGraphics &()> Method { u8"GetGraphics"N, &UWindow::GetGraphics, FMetadata::None };
		};
		struct Method___IDesktop_lref___GetDesktop____const
		{
			static inline const TMethod<UWindow, IDesktop &()> Method { u8"GetDesktop"N, &UWindow::GetDesktop, FMetadata::None };
		};
		struct Method___UWindow_ptr___GetWindow____const
		{
			static inline const TMethod<UWindow, UI::UWindow *()> Method { u8"GetWindow"N, &UWindow::GetWindow, FMetadata::None };
		};
		struct Method___TPointerPtr__FLayoutManager_____GetLayoutManager
		{
			static inline const TMethod<UWindow, TPointerPtr<UI::FLayoutManager>()> Method { u8"GetLayoutManager"N, &UWindow::GetLayoutManager, FMetadata::None };
		};
		struct Method___TVec2__float64_____LocalToGlobal__TVec2__float64______const
		{
			static inline const TMethod<UWindow, FPoint(FPoint)> Method { u8"LocalToGlobal"N, &UWindow::LocalToGlobal, FMetadata::None };
		};
		struct Method___TVec2__float64_____GlobalToLocal__TVec2__float64______const
		{
			static inline const TMethod<UWindow, FPoint(FPoint)> Method { u8"GlobalToLocal"N, &UWindow::GlobalToLocal, FMetadata::None };
		};
		struct Method___void___InvalidateMeasure
		{
			static inline const TMethod<UWindow, void()> Method { u8"InvalidateMeasure"N, &UWindow::InvalidateMeasure, FMetadata::None };
		};
		struct Method___void___InvalidateArrange
		{
			static inline const TMethod<UWindow, void()> Method { u8"InvalidateArrange"N, &UWindow::InvalidateArrange, FMetadata::None };
		};
		struct Method___void___InvalidateVisual
		{
			static inline const TMethod<UWindow, void()> Method { u8"InvalidateVisual"N, &UWindow::InvalidateVisual, FMetadata::None };
		};
		struct Method___void___InvalidateRect__TVec4__float64__
		{
			static inline const TMethod<UWindow, void(FRect)> Method { u8"InvalidateRect"N, &UWindow::InvalidateRect, FMetadata::None };
		};
		struct Method___void___OnHidden
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnHidden"N, &UWindow::OnHidden, FMetadata::None };
		};
		struct Method___void___Show
		{
			static inline const TMethod<UWindow, void()> Method { u8"Show"N, &UWindow::Show, FMetadata::None };
		};
		struct Method___void___OnShowing__FEventArgs_lref
		{
			static inline const TMethod<UWindow, void(FEventArgs &)> Method { u8"OnShowing"N, &UWindow::OnShowing, FMetadata::None };
		};
		struct Method___void___Hide
		{
			static inline const TMethod<UWindow, void()> Method { u8"Hide"N, &UWindow::Hide, FMetadata::None };
		};
		struct Method___void___Close
		{
			static inline const TMethod<UWindow, void()> Method { u8"Close"N, &UWindow::Close, FMetadata::None };
		};
		struct Method___void___OnClosing__FEventArgs_lref
		{
			static inline const TMethod<UWindow, void(FEventArgs &)> Method { u8"OnClosing"N, &UWindow::OnClosing, FMetadata::None };
		};
		struct Method___void___OnClosed__FEventArgs_lref
		{
			static inline const TMethod<UWindow, void(FEventArgs &)> Method { u8"OnClosed"N, &UWindow::OnClosed, FMetadata::None };
		};
		struct Method___void___OnActive
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnActive"N, &UWindow::OnActive, FMetadata::None };
		};
		struct Method___void___OnInactive
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnInactive"N, &UWindow::OnInactive, FMetadata::None };
		};
		struct Method___void___OnActiveChanged__bool
		{
			static inline const TMethod<UWindow, void(bool)> Method { u8"OnActiveChanged"N, &UWindow::OnActiveChanged, FMetadata::None };
		};
		struct Method___void___UpdateCursor__ECursor__bool
		{
			static inline const TMethod<UWindow, void(ECursor, bool)> Method { u8"UpdateCursor"N, &UWindow::UpdateCursor, FMetadata::None };
		};
		struct Method___void___UpdateCapture__bool
		{
			static inline const TMethod<UWindow, void(bool)> Method { u8"UpdateCapture"N, &UWindow::UpdateCapture, FMetadata::None };
		};
		struct Method___void___ShowContextMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void_________const_lref
		{
			static inline const TMethod<UWindow, void(UI::UElement *, FVariant const &, EPopupPosition, TFunction<void()> const &)> Method { u8"ShowContextMenu"N, &UWindow::ShowContextMenu, FMetadata::None };
		};
		struct Method___void___HideContextMenu
		{
			static inline const TMethod<UWindow, void()> Method { u8"HideContextMenu"N, &UWindow::HideContextMenu, FMetadata::None };
		};
		struct Method___void___ShowToolTip__UElement_ptr__FVariant_const_lref
		{
			static inline const TMethod<UWindow, void(UI::UElement *, FVariant const &)> Method { u8"ShowToolTip"N, &UWindow::ShowToolTip, FMetadata::None };
		};
		struct Method___void___HideToolTip
		{
			static inline const TMethod<UWindow, void()> Method { u8"HideToolTip"N, &UWindow::HideToolTip, FMetadata::None };
		};
		struct Method___void___HidePopups
		{
			static inline const TMethod<UWindow, void()> Method { u8"HidePopups"N, &UWindow::HidePopups, FMetadata::None };
		};
		struct Method___void___OnPaintWindow
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnPaintWindow"N, &UWindow::OnPaintWindow, FMetadata::None };
		};
		struct Method___void___OnWindowPlaced
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnWindowPlaced"N, &UWindow::OnWindowPlaced, FMetadata::None };
		};
		struct Method___void___OnWindowDisplaced
		{
			static inline const TMethod<UWindow, void()> Method { u8"OnWindowDisplaced"N, &UWindow::OnWindowDisplaced, FMetadata::None };
		};
		struct Method___void___OnWindowLocationChanged__TVec2__float64__
		{
			static inline const TMethod<UWindow, void(FPoint)> Method { u8"OnWindowLocationChanged"N, &UWindow::OnWindowLocationChanged, FMetadata::None };
		};
		struct Method___void___OnWindowSizeChanged__TVec2__float64__
		{
			static inline const TMethod<UWindow, void(FSize)> Method { u8"OnWindowSizeChanged"N, &UWindow::OnWindowSizeChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnMeasured::Method,
			Method___TReferPtr__IResource_____FindResource__FName__FType_const_lref::Method,
			Method___TReferPtr__IResource_____FindResource__FType_const_lref__FType_const_lref::Method,
			Method___void___OnKeyDown__FKeyboardEventArgs_lref::Method,
			Method___IGraphics_lref___GetGraphics____const::Method,
			Method___IDesktop_lref___GetDesktop____const::Method,
			Method___UWindow_ptr___GetWindow____const::Method,
			Method___TPointerPtr__FLayoutManager_____GetLayoutManager::Method,
			Method___TVec2__float64_____LocalToGlobal__TVec2__float64______const::Method,
			Method___TVec2__float64_____GlobalToLocal__TVec2__float64______const::Method,
			Method___void___InvalidateMeasure::Method,
			Method___void___InvalidateArrange::Method,
			Method___void___InvalidateVisual::Method,
			Method___void___InvalidateRect__TVec4__float64__::Method,
			Method___void___OnHidden::Method,
			Method___void___Show::Method,
			Method___void___OnShowing__FEventArgs_lref::Method,
			Method___void___Hide::Method,
			Method___void___Close::Method,
			Method___void___OnClosing__FEventArgs_lref::Method,
			Method___void___OnClosed__FEventArgs_lref::Method,
			Method___void___OnActive::Method,
			Method___void___OnInactive::Method,
			Method___void___OnActiveChanged__bool::Method,
			Method___void___UpdateCursor__ECursor__bool::Method,
			Method___void___UpdateCapture__bool::Method,
			Method___void___ShowContextMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void_________const_lref::Method,
			Method___void___HideContextMenu::Method,
			Method___void___ShowToolTip__UElement_ptr__FVariant_const_lref::Method,
			Method___void___HideToolTip::Method,
			Method___void___HidePopups::Method,
			Method___void___OnPaintWindow::Method,
			Method___void___OnWindowPlaced::Method,
			Method___void___OnWindowDisplaced::Method,
			Method___void___OnWindowLocationChanged__TVec2__float64__::Method,
			Method___void___OnWindowSizeChanged__TVec2__float64__::Method,
		};

		// --- 2 Fields
		struct Field___ContextMenuWindow
		{
			static inline const TField Field { u8"ContextMenuWindow"N, &UWindow::ContextMenuWindow, FMetadata::None };
		};
		struct Field___ToolTipWindow
		{
			static inline const TField Field { u8"ToolTipWindow"N, &UWindow::ToolTipWindow, FMetadata::None };
		};
		struct Field___Title
		{
			static inline const TField Field { u8"Title"N, &UWindow::Title, FMetadata::None };
		};
		struct Field___Owner
		{
			static inline const TField Field { u8"Owner"N, &UWindow::Owner, FMetadata::None };
		};
		struct Field___WindowShowPosition
		{
			static inline const TField Field { u8"WindowShowPosition"N, &UWindow::WindowShowPosition, FMetadata::None };
		};
		struct Field___WindowShowMode
		{
			static inline const TField Field { u8"WindowShowMode"N, &UWindow::WindowShowMode, FMetadata::None };
		};
		struct Field___WindowStyle
		{
			static inline const TField Field { u8"WindowStyle"N, &UWindow::WindowStyle, FMetadata::None };
		};
		struct Field___MouseActivatable
		{
			static inline const TField Field { u8"MouseActivatable"N, &UWindow::MouseActivatable, FMetadata::None };
		};
		struct Field___WindowState
		{
			static inline const TField Field { u8"WindowState"N, &UWindow::WindowState, FMetadata::None };
		};
		struct Field___ExitOnClose
		{
			static inline const TField Field { u8"ExitOnClose"N, &UWindow::ExitOnClose, FMetadata::None };
		};
		struct Field___WindowHandle
		{
			static inline const TField Field { u8"WindowHandle"N, &UWindow::WindowHandle, FMetadata::None };
		};
		struct Field___WindowPainter
		{
			static inline const TField Field { u8"WindowPainter"N, &UWindow::WindowPainter, FMetadata::None };
		};
		struct Field___WindowBitmap
		{
			static inline const TField Field { u8"WindowBitmap"N, &UWindow::WindowBitmap, FMetadata::None };
		};
		struct Field___Showing
		{
			static inline const TField Field { u8"Showing"N, &UWindow::Showing, FMetadata::None };
		};
		struct Field___Closing
		{
			static inline const TField Field { u8"Closing"N, &UWindow::Closing, FMetadata::None };
		};
		struct Field___Closed
		{
			static inline const TField Field { u8"Closed"N, &UWindow::Closed, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ContextMenuWindow::Field,
			Field___ToolTipWindow::Field,
			Field___Title::Field,
			Field___Owner::Field,
			Field___WindowShowPosition::Field,
			Field___WindowShowMode::Field,
			Field___WindowStyle::Field,
			Field___MouseActivatable::Field,
			Field___WindowState::Field,
			Field___ExitOnClose::Field,
			Field___WindowHandle::Field,
			Field___WindowPainter::Field,
			Field___WindowBitmap::Field,
			Field___Showing::Field,
			Field___Closing::Field,
			Field___Closed::Field,
		};

		// --- 3 Properties
		struct Property___Active
		{
			static inline const TProxyProperty Property { u8"Active"N, &UWindow::__Active, &UWindow::__GetActive, &UWindow::__SetActive, UWindow::FPropertyMetadata::None };
		};
		struct Property___WindowChrome
		{
			static inline const TAttachedProperty<UWindow, UI::FWindowChrome> Property { u8"WindowChrome"N, UWindow::__WindowChrome__, UWindow::FPropertyMetadata::None };
		};
		struct Property___FrameThickness
		{
			static inline const TDependencyProperty Property { u8"FrameThickness"N, &UWindow::__FrameThickness, &UWindow::__GetFrameThickness, &UWindow::__SetFrameThicknessValue, UWindow::__FrameThickness__, UWindow::FPropertyMetadata::None };
		};
		struct Property___WindowLocation
		{
			static inline const TDependencyProperty Property { u8"WindowLocation"N, &UWindow::__WindowLocation, &UWindow::__GetWindowLocation, &UWindow::__SetWindowLocationValue, UWindow::__WindowLocation__, UWindow::FPropertyMetadata::None };
		};
		struct Property___WindowSize
		{
			static inline const TDependencyProperty Property { u8"WindowSize"N, &UWindow::__WindowSize, &UWindow::__GetWindowSize, &UWindow::__SetWindowSizeValue, UWindow::__WindowSize__, UWindow::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Active::Property,
			Property___WindowChrome::Property,
			Property___FrameThickness::Property,
			Property___WindowLocation::Property,
			Property___WindowSize::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UView>(),
		};

		// --- 5 Constructor
		UWindow_Type(FName Name = u8"UWindow"N) : TClass(Name, Classof<UI::UView>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::UWindow::ActiveProperty() { return UWindow_Type::Property___Active::Property; }

	const TProperty<UI::FWindowChrome> & UI::UWindow::WindowChromeProperty() { return UWindow_Type::Property___WindowChrome::Property; }

	const TProperty<FThickness> & UI::UWindow::FrameThicknessProperty() { return UWindow_Type::Property___FrameThickness::Property; }

	const TProperty<FPoint> & UI::UWindow::WindowLocationProperty() { return UWindow_Type::Property___WindowLocation::Property; }

	const TProperty<FSize> & UI::UWindow::WindowSizeProperty() { return UWindow_Type::Property___WindowSize::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UWindow>
	{
		static inline UWindow_Type UWindowType { u8"UWindow"N };

		static FType & Type()
		{
			return UWindowType;
		}
	};

	FType & UI::UWindow::StaticType()
	{
		return TStaticType<UI::UWindow>().Type();
	}

	static TTypeRegister<UI::UWindow> UWindowTypeRegister { TStaticType<UI::UWindow>().Type() };
}

// Generated code end.
