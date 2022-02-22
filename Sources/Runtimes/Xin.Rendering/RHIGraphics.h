#pragma once

#include "Xin.Rendering.Types.h"

#include "RHIPainter.h"

namespace Xin::Rendering
{
	class RENDERING_API FRHIGraphics : public IGraphics
	{
	public:
		FRHIGraphics(FCommandExecutor & Executor);
		~FRHIGraphics() override;

	public:
		void Initialize(IGraphics * OfflineGraphic);
		void Finalize();

	public:
		IPainterRef CreatePainter() override;
		IPainterRef CreateWindowPainter(voidp WindowHandle) override;
		IPainterRef CreateDCPainter(voidp HDCHandle, const RectU & Rect) override { return nullptr; }

	public:
		IFontFamilyRef CreateFontFamily(FStringV FamilyName) override { return nullptr; }
		IFontFaceRef CreateFontFace(const FFontFace & FontFace) override { return nullptr; }

		ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None) override;

	public:
		IPathRef CreatePath() override;
		IPathRef CreateTextPath(ITextBlob & TextLayout, const Point2F & Position) override;

	public:
		IBitmapRef CreateBitmap(FStringV FilePath) override;
		IBitmapRef CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage) override;
		IBitmapRef CreateBitmap(const IPixmap & Pixmap) override;
		IBitmapRef CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage) override;

	public:
		IBitmapRef CreateBitmap(IRHIResource * Resource, EBitmapUsage BitmapUsage);
		FRHIBitmapRef RHICreateBitmap(const FImageData & ImageData);

		void MapGlyphs(IFontFace & Font, float32 FontSize, TView<uint16> GlyphIndices);
		FFontTextureRef GetFontTexture(IFontFace & Font, float32 FontSize);

	public:
		FCommandExecutor & Executor;
		IGraphicsRef OfflineGraphics;
		IPainterRef OfflinePainter;

	private:
		TMap<TPair<IFontFaceRef, float32>, FFontTextureRef> FontTextures;
	};
	using FRHIGraphicsRef = TReferPtr<FRHIGraphics>;
}
