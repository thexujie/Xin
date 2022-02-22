#pragma once

#include "X2D.Types.h"

#include "Path.h"
#include "Painter.h"
#include "../Media/Image.h"

namespace Xin
{
	enum class EGraphicsRHI
	{
		None = 0,
		D2D = MakeFourCC("D2D "),
		GDI = MakeFourCC("GDI "),
		GDIP = MakeFourCC("GDIP"),
		Skia = MakeFourCC("SKIA"),
		RHI = MakeFourCC("RHI "),
		Custom,
	};

	class CORE_API ITextShaper : public IX2DObject
	{
	public:
		virtual IFontFamilyRef CreateFontFamily(FStringV FamilyName) = 0;
		virtual IFontFaceRef CreateFontFace(const FFontFace & FontFace) = 0;

		virtual ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None) = 0;
	};

	using ITextShaperRef = TReferPtr<ITextShaper>;

	class CORE_API IGraphics : public IX2DObject
	{
	public:
		IGraphics(EGraphicsRHI GraphicsRHI) : GraphicsRHI(GraphicsRHI) {}

		virtual IPainterRef CreatePainter() = 0;
		virtual IPainterRef CreateWindowPainter(voidp WindowHandle) = 0;
		virtual IPainterRef CreateDCPainter(voidp HDCHandle, const RectU & Rect) = 0;

	public: // Text
		virtual IFontFamilyRef CreateFontFamily(FStringV FamilyName) = 0;
		virtual IFontFaceRef CreateFontFace(const FFontFace & FontFace) = 0;

		virtual ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None) = 0;

		virtual IPathRef CreatePath() = 0;
		virtual IPathRef CreateTextPath(ITextBlob & TextLayout, const Point2F & Position) = 0;

	public: // Image
		virtual IBitmapRef CreateBitmap(FStringV FilePath) = 0;
		virtual IBitmapRef CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage) = 0;
		virtual IBitmapRef CreateBitmap(const IPixmap & Pixmap) = 0;

		virtual bool SupportSharedBitmap(uint32 DeviceRHI) const { return false; }
		/**
		 * 
		 * @param SharedHandle type of ID3D12Resource * for RHI.D3D12
		 * @param BitmapUsage 
		 * @return 
		 */
		virtual IBitmapRef CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage) = 0;

	public:
		EGraphicsRHI GraphicsRHI = EGraphicsRHI::None;
	};

	using IGraphicsRef = TReferPtr<IGraphics>;
}
