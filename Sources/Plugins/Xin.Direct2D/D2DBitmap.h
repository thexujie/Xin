#pragma once

#include "D2DTypes.h"
#include <d3d12.h>

namespace Xin::D2D
{
	struct FD2DMappedRect : public D2D1_MAPPED_RECT
	{
		FD2DMappedRect(ID2D1Bitmap1 * D2D1Bitmap1, D2D1_MAP_OPTIONS MapOptions) : D2D1Bitmap1(D2D1Bitmap1)
		{
			HRESULT Result = D2D1Bitmap1->Map(MapOptions, this);
			if (Failed(Result))
				bits = nullptr;
		}

		~FD2DMappedRect()
		{
			if (D2D1Bitmap1)
				D2D1Bitmap1->Unmap();
		}

		ID2D1Bitmap1Ref D2D1Bitmap1;
	};

	class D2D_API FD2DBitmap : public FD2DBitmapBase
	{
	public:
		FD2DBitmap(FD2DGraphics & Graphics);
		FD2DBitmap(FD2DGraphics & Graphics, const IPixmap & Pixmap);
		FD2DBitmap(FD2DGraphics & Graphics, ID2D1Bitmap1 * D2D1Bitmap1);
		FD2DBitmap(FD2DGraphics & Graphics, ID3D12Resource * D3D12Resource, EBitmapUsage BitmapUsage = EBitmapUsage::ShaderResource);
		FD2DBitmap(FD2DGraphics & D2DGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage);
		~FD2DBitmap();
		IGraphics & GetGraphics() const override { return (IGraphics &)Graphics; }

	public:
		void AcquireSurface() override;
		void ReleaseSurface() override;

		FLockedBitmap Lock(EBitmapLock BitmapLock) override;
		void Unlock() override;

		bool Save(FStringV FilePath, EImageFormat ImageFormat) const override;

	public:
		FD2DGraphics & Graphics;

		ID2D1Bitmap1Ref d2d1Bitmap1;

		mutable ID2D1Bitmap1Ref d2d1Bitmap1Staging;
		ID3D11ResourceRef d3d11Resource;
	};
	using FD2DBitmapRef = TReferPtr<FD2DBitmap>;
}
