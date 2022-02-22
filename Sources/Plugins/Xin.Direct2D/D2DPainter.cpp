#include "PCH.h"
#include "D2DPainter.h"

#include "D2DBitmap.h"
#include "D2DGraphics.h"
#include "D2DPath.h"
#include "WICBitmap.h"

namespace Xin::D2D
{
	static uint32 DebugColors[] = { Colors::Red, Colors::Green, Colors::Khaki, Colors::LightCoral, Colors::FireBrick, Colors::Pink, Colors::PeachPuff, Colors::Gold , Colors::LightGreen };

	ID2DPainter::ID2DPainter(FD2DGraphics & D2DDevice) : IPainter(D2DDevice), D2DGraphics(D2DDevice)
	{
	}

	ID2DPainter::~ID2DPainter()
	{
		
	}

	void ID2DPainter::BeginPaint(IBitmap * Bitmap)
	{
#if XIN_DEBUG
		DebugColorIndex = 0;
#endif
		d2d1RenderTarget->BeginDraw();
	}

	void ID2DPainter::EndPaint()
	{
		d2d1RenderTarget->EndDraw();
	}

	void ID2DPainter::Clear(FColor Color)
	{
		d2d1RenderTarget->Clear(AffineColor(Color));
	}

	void ID2DPainter::PushViewport(const RectF & Rect)
	{
		RectF LastViewport = Viewports.Size ? Viewports.Back() : RectF::Zero;
		RectF Viewport = { Rect.Position + LastViewport.Position, Rect.Size };

		FTransform3x2F Transform;
		Transform.Tx = Viewport.X;
		Transform.Ty = Viewport.Y;

		d2d1RenderTarget->SetTransform(AffineTransform(Transform));
		d2d1RenderTarget->PushAxisAlignedClip(AffineClipRect({0, 0, Rect.Width, Rect.Height}), AntialiasMode);

		Viewports.PushBack(Viewport);
	}

	void ID2DPainter::PopViewport()
	{
		Viewports.PopBack();

		d2d1RenderTarget->PopAxisAlignedClip();
		if (Viewports.Size)
		{
			const RectF & Viewport = Viewports.Back();
			FTransform3x2F Transform;
			Transform.Tx = Viewport.X;
			Transform.Ty = Viewport.Y;
			d2d1RenderTarget->SetTransform(AffineTransform(Transform));
		}
		else
		{
			d2d1RenderTarget->SetTransform(AffineTransform(FTransform3x2F::Identity));
		}
	}

	void ID2DPainter::PushClip(const RectF & Rect)
	{
	}

	void ID2DPainter::PopClip()
	{
	}

	void ID2DPainter::PushTransform(const FTransform3x2F & Transform)
	{
	}

	void ID2DPainter::PopTransform()
	{
	}

	void ID2DPainter::DrawLine(const PointF & PointA, const PointF & PointB, const IPen & Pen)
	{
		if (Pen.Brush.IsA<FSolidColorBrush>())
			d2d1RenderTarget->DrawLine(AffinePoint(PointA), AffinePoint(PointB), FetchSolidColorBrush(Pen.Brush), Pen.Thickness, nullptr);
	}

	void ID2DPainter::DrawRect(const RectF & Rect, const IBrush & Brush, const IPen & Pen, const SizeF & Radius)
	{
		if (Brush.IsA<FSolidColorBrush>())
		{
			if (Radius.AllZero())
				d2d1RenderTarget->FillRectangle(AffineRect(Rect), FetchSolidColorBrush(Brush));
			else
				d2d1RenderTarget->FillRoundedRectangle(AffineRoundRect(Rect, Radius), FetchSolidColorBrush(Brush));
		}

		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			D2D1_RECT_F D2DRect = AffineRect(Rect);
			float32 Thickness = Pen.Thickness;
			float32 HalfThickness = Thickness * 0.5f;
			D2DRect.left += HalfThickness;
			D2DRect.top += HalfThickness;
			D2DRect.right -= HalfThickness;
			D2DRect.bottom -= HalfThickness;
			if (!LessThanOrCloseZero(D2DRect.right - D2DRect.left) 
				&& !LessThanOrCloseZero(D2DRect.bottom - D2DRect.top))
			{
				if (Radius.AllZero())
					d2d1RenderTarget->DrawRectangle(D2DRect, FetchSolidColorBrush(Pen.Brush), Thickness);
				else
					d2d1RenderTarget->DrawRoundedRectangle({ D2DRect, Radius.X, Radius.Y }, FetchSolidColorBrush(Pen.Brush), Thickness);
			}
		}
	}

	void ID2DPainter::DrawEllipse(const PointF & Position, const Vec2F & Radius, const IBrush & Brush, const IPen & Pen)
	{
		D2D1_ELLIPSE D2DEllipse;
		D2DEllipse.point = AffinePoint(Position);
		D2DEllipse.radiusX = Radius.X;
		D2DEllipse.radiusY = Radius.Y;
		if (Brush)
			d2d1RenderTarget->FillEllipse(D2DEllipse, FetchSolidColorBrush(Brush));
		if (Pen)
		{
			D2DEllipse.radiusX -= Pen.Thickness * 0.5f;
			D2DEllipse.radiusY -= Pen.Thickness * 0.5f;
			d2d1RenderTarget->DrawEllipse(D2DEllipse, FetchSolidColorBrush(Pen.Brush), Pen.Thickness);
		}
	}

	void ID2DPainter::DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush, const IPen & Pen)
	{
		FD2DPath & D2DPath = StaticCast<FD2DPath>(Path);
		if (!D2DPath.d2d1Geometry)
			return;

		if (Transform.IsIdentity())
		{
			if (Brush && Brush.IsA<FSolidColorBrush>())
				d2d1RenderTarget->FillGeometry(D2DPath.d2d1Geometry, FetchSolidColorBrush(Brush), nullptr);

			if (Pen.Brush && Pen.Brush.IsA<FSolidColorBrush>() && Pen.Thickness > 0)
				d2d1RenderTarget->DrawGeometry(D2DPath.d2d1Geometry, FetchSolidColorBrush(Pen.Brush), Pen.Thickness, FetchStrokeStyle(Pen.StrokeStyle));
		}
		else
		{
			RectF Viewport = Viewports.Size ? Viewports.Back() : RectF::Zero;

			D2D1_MATRIX_3X2_F d2d1MatrixBefore;
			d2d1RenderTarget->GetTransform(&d2d1MatrixBefore);

			D2D1_MATRIX_3X2_F d2d1Matrix = AffineTransform(Transform);
			d2d1Matrix.dx += Viewport.X;
			d2d1Matrix.dy += Viewport.Y;
			d2d1RenderTarget->SetTransform(d2d1Matrix);

			if (Brush.IsA<FSolidColorBrush>())
				d2d1RenderTarget->FillGeometry(D2DPath.d2d1Geometry, FetchSolidColorBrush(Brush), nullptr);

			if (Pen.Brush && Pen.Brush.IsA<FSolidColorBrush>() && Pen.Thickness > 0)
				d2d1RenderTarget->DrawGeometry(D2DPath.d2d1Geometry, FetchSolidColorBrush(Pen.Brush), Pen.Thickness, FetchStrokeStyle(Pen.StrokeStyle));

			d2d1RenderTarget->SetTransform(d2d1MatrixBefore);
		}
	}

	void ID2DPainter::DrawBitmap(IBitmap & Bitmap, const PointF & Point)
	{
		ID2D1Bitmap1Ref D2D1Bitmap1 = CacheBitmap(Bitmap);
		if (D2D1Bitmap1)
			d2d1RenderTarget->DrawBitmap(D2D1Bitmap1.Get(), AffineRect({ Point , (SizeF)Bitmap.Size }));
	}

	void ID2DPainter::DrawBitmap(IBitmap & Bitmap, const RectF & Rect)
	{
		ID2D1Bitmap1Ref D2D1Bitmap1 = CacheBitmap(Bitmap);
		if (D2D1Bitmap1)
			d2d1RenderTarget->DrawBitmap(D2D1Bitmap1.Get(), AffineRect(Rect));
	}

	void ID2DPainter::DrawString(FStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat)
	{
		IDWriteTextFormat * DWriteTextFormat = D2DGraphics.DWTextShaper.FindDWriteTextFormat(FontFormat);
		auto D2DRect = AffineRect({ Point, { TextBlobSizeMax, TextBlobSizeMax } });
		FWString WString = FEncoding::ToWide(String);
		d2d1RenderTarget->DrawTextW(WString.Data, static_cast<UINT32>(WString.Length), DWriteTextFormat, D2DRect, FetchSolidColorBrush(Color));
	}

	void ID2DPainter::DrawString(FStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat)
	{
		IDWriteTextFormat * DWriteTextFormat = D2DGraphics.DWTextShaper.FindDWriteTextFormat(FontFormat);
		auto D2DRect = AffineRect(Rect);
		FWString WString = FEncoding::ToWide(String);
		d2d1RenderTarget->DrawTextW(WString.Data, static_cast<UINT32>(WString.Length), DWriteTextFormat, D2DRect, FetchSolidColorBrush(Color));
	}

	void ID2DPainter::DrawStringW(FWStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat)
	{
		IDWriteTextFormat * DWriteTextFormat = D2DGraphics.DWTextShaper.FindDWriteTextFormat(FontFormat);
		auto D2DRect = AffineRect({ Point, { TextBlobSizeMax, TextBlobSizeMax } });
		d2d1RenderTarget->DrawTextW(String.Data, (uint32)String.Size, DWriteTextFormat, D2DRect, FetchSolidColorBrush(Color));
	}

	void ID2DPainter::DrawStringW(FWStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat)
	{
		IDWriteTextFormat * DWriteTextFormat = D2DGraphics.DWTextShaper.FindDWriteTextFormat(FontFormat);
		auto D2DRect = AffineRect(Rect);
		d2d1RenderTarget->DrawTextW(String.Data, (uint32)String.Size, DWriteTextFormat, D2DRect, FetchSolidColorBrush(Color));
	}

	void ID2DPainter::DrawGlyph(uint16 GlyphIndex, PointF Point, IFontFace & FontFace, float32 FontSize, FColor Color)
	{
		uint16 GlyphIndices[] = { GlyphIndex };
		float32 GlyphAdvances[] = { 0.0f };
		DWRITE_GLYPH_OFFSET GlyphOffsets[] = { { 0.0f, 0.0f } };

		ID2D1SolidColorBrush * Brush = FetchSolidColorBrush(Color);
		const FDWFontFace & DWFontFace = StaticCast<FDWFontFace>(FontFace);
		DWRITE_GLYPH_RUN DWriteGlyphRun {};
		DWriteGlyphRun.fontFace = DWFontFace.dwFontFace;
		DWriteGlyphRun.fontEmSize = FontSize;
		DWriteGlyphRun.glyphCount = 1;
		DWriteGlyphRun.glyphIndices = GlyphIndices;
		DWriteGlyphRun.glyphAdvances = GlyphAdvances;
		DWriteGlyphRun.glyphOffsets = GlyphOffsets;
		DWriteGlyphRun.isSideways = false;
		DWriteGlyphRun.bidiLevel = 0;
		//D2D1RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		d2d1RenderTarget->DrawGlyphRun(
			AffinePoint(Point + PointF { 0.0f, DWFontFace.Ascent * FontSize }),
			&DWriteGlyphRun,
			Brush,
			DWRITE_MEASURING_MODE_NATURAL);
	}

	void ID2DPainter::DrawTextBlob(ITextBlob & TextBlob, const PointF & BaseLinePoint, FColor Color)
	{
		FDWTextBlob & DWTextBlob = StaticCast<FDWTextBlob>(TextBlob);
		ID2D1SolidColorBrush * Brush = FetchSolidColorBrush(Color);

		float32 LineY = BaseLinePoint.Y;
		for (sizet LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			const FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			float32 SpanX = BaseLinePoint.X;

			for (sizet SpanIndex = 0; SpanIndex < LayoutLine.RunLength; ++SpanIndex)
			{
				const FClusterRun & ClusterRun = TextBlob.ClusterRuns[LayoutLine.RunIndex + SpanIndex];
				if (!ClusterRun.GlyphIndices.Size)
					continue;

				//uint32 DebugColor = DebugColors[(DebugColorIndex++) % Size(DebugColors)];
				//FillRect({ SpanX, LineY, ClusterRun.Size.Width, LayoutLine.Height }, DebugColor);

				//FillRect({ SpanX, LineY, LayoutSpan.Width, LayoutLine.Height }, Colors::Red);
				// top
				//PointF SpanPosition { SpanX, LineY };
				// base line
				//PointF SpanPosition { SpanX, LineY + LayoutLine.Ascent - LayoutSpan.Font.Ascent * LayoutSpan.FontSize };
				// center
				PointF SpanPosition { SpanX, LineY + (LayoutLine.Height - ClusterRun.FontFace.Height * ClusterRun.FontSize) * 0.5f };
				SpanPosition.Y += ClusterRun.FontFace.Ascent * ClusterRun.FontSize;

				if (ClusterRun.BiDiLevel & EBiDiLevel::RTL)
					SpanPosition.X += ClusterRun.Size.Width;

				const FDWFontFace & DWFontFace = StaticCast<FDWFontFace>(ClusterRun.FontFace);

				DWRITE_GLYPH_RUN DWriteGlyphRun { };
				DWriteGlyphRun.fontFace = DWFontFace.dwFontFace;
				DWriteGlyphRun.fontEmSize = ClusterRun.FontSize;
				DWriteGlyphRun.glyphCount = uint32(ClusterRun.GlyphIndices.Size);
				DWriteGlyphRun.glyphIndices = ClusterRun.GlyphIndices.Data;
				DWriteGlyphRun.glyphAdvances = ClusterRun.GlyphAdvances.Data;
				DWriteGlyphRun.glyphOffsets = (const DWRITE_GLYPH_OFFSET *)ClusterRun.GlyphOffsets.Data;
				DWriteGlyphRun.isSideways = ClusterRun.Rotate90;
				DWriteGlyphRun.bidiLevel = uint32(ClusterRun.BiDiLevel);

				//D2D1RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
				D2D1_POINT_2F BaseLineOrigin = AffinePoint(SpanPosition);
				DWRITE_MEASURING_MODE MeasuringMode = DWRITE_MEASURING_MODE_NATURAL;

				HRESULT Result = E_FAIL;
				static bool DrawColorGlyph = true;
				if (DrawColorGlyph)
				{
					if (D2DGraphics.DWTextShaper.dwFactory2)
					{
						DWRITE_GLYPH_RUN_DESCRIPTION DWriteGlyphRunDescription;
						DWriteGlyphRunDescription.localeName = D2DGraphics.DWTextShaper.DefaultLocalName.Data;
						DWriteGlyphRunDescription.string = (const charw *)DWTextBlob.TextShape.U16String.Data + ClusterRun.TextIndex;
						DWriteGlyphRunDescription.stringLength = ClusterRun.TextLength;
						DWriteGlyphRunDescription.clusterMap = DWTextBlob.TextShape.Utf16Clusters.Data + ClusterRun.TextIndex;
						DWriteGlyphRunDescription.textPosition = ClusterRun.TextIndex;

						TReferPtr<IDWriteColorGlyphRunEnumerator> ColorGlyphRunEnumerator;
						DWRITE_GLYPH_IMAGE_FORMATS RequestedFormats = DWRITE_GLYPH_IMAGE_FORMATS_PNG | DWRITE_GLYPH_IMAGE_FORMATS_SVG;

						Result = D2DGraphics.DWTextShaper.dwFactory2->TranslateColorGlyphRun(
							BaseLineOrigin.x, BaseLineOrigin.y,
							&DWriteGlyphRun, &DWriteGlyphRunDescription,
							//RequestedFormats, // The glyph formats supported by this renderer.
							MeasuringMode,
							nullptr,
							0,
							ColorGlyphRunEnumerator.GetPP()// On return, may contain color glyph runs.
						);

						if (SUCCEEDED(Result))
						{
							// Retrieve the color glyph run.
							BOOL HasRun = TRUE;
							while (true)
							{
								Result = ColorGlyphRunEnumerator->MoveNext(&HasRun);
								if (FAILED(Result) || !HasRun)
									break;

								const DWRITE_COLOR_GLYPH_RUN * ColorGlyphRun;
								Result = ColorGlyphRunEnumerator->GetCurrentRun(&ColorGlyphRun);
								if (SUCCEEDED(Result))
								{
									ID2D1SolidColorBrush * ColorBrush =
										ColorGlyphRun->paletteIndex == DWRITE_NO_PALETTE_INDEX ?
										Brush : FetchSolidColorBrush(ColorGlyphRun->runColor);
									d2d1RenderTarget->DrawGlyphRun(
										{ ColorGlyphRun->baselineOriginX, ColorGlyphRun->baselineOriginY },
										&ColorGlyphRun->glyphRun,
										ColorBrush,
										MeasuringMode);
								}
							}

							//do
							//{
							//	const DWRITE_COLOR_GLYPH_RUN * ColorGlyphRun;
							//	Result = ColorGlyphRunEnumerator->GetCurrentRun(&ColorGlyphRun);
							//	if (SUCCEEDED(Result))
							//	{
							//		ID2D1SolidColorBrush * ColorBrush =
							//			ColorGlyphRun->paletteIndex == DWRITE_NO_PALETTE_INDEX ?
							//			Brush : FetchSolidColorBrush(ColorGlyphRun->runColor);
							//		D2D1RenderTarget->DrawGlyphRun(
							//			{ ColorGlyphRun->baselineOriginX, ColorGlyphRun->baselineOriginY },
							//			&ColorGlyphRun->glyphRun,
							//			ColorBrush,
							//			MeasuringMode);
							//	}
							//	Result = ColorGlyphRunEnumerator->MoveNext(&HasRun);
							//}
							//while (SUCCEEDED(Result) && HasRun);
						}
						else if (Result == DWRITE_E_NOCOLOR)
						{
							// The glyph run has no color glyphs. Draw it as a monochrome glyph 
						}
						else {}
					}
				}

				if (FAILED(Result))
				{
					d2d1RenderTarget->DrawGlyphRun(
						BaseLineOrigin,
						&DWriteGlyphRun,
						Brush,
						MeasuringMode);
				}

				SpanX += ClusterRun.Size.Width;
			}

			LineY += LayoutLine.Height;
		}
	}

	void ID2DPainter::CopyBitmap(IBitmap * Destination, IBitmap * Source)
	{
		FD2DBitmap * D2DBitmapDestination = StaticCast<FD2DBitmap>(Destination);
		FD2DBitmap * D2DBitmapSource = StaticCast<FD2DBitmap>(Source);
		HRESULT Result = D2DBitmapDestination->d2d1Bitmap1->CopyFromBitmap(nullptr, D2DBitmapSource->d2d1Bitmap1.Get(), nullptr);
		if (Failed(Result))
			LogInfo(u8"ID2D1Bitmap1::CopyFromBitmap failed."V);
	}

	ID2D1SolidColorBrush * ID2DPainter::FetchSolidColorBrush(const FColor & Color) const
	{
		if (auto Iter = CachedSolidBrushs.Find(Color); Iter != CachedSolidBrushs.End())
			return Iter->Second.Get();

		ID2D1SolidColorBrushRef d2d1SolidColorBrush;
		D2D1_COLOR_F d2d1ColorF = AffineColor(Color);
		d2d1RenderTarget->CreateSolidColorBrush(d2d1ColorF, d2d1SolidColorBrush.GetPP());
		CachedSolidBrushs[Color] = d2d1SolidColorBrush;
		return d2d1SolidColorBrush;
	}

	ID2D1StrokeStyle1 * ID2DPainter::FetchStrokeStyle(const FStrokeStyle & StrokeStyle) const
	{
		if (StrokeStyle == FStrokeStyle())
			return nullptr;

		ID2D1StrokeStyle1Ref D2D1StrokeStyle;
		D2D1_STROKE_STYLE_PROPERTIES1 D2D1StrokeStyleProperties {};
		D2D1StrokeStyleProperties.dashOffset = StrokeStyle.DashOffset;
		switch (StrokeStyle.LineJoin)
		{
		default:
		case ELineJoin::Miter:
			D2D1StrokeStyleProperties.lineJoin = D2D1_LINE_JOIN_MITER;
			break;
		case ELineJoin::Bevel:
			D2D1StrokeStyleProperties.lineJoin = D2D1_LINE_JOIN_BEVEL;
			break;
		case ELineJoin::Round:
			D2D1StrokeStyleProperties.lineJoin = D2D1_LINE_JOIN_ROUND;
			break;
		}
		HRESULT Result = D2DGraphics.d2d1Factory1->CreateStrokeStyle(D2D1StrokeStyleProperties, nullptr, 0, D2D1StrokeStyle.GetPP());

		CachedStrokeStyles[StrokeStyle] = D2D1StrokeStyle;
		return D2D1StrokeStyle.Get();
	}

	void ID2DPainter::ResetRenderStates()
	{
		if (d2d1RenderTarget)
		{
			d2d1RenderTarget->SetAntialiasMode(AntialiasMode);
			d2d1RenderTarget->SetTextAntialiasMode(TextAntialiasMode);
		}
	}

	ID2D1Bitmap1Ref ID2DPainter::CacheBitmap(IBitmap & Bitmap) const
	{
		if (StaticCast<FD2DBitmapBase>(Bitmap).IsWICBitmap())
		{
			IWICBitmapSourceRef wicBitmapSource = StaticCast<FWICBitmap>(Bitmap).GetWICBitmapSource();
			if (!wicBitmapSource)
				return nullptr;

			ID2D1Bitmap1Ref D2D1Bitmap1;
			FWICBitmap & WICBitmap = StaticCast<FWICBitmap>(Bitmap);
			if (auto Iter = CachedBitmaps.Find(wicBitmapSource); Iter != CachedBitmaps.End())
				D2D1Bitmap1 = Iter->Second;
			else
			{
				ID2D1BitmapRef D2D1Bitmap;
				// Create a Direct2D bitmap from the WIC bitmap.
				HRESULT	Result = d2d1RenderTarget->CreateBitmapFromWicBitmap( 
					wicBitmapSource,
					NULL,
					D2D1Bitmap.GetPP()
				);
				AssertReturn(SUCCEEDED(Result), nullptr);

				D2D1Bitmap1 = D2D1Bitmap;
				CachedBitmaps[wicBitmapSource] = D2D1Bitmap1;
			}
			return D2D1Bitmap1;
		}

		return StaticCast<FD2DBitmap>(Bitmap).d2d1Bitmap1;
	}


	FD2DPainter::FD2DPainter(FD2DGraphics & D2DDevice) : ID2DPainter(D2DDevice)
	{
		D2D1_DEVICE_CONTEXT_OPTIONS D2DDeviceContextOptions = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
		HRESULT Result = D2DDevice.d2d1Device->CreateDeviceContext(D2DDeviceContextOptions, d2d1DeviceContext.GetPP());
		if (Failed(Result))
			LogInfo(u8"ID2D1Device::CreateDeviceContext failed."V);
		d2d1RenderTarget = d2d1DeviceContext;

		FLOAT DpiX, DpiY;
		d2d1DeviceContext->GetDpi(&DpiX, &DpiY);
		//AssertExpresion(NearOrEqual(DpiX, USER_DEFAULT_SCREEN_DPI));
		ResetRenderStates();
	}

	FD2DPainter::FD2DPainter(FD2DGraphics & D2DDevice, HDC HDCHandle, const RectU & Rect) : ID2DPainter(D2DDevice)
	{
		// https://learn.microsoft.com/en-us/windows/win32/Direct2D/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1hwndrendertarget
		D2D1_RENDER_TARGET_PROPERTIES RenderTargetProperties { };
		RenderTargetProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		RenderTargetProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
		RenderTargetProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
		RenderTargetProperties.dpiY = USER_DEFAULT_SCREEN_DPI;
		TReferPtr<ID2D1DCRenderTarget> D2D1DCRenderTarget;
		HRESULT Result = D2DDevice.d2d1Factory1->CreateDCRenderTarget(&RenderTargetProperties, D2D1DCRenderTarget.GetPP());
		if (Failed(Result))
			LogInfo(u8"ID2D1Factory::CreateDCRenderTarget failed."V);

		RECT rcPaint { (LONG)Rect.X, (LONG)Rect.Y, (LONG)Rect.Right, (LONG)Rect.Bottom };
		Result = D2D1DCRenderTarget->BindDC(HDCHandle, &rcPaint);
		if (Failed(Result))
			LogInfo(u8"ID2D1DCRenderTarget::BindDC failed."V);
		d2d1RenderTarget = D2D1DCRenderTarget;
		ResetRenderStates();
	}

	FD2DPainter::~FD2DPainter()
	{

	}

	void FD2DPainter::BeginPaint(IBitmap * Bitmap)
	{
		if (Bitmap)
		{
			TargetBitmap = Bitmap;
			Bitmap->AcquireSurface();
			FD2DBitmap & D2DBitmap = StaticCast<FD2DBitmap>(*Bitmap);
			d2d1DeviceContext->SetTarget(D2DBitmap.d2d1Bitmap1.Get());
		}

		ID2DPainter::BeginPaint(Bitmap);
	}

	void FD2DPainter::EndPaint()
	{
		ID2DPainter::EndPaint();
		//D2D1RenderTarget->Flush();
		if (TargetBitmap)
		{
			TargetBitmap->ReleaseSurface();
			TargetBitmap.Reset();
		}
	}

	FD2DWindowPainter::FD2DWindowPainter(FD2DGraphics & D2DDevice, HWND WindowHandle) : ID2DPainter(D2DDevice), WindowHandle(WindowHandle)
	{
		
	}

	FD2DWindowPainter::~FD2DWindowPainter()
	{
		
	}

	void FD2DWindowPainter::BeginPaint(IBitmap * Bitmap)
	{
		AssertExpr(!Bitmap);
		SizeU ClientSize = Windows::GetClientSize(WindowHandle);
		AssertExpr(!ClientSize.AnyZero());

		if (!d2d1HwndRenderTarget || d2d1HwndRenderTarget->GetPixelSize() != D2D1_SIZE_U{ClientSize.X, ClientSize.Y})
		{
			CachedSolidBrushs.Clear();
			CachedStrokeStyles.Clear();
			CachedBitmaps.Clear();
			d2d1HwndRenderTarget = nullptr;

			// https://learn.microsoft.com/en-us/windows/win32/Direct2D/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1hwndrendertarget
			D2D1_RENDER_TARGET_PROPERTIES RenderTargetProperties {};
			RenderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
			RenderTargetProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			RenderTargetProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
			RenderTargetProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
			RenderTargetProperties.dpiY = USER_DEFAULT_SCREEN_DPI;

			D2D1_HWND_RENDER_TARGET_PROPERTIES HwndRenderTargetProperties {};
			HwndRenderTargetProperties.hwnd = WindowHandle;
			HwndRenderTargetProperties.pixelSize = { ClientSize.Width, ClientSize.Height };
			HRESULT Result = D2DGraphics.d2d1Factory1->CreateHwndRenderTarget(RenderTargetProperties, HwndRenderTargetProperties, d2d1HwndRenderTarget.GetPP());
			if (Failed(Result))
				LogInfo(u8"ID2D1Factory::CreateHwndRenderTarget failed."V);
			AssertExpr(d2d1HwndRenderTarget);

			d2d1RenderTarget = d2d1HwndRenderTarget;
			ResetRenderStates();
		}

		ID2DPainter::BeginPaint(Bitmap);
	}

	void FD2DWindowPainter::EndPaint()
	{
		ID2DPainter::EndPaint();
	}

	FD2DWICPainter::FD2DWICPainter(FD2DGraphics & D2DDevice) : ID2DPainter(D2DDevice)
	{
	}

	FD2DWICPainter::~FD2DWICPainter()
	{

	}

	void FD2DWICPainter::BeginPaint(IBitmap * Bitmap)
	{
		TargetBitmap = Bitmap;

		AssertExpr(Bitmap);
		D2D1_RENDER_TARGET_PROPERTIES RenderTargetProperties {};
		RenderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		RenderTargetProperties.pixelFormat.format = FromFormat(Bitmap->Format);
		RenderTargetProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		RenderTargetProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
		RenderTargetProperties.dpiY = USER_DEFAULT_SCREEN_DPI;
		HRESULT Result = D2DGraphics.d2d1Factory1->CreateWicBitmapRenderTarget(StaticCastRef<FWICBitmap>(Bitmap).wicBitmap, RenderTargetProperties, d2d1RenderTarget.GetPP());
		HRWarning(Result, u8"CreateWicBitmapRenderTarget failed."V);

		ID2DPainter::BeginPaint(Bitmap);
	}

	void FD2DWICPainter::EndPaint()
	{
		ID2DPainter::EndPaint();
		//D2D1RenderTarget->Flush();
		d2d1RenderTarget = nullptr;
		TargetBitmap = nullptr;
	}
}
