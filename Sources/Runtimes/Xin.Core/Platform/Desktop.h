#pragma once

#include "X2D/Graphics.h"

namespace Xin
{
	enum class EWindowShowMode
	{
		None = 0,
		NoneActivate,
	};

	enum class EWindowShowPosition
	{
		None = 0,
		ScreenCenter,
		DesktopCenter,
	};

	enum class EWindowStyle
	{
		None = 0,
		Normal,
		Frameless,
	};

	enum class EWindowState
	{
		Closed = 0,
		Hidden,
		Normal,
		Maximized,
		Minimized,
	};

	enum class [[Meta]] ECursor
	{
		None,
		Arrow,
		Cross,
		Hand,
		Help,
		Hiden,
		IBeam,
		Pen,
		SizeAll,

		SizeNESW,
		SizeVertical,
		SizeNWSE,
		SizeHorizontal,

		UpArrow,
		Wait,
	};

	enum class [[Meta]] EHitResult
	{
		None = 0,
		Ignore,

		Inside,

		Minimun,
		Maximum,
		Close,
		Title,
		Caption,
		SystemMenu,

		Left,
		Right,
		Top,
		Bottom,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
	};

	enum class [[Meta]] EPopupPosition
	{
		Absolute = 0,
		Relative,
		Left,
		Top,
		Right,
		Bottom,
		Center,
		Mouse,
		Cursor,
		Custom,
	};

	class IWindowHandle;
	using IWindowHandleRef = TReferPtr<IWindowHandle>;
	using IWindowHandleObj = TEntryPtr<IWindowHandle>;

	struct FWindowPlacement
	{
		FText Title;
		EWindowStyle WindowStyle = EWindowStyle::None;
		RectF WindowRect;

		IWindowHandleObj Owner;
		bool MouseActivatable = true;
		FThickness FrameThickness;
	};

	class IDesktop;

	class CORE_API IWindowHandle : public IObject
	{
	public:
		TDelegate<void(EWindowState)> OnState;
		TDelegate<void(bool)> OnShown;
		TDelegate<void(bool)> OnActive;

		TDelegate<void()> OnPaint;

		TDelegate<void(bool Focus)> OnFocus;
		TDelegate<void()> OnClose;
		TDelegate<void()> OnDestroy;

		TDelegate<EHitResult(PointF)> OnHitTest;

		TDelegate<void(SizeF)> OnSize;
		TDelegate<void(PointF)> OnMove;

		TDelegate<void(EKeyCode)> OnKeyDown;
		TDelegate<void(EKeyCode)> OnKeyUp;
		TDelegate<void(EMouseButton, PointF)> OnMouseDown;
		TDelegate<void(EMouseButton, PointF)> OnMouseUp;
		TDelegate<void(PointF)> OnMouseMove;
		TDelegate<void(PointF)> OnMouseLeave;
		TDelegate<void(PointF, Vec2I)> OnMouseWhell;

	public:
		IWindowHandle(IDesktop & Desktop) : Desktop(Desktop) {}
		~IWindowHandle() override = default;

	public:
		virtual voidp GetWindowHandle() const = 0;

		virtual void ShowWindow(EWindowShowMode WindowShowMode, EWindowShowPosition WindowShowPosition = EWindowShowPosition::None) = 0;
		virtual void HideWindow() = 0;
		virtual void CloseWindow() = 0;
		virtual void PresentWindow(IBitmap & Bitmap) = 0;

		virtual void InvalidVisual() = 0;
		virtual void InvalidateRect(FRect Rect) = 0;

		virtual RectI GetWorkingRect() const = 0;

		virtual void SetWindowRect(const FRect & Rect) = 0;
		virtual void SetCapture(bool Capture) = 0;
		virtual void SetCursor(ECursor Cursor, bool ForceUpdate) = 0;

	public:
		IDesktop & Desktop;
	};


	class CORE_API IDesktop : public IObject
	{
	public:
		IDesktop() = default;

		virtual IWindowHandleRef PlaceWindow(const FWindowPlacement & WindowPlacement) = 0;

		virtual float32 GetScaleRatio() const = 0;
		virtual RectI GetWorkingRect() const = 0;
		virtual PointI GetCursorPosition() const = 0;
		virtual SizeI GetCursorSize() const = 0;

	public:
		void SetDefaultGraphics(IGraphicsRef Graphics) { DefaultGraphics = Graphics; }
		IGraphicsRef GetDefaultGraphics() const { return DefaultGraphics;}

	public:
		IGraphicsRef DefaultGraphics;

	public:
		static IDesktop & Default();
	};

	class FDesktopGraphicsInitializer
	{
	public:
		FDesktopGraphicsInitializer(IGraphicsRef Graphics)
		{
			IDesktop::Default().SetDefaultGraphics(Graphics);
		}

		~FDesktopGraphicsInitializer()
		{
			IDesktop::Default().SetDefaultGraphics(nullptr);
		}
	};

	class IDesktop;
	using IDesktopRef = TReferPtr<IDesktop>;
}
