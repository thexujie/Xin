#include "PCH.h"
#include "Window.h"

#include "ToolTip.h"
#include "ContextMenu.h"

namespace Xin::UI
{
	UWindow::UWindow(FText Title, UWindow * Owner)
		: Title(Title), Owner(Owner)
	{
	}

	UWindow::~UWindow()
	{
		
	}

	void UWindow::OnConstruct()
	{
		UView::OnConstruct();

		FontFamily = FFontFace::Default.FaceName;
		FontSize = FFontFormat::Default.FontSize;
		FontWeight = FFontFace::Default.FontWeight;
		FontStyle = FFontFace::Default.FontStyle;

		Foreground = Colors::Black;
	}

	void UWindow::OnInitialize()
	{
		ScaleRatio = Desktop.GetScaleRatio();
		UView::OnInitialize();
	}

	void UWindow::OnFinalize()
	{
		ResetValue(UWindow::WindowSizeProperty());
		UnsetEvaluatedValue<FDimen2>(UElement::SizeProperty());
		HidePopups();

		UView::OnFinalize();
	}

	void UWindow::OnMeasured()
	{
		UView::OnMeasured();
	}

	IResourceRef UWindow::FindResource(FName Name, const FType & ResourceType)
	{
		if (IResourceRef Resource = UView::FindResource(Name, ResourceType))
			return Resource;

		if (UWindowRef OwnerWindow = Window.Owner.Lock())
		{
			IResourceRef Resource = OwnerWindow->FindResource(Name, ResourceType);
			return Resource;
		}

		return nullptr;
	}

	IResourceRef UWindow::FindResource(const FType & TargetType, const FType & ResourceType)
	{
		if (IResourceRef Resource = UView::FindResource(TargetType, ResourceType))
			return Resource;

		if (UWindowRef OwnerWindow = Window.Owner.Lock())
		{
			IResourceRef Resource = OwnerWindow->FindResource(TargetType, ResourceType);
			return Resource;
		}

		return nullptr;
	}

	IGraphics & UWindow::GetGraphics() const
	{
		if (UWindowRef OwnerWindow = Window.Owner.Lock())
			return OwnerWindow->GetGraphics();
		return *IDesktop::Default().GetDefaultGraphics();
	}

	UElement * UWindow::HitElement(FPoint Point) const
	{
		if (ContextMenuWindow && ContextMenuWindow->Shown)
			return ContextMenuWindow;
		else
			return UView::HitElement(Point);
	}

	void UWindow::PreKeyDown(FKeyboardEventArgs & Args)
	{
		if (ContextMenuWindow && ContextMenuWindow->Shown)
			ContextMenuWindow->PreKeyDown(Args);
		else
		{
#if XIN_DEBUG || 1
			if (Args.Key == EKeyCode::F7)
				DumpVisualTree(Window, 0);
			//else if (Args.Key == EKeyCode::L)
			//	DumpLogicalTree(Window, 0);
			//else if (Args.Key == EKeyCode::R)
			//	Window.InvalidateVisual();
			//else {}
#endif
			UView::PreKeyDown(Args);
		}
	}

	void UWindow::PreMouseMove(FMouseEventArgs & Args)
	{
		if (ContextMenuWindow && ContextMenuWindow->Shown)
			ContextMenuWindow->PreMouseMove(Args);
		else
			UView::PreMouseMove(Args);
	}

	void UWindow::Show()
	{
		if (Visibility != EVisibility::Visible)
			return;

		Initialize();
		Measure(Desktop.GetWorkingRect().Size);
		Arrange({ FPoint::Zero, DesiredSize });

		AssertExpr(!DesiredSize.AnyNaN());
		WindowSize = DesiredSize;

		IDesktop & Desktop = IDesktop::Default();
		if (!WindowHandle)
		{
			FWindowPlacement WindowPlacement;
			if (auto OwnerHostWindow = Owner ? Owner.Lock() : nullptr)
				WindowPlacement.Owner = OwnerHostWindow ? OwnerHostWindow->WindowHandle : nullptr;
			WindowPlacement.Title = Title;
			WindowPlacement.WindowStyle = WindowStyle;
			WindowPlacement.WindowRect = { WindowLocation, WindowSize };

			WindowPlacement.MouseActivatable = MouseActivatable;
			WindowPlacement.FrameThickness = FrameThickness;
			WindowHandle = Desktop.PlaceWindow(WindowPlacement);
			OnWindowPlaced();
		}
		else
			AssertExpr(&(WindowHandle->Desktop) == &Desktop);

		{
			FEventArgs Args;
			OnShowing(Args);
		}

		WindowHandle->ShowWindow(WindowShowMode, WindowShowPosition);
	}

	void UWindow::OnShowing(FEventArgs & Args)
	{
		Showing(Args);
	}

	void UWindow::Hide()
	{
		if (!Shown)
			return;

		if (WindowHandle)
			WindowHandle->HideWindow();
	}

	void UWindow::Close()
	{
		if (!Shown)
			return;

		{
			FEventArgs Args;
			OnClosing(Args);
			if (Args.Handled)
				return;
		}

		{
			FEventArgs Args;
			OnClosed(Args);
		}

		if (WindowHandle)
		{
			WindowHandle->CloseWindow();
			OnWindowDisplaced();
			WindowHandle = nullptr;
		}

		Finalize();
	}

	FPoint UWindow::LocalToGlobal(FPoint Point) const
	{
		return Point + WindowLocation;
	}

	FPoint UWindow::GlobalToLocal(FPoint Point) const
	{
		return Point - WindowLocation;
	}

	void UWindow::InvalidateMeasure()
	{
		UView::InvalidateMeasure();
		DispatchLayout();
	}

	void UWindow::InvalidateArrange()
	{
		UView::InvalidateArrange();
		DispatchLayout();
	}

	void UWindow::InvalidateVisual()
	{
		if (WindowHandle)
			WindowHandle->InvalidVisual();
	}

	void UWindow::InvalidateRect(FRect Rect)
	{
		if (WindowHandle)
			WindowHandle->InvalidateRect(Rect);
	}

	void UWindow::OnHidden()
	{
		HideContextMenu();
		UView::OnHidden();
	}

	void UWindow::OnClosing(FEventArgs & Args)
	{
		WindowBitmap = nullptr;
		WindowPainter = nullptr;
		Closing(Args);
	}

	void UWindow::OnClosed(FEventArgs & Args)
	{
		Shown = false;
		Closed(Args);

		if (ExitOnClose)
			FApplication::Instance().Exit(0);
	}

	void UWindow::UpdateCursor(ECursor Cursor, bool ForceUpdate)
	{
		if (WindowHandle)
			WindowHandle->SetCursor(Cursor, false);
	}

	void UWindow::UpdateCapture(bool Capture)
	{
		if (WindowHandle)
			WindowHandle->SetCapture(Capture);
	}

	void UWindow::ShowContextMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, const TFunction<void()> & OnClosed)
	{
		if (Element && Menu)
		{
			if (!ContextMenuWindow)
			{
				ContextMenuWindow = MakeRefer<UContextMenu>();
				ContextMenuWindow->Owner = this;
			}

			ContextMenuWindow->ShowMenu(Element, Menu, PlacementMode, OnClosed);
		}
		else
		{
			if (ContextMenuWindow)
				ContextMenuWindow->Close();
		}
	}

	void UWindow::HideContextMenu()
	{
		if (ContextMenuWindow)
		{
			ContextMenuWindow->Close();
			ContextMenuWindow = nullptr;
		}
	}

	void UWindow::ShowToolTip(UElement * Element, const FVariant & ToolTip)
	{
		if (Element && !ToolTipWindow)
		{
			ToolTipWindow = MakeRefer<UToolTip>();
			ToolTipWindow->Owner = this;
		}

		if (ToolTipWindow)
			ToolTipWindow->ShowElementToolTip(Element);
	}

	void UWindow::HideToolTip()
	{
		if (ToolTipWindow)
		{
			ToolTipWindow->Close();
			ToolTipWindow = nullptr;
		}
	}

	void UWindow::HidePopups()
	{
		HideContextMenu();
		HideToolTip();
	}

	void UWindow::OnPaintWindow()
	{
		if (!Shown)
			return;

		SizeU ClientSize = WindowSize;
		if (!WindowPainter)
		{
			WindowPainter = Graphics.CreateWindowPainter(WindowHandle->GetWindowHandle());
			if (!WindowPainter)
			{
				WindowPainter = Graphics.CreatePainter();
				WindowBitmap = Graphics.CreateBitmap(EFormat::B8G8R8A8UF, ClientSize, EBitmapUsage::RenderTarget);
			}
		}
		else if (WindowBitmap)
		{
			if (WindowBitmap->Size.Width < ClientSize.Width || WindowBitmap->Size.Height < ClientSize.Height)
			{
				WindowBitmap = nullptr;
				WindowBitmap = Graphics.CreateBitmap(EFormat::B8G8R8A8UF, ClientSize, EBitmapUsage::RenderTarget);
			}
		}

		WindowPainter->BeginPaint(WindowBitmap);
		WindowPainter->Clear(Colors::White);

		Paint(*WindowPainter);
		WindowPainter->EndPaint();

		if (WindowBitmap)
		{
			WindowHandle->PresentWindow(*WindowBitmap);
		}
	}

	void UWindow::OnWindowPlaced()
	{
		AssertExpr(WindowHandle);

		WindowHandle->OnSize = [this](SizeF Size) {  WindowSize = Size; };
		WindowHandle->OnMove = [this](PointF Position) {  WindowLocation = Position; };

		WindowHandle->OnState = [this](EWindowState State) {  WindowState = State; };
		WindowHandle->OnShown = [this](bool Shown_) {  Shown = Shown_; };
		WindowHandle->OnActive = [this](bool Active_)
		{
			Active = Active_;
		};

		WindowHandle->OnPaint = [this]() { OnPaintWindow(); };

		WindowHandle->OnFocus = [this](bool Focus_)
		{
			//if (!Focus_)
				//HidePopups();
		};

		WindowHandle->OnClose = [this]() { Close(); };

		WindowHandle->OnDestroy = [this]() { Close(); };

		WindowHandle->OnHitTest = [this](FPoint Point)
		{
			UElement * Element = HitElement(Point);
			if (!Element)
				return EHitResult::None;

			EHitResult HitResult = Element->HitTest(Element->WindowToLocal(Point));
			return HitResult;
		};

		WindowHandle->OnKeyDown = [this](EKeyCode KeyCode)
		{
			FKeyboardEventArgs KeyboardEventArgs { *this, KeyCode };
			OnKeyDown(KeyboardEventArgs);
		};

		WindowHandle->OnKeyUp = [this](EKeyCode KeyCode)
			{
				FKeyboardEventArgs KeyboardEventArgs { *this, KeyCode };
				OnKeyUp(KeyboardEventArgs);
			};

		WindowHandle->OnMouseDown = [this](EMouseButton MouseButton, FPoint Position)
		{
			HidePopups();
			if (UElement * Element = HitElement(Position))
			{
				//FConsole::WriteLine(u8"WM_LBUTTONDOWN hit {}"V, Element->Type().Name);
				FMouseButtonEventArgs MouseEventArgs { *Element, MouseButton };
				MouseEventArgs.Position = Element->WindowToLocal(Position);
				Element->PreMouseDown(MouseEventArgs);
			}
		};

		WindowHandle->OnMouseUp = [this](EMouseButton MouseButton, FPoint Position)
		{
			if (UElement * Element = HitElement(Position))
			{
				//FConsole::WriteLine(u8"WM_LBUTTONDOWN hit {}"V, Element->Type().Name);
				FMouseButtonEventArgs MouseEventArgs { *Element, MouseButton };
				MouseEventArgs.Position = Element->WindowToLocal(Position);
				Element->PreMouseUp(MouseEventArgs);
			}
		};

		WindowHandle->OnMouseMove = [this](FPoint Position)
		{
			UElement * Element = HitElement(Position);
			if (Element)
			{
				FMouseEventArgs MouseMoveArgs { *this };
				MouseMoveArgs.Position = Element->WindowToLocal(Position);
				Element->PreMouseMove(MouseMoveArgs);
			}

			Element = HitElement(Position);
			SetHoveredElement(Element, Position);
		};

		WindowHandle->OnMouseLeave = [this](FPoint Position)
		{
			HideToolTip();
			SetHoveredElement(nullptr, Position);
		};

		WindowHandle->OnMouseWhell = [this](FPoint Position, Vec2I Whell)
		{
			HidePopups();

			FMouseWhellEventArgs MouseWhellEventArgs { *this, Whell };
			if (UElementRef Element = HoveredElement.Lock())
				Element->PreMouseWhell(MouseWhellEventArgs);

			UElement * Element = HitElement(Position);
			SetHoveredElement(Element, Position);
		};
	}

	void UWindow::OnWindowDisplaced()
	{
		WindowBitmap = nullptr;
		WindowPainter = nullptr;

		WindowHandle->OnSize = nullptr;
		WindowHandle->OnMove = nullptr;

		WindowHandle->OnState = nullptr;
		WindowHandle->OnShown = nullptr;
		WindowHandle->OnActive = nullptr;

		WindowHandle->OnPaint = nullptr;

		WindowHandle->OnFocus = nullptr;

		WindowHandle->OnClose = nullptr;
		WindowHandle->OnDestroy = nullptr;

		WindowHandle->OnHitTest = nullptr;

		WindowHandle->OnKeyDown = nullptr;

		WindowHandle->OnMouseDown = nullptr;

		WindowHandle->OnMouseUp = nullptr;

		WindowHandle->OnMouseMove = nullptr;

		WindowHandle->OnMouseLeave = nullptr;

		WindowHandle->OnMouseWhell = nullptr;
	}

	void UWindow::OnWindowLocationChanged(FPoint WindowLocationBefore)
	{
		if (WindowHandle)
			WindowHandle->SetWindowRect({ WindowLocation, WindowSize });
	}

	void UWindow::OnWindowSizeChanged(FSize WindowSizeBefore)
	{
		//FDimen2 BaseSize = GetBaseValueT<FDimen2>(SizeProperty(), FDimen2::NaN);
		//if (!BaseSize.AnyNaN())
		//	Size = WindowSize;
		//else

		SetEvaluatedValue<FDimen2>(UElement::SizeProperty(), WindowSize);

		if (!Initialized)
			return;

		//MeasureDone = false;
		Measure(AvailableSize);
		Arrange({ FPoint::Zero, DesiredSize });

		if (WindowHandle)
			WindowHandle->SetWindowRect({ WindowLocation, WindowSize });
	}
}
