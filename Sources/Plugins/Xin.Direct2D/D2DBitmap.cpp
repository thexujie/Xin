#include "PCH.h"
#include "D2DBitmap.h"
#include "D2DGraphics.h"
#include "D2DPainter.h"

namespace Xin::D2D
{
	static FBitmapDesc BitmapDescFromID2D1Bitmap1(ID2D1Bitmap1 * D2D1Bitmap1)
	{
		D2D1_SIZE_U D2DSize = D2D1Bitmap1->GetPixelSize();
		FBitmapDesc BitmapDesc;
		BitmapDesc.Size = { D2DSize.width, D2DSize.height };
		BitmapDesc.Format = ToFormat(D2D1Bitmap1->GetPixelFormat().format);
		BitmapDesc.Usage = EBitmapUsage::None;
		D2D1_BITMAP_OPTIONS D2DBitmapOptions = D2D1Bitmap1->GetOptions();
		if (D2DBitmapOptions & D2D1_BITMAP_OPTIONS_TARGET)
			BitmapDesc.Usage |= EBitmapUsage::RenderTarget;
		if (!(D2DBitmapOptions & D2D1_BITMAP_OPTIONS_CANNOT_DRAW))
			BitmapDesc.Usage |= EBitmapUsage::ShaderResource;
		if (D2DBitmapOptions & D2D1_BITMAP_OPTIONS_CPU_READ)
			BitmapDesc.Usage |= EBitmapUsage::ReadBack;
		return BitmapDesc;
	}

	FD2DBitmap::FD2DBitmap(FD2DGraphics & Graphics) : Graphics(Graphics)
	{
		
	}

	FD2DBitmap::FD2DBitmap(FD2DGraphics & Graphics, const IPixmap & Pixmap)
		: FD2DBitmapBase(Pixmap.Format, Pixmap.Size, EBitmapUsage::ShaderResource), Graphics(Graphics)
	{
		
	}

	FD2DBitmap::FD2DBitmap(FD2DGraphics & Graphics, ID2D1Bitmap1 * D2D1Bitmap1)
		: FD2DBitmapBase(BitmapDescFromID2D1Bitmap1(D2D1Bitmap1)), Graphics(Graphics), d2d1Bitmap1(D2D1Bitmap1) {}

	FD2DBitmap::FD2DBitmap(FD2DGraphics & Graphics, ID3D12Resource * D3D12Resource, EBitmapUsage BitmapUsage)
		: FD2DBitmapBase(), Graphics(Graphics)
	{
		D3D12_RESOURCE_DESC D3D12ResourceDesc = D3D12Resource->GetDesc();
		Format = ToFormat(D3D12ResourceDesc.Format);
		Size = { (uint32)D3D12ResourceDesc.Width, (uint32)D3D12ResourceDesc.Height };

		AssertExpr(D3D12ResourceDesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM || D3D12ResourceDesc.Format == DXGI_FORMAT_R8G8B8A8_TYPELESS);

		D3D11_RESOURCE_FLAGS D3D11ResourceFlags {};
		SetBits<UINT>(D3D11ResourceFlags.BindFlags, D3D11_BIND_RENDER_TARGET, BitmapUsage & EBitmapUsage::RenderTarget);
		SetBits<UINT>(D3D11ResourceFlags.BindFlags, D3D11_BIND_SHADER_RESOURCE, BitmapUsage & EBitmapUsage::ShaderResource);
		//D3D12_RESOURCE_STATES ResourceState = FromResourceAccess(RHITexture.ResourceDesc.InitialAccess);
		D3D12_RESOURCE_STATES ResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		HRESULT Result = Graphics.d3d11On12Device->CreateWrappedResource(
			D3D12Resource,
			&D3D11ResourceFlags,
			ResourceState,
			ResourceState,
			IID_PPV_ARGS(d3d11Resource.GetPP()));
		if (Failed(Result))
			return;

		IDXGISurfaceRef DXGISurface = d3d11Resource.QueryInterface<IDXGISurface>();
		if (!DXGISurface)
			return;

		D2D1_BITMAP_PROPERTIES1 D2D1BitmapProperties {};
		D2D1BitmapProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
		D2D1BitmapProperties.dpiY = USER_DEFAULT_SCREEN_DPI;
		D2D1BitmapProperties.pixelFormat.format = D3D12ResourceDesc.Format;
		D2D1BitmapProperties.pixelFormat.alphaMode = AlphaModeFromFormat(ToFormat(D3D12ResourceDesc.Format));
		SetFlags(D2D1BitmapProperties.bitmapOptions, D2D1_BITMAP_OPTIONS_TARGET, BitmapUsage & EBitmapUsage::RenderTarget);
		SetFlags(D2D1BitmapProperties.bitmapOptions, D2D1_BITMAP_OPTIONS_CANNOT_DRAW, !(BitmapUsage & EBitmapUsage::ShaderResource));

		Result = Graphics.d2d1DeviceContext->CreateBitmapFromDxgiSurface(DXGISurface.Get(), D2D1BitmapProperties, d2d1Bitmap1.GetPP());
		if (Failed(Result))
			LogInfo(u8"ID2D1DeviceContext::CreateBitmapFromDxgiSurface failed."V);
	}

	FD2DBitmap::FD2DBitmap(FD2DGraphics & D2DGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage)
		: FD2DBitmapBase(Format, Size, BitmapUsage), Graphics(D2DGraphics)
	{
		AssertExpr(!HasFlags(BitmapUsage, EBitmapUsage::ReadBack) || !HasFlags(BitmapUsage, EBitmapUsage::RenderTarget));
		D2D1_BITMAP_PROPERTIES1 D2D1BitmapProperties {};
		D2D1BitmapProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
		D2D1BitmapProperties.dpiY = USER_DEFAULT_SCREEN_DPI;
		D2D1BitmapProperties.pixelFormat.format = FromFormat(Format);
		D2D1BitmapProperties.pixelFormat.alphaMode = AlphaModeFromFormat(Format);
		SetFlags(D2D1BitmapProperties.bitmapOptions, D2D1_BITMAP_OPTIONS_TARGET, BitmapUsage & EBitmapUsage::RenderTarget);
		SetFlags(D2D1BitmapProperties.bitmapOptions, D2D1_BITMAP_OPTIONS_CANNOT_DRAW, !(BitmapUsage & EBitmapUsage::ShaderResource));
		SetFlags(D2D1BitmapProperties.bitmapOptions, D2D1_BITMAP_OPTIONS_CPU_READ, BitmapUsage & EBitmapUsage::ReadBack);

		HRESULT Result = D2DGraphics.d2d1DeviceContext->CreateBitmap(D2D1_SIZE_U { Size.Width, Size.Height }, nullptr, 0, D2D1BitmapProperties, d2d1Bitmap1.GetPP());
		HRWarning(Result, u8"ID2D1DeviceContext::CreateBitmap failed."V);
		//D2DPainter.D2D1RenderTarget->CreateBitmap();
	}

	FD2DBitmap::~FD2DBitmap() {}

	void FD2DBitmap::AcquireSurface()
	{
		if (d3d11Resource)
		{
			Graphics.d3d11On12Device->AcquireWrappedResources(d3d11Resource.GetPP(), 1);
		}
	}

	void FD2DBitmap::ReleaseSurface()
	{
		if (d3d11Resource)
		{
			Graphics.d3d11On12Device->ReleaseWrappedResources(d3d11Resource.GetPP(), 1);
			Graphics.d3d11DeviceContext->Flush();
		}
	}

	FLockedBitmap FD2DBitmap::Lock(EBitmapLock BitmapLock)
	{
		HRESULT Result;
		if (!(Usage & EBitmapUsage::ReadBack) && !d2d1Bitmap1Staging)
		{
			D2D1_BITMAP_PROPERTIES1 D2D1BitmapProperties {};
			D2D1BitmapProperties.dpiX = USER_DEFAULT_SCREEN_DPI;
			D2D1BitmapProperties.dpiY = USER_DEFAULT_SCREEN_DPI;
			D2D1BitmapProperties.pixelFormat.format = FromFormat(Format);
			D2D1BitmapProperties.pixelFormat.alphaMode = AlphaModeFromFormat(Format);
			D2D1BitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
			Result = Graphics.d2d1DeviceContext->CreateBitmap(D2D1_SIZE_U { Size.Width, Size.Height }, nullptr, 0, D2D1BitmapProperties, d2d1Bitmap1Staging.GetPP());
		}

		if (d2d1Bitmap1Staging)
		{
			Result = d2d1Bitmap1Staging->CopyFromBitmap(nullptr, d2d1Bitmap1, nullptr);
		}

		ID2D1Bitmap1 * Bitmap = d2d1Bitmap1Staging ? d2d1Bitmap1Staging : d2d1Bitmap1;

		D2D1_MAPPED_RECT MappedRect { };
		Result = Bitmap->Map(FromBitmapLock(BitmapLock), &MappedRect);

		FLockedBitmap LockedBitmap;
		LockedBitmap.Bitmap = this;
		LockedBitmap.Bytes = (byte *)MappedRect.bits;
		LockedBitmap.Pitch = MappedRect.pitch;
		return LockedBitmap;
	}

	void FD2DBitmap::Unlock()
	{
		ID2D1Bitmap1 * Bitmap = d2d1Bitmap1Staging ? d2d1Bitmap1Staging : d2d1Bitmap1;
		if (Bitmap)
		{
			Bitmap->Unmap();
			// just keep.....
			//StagingD2D1Bitmap1 = nullptr;
		}
		
	}

	bool FD2DBitmap::Save(FStringV FilePath, EImageFormat ImageFormat) const
	{
		if (FLockedBitmap LockedBitmap = ConstCast(*this).Lock(EBitmapLock::Read))
		{
			FImageData ImageData;
			ImageData.Format = ToFormat(d2d1Bitmap1->GetPixelFormat().format);
			ImageData.Width = Size.Width;
			ImageData.Height = Size.Height;
			ImageData.MipLevels = 1;
			ImageData.NumLayers = 1;
			ImageData.Bytes = { LockedBitmap.Bytes, LockedBitmap.Pitch * ImageData.Height };

			return IImage::Save(ImageData, FilePath, ImageFormat);;
		}
		return false;
#if 0
		TReferPtr<IWICStream> WICStream;
		HRESULT Result = D2DPainter.D2DDevice.WICImagingFactory2->CreateStream(WICStream.GetPP());
		if (SUCCEEDED(Result))
		{
			FWString FilePathW = Strings::ToWide(FilePath);
			Result = WICStream->InitializeFromFilename(FilePathW.Data, GENERIC_WRITE);
		}

		IWICBitmapRef WICBitmap;
		Result = D2DPainter.D2DDevice.WICImagingFactory2->CreateBitmapFromMemory(
			Size.Width, Size.Height, WIC::WICPremultipliedFormatFromFormat(Format),
			D2DMappedRect.pitch, D2DMappedRect.pitch * Size.Height, D2DMappedRect.bits,
			WICBitmap.GetPP());

		TReferPtr<IWICBitmapEncoder> WICBitmapEncoder;
		const GUID & WICContainerFormat = WIC::WICContainerFormatFromImageFormat(ImageFormat);
		if (SUCCEEDED(Result))
			Result = D2DPainter.D2DDevice.WICImagingFactory2->CreateEncoder(WICContainerFormat, NULL, WICBitmapEncoder.GetPP());

		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->Initialize(WICStream.Get(), WICBitmapEncoderNoCache);

		TReferPtr<IWICBitmapFrameEncode> WICBitmapFrameEncode;
		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->CreateNewFrame(WICBitmapFrameEncode.GetPP(), NULL);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->Initialize(NULL);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->SetSize(Size.Width, Size.Height);

		GUID WICPixelFormat = WIC::WICFormatFromFormat(Format);
		//WICPixelFormat = GUID_WICPixelFormatUndefined;
		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->SetPixelFormat(&WICPixelFormat);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->WriteSource(WICBitmap.Get(), nullptr);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->Commit();

		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->Commit();

		return SUCCEEDED(Result);
#endif
	}
}
