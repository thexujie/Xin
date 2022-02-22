#pragma once

#include "Xin.Rendering.Types.h"
#include "RHIVertex.h"
#include "CommandList.h"

namespace Xin::Rendering
{
	class FRHIGraphics;

	class RENDERING_API FFontTexture : public IRenderingObject
	{
	public:
		FFontTexture(FRHIGraphics & RHIGraphics, IFontFace & Font, float32 FontSize);
		~FFontTexture() override;

		// X Y ArrayIndex
		void MapGlyphs(TView<uint16> GlyphIndices);

		void FlushGlyphs();

	public:
		FRHIGraphics & RHIGraphics;

		SizeU TextureSize { 1024, 1024 };
		IFontFace & Font;
		float32 FontSize = 0.0f;

		TList<IRHIResourceRef> Textures;
		TList<IBitmapRef> Bitmaps;
		uint3 GlyphCount;
		SizeF CellSizeF;

		TList<uint16> CellGlyphIndices;
		TArray<uint3, 0xFFFF> GlyphCellcoords {};

		struct FPenddingGlyph
		{
			uint16 GlyphIndex;
			uint3 CellPosition;
		};
		TList<FPenddingGlyph> PenddingGlyphs;
	};
	using FFontTextureRef = TReferPtr<FFontTexture>;

	class RENDERING_API FRHIBitmap : public IBitmap
	{
	public:
		FRHIBitmap(FRHIGraphics & RHIGraphics, SizeU Size, EFormat Format, EBitmapUsage BitmapUsage);
		FRHIBitmap(FRHIGraphics & RHIGraphics, IRHIResource * Texture, EBitmapUsage BitmapUsage);
		FRHIBitmap(FRHIGraphics & RHIGraphics, const IPixmap & Pixmap);
		~FRHIBitmap() override;
		IGraphics & GetGraphics() const override { return (IGraphics &)RHIGraphics; }

	public:
		bool Save(FStringV FilePath, EImageFormat ImageFormat) const override;
		FLockedBitmap Lock(EBitmapLock BitmapLock) override { return { }; }
		void Unlock() override {}

	public:
		FRHIGraphics & RHIGraphics;
		IRHIResourceRef Texture;
	};
	using FRHIBitmapRef = TReferPtr<FRHIBitmap>;


	class RENDERING_API IRHIPainter : public IPainter
	{
	public:
		IRHIPainter(FRHIGraphics & RHIGraphic);
		~IRHIPainter() override;

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void BeginPaint(IBitmap * Bitmap, bool ClearBitmap);
		void EndPaint() override;
		void Clear(FColor Color) override;

	public:
		void PushViewport(const RectF & RelativeViewport) override;
		void PopViewport() override;
		void PushClip(const RectF & Rect) override {}
		void PopClip() override {}
		void PushTransform(const FTransform3x2F & Transform) override {}
		void PopTransform() override {}

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

		void DrawGlyph(uint16 GlyphIndex, PointF Point, IFontFace & FontFace, float32 FontSize, FColor Color) override {}
		void DrawTextBlob(ITextBlob & TextBlob, const PointF & Position, FColor Color) override;

	public:
		void CopyBitmap(IBitmap * Destination, IBitmap * Source) override;

	private:
		void ReserveVertices(uint32 VerticesCount);

		void AppendGeometry(TView<FRHIVertex2D> Vertices);
		void AppendRectangle(IRHIResource * Texture, const RectF & DestinationRect, const RectF & SourceRect);
		void AppendRectangle(IRHIResource * Texture, const PointF Points[4], const RectF & SourceRect);
		void AppendTriangle(IRHIResource * Texture, const FRHIVertex2D & PointA, const FRHIVertex2D & PointB, const FRHIVertex2D & PointC);

		void SetScissorRect(const RectF & ScissorRect);
		void SetColor(FColor Color);
		void SetVShader(IShaderInstance * Shader);
		void SetPShader(IShaderInstance * Shader);
		void FlushDrawCalls();

	protected:
		void BeginPaintPrivate(IRHIResource * Texture, bool ClearBitmap = true);
		void EndPaintPrivate();

	protected:
		FRHIGraphics & RHIGraphics;
		FCommandList CommandList;

		IRHIResourceRef RenderTarget;

		TList<RectF> Viewports;
		TList<RectF> ScissorRects;

		uint32 PenddingVertexIndex = 0;
		uint32 PenddingVertexCount = 0;
		TList<IRHIResourceRef> VertexBuffers;

		static constexpr uint32 MaxPenddingVertices = 4096;
		TList<FRHIVertex2D> PenddingVertices { MaxPenddingVertices, EInitializeMode::None };

		struct FVertexShaderParameter
		{
			float4x4 ScreenTransform;
		};
		FVertexShaderParameter VertexShaderParameter {};

		struct FPixelShaderParameter
		{
			float4 Color;
		};
		FPixelShaderParameter PixelShaderParameter {};

		struct FPainterState
		{
			uintx PipelineState = 0;

			RectI ScissorRect = RectI::Zero;
			IShaderInstance * VertexShader = nullptr;
			IShaderInstance * PixelShader = nullptr;

			IRHIResourceRef VertexShaderParameter = nullptr;
			IRHIResourceRef PixelShaderParameter = nullptr;

			IRHIResourceRef Texture = nullptr;
		};
		FPainterState PainterState;

		TList<IRHIResourceRef> InflightResources;
	};
	using IRHIPainterRef = TReferPtr<IRHIPainter>;

	class RENDERING_API FRHIPainter : public IRHIPainter
	{
	public:
		FRHIPainter(FRHIGraphics & RHIGraphic);
		~FRHIPainter() override;
	};

	class RENDERING_API FRHIViewportPainter : public IRHIPainter
	{
	public:
		FRHIViewportPainter(FRHIGraphics & RHIGraphic, void * WindowHandle);
		~FRHIViewportPainter() override;

		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;

	public:
		IRHIViewportRef RHIViewport;
	};
}
