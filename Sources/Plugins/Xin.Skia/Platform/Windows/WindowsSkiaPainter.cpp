#include "PCH.h"
#include "WindowsSkiaPainter.h"

#include "SkiaBitmap.h"
#include "SkiaGraphics.h"
#include "SkiaPath.h"

#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

namespace Xin::Skia
{
	FWindowsSkiaPainter::FWindowsSkiaPainter(FSkiaGraphics & SkiaDevice, void * WindowHandle) : ISkiaPainter(SkiaDevice), WindowHandle(WindowHandle)
	{
	}

	FWindowsSkiaPainter::~FWindowsSkiaPainter()
	{
		skBitmap = nullptr;
	}

	void FWindowsSkiaPainter::BeginPaint(IBitmap * Bitmap)
	{
		AssertExpr(!Bitmap);
		SizeU ClientSize = Windows::GetClientSize(HWND(WindowHandle));
		SkImageInfo skImageInfo = SkImageInfo::Make(int32(ClientSize.Width), int32(ClientSize.Height), FromFormat(EFormat::B8G8R8A8), kPremul_SkAlphaType);

		//sk_sp<SkSurface> skSurface = SkSurface::makeSurface(skImageInfo);
		skBitmap = new SkBitmap();
		skBitmap->allocPixels(skImageInfo);
		
		skCanvas = new SkCanvas(*skBitmap);

		ISkiaPainter::BeginPaint(Bitmap);
	}

	void FWindowsSkiaPainter::EndPaint()
	{
		//skCanvas.Release();

		ISkiaPainter::EndPaint();

		SizeU ClientSize = Windows::GetClientSize(HWND(WindowHandle));
		BITMAPINFO Bitmapinfo {};
		Bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		Bitmapinfo.bmiHeader.biWidth = int32(ClientSize.Width);
		Bitmapinfo.bmiHeader.biHeight = -int32(ClientSize.Height); // top-down image
		Bitmapinfo.bmiHeader.biPlanes = 1;
		Bitmapinfo.bmiHeader.biBitCount = 32;
		Bitmapinfo.bmiHeader.biCompression = BI_RGB;
		Bitmapinfo.bmiHeader.biSizeImage = 0;

		SkASSERT(skBitmap->width() * skBitmap->bytesPerPixel() == skBitmap->rowBytes());

		HDC hdcWindow = ::GetDC(HWND(WindowHandle));
		::SetDIBitsToDevice(hdcWindow, 0, 0, ClientSize.Width, ClientSize.Height, 0, 0, 0, ClientSize.Height, skBitmap->pixmap().addr32(), &Bitmapinfo, DIB_RGB_COLORS);
		::ReleaseDC(HWND(WindowHandle), hdcWindow);
	}
}
