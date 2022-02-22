#include "PCH.h"
#include "WICBitmap.h"

#include "D2DGraphics.h"

namespace Xin::D2D
{
	static EFormat WICMappingFormatRGB2BGR(EFormat Format, EBitmapUsage BitmapUsage)
	{
		//if (BitmapUsage & EBitmapUsage::RenderTarget)
		//{
		//	switch (Format)
		//	{
		//	case EFormat::R8G8B8A8: Format = EFormat::B8G8R8A8; break;
		//	case EFormat::R8G8B8A8UF: Format = EFormat::B8G8R8A8UF; break;
		//	case EFormat::R8G8B8A8SF: Format = EFormat::B8G8R8A8SF; break;
		//	case EFormat::R8G8B8A8UFsRGB: Format = EFormat::B8G8R8A8UFsRGB; break;
		//	case EFormat::R8G8B8A8UI: Format = EFormat::B8G8R8A8UF; break;
		//	case EFormat::R8G8B8A8SI: Format = EFormat::B8G8R8A8UF; break;
		//	case EFormat::R8G8B8X8: Format = EFormat::B8G8R8X8; break;
		//	default: break;
		//	}
		//}
		return Format;
	}

	FWICBitmap::FWICBitmap(FD2DGraphics & D2DGraphics) : FD2DBitmapBase(),  D2DGraphics(D2DGraphics)
	{
	}

	FWICBitmap::FWICBitmap(FD2DGraphics & Graphics, const IPixmap & Pixmap): FD2DBitmapBase(Pixmap.Format, Pixmap.Size, EBitmapUsage::ShaderResource),  D2DGraphics(Graphics)
	{
		HRESULT Result = D2DGraphics.wicImagingFactory2->CreateBitmapFromMemory(
			Pixmap.Size.Width, Pixmap.Size.Height, WIC::WICPremultipliedFormatFromFormat(Pixmap.Format),
			Pixmap.Pitch, Pixmap.Bytes.Size, (BYTE *)Pixmap.Bytes.Data,
			wicBitmap.GetPP());
		HRWarning(Result, u8"IWICImagingFactory2::CreateBitmapFromMemory failed."V);
	}

	FWICBitmap::FWICBitmap(FD2DGraphics & D2DGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage): FD2DBitmapBase(WICMappingFormatRGB2BGR(Format, BitmapUsage), Size, BitmapUsage), D2DGraphics(D2DGraphics)
	{
		if ((BitmapUsage & EBitmapUsage::RenderTarget) && GetPixelFormatComponents(this->Format).W)
		{
			HRESULT Result = D2DGraphics.wicImagingFactory2->CreateBitmap(Size.Width, Size.Height, WICPremultipliedFormatFromFormat(this->Format), WICBitmapCacheOnDemand, wicBitmap.GetPP());
			HRWarning(Result, u8"IWICImagingFactory2::CreateBitmap failed."V);
		}
		else
		{
			HRESULT Result = D2DGraphics.wicImagingFactory2->CreateBitmap(Size.Width, Size.Height, WICFormatFromFormat(this->Format), WICBitmapCacheOnDemand, wicBitmap.GetPP());
			HRWarning(Result, u8"IWICImagingFactory2::CreateBitmap failed."V);
		}
	}

	FWICBitmap::FWICBitmap(FD2DGraphics & D2DGraphics, FStringV FilePath) : FD2DBitmapBase(), D2DGraphics(D2DGraphics)
	{
		LoadFromFile(FilePath);
	}

	FWICBitmap::FWICBitmap(FD2DGraphics & D2DGraphics, HBITMAP hBitmap) : FD2DBitmapBase(), D2DGraphics(D2DGraphics)
	{
		LoadFromHBITMAP(hBitmap);
	}

	FWICBitmap::~FWICBitmap()
	{
		
	}

	IWICBitmapSourceRef FWICBitmap::GetWICBitmapSource() const
	{
		if (wicFormatConverter)
			return wicFormatConverter;
		else if (wicBitmap)
			return wicBitmap;
		else if (wicBitmapFrameDecode)
			return wicBitmapFrameDecode;
		return nullptr;
	}

	bool FWICBitmap::LoadFromFile(FStringV FilePath)
	{
		TReferPtr<IWICBitmapDecoder> WICBitmapDecoder;

		FWString FilePathW = FEncoding::ToWide(FilePath);
		HRESULT Result = D2DGraphics.wicImagingFactory2->CreateDecoderFromFilename(
			FilePathW.Data,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&WICBitmapDecoder
		);
		HRWarning(Result, u8"CreateDecoderFromFilename failed."V);

		if (Succeeded(Result))
		{
			Result = WICBitmapDecoder->GetFrame(0, &wicBitmapFrameDecode);
		}

		if (Succeeded(Result))
		{
			// Convert the image format to 32bppPBGRA
			// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
			Result = D2DGraphics.wicImagingFactory2->CreateFormatConverter(&wicFormatConverter);
		}

		if (Succeeded(Result))
		{
			WICPixelFormatGUID FormatGUID;
			wicBitmapFrameDecode->GetPixelFormat(&FormatGUID);
			EFormat PFormat = WICFormatToFormat(FormatGUID);
			UByte4 PixelFormatComponnets = GetPixelFormatComponents(PFormat);
			//if (PixelFormatComponnets.AllZero())

			// GUID_WICPixelFormat32bppRGBA
			// The WIC pixel format supplied is not supported by Direct2D. In particular, Direct2D does not support WIC bitmaps that use straight alpha unless the depth is 8bpp.
			Result = wicFormatConverter->Initialize(
				wicBitmapFrameDecode.Get(),
				PixelFormatComponnets.A ? GUID_WICPixelFormat32bppPRGBA : GUID_WICPixelFormat32bppRGB,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}

		if (Succeeded(Result))
		{
			UINT Width, Height;
			wicFormatConverter->GetSize(&Width, &Height);
			Size = { Width, Height };

			WICPixelFormatGUID wicPixelFormatGUID;
			wicFormatConverter->GetPixelFormat(&wicPixelFormatGUID);
			Format = WICFormatToFormat(wicPixelFormatGUID);
		}
		return Succeeded(Result);
	}

	bool FWICBitmap::LoadFromHBITMAP(HBITMAP hBitmap)
	{
		WICBitmapAlphaChannelOption AlphaChannelOption { };
		HRESULT Result = D2DGraphics.wicImagingFactory2->CreateBitmapFromHBITMAP(hBitmap, NULL, AlphaChannelOption, wicBitmap.GetPP());

		if (Succeeded(Result))
		{
			uint32 Width, Height;
			wicBitmap->GetSize(&Width, &Height);

			WICPixelFormatGUID wicPixelFormatGUID;
			wicBitmap->GetPixelFormat(&wicPixelFormatGUID);

			Size = { Width, Height };
			Format = WICFormatToFormat(wicPixelFormatGUID);
		}

		return Succeeded(Result);
	}

	bool FWICBitmap::Save(FStringV FilePath, EImageFormat ImageFormat) const
	{
		if (!(Usage & EBitmapUsage::ReadBack))
			return false;

		TReferPtr<IWICStream> WICStream;
		HRESULT Result = D2DGraphics.wicImagingFactory2->CreateStream(WICStream.GetPP());
		if (SUCCEEDED(Result))
		{
			FWString FilePathW = FEncoding::ToWide(FilePath);
			Result = WICStream->InitializeFromFilename(FilePathW.Data, GENERIC_WRITE);
		}

		//IWICBitmapRef WICBitmap;
		//Result = D2DPainter.D2DDevice.WICImagingFactory2->CreateBitmapFromMemory(
		//	Size.Width, Size.Height, WICPremultipliedFormatFromFormat(Format),
		//	D2DMappedRect.pitch, D2DMappedRect.pitch * Size.Height, D2DMappedRect.bits,
		//	WICBitmap.GetPP());

		TReferPtr<IWICBitmapEncoder> WICBitmapEncoder;
		const GUID & WICContainerFormat = WICContainerFormatFromImageFormat(ImageFormat);
		if (SUCCEEDED(Result))
			Result = D2DGraphics.wicImagingFactory2->CreateEncoder(WICContainerFormat, NULL, WICBitmapEncoder.GetPP());

		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->Initialize(WICStream.Get(), WICBitmapEncoderNoCache);

		TReferPtr<IWICBitmapFrameEncode> WICBitmapFrameEncode;
		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->CreateNewFrame(WICBitmapFrameEncode.GetPP(), NULL);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->Initialize(NULL);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->SetSize(Size.Width, Size.Height);

		GUID WICPixelFormat = WICFormatFromFormat(Format);
		//WICPixelFormat = GUID_WICPixelFormatUndefined;
		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->SetPixelFormat(&WICPixelFormat);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->WriteSource(wicBitmapFrameDecode.Get(), nullptr);

		if (SUCCEEDED(Result))
			Result = WICBitmapFrameEncode->Commit();

		if (SUCCEEDED(Result))
			Result = WICBitmapEncoder->Commit();

		return SUCCEEDED(Result);
	}

	FLockedBitmap FWICBitmap::Lock(EBitmapLock BitmapLock)
	{
		AssertExpr(!LockedBitmapLock);

		HRESULT Result = wicBitmap->Lock(nullptr, (BitmapLock & EBitmapLock::Write) ? WICBitmapLockWrite : WICBitmapLockRead, LockedBitmapLock.GetPP());

		uint32 BufferSize;
		WICInProcPointer ProcPointer;
		Result = LockedBitmapLock->GetDataPointer(&BufferSize, &ProcPointer);

		uint32 Pitch;
		LockedBitmapLock->GetStride(&Pitch);

		return { this, ProcPointer, Pitch };
		
	}

	void FWICBitmap::Unlock()
	{
		if (LockedBitmapLock)
		{
			LockedBitmapLock = nullptr;
		}
	}
}
