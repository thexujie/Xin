#pragma once

#include "SkiaTypes.h"
#include "../../SkiaPainter.h"

namespace Xin::Skia
{
	class SKIA_API FWindowsSkiaPainter : public ISkiaPainter
	{
	public:
		FWindowsSkiaPainter(FSkiaGraphics & SkiaDevice, void * WindowHandle);
		~FWindowsSkiaPainter();

	public:
		void BeginPaint(IBitmap * Bitmap) override;
		void EndPaint() override;

	private:
		void * WindowHandle = nullptr;
		TUniquePtr<SkBitmap> skBitmap;
	};
}
