#pragma once

#include "X2D.Types.h"

#include "Transform3x2.h"
#include "TextLayout.h"
#include "Bitmap.h"

namespace Xin
{
	class IGeometry;
	class IPath;

	class IPainter : public IX2DObject
	{
	public:
		IPainter(IX2DObject & Device) : Device(Device) { }

	public:
		virtual void BeginPaint(IBitmap * Bitmap) = 0;
		virtual void EndPaint() = 0;
		virtual void Clear(FColor Color) = 0;

	public:
		virtual void PushViewport(const RectF & Rect) = 0;
		virtual void PopViewport() = 0;

		virtual void PushClip(const RectF & Rect) = 0;
		virtual void PopClip() = 0;
		virtual void PushTransform(const FTransform3x2F & Transform) = 0;
		virtual void PopTransform() = 0;

	public:
		virtual void DrawLine(const PointF & PointA, const PointF & PointB, const IPen & Pen) = 0;
		void DrawLine(const PointF & PointA, const PointF & PointB, FColor Color, float32 Thickness)
		{
			DrawLine(PointA, PointB, IPen { FSolidColorBrush { Color }, Thickness });
		}

		virtual void DrawRect(const RectF & Rect, const IBrush & Brush, const IPen & Pen, const SizeF & Radius = SizeF::Zero) = 0;
		void DrawRect(const RectF & Rect, FColor Color, float32 Thickness, const SizeF & Radius = SizeF::Zero)
		{
			DrawRect(Rect, IBrush::None, IPen { FSolidColorBrush { Color }, Thickness }, Radius);
		}
		void FillRect(const RectF & Rect, FColor Color, const SizeF & Radius = SizeF::Zero)
		{
			DrawRect(Rect, FSolidColorBrush { Color }, IPen::None, Radius);
		}
		void FillRect(const RectF & Rect, const IBrush & Brush, const SizeF & Radius = SizeF::Zero)
		{
			DrawRect(Rect, Brush, IPen::None, Radius);
		}

		virtual void DrawEllipse(const PointF & Position, const Vec2F & Radius, const IBrush & Brush, const IPen & Pen) = 0;
		void DrawEllipse(const PointF & Position, const Vec2F & Radius, FColor Color, float32 Thickness)
		{
			DrawEllipse(Position, Radius, IBrush::None, IPen { FSolidColorBrush { Color }, Thickness });
		}
		void FillEllipse(const PointF & Position, const Vec2F & Radius, FColor Color)
		{
			DrawEllipse(Position, Radius, FSolidColorBrush { Color }, IPen::None);
		}
		void DrawEllipse(const RectF & Rect, const IBrush & Brush, const IPen & Pen)
		{
			DrawEllipse(Rect.Center, Rect.Radius, Brush, Pen);
		}
		void DrawCircle(const PointF & Position, float Radius, FColor Color, float32 Thickness)
		{
			DrawEllipse(Position, { Radius, Radius }, Color, Thickness);
		}
		void FillCircle(const PointF & Position, float Radius, FColor Color)
		{
			FillEllipse(Position, { Radius, Radius }, Color);
		}

		void DrawPath(IPath & Path, const PointF & Point, const IBrush & Brush, const IPen & Pen)
		{
			DrawPath(Path, FTransform3x2F(Point), Brush, Pen);
		}
		void DrawPath(IPath & Path, const PointF & Point, const IPen & Pen)
		{
			DrawPath(Path, Point, IBrush::None, Pen);
		}
		void DrawPath(IPath & Path, const PointF & Point, const IBrush & Brush)
		{
			DrawPath(Path, Point, Brush, IPen::None);
		}

		virtual void DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush, const IPen & Pen) = 0;
		void DrawPath(IPath & Path, const FTransform3x2F & Transform, const IPen & Pen)
		{
			DrawPath(Path, Transform, IBrush::None, Pen);
		}
		void DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush)
		{
			DrawPath(Path, Transform, Brush, IPen::None);
		}

	public:
		virtual void DrawBitmap(IBitmap & Bitmap, const PointF & Point) = 0;
		virtual void DrawBitmap(IBitmap & Bitmap, const RectF & Rect) = 0;

	public:
		virtual void DrawString(FStringV String, const PointF & Point, FColor Color, const FTextFormat & TextFormat) = 0;
		virtual void DrawString(FStringV String, const RectF & Rect, FColor Color, const FTextFormat & TextFormat) = 0;

		virtual void DrawStringW(FWStringV String, const PointF & Point, FColor Color, const FTextFormat & TextFormat) = 0;
		virtual void DrawStringW(FWStringV String, const RectF & Rect, FColor Color, const FTextFormat & TextFormat) = 0;

		virtual void DrawGlyph(uint16 GlyphIndex, PointF Point, IFontFace & FontFace, float32 FontSize, FColor Color) = 0;
		virtual void DrawTextBlob(ITextBlob & TextBlob, const PointF & BaseLinePoint, FColor Color) = 0;

	public:
		virtual void CopyBitmap(IBitmap * Destination, IBitmap * Source) = 0;

	public:
		IX2DObject & Device;
	};
	using IPainterRef = TReferPtr<IPainter>;
}
