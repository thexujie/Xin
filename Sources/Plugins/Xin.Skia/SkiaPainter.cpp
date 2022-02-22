#include "PCH.h"
#include "SkiaPainter.h"

#include "SkiaBitmap.h"
#include "SkiaGraphics.h"
#include "SkiaPath.h"

namespace Xin::Skia
{
	ISkiaPainter::ISkiaPainter(FSkiaGraphics & SkiaDevice) : IPainter(SkiaDevice), SkiaDevice(SkiaDevice)
	{
	}

	ISkiaPainter::~ISkiaPainter()
	{
		
	}

	void ISkiaPainter::BeginPaint(IBitmap * Bitmap)
	{
	}

	void ISkiaPainter::EndPaint()
	{
		skCanvas = nullptr;
	}

	void ISkiaPainter::Clear(FColor Color)
	{
		skCanvas->clear(Color);
	}

	void ISkiaPainter::PushViewport(const RectF & Rect)
	{
		PointF LastOrigion = Viewports.Size ? Viewports.Back().Position : PointF(0.0f, 0.0f);
		RectF Viewport = { Rect.Position + LastOrigion, Rect.Size };
		//RectF Viewport = { Rect.Position + LastOrigion, Rect.Size };

		SkM44 skMatrix;
		skMatrix.setTranslate(Viewport.X, Viewport.Y);
		//skMatrix.preScale(Transform.Sx, Transform.Sy);
		skMatrix.preScale(1.0f, 1.0f);

		skCanvas->save();
		skCanvas->setMatrix(skMatrix);
		skCanvas->clipRect({ 0.0f, 0.0f, Viewport.Width, Viewport.Height }, SkClipOp::kIntersect, AntialiasMode);

		Viewports.PushBack(Viewport);
	}

	void ISkiaPainter::PopViewport()
	{
		Viewports.PopBack();
		//if (Viewports.Size > 0)
		//{
		//	RectF & Viewport = Viewports.Back();

		//	SkM44 skMatrix;
		//	skMatrix.setTranslate(Viewport.X, Viewport.Y);
		//	//skMatrix.preScale(Transform.Sx, Transform.Sy);
		//	skMatrix.preScale(1.0f, 1.0f);
		//	skCanvas->setMatrix(skMatrix);
		//}
		//else
		//{
		//	skCanvas->resetMatrix();
		//}
		skCanvas->restore();
	}

	void ISkiaPainter::PushClip(const RectF & Rect)
	{
	}

	void ISkiaPainter::PopClip()
	{
	}

	void ISkiaPainter::PushTransform(const FTransform3x2F & Transform)
	{
	}

	void ISkiaPainter::PopTransform()
	{
	}

	void ISkiaPainter::DrawLine(const PointF & PointA, const PointF & PointB, const IPen & Pen)
	{
		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			SkPaint skPaint;
			ApplyStrokeStates(skPaint, Pen);
			skCanvas->drawLine({ PointA.X, PointA.Y }, { PointB.X, PointB.Y }, skPaint);
		}
	}

	void ISkiaPainter::DrawRect(const RectF & Rect_, const IBrush & Brush, const IPen & Pen, const SizeF & Radius)
	{
		//RectF Rect = Rect_.Offset(Translates.Back());
		RectF Rect = Rect_;
		if (Brush.IsA<FSolidColorBrush>())
		{
			SkPaint skPaint;
			ApplyFillStates(skPaint, StaticCast<FSolidColorBrush>(Brush));
			if (Radius.AllZero())
				skCanvas->drawRect(FromRect(Rect), skPaint);
			else
				skCanvas->drawRoundRect(FromRect(Rect), Radius.Width, Radius.Height, skPaint);
		}

		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			float32 Thickness = Pen.Thickness;
			float32 HalfThickness = Thickness * 0.5f;
			RectF StrokeRect =
			{
				Rect.X + HalfThickness,
				Rect.Y + HalfThickness,
				Rect.Width - Thickness,
				Rect.Height - Thickness,
			};

			if (!LessThanOrCloseZero(StrokeRect.Width) && !LessThanOrCloseZero(StrokeRect.Height))
			{
				SkPaint skPaint;
				ApplyStrokeStates(skPaint, Pen);
				if (Radius.AllZero())
					skCanvas->drawRect(FromRect(StrokeRect), skPaint);
				else
					skCanvas->drawRoundRect(FromRect(StrokeRect), Radius.Width, Radius.Height, skPaint);
			}
		}
	}

	void ISkiaPainter::DrawEllipse(const PointF & Position, const Vec2F & Radius, const IBrush & Brush, const IPen & Pen)
	{
		if (Brush.IsA<FSolidColorBrush>())
		{
			SkPaint skPaint;
			ApplyFillStates(skPaint, StaticCast<FSolidColorBrush>(Brush));
			skCanvas->drawOval({Position.X - Radius.Width, Position.Y - Radius.Height, Position.X + Radius.Width, Position.Y + Radius.Height}, skPaint);
		}

		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			float32 Thickness = Pen.Thickness;
			float32 HalfThickness = Thickness * 0.5f;
			SkRect skRect = SkRect::MakeXYWH
			(
				Position.X - Radius.Width + HalfThickness,
				Position.Y - Radius.Height + HalfThickness,
				Radius.Width * 2 - Thickness,
				Radius.Height * 2 - Thickness
			);
			if (!LessThanOrCloseZero(skRect.width()) && !LessThanOrCloseZero(skRect.height()))
			{
				SkPaint skPaint;
				ApplyStrokeStates(skPaint, Pen);
				skCanvas->drawOval(skRect, skPaint);
			}
		}
	}

	void ISkiaPainter::DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush, const IPen & Pen)
	{
		FSkiaPath & SkiaPath = StaticCast<FSkiaPath>(Path);
		if (!SkiaPath.skPath)
			return;

		bool NeedTransform = !Transform.IsIdentity();
		if (NeedTransform)
		{
			skCanvas->save();
			RectF Viewport = Viewports.Size ? Viewports.Back() : RectF::Zero;
			SkMatrix skMatrix;
			skMatrix.setTranslate(Viewport.X + Transform.Tx, Viewport.Y + Transform.Ty);
			skMatrix.preScale(Transform.Sx, Transform.Sy);
			skMatrix.preSkew(Transform.Kx, Transform.Ky);
			skCanvas->setMatrix(skMatrix);
		}

		if (Brush.IsA<FSolidColorBrush>())
		{
			const FSolidColorBrush & SolidColorBrush = StaticCast<FSolidColorBrush>(Brush);
			if (SolidColorBrush.Color.W > 0)
			{
				SkPaint skPaint;
				ApplyFillStates(skPaint, SolidColorBrush);
				skCanvas->drawPath(*SkiaPath.skPath, skPaint);
			}
		}

		if (Pen.Brush.IsA<FSolidColorBrush>() && Pen.Thickness > 0)
		{
			const FSolidColorBrush & SolidColorBrush = StaticCast<FSolidColorBrush>(Pen.Brush);
			if (SolidColorBrush.Color.W > 0)
			{
				SkPaint skPaint;
				ApplyStrokeStates(skPaint, Pen);
				skCanvas->drawPath(*SkiaPath.skPath, skPaint);
			}
		}

		if (NeedTransform)
		{
			//skCanvas->resetMatrix();
			skCanvas->restore();
		}
	}

	void ISkiaPainter::DrawBitmap(IBitmap & Bitmap, const PointF & Point)
	{
		FSkiaBitmap & SkiaBitmap = StaticCast<FSkiaBitmap>(Bitmap);

		skCanvas->drawImage(SkiaBitmap.skImage, Point.X, Point.Y);
	}

	void ISkiaPainter::DrawBitmap(IBitmap & Bitmap, const RectF & Rect)
	{
		FSkiaBitmap & SkiaBitmap = StaticCast<FSkiaBitmap>(Bitmap);
		if (!SkiaBitmap.skImage)
			return;

		SkPaint skPaint;
		skCanvas->drawImageRect(SkiaBitmap.skImage, FromRect(Rect), SkSamplingOptions{}, &skPaint);
	}

	void ISkiaPainter::DrawString(FStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat)
	{
		//skCanvas->drawTextBlob();
	}

	void ISkiaPainter::DrawString(FStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat)
	{
		//skCanvas->drawTextBlob();
	}

	void ISkiaPainter::DrawStringW(FWStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat)
	{
		//skCanvas->drawTextBlob();
	}

	void ISkiaPainter::DrawStringW(FWStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat)
	{
		//skCanvas->drawTextBlob();
	}

	void ISkiaPainter::DrawGlyph(uint16 GlyphIndex, PointF Point, IFontFace & FontFace, float32 FontSize, FColor Color)
	{
		SkPaint Paint;
		Paint.setStyle(SkPaint::kFill_Style);
		Paint.setColor(Color);
		Paint.setAntiAlias(true);

		SkFont skFont { StaticCast<FSkiaFontFace>(FontFace).skTypeface, FontSize };

		SkPoint GlyphPosition { };
		skCanvas->drawGlyphs(
			1, 
			(const SkGlyphID *)&GlyphIndex, 
			(const SkPoint *)&GlyphPosition, 
			SkPoint{ Point.X, Point.Y + FontFace.Ascent * FontSize },
			skFont, Paint);
		//skCanvas->drawTextBlob();
	}

	void ISkiaPainter::DrawTextBlob(ITextBlob & TextBlob, const PointF & BaseLinePoint, FColor Color)
	{
		SkPaint Paint;
		Paint.setStyle(SkPaint::kFill_Style);
		Paint.setColor(Color.ToARGB());
		Paint.setAntiAlias(AntialiasMode);

		float32 LineY = BaseLinePoint.Y;
		for (sizet LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			const FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			float32 SpanBaseLineX = BaseLinePoint.X;

			for (sizet SpanIndex = 0; SpanIndex < LayoutLine.RunLength; ++SpanIndex)
			{
				const FClusterRun & ClusterRun = TextBlob.ClusterRuns[LayoutLine.RunIndex + SpanIndex];
				if (!ClusterRun.GlyphIndices.Size)
					continue;

				float32 RunY = LineY + LayoutLine.Ascent;
				SkFont skFont { StaticCast<FSkiaFontFace>(ClusterRun.FontFace).skTypeface, ClusterRun.FontSize };
				skCanvas->drawGlyphs(
					ClusterRun.GlyphLength, 
					(const SkGlyphID *)ClusterRun.GlyphIndices.Data, 
					(const SkPoint *)ClusterRun.GlyphPositions.Data, 
					SkPoint{ SpanBaseLineX, RunY},
					skFont, Paint);
				SpanBaseLineX += ClusterRun.Size.Width;
			}

			LineY += LayoutLine.Height;
		}
	}

	void ISkiaPainter::CopyBitmap(IBitmap * Destination, IBitmap * Source)
	{
	}


	void ISkiaPainter::ResetRenderStates()
	{
	}

	void ISkiaPainter::ApplyFillStates(SkPaint & Paint, const FSolidColorBrush & ColorBrush)
	{
		Paint.setStyle(SkPaint::kFill_Style);
		Paint.setColor(ColorBrush.Color.ToARGB());
		Paint.setAntiAlias(AntialiasMode);
	}

	void ISkiaPainter::ApplyStrokeStates(SkPaint & Paint, const IPen & Pen)
	{
		const FSolidColorBrush & ColorBrush = StaticCast<FSolidColorBrush>(Pen.Brush);
		Paint.setStyle(SkPaint::kStroke_Style);
		Paint.setColor(ColorBrush.Color);
		Paint.setStrokeWidth(Pen.Thickness);
		Paint.setBlendMode(SkBlendMode::kSrcOver);
		Paint.setStrokeJoin(FromLineJoin(Pen.StrokeStyle.LineJoin));
		if (Pen.StrokeStyle.MiterValue > 0.0f)
			Paint.setStrokeMiter(Pen.StrokeStyle.MiterValue);
		if (Pen.StrokeStyle.Pattern == EStrokePattern::Dashed && Pen.StrokeStyle.DashOffset > 0.0f)
		{
			SkScalar DashIntervals[] = { Pen.StrokeStyle.DashOffset, Pen.StrokeStyle.DashOffset };
			Paint.setPathEffect(SkDashPathEffect::Make(DashIntervals, ArraySize(DashIntervals), 0));
		}
		Paint.setAntiAlias(AntialiasMode);
	}

	void FSkiaPainter::BeginPaint(IBitmap * Bitmap)
	{
		FSkiaBitmap & SkiaBitmap = StaticCast<FSkiaBitmap>(*Bitmap);
		skCanvas = new SkCanvas(*SkiaBitmap.skBitmap);

		ISkiaPainter::BeginPaint(Bitmap);
	}

	void FSkiaPainter::EndPaint()
	{
		ISkiaPainter::EndPaint();

		skCanvas = nullptr;
	}
}
