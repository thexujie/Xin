#pragma once

#include "Xin.UI.Types.h"
#include "View.h"

namespace Xin::UI
{
	class UContextMenu;
	using UContextMenuRef = TReferPtr<UContextMenu>;

	class UToolTip;
	using UToolTipRef = TReferPtr<UToolTip>;

	using UWindowRef = TReferPtr<UWindow>;
	using UWindowObj = TEntryPtr<UWindow>;


	class UI_API FWindowChrome
	{
	public:
		ProxyPropertyRW(dimenx, CaptionHeight) = 0;
		ProxyPropertyRW(FThickness, BorderThickness) = {};
		ProxyPropertyRW(FThickness, GlassThickness) = {};
	};

	class UI_API UWindow : public UView
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UWindow(FText Title = FText::None, UWindow * Owner = nullptr);
		~UWindow();

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		void OnMeasured() override;

	public:
		IResourceRef FindResource(FName Name, const FType & ResourceType) override;
		IResourceRef FindResource(const FType & TargetType, const FType & ResourceType) override;

	public:
		UElement * HitElement(FPoint Point) const override;
		void PreKeyDown(FKeyboardEventArgs & Args) override;
		void PreMouseMove(FMouseEventArgs & Args) override;

	public:
		IGraphics & GetGraphics() const override;

		IDesktop & GetDesktop() const override { return IDesktop::Default(); }

		UWindow * GetWindow() const final { return ConstCast(this); }

		TPointerPtr<FLayoutManager> GetLayoutManager() final { return &LayoutManager; }

	public:
		FPoint LocalToGlobal(FPoint Point) const override;
		FPoint GlobalToLocal(FPoint Point) const override;

	public:
		void InvalidateMeasure() override;
		void InvalidateArrange() override;
		void InvalidateVisual() override;
		void InvalidateRect(FRect Rect) override;
		void OnHidden() override;

	public:
		void Show();
		virtual void OnShowing(FEventArgs & Args);
		void Hide();
		void Close();

	public:
		virtual void OnClosing(FEventArgs & Args);
		virtual void OnClosed(FEventArgs & Args);

	public:
		virtual void OnActive() {}
		virtual void OnInactive(){}
		virtual void OnActiveChanged(bool ActiveBefore) {}
		ProxyPropertyRW(bool, Active) = false;

	public:
		void UpdateCursor(ECursor Cursor, bool ForceUpdate) override;
		void UpdateCapture(bool Capture) override;

	public:
		void ShowContextMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, const TFunction<void()> & OnClosed = nullptr) override;
		void HideContextMenu();
		UContextMenuRef ContextMenuWindow;

	public:
		void ShowToolTip(UElement * Element, const FVariant & ToolTip) override;
		void HideToolTip();
		UToolTipRef ToolTipWindow;

	public:
		void HidePopups();

	public:
		AttachedPropertyRW(FWindowChrome, WindowChrome) = noval;
		DependencyPropertyRW(FThickness, FrameThickness) = noval;

	public:
		FText Title;
		UWindowObj Owner;
		EWindowShowPosition WindowShowPosition = EWindowShowPosition::None;
		EWindowShowMode WindowShowMode = EWindowShowMode::None;
		EWindowStyle WindowStyle = EWindowStyle::None;
		bool MouseActivatable = true;
		EWindowState WindowState = EWindowState::Closed;
		bool ExitOnClose = false;

		virtual void OnPaintWindow();
		void OnWindowPlaced();
		void OnWindowDisplaced();

	public:
		void OnWindowLocationChanged(FPoint WindowLocationBefore);
		DependencyPropertyRW(FPoint, WindowLocation) = FPoint::NaN;

		void OnWindowSizeChanged(FSize WindowSizeBefore);
		DependencyPropertyRW(FSize, WindowSize) = FSize::NaN;

	protected:
		FLayoutManager LayoutManager;

	public:
		IWindowHandleRef WindowHandle;
		IPainterRef WindowPainter;
		IBitmapRef WindowBitmap;

	public:
		FDelegate Showing;
		FDelegate Closing;
		FDelegate Closed;
	};
}
