#pragma once

#include "D2DTypes.h"
#include "Xin.Core/Platform/Windows/WIC.Types.h"

namespace Xin::D2D
{
	using namespace Xin::WIC;

	class FD2DGraphics;

	class D2D_API FWICBitmap : public FD2DBitmapBase
	{
	public:
		FWICBitmap(FD2DGraphics & D2DGraphics);
		FWICBitmap(FD2DGraphics & Graphics, const IPixmap & Pixmap);
		FWICBitmap(FD2DGraphics & D2DGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage);
		FWICBitmap(FD2DGraphics & D2DGraphics, FStringV FilePath);
		FWICBitmap(FD2DGraphics & D2DGraphics, HBITMAP hBitmap);
		~FWICBitmap();
		IGraphics & GetGraphics() const override { return (IGraphics &)D2DGraphics; }

		bool IsWICBitmap() const override { return true; }

	public:
		bool LoadFromFile(FStringV FilePath);
		bool LoadFromHBITMAP(HBITMAP hBitmap);
		bool Save(FStringV FilePath, EImageFormat ImageFormat) const override;

		FLockedBitmap Lock(EBitmapLock BitmapLock) override;
		void Unlock() override;

	public:
		FD2DGraphics & D2DGraphics;
		TReferPtr<IWICBitmapFrameDecode> wicBitmapFrameDecode;
		TReferPtr<IWICFormatConverter> wicFormatConverter;
		TReferPtr<IWICBitmap> wicBitmap;

		TReferPtr<IWICBitmapLock> LockedBitmapLock;

	public:
		IWICBitmapSourceRef GetWICBitmapSource() const;
	};
	using FWICBitmapRef = TReferPtr<FWICBitmap>;
}
