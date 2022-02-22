#pragma once

#include "SkiaTypes.h"

namespace Xin::Skia
{
	class FSkiaGraphics;

	class SKIA_API FSkiaImageCodec
	{
	public:
		static sk_sp<SkImage> LoadFromFile(FStringV FilePath);
		static bool SaveToFile(const SkPixmap & skPixmap, FStringV FilePath, EImageFormat ImageFormat);
		static bool SaveToFile(const SkImage & skImage, FStringV FilePath, EImageFormat ImageFormat);
	};

	class SKIA_API FSkiaBitmap : public IBitmap
	{
	public:
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics);
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics, FStringV FilePath) : FSkiaBitmap(SkiaGraphics, FSkiaImageCodec::LoadFromFile(FilePath)) {}
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics, const IPixmap & Pixmap);
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics, sk_sp<SkImage> skImage);
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics, TUniquePtr<SkBitmap> && Bitmap);
		FSkiaBitmap(FSkiaGraphics & SkiaGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage);
		~FSkiaBitmap();
		IGraphics & GetGraphics() const override { return (IGraphics &)Graphics; }

	public:
		void AcquireSurface() override;
		void ReleaseSurface() override;

		FLockedBitmap Lock(EBitmapLock BitmapLock) override;
		void Unlock() override;

		bool Save(FStringV FilePath, EImageFormat ImageFormat) const override;

	public:
		FSkiaGraphics & Graphics;
		TUniquePtr<SkBitmap> skBitmap;
		sk_sp<SkImage> skImage;

	private:
		SkPixmap skPixmap;
	};
	using FSkiaBitmapRef = TReferPtr<FSkiaBitmap>;
}
