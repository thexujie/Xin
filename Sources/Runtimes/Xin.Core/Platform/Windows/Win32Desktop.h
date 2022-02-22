#pragma once

#include "../Desktop.h"
#include "WindowsPrivate.h"

namespace Xin::Windows
{
	class FWin32Desktop;

	class FWin32Window : public IWindowHandle
	{
	public:
		FWin32Window(FWin32Desktop & Win32Desktop, const FWindowPlacement & WindowPlacement);
		~FWin32Window();

	public:
		voidp GetWindowHandle() const override { return hWindow; }

		void ShowWindow(EWindowShowMode WindowShowMode, EWindowShowPosition WindowShowPosition = EWindowShowPosition::None) override;
		void HideWindow() override;
		void CloseWindow() override;
		void PresentWindow(IBitmap & Bitmap) override;

		void InvalidVisual() override;
		void InvalidateRect(FRect Rect) override;

		RectI GetWorkingRect() const override;

		void SetWindowRect(const FRect & Rect) override;
		void SetCapture(bool Capture) override;
		void SetCursor(ECursor Cursor, bool ForceUpdate) override;

	public:
		virtual LRESULT WindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	private:
		void DecideWindowRect(RectI & WindowRect) const;
		uint32 WindowHitTest(FPoint LocalPoint);

	public:
		FWin32Desktop & Win32Desktop;
		FWindowPlacement WindowPlacement;

	public:
		HWND hWindow = NULL;
		uint32 WindowStyle = 0;
		uint32 WindowStyleEx = 0;
		bool TrackingMouse = false;

	public:
		ECursor CurrentCursor = ECursor::None;

		bool SizeAdjusting = false;

	private:
		void PaintWindow(RectU PaintRect = RectU::Zero);
		uintx Refreshing = 0;
	};

	class FWin32Desktop : public IDesktop
	{
	public:
		FWin32Desktop();
		~FWin32Desktop();

		IWindowHandleRef PlaceWindow(const FWindowPlacement & WindowPlacement) override;

		float32 GetScaleRatio() const override;
		RectI GetWorkingRect() const override;

		PointI GetCursorPosition() const override;
		SizeI GetCursorSize() const override;

	public:
		uint32 CaptionHeight = 0;
		uint32 BorderThickness = 0;
		uint32 ResizeFrameThickness = 0;

	public:
		static constexpr wchar_t Message_Refresh[] = L"Message.Refresh.{E81450A1-9DBB-49CB-9381-5B5B0C632456}";
		UINT RefreshMessage = 0;
	};
}
