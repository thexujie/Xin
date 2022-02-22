#pragma once

#include "D2DTypes.h"
#include "D2DBitmap.h"
#include "Xin.Core/Platform/Windows/WIC.Types.h"

namespace Xin::D2D
{
	/**
	 * @link: https://learn.microsoft.com/zh-cn/windows/win32/learnwin32/dpi-and-device-independent-pixels
	 * @link: https://learn.microsoft.com/en-us/windows/win32/directwrite/color-fonts
	 * @link: https://github.com/Microsoft/Windows-universal-samples/tree/main/Samples/DWriteColorGlyph
	 * @link: https://learn.microsoft.com/en-us/windows/win32/Direct2D/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1hwndrendertarget
	 */

	class D2D_API ID2DPainter : public IPainter
	{
	public:
		ID2DPainter(FD2DGraphics & D2DDevice);
		~ID2DPainter();

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;
		void Clear(FColor Color) override;

	public:
		void PushViewport(const RectF & Rect) override;
		void PopViewport() override;
		void PushClip(const RectF & Rect) override;
		void PopClip() override;
		void PushTransform(const FTransform3x2F & Transform) override;
		void PopTransform() override;

	public:
		using IPainter::DrawLine;
		void DrawLine(const PointF & PointA, const PointF & PointB, const IPen & Pen) override;

		using IPainter::DrawRect;
		void DrawRect(const RectF & Rect, const IBrush & Brush, const IPen & Pen, const SizeF & Radius = SizeF::Zero) override;

		using IPainter::DrawEllipse;
		void DrawEllipse(const PointF & Position, const Vec2F & Radius, const IBrush & Brush, const IPen & Pen) override;

		using IPainter::DrawPath;
		void DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush, const IPen & Pen) override;

	public:
		void DrawBitmap(IBitmap & Bitmap, const PointF & Point) override;
		void DrawBitmap(IBitmap & Bitmap, const RectF & Rect) override;

	public:
		void DrawString(FStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat) override;
		void DrawString(FStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat) override;

		void DrawStringW(FWStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat) override;
		void DrawStringW(FWStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat) override;

		void DrawGlyph(uint16 GlyphIndex, PointF Point, IFontFace & FontFace, float32 FontSize, FColor Color) override;
		void DrawTextBlob(ITextBlob & TextBlob, const PointF & BaseLinePoint, FColor Color) override;

	public:
		void CopyBitmap(IBitmap * Destination, IBitmap * Source) override;

	public:
		ID2D1SolidColorBrush * FetchSolidColorBrush(D2D_COLOR_F D2DColor) const
		{
			return FetchSolidColorBrush(FColor {
				(uint8)Clamp<uint32>(uint32(D2DColor.r * 255.0f), 0, 255),
				(uint8)Clamp<uint32>(uint32(D2DColor.g * 255.0f), 0, 255),
				(uint8)Clamp<uint32>(uint32(D2DColor.b * 255.0f), 0, 255),
				(uint8)Clamp<uint32>(uint32(D2DColor.a * 255.0f), 0, 255)
				});
		}
		ID2D1SolidColorBrush * FetchSolidColorBrush(const FColor & Color) const;
		ID2D1SolidColorBrush * FetchSolidColorBrush(const IBrush & Brush) const
		{
			return FetchSolidColorBrush(StaticCast<FSolidColorBrush>(Brush).Color);
		}
		mutable TMap<FColor, ID2D1SolidColorBrushRef> CachedSolidBrushs;

	public:
		ID2D1StrokeStyle1 * FetchStrokeStyle(const FStrokeStyle & StrokeStyle) const;

		mutable TMap<FStrokeStyle, ID2D1StrokeStyle1Ref> CachedStrokeStyles;

	protected:
		void ResetRenderStates();

		ID2D1Bitmap1Ref CacheBitmap(IBitmap & Bitmap) const;

		mutable TMap<WIC::IWICBitmapSourceRef, ID2D1Bitmap1Ref> CachedBitmaps;

	public:
		FD2DGraphics & D2DGraphics;
		ID2D1RenderTargetRef d2d1RenderTarget;

	protected:
		D2D1_ANTIALIAS_MODE AntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
		D2D1_TEXT_ANTIALIAS_MODE TextAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
#if XIN_DEBUG
		uintx DebugColorIndex = 0;
#endif
		TList<RectF> Viewports;
	};

	class D2D_API FD2DPainter : public ID2DPainter
	{
	public:
		FD2DPainter(FD2DGraphics & D2DDevice);
		FD2DPainter(FD2DGraphics & D2DDevice, HDC HDCHandle, const RectU & Rect);
		~FD2DPainter();

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;

	public:
		ID2D1DeviceContextRef d2d1DeviceContext;
		IBitmapRef TargetBitmap;
	};
	using FD2DPainterRef = TReferPtr<FD2DPainter>;

	class D2D_API FD2DWindowPainter : public ID2DPainter
	{
	public:
		FD2DWindowPainter(FD2DGraphics & D2DDevice, HWND WindowHandle);
		~FD2DWindowPainter();

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;

	public:
		HWND WindowHandle = NULL;
		TReferPtr<ID2D1HwndRenderTarget> d2d1HwndRenderTarget;
	};
	using FD2DPainterRef = TReferPtr<FD2DPainter>;

	class D2D_API FD2DWICPainter : public ID2DPainter
	{
	public:
		FD2DWICPainter(FD2DGraphics & D2DDevice);
		~FD2DWICPainter();

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;

	public:
		IBitmapRef TargetBitmap;
	};
}
