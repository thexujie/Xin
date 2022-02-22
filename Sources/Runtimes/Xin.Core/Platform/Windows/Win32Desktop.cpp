#include "PCH.h"
#include "Win32Desktop.h"
//#include <dwmapi.h>

namespace Xin
{
	IDesktop & IDesktop::Default()
	{
		static TReferPtr<IDesktop> GWindowContext = MakeRefer<Windows::FWin32Desktop>();
		return *GWindowContext;
	}
}

namespace Xin::Windows
{
	static uint32 HitTestBorderThickness(FPoint LocalPoint, FSize WindowActualSize, FThickness FrameThickness)
	{
		uint32 HitTestValues[9] = { HTTOPLEFT, HTTOP, HTTOPRIGHT, HTLEFT, HTCLIENT, HTRIGHT, HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT };

		uintx TestColIndex;
		if (LocalPoint.X < 0)
			TestColIndex = NullIndex;
		else if (LocalPoint.X < FrameThickness.Left)
			TestColIndex = 0;
		else if (LocalPoint.X < WindowActualSize.Width - FrameThickness.Right)
			TestColIndex = 1;
		else if (LocalPoint.X < WindowActualSize.Width)
			TestColIndex = 2;
		else
			TestColIndex = NullIndex;

		uintx TestRowIndex;
		if (LocalPoint.Y < 0)
			TestRowIndex = NullIndex;
		else if (LocalPoint.Y < FrameThickness.Top)
			TestRowIndex = 0;
		else if (LocalPoint.Y < WindowActualSize.Height - FrameThickness.Bottom)
			TestRowIndex = 1;
		else if (LocalPoint.Y < WindowActualSize.Height)
			TestRowIndex = 2;
		else
			TestRowIndex = NullIndex;

		if (TestRowIndex == NullIndex || TestColIndex == NullIndex)
			return HTNOWHERE;
		return HitTestValues[TestRowIndex * 3 + TestColIndex];
	}

	//static FRect AdjustWindowRect(const FRect & WindowRect, EWindowStyle WindowStyle)
	//{
	//	RECT Rect = { 0, 0, (LONG)WindowRect.Width, (LONG)WindowRect.Height };
	//	switch (WindowStyle)
	//	{
	//	default:
	//	case EWindowStyle::None:
	//		AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, FALSE);
	//		return { (dimenx)Rect.left, (dimenx)Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top };
	//	case EWindowStyle::Frameless:
	//		::InflateRect(&WindowRect, DesktopContext.BorderThickness, DesktopContext.BorderThickness);
	//		break;
	//	case EWindowStyle::Chrome:
	//		break;
	//	}
	//}
	const wchar_t WindowClassName[] = L"Window.E9B39421-7D98-419E-BABA-2A27896B7DCD";
	const wchar_t GuidWindowPointer[] = L"Window.Prop.3846C5AA-E4A6-42C3-B7E7-ED869E63FE8F";

	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		//if (Message == WM_CREATE)
		if (Message == WM_NCCREATE)
		{
			CREATESTRUCT & CreateStruct = InterpretCastRef<CREATESTRUCT>((void *)lParam);
			FWin32Desktop * DesktopWindow = InterpretCast<FWin32Desktop>(CreateStruct.lpCreateParams);
			//SetPropW(HWND(hWnd), GuidWindowPointer, (HANDLE)(void *)DesktopWindow);
			::SetWindowLongPtrW(hWnd, 0, (LONG_PTR)DesktopWindow);
		}

		//FDesktopWindow * WindowHandle = InterpretCast<FDesktopWindow>(GetPropW(hWnd, GuidWindowPointer));
		FWin32Window * Win32Window = (FWin32Window *)GetWindowLongPtrW(hWnd, 0);
		if (!Win32Window)
			return DefWindowProcW(hWnd, Message, wParam, lParam);

		//return DefWindowProcW(hWnd, Message, wParam, lParam);
		return Win32Window->WindowProc(hWnd, Message, wParam, lParam);
	}

	FWin32Window::FWin32Window(FWin32Desktop & Win32Desktop, const FWindowPlacement & WindowPlacement)
		: IWindowHandle(Win32Desktop), Win32Desktop(Win32Desktop), WindowPlacement(WindowPlacement) {}

	FWin32Window::~FWin32Window()
	{
		if (hWindow)
		{
			::DestroyWindow(hWindow);
			hWindow = nullptr;
		}
	}

	void FWin32Window::ShowWindow(EWindowShowMode WindowShowMode, EWindowShowPosition WindowShowPosition)
	{
		if (!hWindow)
		{
			HINSTANCE hInstance = (HINSTANCE)FSystem::Instance();

			static bool WindowClassRegistered = false;
			if (!WindowClassRegistered)
			{
				WindowClassRegistered = true;

				WNDCLASSEXW WindowClassExW = { sizeof(WNDCLASSEXW) };
				WindowClassExW.cbSize = sizeof(WNDCLASSEXW);
				WindowClassExW.style = 0;
				WindowClassExW.lpfnWndProc = MainWndProc;
				WindowClassExW.cbClsExtra = 0;
				WindowClassExW.cbWndExtra = sizeof(LONG_PTR);
				WindowClassExW.hInstance = hInstance;
				WindowClassExW.hIcon = NULL;
				WindowClassExW.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
				//WindowClassExW.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
				WindowClassExW.lpszMenuName = NULL;
				WindowClassExW.lpszClassName = WindowClassName;
				WindowClassExW.hIconSm = NULL;

				RegisterClassExW(&WindowClassExW);
			}

			WindowStyleEx = 0;
			if (WindowPlacement.WindowStyle == EWindowStyle::None)
			{
				WindowStyle = WS_OVERLAPPEDWINDOW;
			}
			else
			{
				//WindowStyle |= WS_VISIBLE;

				//WindowStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
				//WindowStyle |= WS_THICKFRAME;
				//WindowStyle |= WS_DLGFRAME;

				if (WindowPlacement.WindowStyle == EWindowStyle::Normal)
				{
					WindowStyle = WS_OVERLAPPED;
					WindowStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
					WindowStyle |= WS_THICKFRAME;
				}
				else
				{
					WindowStyle = WS_POPUP;
				}
				WindowStyle |= WS_BORDER;

				//WindowStyleEx = WS_EX_DLGMODALFRAME;
			}

			HWND hWndParent = NULL;
			if (auto ParentWindow = WindowPlacement.Owner.Lock())
				hWndParent = HWND(ParentWindow->GetWindowHandle());

			//If x is set to CW_USEDEFAULT, the system selects the default position for the window's upper-left corner and ignores the y parameter.
			//CW_USEDEFAULT is valid only for overlapped windows; if it is specified for a pop-up or child window, the x and y parameters are set to zero.
			RectI WindowRect { PointI::Zero, SizeI(WindowPlacement.WindowRect.Size) };

			if (WindowShowPosition == EWindowShowPosition::ScreenCenter)
			{
				SizeI ScreenSize = Win32Desktop.GetWorkingRect().Size;
				WindowRect.Position = { (ScreenSize.Width - WindowRect.Width) / 2, (ScreenSize.Height - WindowRect.Height) / 2 };
			}
			else if (WindowPlacement.WindowRect.Position.AnyNaN())
			{
				if (hWndParent)
				{
					SizeI ScreenSize = Win32Desktop.GetWorkingRect().Size;
					WindowRect.Position = { (ScreenSize.Width - WindowRect.Width) / 2, (ScreenSize.Height - WindowRect.Height) / 2 };
				}
				else
				{
					WindowRect.Position = { CW_DEFAULT, 0 };
				}
			}
			else
			{
				WindowRect.Position = PointI(WindowPlacement.WindowRect.Position);
			}

			DecideWindowRect(WindowRect);

			FWString Title = FEncoding::ToWide(WindowPlacement.Title.Resolve());
			hWindow = CreateWindowExW(
				0, WindowClassName, Title.Data, WindowStyle,
				WindowRect.X, WindowRect.Y, WindowRect.Width, WindowRect.Height,
				hWndParent, NULL, hInstance, (LPVOID)this);
			//::ImmAssociateContext(HWND(hWindow), NULL);
		}
		else
		{
			//uint32_t SWPFlags = /*SWP_FRAMECHANGED |*/ SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOOWNERZORDER;
			//RectI WindowRectBefore = Windows::GetWindowRect(hWindow);
			//HWND hWindowInsert = NULL;

			RectI WindowRect(WindowPlacement.WindowRect);
			DecideWindowRect(WindowRect);

			//if (WindowRect.Position != WindowRectBefore.Position)
			//	SWPFlags &= ~SWP_NOMOVE;
			//if (WindowRect.Size != WindowRectBefore.Size)
			//	SWPFlags &= ~SWP_NOSIZE;
			//::SetWindowPos(hWindow, hWindowInsert, WindowRect.Left, WindowRect.Top, WindowRect.Width, WindowRect.Height, SWPFlags);
			//::MoveWindow(hWindow, WindowRect.Left, WindowRect.Top, WindowRect.Width, WindowRect.Height, TRUE);
		}

		switch (WindowShowMode)
		{
		case EWindowShowMode::NoneActivate:
			::ShowWindow(HWND(hWindow), SW_SHOWNOACTIVATE);
			break;
		default:
		case EWindowShowMode::None:
			::ShowWindow(HWND(hWindow), SW_SHOWNORMAL);
			break;
		}

		::UpdateWindow(HWND(hWindow));
	}

	void FWin32Window::HideWindow()
	{
		if (!hWindow)
			return;

		if (TrackingMouse)
		{
			TrackingMouse = false;
			OnMouseLeave(FPoint::Infinity);
		}

		::ShowWindow(HWND(hWindow), SW_HIDE);
		//Window.Shown = false;
	}

	void FWin32Window::CloseWindow()
	{
		if (!hWindow)
			return;

		if (TrackingMouse)
		{
			TrackingMouse = false;
			OnMouseLeave(FPoint::Infinity);
		}

		//Window.Shown = false;
		::DestroyWindow(hWindow);
		hWindow = NULL;
	}

	void FWin32Window::PresentWindow(IBitmap & Bitmap)
	{
		SizeU ClientSize = Windows::GetClientSize(hWindow);
		if (FLockedBitmap LockedBitmap = Bitmap.Lock(EBitmapLock::Read))
		{
			BITMAPINFO Bitmapinfo {};
			Bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			Bitmapinfo.bmiHeader.biWidth = int32(LockedBitmap.Pitch > 0 ? LockedBitmap.Pitch / 4 : Bitmap.Size.Width);
			Bitmapinfo.bmiHeader.biHeight = -int32(Bitmap.Size.Height); // top-down image
			Bitmapinfo.bmiHeader.biPlanes = 1;
			Bitmapinfo.bmiHeader.biBitCount = 32;
			Bitmapinfo.bmiHeader.biCompression = BI_RGB;
			Bitmapinfo.bmiHeader.biSizeImage = 0;

			HDC hdcWindow = ::GetDC(HWND(hWindow));
			::SetDIBitsToDevice(hdcWindow, 0, 0, ClientSize.Width, ClientSize.Height, 0, 0, 0, ClientSize.Height, LockedBitmap.Bytes, &Bitmapinfo, DIB_RGB_COLORS);
			::ReleaseDC(HWND(hWindow), hdcWindow);
		}
	}

	void FWin32Window::InvalidVisual()
	{
		//if (hWindow)
		//	::InvalidateRect(hWindow, NULL, FALSE);

		if (hWindow && !Refreshing)
		{
			++Refreshing;
			::PostMessageW(hWindow, Win32Desktop.RefreshMessage, 0, 0);
		}
	}

	void FWin32Window::InvalidateRect(FRect Rect)
	{
		if (hWindow && !Refreshing)
		{
			++Refreshing;
			::PostMessageW(hWindow, Win32Desktop.RefreshMessage, 0, 0);
		}
	}

	RectI FWin32Window::GetWorkingRect() const
	{
		HMONITOR hMonitor = MonitorFromWindow(hWindow, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEXW MonitorInfoExW = { sizeof(MONITORINFOEXW) };
		GetMonitorInfoW(hMonitor, &MonitorInfoExW);
		return
		{
			MonitorInfoExW.rcWork.left,
			MonitorInfoExW.rcWork.top,
			MonitorInfoExW.rcWork.right - MonitorInfoExW.rcWork.left,
			MonitorInfoExW.rcWork.bottom - MonitorInfoExW.rcWork.top
		};
	}

	void FWin32Window::SetWindowRect(const FRect & Rect)
	{
		RectI WindowRect {};
		if (Rect.Position.AnyNaN())
			WindowRect.Position = { CW_USEDEFAULT, CW_USEDEFAULT };
		else
			WindowRect.Position = PointI(Rect.Position);

		if (!Rect.Size.AnyNaN())
			WindowRect.Size = SizeI(Rect.Size);

		if (RectF(WindowRect) != WindowPlacement.WindowRect)
		{
			WindowPlacement.WindowRect = RectF(WindowRect);

			if (hWindow)
			{
				DecideWindowRect(WindowRect);
				::MoveWindow(hWindow, WindowRect.Left, WindowRect.Top, WindowRect.Width, WindowRect.Height, TRUE);
			}
		}
	}

	void FWin32Window::SetCapture(bool Capture)
	{
		AssertExpr(hWindow);
		if (Capture)
			::SetCapture(hWindow);
		else
			::ReleaseCapture();
	}

	void FWin32Window::SetCursor(ECursor Cursor, bool ForceUpdate)
	{
		if (!ForceUpdate && CurrentCursor == Cursor)
			return;

		CurrentCursor = Cursor;

		switch (Cursor)
		{
		case ECursor::None:
			::SetCursor(::LoadCursorW(NULL, IDC_ARROW));
			break;
		case ECursor::Arrow:
			::SetCursor(::LoadCursorW(NULL, IDC_ARROW));
			break;
		case ECursor::Cross:
			::SetCursor(::LoadCursorW(NULL, IDC_CROSS));
			break;
		case ECursor::Hand:
			::SetCursor(::LoadCursorW(NULL, IDC_HAND));
			break;
		case ECursor::Help:
			::SetCursor(::LoadCursorW(NULL, IDC_HELP));
			break;
		case ECursor::Hiden:
			::SetCursor(NULL);
			break;
		case ECursor::IBeam:
			::SetCursor(::LoadCursorW(NULL, IDC_IBEAM));
			break;
		//case ECursor::Pen:
		//	::SetCursor(::LoadCursorW(NULL, IDC_SIZEWE));
		//	break;
		case ECursor::SizeAll:
			::SetCursor(::LoadCursorW(NULL, IDC_SIZEALL));
			break;
		case ECursor::SizeNESW:
			::SetCursor(::LoadCursorW(NULL, IDC_SIZENESW));
			break;
		case ECursor::SizeVertical:
			::SetCursor(::LoadCursorW(NULL, IDC_SIZENS));
			break;
		case ECursor::SizeNWSE:
			::SetCursor(::LoadCursorW(NULL, IDC_SIZENWSE));
			break;
		case ECursor::SizeHorizontal:
			::SetCursor(::LoadCursorW(NULL, IDC_SIZEWE));
			break;
		case ECursor::UpArrow:
			::SetCursor(::LoadCursorW(NULL, IDC_UPARROW));
			break;
		case ECursor::Wait:
			::SetCursor(::LoadCursorW(NULL, IDC_WAIT));
			break;
		default: ;
		}
	}

	LRESULT FWin32Window::WindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == Win32Desktop.RefreshMessage && Refreshing)
		{
			AssertExpr(Refreshing > 0);
			AssertExpr(hWnd == hWindow);
			PaintWindow();
			return 0;
		}

		switch (Message)
		{
		case WM_DESTROY:
		{
			OnDestroy();
			hWindow = NULL;
			break;
		}
		case WM_CLOSE:
		{
			OnClose();
			return 0;
		}

		case WM_SHOWWINDOW:
		{
			// Just for ShowWindow, not for DestroyWindow...
			// Use WM_WINDOWPOSCHANGED
			break;
		}
		case WM_SYSCOMMAND:
			//if (wParam == SC_CLOSE)
			//{
			//	FEventArgs Args;
			//	Window.OnClosing(Args);
			//	if (Args.Handled)
			//		return FALSE;
			//}
			break;
		case WM_MOUSEACTIVATE:
		{
			if (!WindowPlacement.MouseActivatable)
				return MA_NOACTIVATE;
			break;
		}
		case WM_NCACTIVATE:
		{
			//LRESULT Result = DefWindowProcW(hWnd, Message, 1, -1);
			//return Result;
			if (::IsIconic(hWnd))
				break;
			break;
		}
		case WM_ACTIVATE:
		{
			OnActive(!!wParam);
			break;
		}
		case WM_SETFOCUS:
		{
			//LogInfo(u8"WM_SETFOCUS"V);
			break;
		}
		case WM_KILLFOCUS:
		{
			OnFocus(false);
			break;
		}
		case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS & WindowPos = *(WINDOWPOS *)lParam;
			if (WindowPos.flags & SWP_SHOWWINDOW)
			{
				OnShown(true);

				//ContextMenuHook = SetWindowsHookExW(WH_MOUSE, ContextMenuMouseHook, 0, GetCurrentThreadId());
			}
			else if (WindowPos.flags & SWP_HIDEWINDOW)
			{
				OnShown(false);
				//UnhookWindowsHookEx(ContextMenuHook);
				//ContextMenuHook = NULL;
			}

			break;
		}
		case WM_GETMINMAXINFO:
		{
			if (WindowPlacement.WindowStyle == EWindowStyle::None)
				break;

			//MINMAXINFO & MinMaxInfo = *reinterpret_cast<MINMAXINFO *>(lParam);
			//MinMaxInfo.ptMinTrackSize = { 0, 0 };
			return DefWindowProcW(hWnd, Message, wParam, lParam);
		}
		case WM_NCCALCSIZE:
		{
			if (WindowPlacement.WindowStyle == EWindowStyle::None)
				break;

			ThicknessI BorderThickness {};
			if (WindowPlacement.WindowStyle == EWindowStyle::Normal)
				BorderThickness = Win32Desktop.BorderThickness;

			bool IsAdjustingElseCreating = wParam;
			if (IsAdjustingElseCreating)
			{
				NCCALCSIZE_PARAMS & NoneClientSize = InterpretCastRef<NCCALCSIZE_PARAMS>((void *)lParam);
				RECT TargetRect = NoneClientSize.rgrc[0];
				RECT SourceRect = NoneClientSize.rgrc[1];
				RECT SourceClientRect = NoneClientSize.rgrc[2];

				RECT & FinalClientRect = NoneClientSize.rgrc[0];
				RECT & FinalTargetRect = NoneClientSize.rgrc[1];
				RECT & FinalSourceRect = NoneClientSize.rgrc[2];

				WINDOWPLACEMENT WindowPlacement { sizeof(WINDOWPLACEMENT) };
				::GetWindowPlacement(hWindow, &WindowPlacement);
				if (WindowPlacement.showCmd == SW_MAXIMIZE)
				{
					HMONITOR hMonitor = MonitorFromWindow(hWindow, MONITOR_DEFAULTTONEAREST);
					MONITORINFOEXW MonitorInfoExW = { sizeof(MONITORINFOEXW) };
					GetMonitorInfoW(hMonitor, &MonitorInfoExW);
					FinalClientRect = MonitorInfoExW.rcWork;
				}
				else
				{
					FinalClientRect.left = TargetRect.left + BorderThickness.Left;
					FinalClientRect.top = TargetRect.top + BorderThickness.Top;
					FinalClientRect.right = TargetRect.right - BorderThickness.Right;
					FinalClientRect.bottom = TargetRect.bottom - BorderThickness.Bottom;
				}
				FinalTargetRect = TargetRect;
				FinalSourceRect = SourceRect;
			}
			else
			{
				RECT & ClientRect = InterpretCastRef<RECT>((void *)lParam);
				ClientRect.left = ClientRect.left + BorderThickness.Left;
				ClientRect.top = ClientRect.top + BorderThickness.Top;
				ClientRect.right = ClientRect.right - BorderThickness.Right;
				ClientRect.bottom = ClientRect.bottom - BorderThickness.Bottom;
			}
			return 0;
		}
		case WM_NCHITTEST:
		{
			DWORD DefHitResult = DefWindowProcW(hWnd, Message, wParam, lParam);
			if (DefHitResult != HTCLIENT)
				return DefHitResult;

			PointI Position(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
			::ScreenToClient(hWindow, LPPOINT(&Position));
			uint32 Result = WindowHitTest(Position);
			if (Result != HTERROR)
				return Result;
			break;
		}
		case WM_NCMOUSEMOVE:
		{
			//if (Window.WindowStyle != EWindowStyle::None)
			{
				if (wParam == HTCAPTION || wParam == HTCLOSE || wParam == HTMINBUTTON || wParam == HTMAXBUTTON || wParam == HTSYSMENU)
				{
					if (!TrackingMouse)
					{
						TrackingMouse = true;
						TRACKMOUSEEVENT EventTrack;
						EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
						EventTrack.dwFlags = TME_HOVER | TME_LEAVE | TME_NONCLIENT;
						EventTrack.dwHoverTime = HOVER_DEFAULT;
						EventTrack.hwndTrack = hWnd;
						TrackMouseEvent(&EventTrack);
					}

					PointI Position(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
					::ScreenToClient(hWindow, LPPOINT(&Position));
					//FPoint LocalPoint = GlobalToLocal(WorldPoint);
					OnMouseMove(Position);
				}
			}
			break;
		}
		case WM_NCMOUSELEAVE:
		{
			TrackingMouse = false;
			if (WindowPlacement.WindowStyle != EWindowStyle::None)
			{
				PointI Position = Win32Desktop.GetCursorPosition();
				::ScreenToClient(hWindow, LPPOINT(&Position));

				if (WindowHitTest(Position) != HTCLIENT)
					OnMouseLeave(Position);
			}
			break;
		}
		case WM_NCLBUTTONDOWN:
		{
			if (WindowPlacement.WindowStyle != EWindowStyle::None)
			{
				if (wParam == HTCAPTION)
					break;

				if (wParam != HTCLIENT)
				{
					PointI Position(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
					::ScreenToClient(hWindow, LPPOINT(&Position));
					OnMouseDown(EMouseButton::Left, Position);
				}
				switch (wParam)
				{
				case HTMINBUTTON:
				case HTMAXBUTTON:
				case HTCLOSE:
					return 0;
				default:
					break;
				}
			}
			break;
		}
		case WM_NCLBUTTONUP:
		{
			if (WindowPlacement.WindowStyle != EWindowStyle::None)
			{
				if (wParam == HTMINBUTTON)
				{
					::ShowWindow(hWnd, SW_MINIMIZE);
					return 0;
				}
				else if (wParam == HTMAXBUTTON)
				{
					WINDOWPLACEMENT WindowPlacement { sizeof(WINDOWPLACEMENT) };
					::GetWindowPlacement(hWnd, &WindowPlacement);
					::ShowWindow(hWnd, WindowPlacement.showCmd == SW_MAXIMIZE ? SW_RESTORE : SW_MAXIMIZE);
					//::SendMessageW(hWnd, WM_SYSCOMMAND, WindowPlacement.showCmd == SW_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE, 0);
					return 0;
				}
				else if (wParam == HTCLOSE)
				{
					SendMessageW(hWnd, WM_CLOSE, 0, 0);
					return 0;
				}
				else
				{
					PointI Position(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
					::ScreenToClient(hWindow, LPPOINT(&Position));
					OnMouseUp(EMouseButton::Left, Position);
					return 0;
				}
			}
			break;
		}
		case WM_NCPAINT:
		{
			//if (Window.WindowStyle != EWindowStyle::None)
			//{
			//	LRESULT Result = DefWindowProcW(hWnd, Message, wParam, lParam);
			//	HDC hdcWindow = ::GetWindowDC(hWnd);
			//	RectI ClientRect = { { 1, 1 }, Windows::GetWindowSize(hWnd)  - 2};
			//	IPainterRef DCPainter = DesktopContext.Graphic.CreateDCPainter(hdcWindow, (RectU)ClientRect);
			//	DCPainter->BeginPaint();
			//	DCPainter->Clear(Colors::White);
			//	Window.PerformRender(DCPainter.Ref());
			//	DCPainter->EndPaint();
			//	::ReleaseDC(hWnd, hdcWindow);
			//	return 0;
			//}
			break;
		}
		case WM_PAINT:
		{
			//LogWarning(u8"WM_PAINT {}"V, FDateTime::SteadyMilliseconds());
			PAINTSTRUCT PaintStruct;
			HDC hdcPaint = ::BeginPaint(hWnd, &PaintStruct);
			RectU ClientRect = (RectU)Windows::GetClientRect(hWnd);
			++Refreshing;
			PaintWindow(ClientRect);
			::EndPaint(hWnd, &PaintStruct);
			break;;
		}
		case WM_SETCURSOR:
		{
			if (Low(uint32(lParam)) == HTCLIENT)
			{
				SetCursor(CurrentCursor, true);
				return 0;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!TrackingMouse)
			{
				TrackingMouse = true;

				TRACKMOUSEEVENT EventTrack;
				EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
				EventTrack.dwFlags = TME_HOVER | TME_LEAVE;
				EventTrack.dwHoverTime = HOVER_DEFAULT;
				EventTrack.hwndTrack = hWnd;
				TrackMouseEvent(&EventTrack);
			}
			{
				OnMouseMove(FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam))));
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			FPoint MousePosition = FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
			OnMouseDown(EMouseButton::Left, MousePosition);
			break;
		}
		case WM_LBUTTONUP:
		{
			FPoint MousePosition = FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
			OnMouseUp(EMouseButton::Left, MousePosition);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			FPoint MousePosition = FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
			OnMouseDown(EMouseButton::Right, MousePosition);
			break;
		}
		case WM_RBUTTONUP:
		{
			FPoint MousePosition = FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)));
			OnMouseUp(EMouseButton::Right, MousePosition);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			//UElement::DumpVisualTree(StaticCast<UWindow>(Window));
			break;
		}
		case WM_MOUSEHOVER:
		{
			//Window.WindowMouseHover(MouseEventArgs);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			POINT ClientPoint { LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)) };
			::ScreenToClient(hWnd, &ClientPoint);
			OnMouseWhell(FPoint(ClientPoint.x, ClientPoint.y),
				Vec2I(0, HighAs<int16>((uint32_t)wParam) / -WHEEL_DELTA));
			break;
		}
		case WM_MOUSELEAVE:
		{
			TrackingMouse = false;
			OnMouseLeave(FPoint(LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam))));
			break;
		}
		case WM_KEYDOWN:
		{
			OnKeyDown(Windows::VirtualKeyToKeyCode(uint32(wParam)));
			break;
		}
		case WM_KEYUP:
		{
			OnKeyUp(Windows::VirtualKeyToKeyCode(uint32(wParam)));
			break;
		}
		case WM_MOVE:
		{
			POINTS Points = MAKEPOINTS(lParam);
			PointI Position { Points.x, Points.y };

			if (PointF(Position) != WindowPlacement.WindowRect.Position)
			{
				WindowPlacement.WindowRect.Position = PointF(Position);
				OnMove(Position);
			}
			break;
		}
		case WM_SIZING:
		{
			//RECT & WindowRect = *(RECT *)lParam;
			//FSize ClientSize = SizeU { uint32(WindowRect.right - WindowRect.left), uint32(WindowRect.bottom - WindowRect.top) };
			//if (Window.WindowSize != ClientSize)
			//{
			//	SizeAdjusting = true;
			//	//Window.WindowSize = ClientSize;
			//	SizeAdjusting = false;
			//}
			break;
		}
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				SizeI ClientSize = SizeI { LowAs<int16>(Low(lParam)), HighAs<int16>(Low(lParam)) };
				if (WindowPlacement.WindowRect.Size != SizeF(ClientSize))
				{
					SizeAdjusting = true;
					WindowPlacement.WindowRect.Size = ClientSize;
					OnSize(ClientSize);
					SizeAdjusting = false;
				}
			}

			switch (wParam)
			{
			case SIZE_MINIMIZED:
				OnState(EWindowState::Minimized);
				break;
			case SIZE_MAXIMIZED:
				OnState(EWindowState::Maximized);
				break;
			case SIZE_RESTORED:
				OnState(EWindowState::Normal);
				break;
			default: break;
			}
			break;
		}
		default:
			break;
		}
		return DefWindowProcW(hWnd, Message, wParam, lParam);
	}

	void FWin32Window::DecideWindowRect(RectI & WindowRect) const
	{
		switch (WindowPlacement.WindowStyle)
		{
		default:
		case EWindowStyle::None:
		{
			RECT AdjustedRect { 0, 0, WindowRect.Width, WindowRect.Height };
			AdjustWindowRect(&AdjustedRect, WindowStyle, FALSE);
			WindowRect.Width = AdjustedRect.right - AdjustedRect.left;
			WindowRect.Height = AdjustedRect.bottom - AdjustedRect.top;
			break;
		}
		case EWindowStyle::Frameless:
			break;
		case EWindowStyle::Normal:
			WindowRect.Size += Win32Desktop.BorderThickness * 2;
			break;
		}
	}

	uint32 FWin32Window::WindowHitTest(FPoint LocalPoint)
	{
		if (WindowPlacement.WindowStyle != EWindowStyle::None)
		{
			uint32 HitResult = HitTestBorderThickness(LocalPoint, WindowPlacement.WindowRect.Size, WindowPlacement.FrameThickness);
			if (HitResult != HTTRANSPARENT && HitResult != HTNOWHERE && HitResult != HTCLIENT)
				return HitResult;
		}

		EHitResult ChromeComponent = OnHitTest(LocalPoint);
		switch (ChromeComponent)
		{
		default:
		case EHitResult::None:
			return HTERROR;
		case EHitResult::Ignore:
			return HTTRANSPARENT;

		case EHitResult::Inside:
			return HTCLIENT;

		case EHitResult::Minimun:
			return HTMINBUTTON;
		case EHitResult::Maximum:
			return HTMAXBUTTON;
		case EHitResult::Close:
			return HTCLOSE;
		case EHitResult::Title:
		case EHitResult::Caption:
			return HTCAPTION;
		case EHitResult::SystemMenu:
			return HTSYSMENU;
		case EHitResult::Left:
			return HTLEFT;
		case EHitResult::Right:
			return HTRIGHT;
		case EHitResult::Top:
			return HTTOP;
		case EHitResult::Bottom:
			return HTBOTTOM;
		case EHitResult::TopLeft:
			return HTTOPLEFT;
		case EHitResult::TopRight:
			return HTTOPRIGHT;
		case EHitResult::BottomLeft:
			return HTBOTTOMLEFT;
		case EHitResult::BottomRight:
			return HTBOTTOMRIGHT;
		}
	}

	void FWin32Window::PaintWindow(RectU PaintRect)
	{
		AssertExpr(hWindow != NULL);
		AssertExpr(Refreshing > 0);
		Refreshing = 0;

		if (IsIconic(hWindow) || !IsWindowVisible(hWindow))
			return;

		if (!OnPaint)
			return;

		static bool SkipPaint = false;
		if (SkipPaint)
			return;

		SizeU ClientSize = Windows::GetClientSize(hWindow);
		OnPaint();
	}

	FWin32Desktop::FWin32Desktop()
	{
		BorderThickness = GetSystemMetrics(SM_CYBORDER);
		ResizeFrameThickness = GetSystemMetrics(SM_CYSIZEFRAME);
		CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
		RefreshMessage = ::RegisterWindowMessageW(Message_Refresh);
	}

	FWin32Desktop::~FWin32Desktop() {}

	IWindowHandleRef FWin32Desktop::PlaceWindow(const FWindowPlacement & WindowPlacement)
	{
		return MakeRefer<FWin32Window>(*this, WindowPlacement);
	}

	float32 FWin32Desktop::GetScaleRatio() const
	{
		HDC hdcScreen = ::GetDC(NULL);
		int32 DpiY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		ReleaseDC(NULL, hdcScreen);
		return float32(DpiY) / USER_DEFAULT_SCREEN_DPI;
	}

	RectI FWin32Desktop::GetWorkingRect() const
	{
		//HMONITOR hMonitor = MonitorFromWindow(hWindow, MONITOR_DEFAULTTONEAREST);
		//MONITORINFOEXW MonitorInfoExW = { sizeof(MONITORINFOEXW) };
		//GetMonitorInfoW(hMonitor, &MonitorInfoExW);
		//return
		//{
		//	MonitorInfoExW.rcWork.left,
		//	MonitorInfoExW.rcWork.top,
		//	MonitorInfoExW.rcWork.right - MonitorInfoExW.rcWork.left,
		//	MonitorInfoExW.rcWork.bottom - MonitorInfoExW.rcWork.top
		//};

		HWND hDesktop = ::GetDesktopWindow();
		return Windows::GetWindowRect(hDesktop);
	}

	PointI FWin32Desktop::GetCursorPosition() const
	{
		POINT CursorPosition;
		if (!::GetCursorPos(&CursorPosition))
			return FPoint::Infinity;

		return { CursorPosition.x, CursorPosition.y };
	}

	SizeI FWin32Desktop::GetCursorSize() const
	{
		HCURSOR hCursor = ::GetCursor();
		ICONINFOEXW IconInfoEx { sizeof(ICONINFOEXW) };
		SizeI CursorSize;
		if (::GetIconInfoExW(hCursor, &IconInfoEx))
		{
			bool bBWCursor = IconInfoEx.hbmColor == NULL;
			BITMAP Bitmap = {};
			if (::GetObjectW(IconInfoEx.hbmMask, sizeof(BITMAP), &Bitmap) != 0)
			{
				CursorSize.Width = Bitmap.bmWidth;
				CursorSize.Height = abs(Bitmap.bmHeight) / (bBWCursor ? 2 : 1);
			}

			::DeleteObject(IconInfoEx.hbmColor);
			::DeleteObject(IconInfoEx.hbmMask);
		}
		return CursorSize;
	}
}
