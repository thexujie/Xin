#include "PCH.h"
#include "RHIGraphics.h"

#include "RHIPath.h"

#include "CommandExecutor.h"
#include "Shader.h"
#include "BaseEngine.h"
#include "Xin.Engine/Asset/Texture.h"

namespace Xin::Rendering
{
	DEFINE_SIMPLE_DEFAULT_SHADER(FUIQuadVS, u8"Engine/Shaders/UI/Quad.xsf"V, EShaderStage::Vertex);

	DEFINE_SIMPLE_DEFAULT_SHADER(FUITextPS, u8"Engine/Shaders/UI/Text.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FUIBlitPS, u8"Engine/Shaders/UI/Blit.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FUIGeometryPS, u8"Engine/Shaders/UI/Geometry.xsf"V, EShaderStage::Pixel);

	FFontTexture::FFontTexture(FRHIGraphics & RHIGraphics, IFontFace & Font, float32 FontSize)
		: RHIGraphics(RHIGraphics), Font(Font), FontSize(FontSize)
	{
		float32 GlyphHeight = Font.Height * FontSize;
		uint32 GlyphHeightU = (uint32)Ceil(GlyphHeight);
		SizeU CellSizeU = { GlyphHeightU, GlyphHeightU };
		CellSizeF = SizeF(CellSizeU);
		GlyphCount.XY = TextureSize / CellSizeU;
		GlyphCount.Z = 0;
	}

	FFontTexture::~FFontTexture() {}

	void FFontTexture::MapGlyphs(TView<uint16> GlyphIndices)
	{
		for (sizet GlyphIndexIndex = 0; GlyphIndexIndex < GlyphIndices.Size; ++GlyphIndexIndex)
		{
			uint16 GlyphIndex = GlyphIndices[GlyphIndexIndex];
			uint3 & GlyphCellcoord = GlyphCellcoords[GlyphIndex];
			if (GlyphCellcoord.X || GlyphCellcoord.Y)
				continue;

			sizet EmptyIndex = NullIndex;
			// Cell 0 for invalid
			for (sizet CellIndex = 1; CellIndex < CellGlyphIndices.Size; ++CellIndex)
			{
				if (CellGlyphIndices[CellIndex] == InvalidGlyphIndex && EmptyIndex == NullIndex)
				{
					EmptyIndex = CellIndex;
					break;
				}
			}

			// Add New Glyph ...
			if (EmptyIndex == NullIndex)
			{
				EmptyIndex = CellGlyphIndices.Size ? CellGlyphIndices.Size : 1;
				CellGlyphIndices.ResizeTo(CellGlyphIndices.Size + GlyphCount.X * GlyphCount.Y);
				ArrayFill(CellGlyphIndices.Data + (CellGlyphIndices.Size - GlyphCount.X * GlyphCount.Y), GlyphCount.X * GlyphCount.Y, InvalidGlyphIndex);
				Textures.Add(nullptr);
				Bitmaps.Add(nullptr);
			}

			CellGlyphIndices[EmptyIndex] = GlyphIndex;
			GlyphCellcoord.X = EmptyIndex % GlyphCount.X;
			GlyphCellcoord.Y = EmptyIndex % (GlyphCount.X * GlyphCount.Y) / GlyphCount.X;
			GlyphCellcoord.Z = uint32(EmptyIndex / (GlyphCount.X * GlyphCount.Y));
			PenddingGlyphs.Add({ GlyphIndex, GlyphCellcoord });
		}
	}

	void FFontTexture::FlushGlyphs()
	{
		if (PenddingGlyphs.Size == 0)
			return;

		constexpr EFormat FontTextureFormat = EFormat::R8G8B8A8UF;
		for (sizet TextureIndex = 0; TextureIndex < Textures.Size; ++TextureIndex)
		{
			IRHIResourceRef & Texture = Textures[TextureIndex];
			IBitmapRef & Bitmap = Bitmaps[TextureIndex];

			bool SupportSharedBitmap = RHIGraphics.OfflineGraphics->SupportSharedBitmap(uint32(RHIGraphics.Executor.Engine.Device.GetDeviceRHI()));
			bool NeedClear = false;
			if (!Texture)
			{
				if (SupportSharedBitmap)
				{
					Texture = RHIGraphics.Executor.CreateTexture(FTextureDesc(FontTextureFormat, TextureSize, 
						EResourceUsage::Interoperable | EResourceUsage::RenderTarget | EResourceUsage::SampledTexture | EResourceUsage::CopyWrite, 1, 1, Colors::None), {});
					Bitmap = RHIGraphics.OfflineGraphics->CreateSharedBitmap(Texture->GetSharedHandle(), EBitmapUsage::RenderTarget);
				}
				else
				{
					Texture = RHIGraphics.Executor.CreateTexture(FTextureDesc(FontTextureFormat, TextureSize,
						EResourceUsage::SampledTexture | EResourceUsage::CopyWrite, 1, 1, Colors::None), {});
					Bitmap = RHIGraphics.OfflineGraphics->CreateBitmap(FontTextureFormat, TextureSize, EBitmapUsage::RenderTarget);
				}
				NeedClear = true;
			}

			RHIGraphics.OfflinePainter->BeginPaint(Bitmap);

			if (NeedClear)
			{
				RHIGraphics.OfflinePainter->Clear(Colors::None);
			}

			for (sizet GIndex = 0; GIndex < PenddingGlyphs.Size; ++GIndex)
			{
				const FPenddingGlyph & PenddingGlyph = PenddingGlyphs[GIndex];
				if (PenddingGlyph.CellPosition.Z == TextureIndex)
					RHIGraphics.OfflinePainter->DrawGlyph(PenddingGlyph.GlyphIndex, PenddingGlyph.CellPosition.XY * CellSizeF, Font, FontSize, Colors::White);
			}
			RHIGraphics.OfflinePainter->EndPaint();

			if (!SupportSharedBitmap)
			{
				// Copy bitmap data to textures.
				if (FLockedBitmap LockedBitmap = Bitmap->Lock(EBitmapLock::Read))
				{
					FCommandList CommandList { RHIGraphics.Executor };
					CommandList.UpdateTexture(Texture, FTextureBitmapSource { Bitmap->Format, Bitmap->Size.Width, Bitmap->Size.Height, LockedBitmap.Pitch, 
						{ LockedBitmap.Bytes, LockedBitmap.Pitch * Bitmap->Size.Height } });
					CommandList.Execute();
				}

				//Bitmap->Save(u8"../../Documents/Saved/FontTexture.tga"V, EImageFormat::TARGA);
			}
		}
		PenddingGlyphs.Clear();
	}

	FRHIBitmap::FRHIBitmap(FRHIGraphics & RHIGraphics, SizeU Size, EFormat Format, EBitmapUsage BitmapUsage)
		: IBitmap(Format, Size, BitmapUsage), RHIGraphics(RHIGraphics)
	{
		EResourceUsage ResourceUsage = EResourceUsage::None;
		SetFlags(ResourceUsage, EResourceUsage::RenderTarget, BitmapUsage & EBitmapUsage::RenderTarget);
		SetFlags(ResourceUsage, EResourceUsage::SampledTexture, BitmapUsage & EBitmapUsage::ShaderResource);
		SetFlags(ResourceUsage, EResourceUsage::HostRead, BitmapUsage & EBitmapUsage::ReadBack);
		Texture = RHIGraphics.Executor.CreateTexture({ Format, Size, ResourceUsage });
	}

	FRHIBitmap::FRHIBitmap(FRHIGraphics & RHIGraphics, IRHIResource * Texture, EBitmapUsage BitmapUsage)
		: IBitmap(Texture->ResourceDesc.Format, Texture->ResourceDesc.Size, BitmapUsage), RHIGraphics(RHIGraphics), Texture(Texture) {}

	FRHIBitmap::FRHIBitmap(FRHIGraphics & RHIGraphics, const IPixmap & Pixmap)
		: IBitmap(Pixmap.Format, Pixmap.Size, EBitmapUsage::ShaderResource), RHIGraphics(RHIGraphics)
	{
		Texture = RHIGraphics.Executor.CreateTexture({ Pixmap.Format, Pixmap.Size, EResourceUsage::SampledTexture | EResourceUsage::CopyWrite }, FTextureBitmapSource(Pixmap));
	}

	FRHIBitmap::~FRHIBitmap() {}

	bool FRHIBitmap::Save(FStringV FilePath, EImageFormat ImageFormat) const
	{
		return false;
	}

	IRHIPainter::IRHIPainter(FRHIGraphics & RHIGraphic) : IPainter(RHIGraphic), RHIGraphics(RHIGraphic), CommandList(RHIGraphic.Executor) {}

	IRHIPainter::~IRHIPainter() {}

	void IRHIPainter::BeginPaint(IBitmap * Bitmap)
	{
		FRHIBitmap & RHIBitmap = StaticCastRef<FRHIBitmap>(Bitmap);
		BeginPaintPrivate(RHIBitmap.Texture, true);
	}

	void IRHIPainter::BeginPaint(IBitmap * Bitmap, bool ClearBitmap)
	{
		FRHIBitmap & RHIBitmap = StaticCastRef<FRHIBitmap>(Bitmap);
		BeginPaintPrivate(RHIBitmap.Texture, ClearBitmap);
	}

	void IRHIPainter::EndPaint()
	{
		EndPaintPrivate();
	}

	void IRHIPainter::Clear(FColor Color)
	{
		CommandList.ClearRenderTarget(RenderTarget, Color);
	}

	void IRHIPainter::PushViewport(const RectF & RelativeViewport)
	{
		RectF LastViewport = Viewports.Size ? Viewports.Back() : RectF();
		RectF Viewport = { LastViewport.X + RelativeViewport.X, LastViewport.Y + RelativeViewport.Y, RelativeViewport.Width, RelativeViewport.Height };
		SizeF RenderTargetSize = SizeF(RenderTarget->ResourceDesc.Size);

		//Vec2F Scale = PointF { 2.0f, -2.0f } / RenderTargetSize;
		//VertexShaderParameter.ScreenTransform = FMatrix
		//{
		//	Scale.X, 0, 0, 0,
		//	0, Scale.Y, 0, 0,
		//	0, 0, 1, 0,
		//	Viewport.X, Viewport.Y, 0, 1
		//};

		FMatrix ViewMatrix = FMatrix::LookAtLH({ 0, 0, -1 }, { 0, 0, 0 }, { 0, 1, 0 });
		FMatrix ProjMatrix = FMatrix::OrthographicOffCenterLH(0, RenderTargetSize.Width, RenderTargetSize.Height, 0, 0.001f, 100.0f);
		FMatrix WorldMatrix = FMatrix::Translate(Viewport.X, Viewport.Y, 0);
		WorldMatrix = FMatrix::Identity;
		VertexShaderParameter.ScreenTransform = WorldMatrix * ViewMatrix * ProjMatrix;

		PainterState.VertexShaderParameter = nullptr;

		Viewports.PushBack(Viewport);

		RectF LastScissorRect = ScissorRects.Size ? ScissorRects.Back() : RectF { Viewport.X, Viewport.Y, Viewport.Width, Viewport.Height };
		RectF ScissorRect { Viewport.X, Viewport.Y, Viewport.Width, Viewport.Height };
		ScissorRect = ScissorRect.Intersect(LastScissorRect);
		ScissorRects.PushBack(ScissorRect);

		SetScissorRect(ScissorRect);
	}

	void IRHIPainter::PopViewport()
	{
		Viewports.PopBack();
		ScissorRects.PopBack();

		RectF Viewport = Viewports.Size ? Viewports.Back() : RectF();
		SizeF RenderTargetSize = SizeF(RenderTarget->ResourceDesc.Size);

		//Vec2F Scale = PointF { 2.0f, -2.0f } / RenderTargetSize;
		// 
		//VertexShaderParameter.ScreenTransform = FMatrix
		//{
		//	Scale.X, 0, 0, 0,
		//	0, Scale.Y, 0, 0,
		//	0, 0, 1, 0,
		//	Viewport.X, Viewport.Y, 0, 1
		//};

		FMatrix ViewMatrix = FMatrix::LookAtLH({ 0, 0, -1 }, { 0, 0, 0 }, { 0, 1, 0 });
		FMatrix ProjMatrix = FMatrix::OrthographicOffCenterLH(0, RenderTargetSize.Width, RenderTargetSize.Height, 0, 0.001f, 100.0f);
		FMatrix WorldMatrix = FMatrix::Translate(Viewport.X, Viewport.Y, 0);
		WorldMatrix = FMatrix::Identity;
		VertexShaderParameter.ScreenTransform = WorldMatrix * ViewMatrix * ProjMatrix;
		PainterState.VertexShaderParameter = nullptr;

		SetScissorRect(ScissorRects.Size ? ScissorRects.Back() : RectF(Viewport.X, Viewport.Y, Viewport.Width, Viewport.Height));
	}

	void IRHIPainter::DrawLine(const PointF & PointA, const PointF & PointB, const IPen & Pen)
	{
		if (Pen && Pen.Brush)
		{
			SetColor(StaticCast<FSolidColorBrush>(Pen.Brush).Color);
			SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
			SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIGeometryPS>());

			PointF A = PointA.X < PointB.X ? PointA : PointB;
			PointF B = PointA.X < PointB.X ? PointB : PointA;
			PointF AB = PointB - PointA;
			PointF tAB = PointF{ -AB.Y, AB.X }.Normalized() * Pen.Thickness * 0.5f;

			PointF Points[4] =
			{
				A + tAB,
				B + tAB,
				B - tAB,
				A - tAB,
			};
			AppendRectangle(nullptr, Points, { 0, 0, 1, 1 });
			//FlushDrawCalls();
		}
	}

	void IRHIPainter::DrawRect(const RectF & Rect, const IBrush & Brush, const IPen & Pen, const SizeF & Radius)
	{
		if (Brush.IsA<FSolidColorBrush>())
		{
			SetColor(StaticCast<FSolidColorBrush>(Brush).Color);
			SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
			SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIGeometryPS>());

			AppendRectangle(nullptr, Rect, { 0, 0, 1, 1 });
			//FlushDrawCalls();
		}

		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			IPathRef Path = RHIGraphics.OfflineGraphics->CreatePath();
			if (Path->Open())
			{
				RectF Viewport = Viewports.Size ? Viewports.Back() : RectF();
				float32 Thickness = Pen.Thickness;
				float32 HalfThickness = Thickness * 0.5f;
				RectF StrokeRect =
				{
					Viewport.X + Rect.X + HalfThickness,
					Viewport.Y + Rect.Y + HalfThickness,
					Rect.Width - Thickness,
					Rect.Height - Thickness,
				};

				if (PainterState.Texture)
				{
					FlushDrawCalls();
					PainterState.Texture = nullptr;
					PainterState.PipelineState = 0;
				}

				RectF SourceRect = { 0, 0, 1, 1 };
				TArray<FRHIVertex2D, 4> RectVertices;
				RectVertices[0] = { PointF{ StrokeRect.X, StrokeRect.Y }, { SourceRect.X, SourceRect.Y } };
				RectVertices[1] = { PointF{ StrokeRect.X + StrokeRect.Width, StrokeRect.Y }, { SourceRect.X + SourceRect.Width, SourceRect.Y } };
				RectVertices[2] = { PointF{ StrokeRect.X + StrokeRect.Width, StrokeRect.Y + StrokeRect.Height }, { SourceRect.X + SourceRect.Width, SourceRect.Y + SourceRect.Height } };
				RectVertices[3] = { PointF{ StrokeRect.X, StrokeRect.Y + StrokeRect.Height }, { SourceRect.X, SourceRect.Y + SourceRect.Height } };

				Path->BeginFigure(RectVertices[0].Position.XY);
				Path->AddLines({
					RectVertices[1].Position.XY,
					RectVertices[2].Position.XY,
					RectVertices[3].Position.XY
				});
				Path->EndFigure(true);
				Path->Close();

				FRHIPathVertices RHIPathVertices;
				Path->Widen(RHIPathVertices, Pen.Thickness, false);

				SetColor(StaticCast<FSolidColorBrush>(Pen.Brush).Color);
				SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
				SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIGeometryPS>());

				FTransform3x2F Transform;
				for (uintx TriangleIndex = 0; TriangleIndex < RHIPathVertices.Vertices.Size / 3; ++TriangleIndex)
				{
					AppendTriangle
					(
						nullptr,
						RHIPathVertices.Vertices[TriangleIndex * 3 + 0].Position + Transform.Trans,
						RHIPathVertices.Vertices[TriangleIndex * 3 + 1].Position + Transform.Trans,
						RHIPathVertices.Vertices[TriangleIndex * 3 + 2].Position + Transform.Trans
					);}
			}
		}
	}

	void IRHIPainter::DrawEllipse(const PointF & Position, const Vec2F & Radius, const IBrush & Brush, const IPen & Pen)
	{
		PointF LeftPoint { Position.X - Radius.Width, Position.Y };
		PointF RightPoint { Position.X + Radius.Width, Position.Y };
		PointF EllipseRadius { Radius };

		if (Pen)
		{
			LeftPoint.X += Pen.Thickness * 0.5f;
			RightPoint.X -= Pen.Thickness * 0.5f;
			EllipseRadius -= Pen.Thickness * 0.5f;
		}

		FRHIPath EllipsePth { RHIGraphics };
		EllipsePth.Open();
		EllipsePth.BeginFigure(LeftPoint);
		EllipsePth.AddArc(EllipseRadius, Float32PI * 2, RightPoint, false, false);
		EllipsePth.AddArc(EllipseRadius, Float32PI * 2, LeftPoint, false, false);
		EllipsePth.EndFigure(false);
		EllipsePth.Close();
		DrawPath(EllipsePth, FTransform3x2F::Identity, Brush, Pen);
	}

	void IRHIPainter::DrawPath(IPath & Path, const FTransform3x2F & Transform, const IBrush & Brush, const IPen & Pen)
	{
		SetColor(StaticCast<FSolidColorBrush>(Brush).Color);
		SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
		SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIGeometryPS>());

		RectF Viewport = Viewports.Size ? Viewports.Back() : RectF();
		FRHIPath & RHIPath = StaticCast<FRHIPath>(Path);
		for(uintx TriangleIndex = 0; TriangleIndex < RHIPath.Vertices.Size / 3; ++TriangleIndex)
		{
			AppendTriangle
			(
				nullptr,
				RHIPath.Vertices[TriangleIndex * 3 + 0].Position + FVector4{ Transform.Trans + Viewport.XY, 0, 0},
				RHIPath.Vertices[TriangleIndex * 3 + 1].Position + FVector4{ Transform.Trans + Viewport.XY, 0, 0},
				RHIPath.Vertices[TriangleIndex * 3 + 2].Position + FVector4{ Transform.Trans + Viewport.XY, 0, 0}
			);
		}

		if (Pen.Brush.IsA<FSolidColorBrush>())
		{
			FRHIPathVertices RHIPathVertices;
			RHIPath.OfflinePath->Widen(RHIPathVertices, Pen.Thickness, true);

			SetColor(StaticCast<FSolidColorBrush>(Pen.Brush).Color);
			for (uintx TriangleIndex = 0; TriangleIndex < RHIPathVertices.Vertices.Size / 3; ++TriangleIndex)
			{
				AppendTriangle
				(
					nullptr,
					RHIPathVertices.Vertices[TriangleIndex * 3 + 0].Position+ FVector4{ Transform.Trans + Viewport.XY, 0, 0},
					RHIPathVertices.Vertices[TriangleIndex * 3 + 1].Position+ FVector4{ Transform.Trans + Viewport.XY, 0, 0},
					RHIPathVertices.Vertices[TriangleIndex * 3 + 2].Position+ FVector4{ Transform.Trans + Viewport.XY, 0, 0}
				);
			}
		}
	}

	void IRHIPainter::DrawBitmap(IBitmap & Bitmap, const PointF & Point)
	{
		SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
		SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIBlitPS>());

		FRHIBitmap & RHIBitmap = StaticCast<FRHIBitmap>(Bitmap);
		AppendRectangle(RHIBitmap.Texture, { Point, Bitmap.Size }, { 0, 0, 1, 1 });
	}

	void IRHIPainter::DrawBitmap(IBitmap & Bitmap, const RectF & Rect)
	{
		SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
		SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIBlitPS>());

		FRHIBitmap & RHIBitmap = StaticCast<FRHIBitmap>(Bitmap);
		AppendRectangle(RHIBitmap.Texture, Rect, { 0, 0, 1, 1 });
	}

	void IRHIPainter::DrawString(FStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat) {}
	void IRHIPainter::DrawString(FStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat) {}
	void IRHIPainter::DrawStringW(FWStringV String, const PointF & Point, FColor Color, const FTextFormat & FontFormat) {}
	void IRHIPainter::DrawStringW(FWStringV String, const RectF & Rect, FColor Color, const FTextFormat & FontFormat) {}

	void IRHIPainter::DrawTextBlob(ITextBlob & TextBlob, const PointF & Position, FColor Color)
	{
		SetColor(Color);
		SetVShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUIQuadVS>());
		SetPShader(RHIGraphics.Executor.Engine.ShaderBundle.GetShaderT<FUITextPS>());

		float32 LineY = Position.Y;
		for (sizet LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			const FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			float32 SpanX = Position.X;

			for (sizet SpanIndex = 0; SpanIndex < LayoutLine.RunLength; ++SpanIndex)
			{
				const FClusterRun & ClusterRun = TextBlob.ClusterRuns[LayoutLine.RunIndex + SpanIndex];
				if (!ClusterRun.GlyphIndices.Size)
					continue;

				FFontTextureRef FontTexture = RHIGraphics.GetFontTexture(ClusterRun.FontFace, ClusterRun.FontSize);
				FontTexture->MapGlyphs(ClusterRun.GlyphIndices);
				FontTexture->FlushGlyphs();

				// top
				//PointF SpanPosition { SpanX, LineY };
				// base line
				//PointF SpanPosition { SpanX, LineY + LayoutLine.Ascent - LayoutSpan.Font.Ascent * LayoutSpan.FontSize };
				// center
				PointF SpanPosition { SpanX, LineY + (LayoutLine.Height - ClusterRun.FontFace.Height * ClusterRun.FontSize) * 0.5f };

				if (ClusterRun.BiDiLevel & EBiDiLevel::RTL)
					SpanPosition.X += ClusterRun.Size.Width;

				for (sizet GlyphIndex = 0; GlyphIndex < ClusterRun.GlyphIndices.Size; ++GlyphIndex)
				{
					PointF GlyphPosition = SpanPosition + ClusterRun.GlyphOffsets[GlyphIndex];
					uint3 GlyphCellcoord = FontTexture->GlyphCellcoords[ClusterRun.GlyphIndices[GlyphIndex]];

					SizeF GlyphSize = { ClusterRun.GlyphAdvances[GlyphIndex], ClusterRun.FontFace.Height * ClusterRun.FontSize };
					RectF DestinationRect { GlyphPosition, GlyphSize };
					RectF SourceRect { GlyphCellcoord.XY * FontTexture->CellSizeF, GlyphSize };
					if (ClusterRun.BiDiLevel & EBiDiLevel::RTL)
					{
						SpanPosition.X -= ClusterRun.GlyphAdvances[GlyphIndex];
						DestinationRect.X -= DestinationRect.Width;
					}
					else
						SpanPosition.X += ClusterRun.GlyphAdvances[GlyphIndex];

					AppendRectangle(
						FontTexture->Textures[GlyphCellcoord.Z],
						DestinationRect,
						{ SourceRect.Position / FontTexture->TextureSize, SourceRect.Size / FontTexture->TextureSize });
				}

				SpanX += ClusterRun.Size.Width;
			}

			LineY += LayoutLine.Height;
		}

		FlushDrawCalls();
	}

	void IRHIPainter::CopyBitmap(IBitmap * Destination, IBitmap * Source) {}

	void IRHIPainter::ReserveVertices(uint32 VerticesCount)
	{
		AssertExpr(VerticesCount <= MaxPenddingVertices / 2); // 1/2 at most.

		if (VertexBuffers.Size == 0 || PenddingVertexIndex + PenddingVertexCount + VerticesCount > MaxPenddingVertices)
		{
			FlushDrawCalls();

			IRHIResourceRef VertexBuffer = CommandList.Executor.CreateBuffer(FBufferDesc(sizeof(FRHIVertex2D) * MaxPenddingVertices, EResourceUsage::VertexBuffer | EResourceUsage::HostWrite));
			InflightResources.Add(VertexBuffer);
			VertexBuffers.Add(VertexBuffer);

			PenddingVertexIndex = 0;
			PenddingVertexCount = 0;
		}
		else {}
	}

	void IRHIPainter::AppendGeometry(TView<FRHIVertex2D> Vertices) {}

	forceinline void IRHIPainter::AppendRectangle(IRHIResource * Texture, const RectF & DestinationRect, const RectF & SourceRect)
	{
		RectF Viewport = Viewports.Size ? Viewports.Back() : RectF();

		if (PainterState.Texture.Get() != Texture)
		{
			FlushDrawCalls();
			PainterState.Texture = Texture;
			PainterState.PipelineState = 0;
		}

		TArray<FRHIVertex2D, 4> RectVertices;
		RectVertices[0] = { Viewport.XY + PointF{ DestinationRect.X, DestinationRect.Y }, { SourceRect.X, SourceRect.Y } };
		RectVertices[1] = { Viewport.XY + PointF{ DestinationRect.X + DestinationRect.Width, DestinationRect.Y }, { SourceRect.X + SourceRect.Width, SourceRect.Y } };
		RectVertices[2] = { Viewport.XY + PointF{ DestinationRect.X + DestinationRect.Width, DestinationRect.Y + DestinationRect.Height }, { SourceRect.X + SourceRect.Width, SourceRect.Y + SourceRect.Height } };
		RectVertices[3] = { Viewport.XY + PointF{ DestinationRect.X, DestinationRect.Y + DestinationRect.Height }, { SourceRect.X, SourceRect.Y + SourceRect.Height } };

		ReserveVertices(6);
		TSlice<FRHIVertex2D> Vertices = PenddingVertices.Slice(PenddingVertexIndex + PenddingVertexCount, 6);
		Vertices[0] = RectVertices[0];
		Vertices[1] = RectVertices[1];
		Vertices[2] = RectVertices[2];
		Vertices[3] = RectVertices[2];
		Vertices[4] = RectVertices[3];
		Vertices[5] = RectVertices[0];
		PenddingVertexCount += 6;
		//DrawCalls.Add({ DestinationRect, SourceRect });
	}

	void IRHIPainter::AppendRectangle(IRHIResource * Texture, const PointF Points[4], const RectF & SourceRect)
	{
		RectF Viewport = Viewports.Size ? Viewports.Back() : RectF();

		if (PainterState.Texture.Get() != Texture)
		{
			FlushDrawCalls();
			PainterState.Texture = Texture;
			PainterState.PipelineState = 0;
		}

		TArray<FRHIVertex2D, 4> RectVertices;
		RectVertices[0] = { Viewport.XY + Points[0], { SourceRect.X, SourceRect.Y } };
		RectVertices[1] = { Viewport.XY + Points[1], { SourceRect.X + SourceRect.Width, SourceRect.Y } };
		RectVertices[2] = { Viewport.XY + Points[2], { SourceRect.X + SourceRect.Width, SourceRect.Y + SourceRect.Height } };
		RectVertices[3] = { Viewport.XY + Points[3], { SourceRect.X, SourceRect.Y + SourceRect.Height } };

		ReserveVertices(6);

		TSlice<FRHIVertex2D> Vertices = PenddingVertices.Slice(PenddingVertexIndex + PenddingVertexCount, 6);
		Vertices[0] = RectVertices[0];
		Vertices[1] = RectVertices[1];
		Vertices[2] = RectVertices[2];
		Vertices[3] = RectVertices[2];
		Vertices[4] = RectVertices[3];
		Vertices[5] = RectVertices[0];
		PenddingVertexCount += 6;
	}

	void IRHIPainter::AppendTriangle(IRHIResource * Texture, const FRHIVertex2D & PointA, const FRHIVertex2D & PointB, const FRHIVertex2D & PointC)
	{
		if (PainterState.Texture.Get() != Texture)
		{
			FlushDrawCalls();
			PainterState.Texture = Texture;
			PainterState.PipelineState = 0;
		}

		ReserveVertices(3);

		TSlice<FRHIVertex2D> Vertices = PenddingVertices.Slice(PenddingVertexIndex + PenddingVertexCount, 3);
		Vertices[0] = PointA;
		Vertices[1] = PointB;
		Vertices[2] = PointC;
		PenddingVertexCount += 3;
	}

	void IRHIPainter::SetScissorRect(const RectF & ScissorRect)
	{
		if (ScissorRect.Empty())
		{
			SetScissorRect({ 0, 0, float32(RenderTarget->ResourceDesc.Size.Width), float32(RenderTarget->ResourceDesc.Size.Height) });
			return;
		}

		PointI LeftTop = Floor(ScissorRect.Position);
		PointI RightBottom = Ceil(ScissorRect.RightBottom);
		RectI ScissorRectRounded = { LeftTop, RightBottom - LeftTop };
		if (ScissorRectRounded == PainterState.ScissorRect)
			return;

		FlushDrawCalls();
		PainterState.ScissorRect = ScissorRectRounded;
	}

	forceinline void IRHIPainter::SetColor(FColor Color)
	{
		float4 LinearColor = uint4(Color.R, Color.G, Color.B, Color.A) / 255.0f;
		if (LinearColor == PixelShaderParameter.Color)
			return;

		FlushDrawCalls();
		PixelShaderParameter.Color = LinearColor;
		PainterState.PixelShaderParameter = nullptr;
	}

	forceinline void IRHIPainter::SetVShader(IShaderInstance * Shader)
	{
		if (PainterState.VertexShader == (const IShaderInstance *)Shader)
			return;

		FlushDrawCalls();
		PainterState.VertexShader = Shader;
		PainterState.PipelineState = 0;
	}

	forceinline void IRHIPainter::SetPShader(IShaderInstance * Shader)
	{
		if (PainterState.PixelShader == (const IShaderInstance *)Shader)
			return;

		FlushDrawCalls();
		PainterState.PixelShader = Shader;
		PainterState.PipelineState = 0;
	}

	forceinline void IRHIPainter::FlushDrawCalls()
	{
		if (!PenddingVertexCount)
			return;

		IRHIResource * VertexBuffer = VertexBuffers.Back();
		ArrayCopy((FRHIVertex2D *)(VertexBuffer->BaseAddress.CPUAddress + VertexBuffer->Location.Offset) + PenddingVertexIndex, PenddingVertices.Data + PenddingVertexIndex, PenddingVertexCount);

		if (!PainterState.PipelineState)
		{
			FMeshRenderState MeshRenderState;
			{
				FShaderPermutation ShaderPermutation;
				MeshRenderState.InputElements =
				{
					{ EInputSemantic::Position, 0, EFormat::Float4, 0, 0 },
					{ EInputSemantic::Texcoord, 0, EFormat::Float2, 0, sizeof(float4) },
				};
				MeshRenderState.RasterizeState = FRasterizeState::Default;
				MeshRenderState.BlendState = FBlendState::Transparent;
				MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				MeshRenderState.Shaders[EShaderStage::Vertex] = PainterState.VertexShader;
				MeshRenderState.Shaders[EShaderStage::Pixel] = PainterState.PixelShader;
			}
			MeshRenderState.RasterizeState.CullMode = ECullMode::None;
			//MeshRenderState.RasterizeState.LineAA = true;
			//MeshRenderState.RasterizeState.MSAA = true;

			CommandList.SetMeshRenderState(MeshRenderState);
			PainterState.PipelineState = 1;
		}

		if (!PainterState.VertexShaderParameter)
		{
			PainterState.VertexShaderParameter = CommandList.Executor.CreateShaderParameterT(VertexShaderParameter);
			InflightResources.Add(PainterState.VertexShaderParameter);
		}

		if (!PainterState.PixelShaderParameter)
		{
			PainterState.PixelShaderParameter = CommandList.Executor.CreateShaderParameterT(PixelShaderParameter);
			InflightResources.Add(PainterState.PixelShaderParameter);
		}

		CommandList.SetScissorRects({ { PainterState.ScissorRect.X, PainterState.ScissorRect.Y, PainterState.ScissorRect.Width, PainterState.ScissorRect.Height } });

		if (PainterState.Texture)
			CommandList.SetParameter(u8"Source"N, PainterState.Texture);

		CommandList.SetParameter(u8"VertexShaderParameter"N, PainterState.VertexShaderParameter);
		CommandList.SetParameter(u8"PixelShaderParameter"N, PainterState.PixelShaderParameter);

		CommandList.IASetVertexBuffers(0, { VertexBuffer }, { sizeof(FRHIVertex2D) }, MaxPenddingVertices);
		CommandList.DrawInstanced(PenddingVertexIndex, PenddingVertexCount);

		PenddingVertexIndex += PenddingVertexCount;
		PenddingVertexCount = 0;
	}

	void IRHIPainter::BeginPaintPrivate(IRHIResource * Texture, bool ClearBitmap)
	{
		RenderTarget = Texture;

		SizeF RenderTargetSize = SizeF(RenderTarget->ResourceDesc.Size);
		Vec2F Scale = PointF { 2.0f, -2.0f } / RenderTargetSize;
		Vec2F Translate = PointF { -1.0f, 1.0f };
		VertexShaderParameter.ScreenTransform = FMatrix
		{
			Scale.X, 0, 0, 0,
			0, Scale.Y, 0, 0,
			0, 0, 1, 0,
			Translate.X, Translate.Y, 0, 1
		};

		PainterState.ScissorRect = { { 0, 0 }, SizeI(Texture->ResourceDesc.Size)};

		FViewport Viewport { 0.0f, 0.0f, (float32)Texture->ResourceDesc.Size.Width, (float32)Texture->ResourceDesc.Size.Height, 0.0f, 1.0f };
		CommandList.BeginLabel(u8"Painter"V);
		CommandList.ResourceAccess(RenderTarget, EResourceAccess::RenderTarget);
		CommandList.BeginPass({ { { Texture->GetResourceView(), ClearBitmap ? EResourceLoad::Clear : EResourceLoad::Load } }, nullptr, { Viewport } });
	}

	void IRHIPainter::EndPaintPrivate()
	{
		FlushDrawCalls();

		CommandList.EndPass();
		CommandList.ResourceAccess(RenderTarget, EResourceAccess::None);
		CommandList.EndLabel();

		CommandList.Execute();

		PenddingVertexIndex = 0;
		PenddingVertexCount = 0;
		RenderTarget = nullptr;

		PainterState.VertexShader = nullptr;
		PainterState.PixelShader = nullptr;
		PainterState.PipelineState = 0;

		PainterState.Texture = nullptr;

		for (IRHIResourceRef & InflightResource : InflightResources)
			CommandList.Executor.FenceResource(InflightResource);
		InflightResources.Clear();

		VertexBuffers.Clear();

		PainterState.VertexShaderParameter = nullptr;
		PainterState.PixelShaderParameter = nullptr;

		CommandList.Executor.UpdateContext();
	}

	FRHIPainter::FRHIPainter(FRHIGraphics & RHIGraphic) : IRHIPainter(RHIGraphic)
	{

	}

	FRHIPainter::~FRHIPainter() {}

	FRHIViewportPainter::FRHIViewportPainter(FRHIGraphics & RHIGraphic, void * WindowHandle) : IRHIPainter(RHIGraphic)
	{
		EFormat ViewportFormats[] = { EFormat::RGBAUF, EFormat::BGRAUF, EFormat::RGBAUFsRGB, EFormat::BGRAUFsRGB };
		EFormat ViewportFormat = EFormat::None;
		for (auto Format : ViewportFormats)
		{
			if (RHIGraphic.Executor.Engine.Device.GetForamtAccess(Format) & EResourceAccess::RenderTarget)
			{
				ViewportFormat = Format;
				break;
			}
		}

		RHIViewport = RHIGraphic.Executor.Engine.Device.CreateViewport(RHIGraphic.Executor.PrimaryQueue, WindowHandle, SizeU::Zero, 3, ViewportFormat);
	}

	FRHIViewportPainter::~FRHIViewportPainter() {}

	void FRHIViewportPainter::BeginPaint(IBitmap * Bitmap)
	{
		RHIGraphics.Executor.UpdateContext();

		AssertExpr(!Bitmap);
		RHIViewport->BeginFrame();

		IRHIResource * BackBuffer = RHIViewport->GetBackBuffer(RHIViewport->GetCurrentBufferIndex());
		BeginPaintPrivate(RHIViewport->GetBackBuffer(RHIViewport->GetCurrentBufferIndex()), true);
	}

	void FRHIViewportPainter::EndPaint()
	{
		EndPaintPrivate();

		IRHIResource * BackBuffer = RHIViewport->GetBackBuffer(RHIViewport->GetCurrentBufferIndex());
		CommandList.ResourceAccess(BackBuffer, EResourceAccess::Present);
		CommandList.Execute();
		RHIViewport->EndFrame();
	}
}
