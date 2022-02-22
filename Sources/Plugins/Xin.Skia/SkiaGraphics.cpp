#include "PCH.h"
#include "SkiaGraphics.h"
#include "SkiaBitmap.h"
#include "SkiaPath.h"
#include "SkiaPainter.h"

#include <include/core/SkGraphics.h>
#include "Platform/Windows/WindowsSkiaPainter.h"

namespace Xin::Skia
{
	FSkiaGraphics::FSkiaGraphics() : IGraphics(EGraphicsRHI::Skia)
	{
		InitializeSkia();
	}

	FSkiaGraphics::~FSkiaGraphics()
	{
		//SkGraphics::PurgeAllCaches();
	}

	IPainterRef FSkiaGraphics::CreatePainter()
	{
		return new FSkiaPainter(*this);
	}

	IPainterRef FSkiaGraphics::CreateWindowPainter(voidp WindowHandle)
	{
		return new FWindowsSkiaPainter(*this, WindowHandle);
	}

	IPainterRef FSkiaGraphics::CreateDCPainter(voidp HDCHandle, const RectU & Rect)
	{
		return nullptr;
	}

	IFontFamilyRef FSkiaGraphics::CreateFontFamily(FStringV FamilyName)
	{
		//return DWTextShaper.CreateFontFamily(FamilyName);
		return nullptr;
	}

	IFontFaceRef FSkiaGraphics::CreateFontFace(const FFontFace & FontFace)
	{
		return nullptr;
	}

	ITextBlobRef FSkiaGraphics::CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		return Shaper.CreateTextBlob(String, FontFace, FontSize, LayoutSize, LayoutFlags);
	}

	IPathRef FSkiaGraphics::CreatePath()
	{
		return new FSkiaPath(*this);
	}

	IPathRef FSkiaGraphics::CreateTextPath(ITextBlob & TextLayout, const Point2F & Position)
	{
		return nullptr;
	}

	IBitmapRef FSkiaGraphics::CreateBitmap(FStringV FilePath)
	{
		return new FSkiaBitmap(*this, FilePath);
	}

	IBitmapRef FSkiaGraphics::CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage)
	{
		return new FSkiaBitmap(*this, Format, Size, BitmapUsage);
	}

	IBitmapRef FSkiaGraphics::CreateBitmap(const IPixmap & Pixmap)
	{
		return new FSkiaBitmap(*this, Pixmap);
	}

	bool FSkiaGraphics::SupportSharedBitmap(uint32 DeviceRHI) const
	{
		return false;
	}

	IBitmapRef FSkiaGraphics::CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage)
	{
		return nullptr;
	}

	void FSkiaGraphics::InitializeSkia()
	{
	}
}
