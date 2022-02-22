#pragma once

#include "SkiaTypes.h"
#include "SkiaShaper.h"

namespace Xin::Skia
{
	class SKIA_API FSkiaGraphics : public IGraphics
	{
	public:
		FSkiaGraphics();
		~FSkiaGraphics();

	public:
		IPainterRef CreatePainter() override;
		IPainterRef CreateWindowPainter(voidp WindowHandle) override;
		IPainterRef CreateDCPainter(voidp HDCHandle, const RectU & Rect) override;

	public:
		IFontFamilyRef CreateFontFamily(FStringV FamilyName) override;
		IFontFaceRef CreateFontFace(const FFontFace & FontFace) override;

		ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags) override;

		IPathRef CreatePath() override;
		IPathRef CreateTextPath(ITextBlob & TextLayout, const Point2F & Position) override;

	public: // Image
		IBitmapRef CreateBitmap(FStringV FilePath) override;
		IBitmapRef CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage) override;
		IBitmapRef CreateBitmap(const IPixmap & Pixmap) override;

		bool SupportSharedBitmap(uint32 DeviceRHI) const override;
		IBitmapRef CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage) override;

	private:
		void InitializeSkia();

	public:
		FSkiaShaper Shaper { *this };

	public:
	};

	using FSkiaGraphicsRef = TReferPtr<FSkiaGraphics>;
}
