#pragma once

#include "SkiaTypes.h"
#include "SkiaBitmap.h"

namespace Xin::Skia
{
	/**
	 * @link: https://learn.microsoft.com/zh-cn/windows/win32/learnwin32/dpi-and-device-independent-pixels
	 * @link: https://learn.microsoft.com/en-us/windows/win32/directwrite/color-fonts
	 * @link: https://github.com/Microsoft/Windows-universal-samples/tree/main/Samples/DWriteColorGlyph
	 * @link: https://learn.microsoft.com/en-us/windows/win32/Direct2D/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1hwndrendertarget
	 */

	class SKIA_API ISkiaPainter : public IPainter
	{
	public:
		ISkiaPainter(FSkiaGraphics & SkiaDevice);
		~ISkiaPainter();

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

	protected:
		void ResetRenderStates();

	public:
		FSkiaGraphics & SkiaDevice;
		TUniquePtr<SkCanvas> skCanvas;

	protected:
		bool AntialiasMode = true;
		TList<RectF> Viewports;

	public:
		void ApplyFillStates(SkPaint & Paint, const FSolidColorBrush & ColorBrush);
		void ApplyStrokeStates(SkPaint & Paint, const IPen & Pen);
	};

	class SKIA_API FSkiaPainter : public ISkiaPainter
	{
	public:
		FSkiaPainter(FSkiaGraphics & SkiaDevice) : ISkiaPainter(SkiaDevice) {}
		~FSkiaPainter() = default;

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;
	};
}
